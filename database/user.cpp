
#include "user.h"
#include "../config/config.h"
#include "database.h"
#include "cache.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Parser.h>
#include <cppkafka/cppkafka.h>

#include <exception>
#include <future>
#include <optional>
#include <sstream>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void User::init()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();

            for (auto &hist : database::Database::get_all_hints())
            {
                Statement create_stmt(session);
                create_stmt << "CREATE TABLE IF NOT EXISTS `User` (`id` INT NOT NULL AUTO_INCREMENT,"
                            << "`first_name` VARCHAR(256) NOT NULL,"
                            << "`last_name` VARCHAR(256) NOT NULL,"
                            << "`login` VARCHAR(256) NOT NULL,"
                            << "`password` VARCHAR(256) NOT NULL,"
                            << "`email` VARCHAR(256) NULL,"
                            << "`title` VARCHAR(1024) NULL,"
                            << "PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`));"
                            << hist,
                    now;
                std::cout << create_stmt.toString() << std::endl;
            }

            Statement create_lu(session);

            create_lu << "CREATE TABLE IF NOT EXISTS Last_user_id ("
                      << "id INT  NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                      << "last_id INT);"
                      << "-- sharding:0",
                now;

            std::cout << create_lu.toString() << std::endl;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::optional<User> User::read_from_cache_by_id(long id)
    {

        try
        {
            std::string result;
            if (database::Cache::get().get(id, result))
                return fromJSON(result);
            else
                return std::optional<User>();
        }
        catch (std::exception &err)
        {
            // std::cerr << "error:" << err.what() << std::endl;
            return std::optional<User>();
        }
    }

#include <mutex>
    void User::send_to_queue()
    {
        static cppkafka::Configuration config = {
            {"metadata.broker.list", Config::get().get_queue_host()},
            {"acks", "all"}};
        static cppkafka::Producer producer(config);
        static std::mutex mtx;
        static int message_key{0};
        using Hdr = cppkafka::MessageBuilder::HeaderType;

        std::lock_guard<std::mutex> lock(mtx);
        std::stringstream ss;
        Poco::JSON::Stringifier::stringify(toJSON(), ss);
        std::string message = ss.str();
        bool not_sent = true;

        cppkafka::MessageBuilder builder(Config::get().get_queue_topic());
        std::string mk = std::to_string(++message_key);
        builder.key(mk);                                       // set some key
        builder.header(Hdr{"producer_type", "user writer"}); // set some custom header
        builder.payload(message);                              // set message

        while (not_sent)
        {
            try
            {
                producer.produce(builder);
                not_sent = false;
            }
            catch (...)
            {
            }
        }
    }
    void User::save_to_cache()
    {
        std::stringstream ss;
        Poco::JSON::Stringifier::stringify(toJSON(), ss);
        std::string message = ss.str();
        database::Cache::get().put(_id, message);
    }



    Poco::JSON::Object::Ptr User::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("first_name", _first_name);
        root->set("last_name", _last_name);
        root->set("email", _email);
        root->set("title", _title);
        root->set("login", _login);
        root->set("password", _password);

        return root;
    }

    User User::fromJSON(const std::string &str)
    {
        User user;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        user.id() = object->getValue<long>("id");
        user.first_name() = object->getValue<std::string>("first_name");
        user.last_name() = object->getValue<std::string>("last_name");
        user.email() = object->getValue<std::string>("email");
        user.title() = object->getValue<std::string>("title");
        user.login() = object->getValue<std::string>("login");
        user.password() = object->getValue<std::string>("password");

        return user;
    }

    std::optional<long> User::auth(std::string &login, std::string &password)
    {
        try
        {
            std::vector<long> result;
            // get all hints for shards
            std::vector<std::string> hints = database::Database::get_all_hints();

            std::vector<std::future<std::vector<long>>> futures;

            for (const std::string &hint : hints)
            {
                auto handle = std::async(
                    std::launch::async,
                    [&login, &password, hint]() -> std::vector<long>

                    {
                        std::vector<long> ids;
                        Poco::Data::Session session = database::Database::get().create_session();
                        Poco::Data::Statement select(session);
                        long id;
                        select << "SELECT id FROM User where login=? and password=?"
                               << hint,
                            into(id),
                            use(login), use(password),
                            range(0, 1); //  iterate over result set one row at a time

                        select.execute();
                        Poco::Data::RecordSet rs(select);
                        if (rs.moveFirst())
                            ids.push_back(id);

                        return ids;
                    });

                futures.emplace_back(std::move(handle));
            }
            for (auto &future : futures)
            {
                std::vector<long> ids = future.get();
                result.insert(result.end(), ids.begin(), ids.end());
            }

            return result[0];
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
        }
        return {};
    }

    std::optional<User> User::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            User a;
            std::string sharding_hint = database::Database::sharding_user(id);
            std::string sql = "SELECT id, first_name, last_name, email, "
                              "title,login,password FROM User where id=? " +
                              sharding_hint;
            // std::cout << sql << std::endl;
            select << sql, into(a._id), into(a._first_name), into(a._last_name),
                into(a._email), into(a._title), into(a._login), into(a._password),
                use(id), range(0, 1); //  iterate over result set one row at a time

            // std::cout << "SQL executed:";
            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst())
            {
                // std::cout << " record found" << std::endl;
                return a;
            }
            else
            {
                // std::cout << " no record found" << std::endl;
            }
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
        }
        return {};
    }

    std::vector<User> User::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<User> result;
            User a;
            select << "SELECT id, first_name, last_name, email, title, login, password "
                      "FROM User",
                into(a._id), into(a._first_name), into(a._last_name), into(a._email),
                into(a._title), into(a._login), into(a._password),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::vector<User> User::search(std::string first_name, std::string last_name)
    {
        try
        {
            std::vector<User> result;
            // get all hints for shards
            std::vector<std::string> hints = database::Database::get_all_hints();

            std::vector<std::future<std::vector<User>>> futures;

            for (const std::string &hint : hints)
            {
                auto handle = std::async(
                    std::launch::async,
                    [first_name, last_name, hint]() -> std::vector<User>

                    {
                        std::vector<User> users;
                        Poco::Data::Session session =
                            database::Database::get().create_session();
                        Statement select(session);

                        User a;
                        std::string like_fn = first_name;
                        std::string like_ln = last_name;

                        like_fn += std::string("%");
                        like_ln += std::string("%");

                        select << "SELECT id, first_name, last_name, email, title, login, password"
                               << " FROM User where first_name LIKE ? and last_name LIKE ?"
                               << hint,
                            into(a._id), into(a._first_name), into(a._last_name),
                            into(a._email), into(a._title), into(a._login),
                            into(a._password), use(like_fn), use(like_ln),
                            range(0, 1); //  iterate over result set one row at a time

                        while (!select.done())
                        {
                            if (select.execute())
                                users.push_back(a);
                        }
                        return users;
                    });

                futures.emplace_back(std::move(handle));
            }

            // reduce phase
            for (auto &future : futures)
            {
                std::vector<User> users = future.get();
                result.insert(result.end(), users.begin(), users.end());
            }

            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::vector<User> User::search_login(std::string login)
    {
        try
        {

            std::vector<User> result;
            // get all hints for shards
            std::vector<std::string> hints = database::Database::get_all_hints();

            std::vector<std::future<std::vector<User>>> futures;

            for (const std::string &hint : hints)
            {
                auto handle = std::async(
                    std::launch::async,
                    [login, hint]() -> std::vector<User>

                    {
                        std::vector<User> users;
                        Poco::Data::Session session = database::Database::get().create_session();
                        Statement select(session);
                        
                        User a;
                        std::string like_lo = login;
                        like_lo += "%";
                        select << "SELECT id, first_name, last_name, email, title, login, password" 
                        << " FROM User WHERE login LIKE ?"
                        << hint,
                            into(a._id), into(a._first_name), into(a._last_name), into(a._email),
                            into(a._title), into(a._login), into(a._password), use(like_lo),
                            range(0, 1); 

                        while (!select.done())
                        {
                            if (select.execute())
                                users.push_back(a);
                        }
                        return users; });

                futures.emplace_back(std::move(handle));
            }
            for (auto &future : futures)
            {
                std::vector<User> users = future.get();
                result.insert(result.end(), users.begin(), users.end());
            }

            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    void User::save_to_mysql()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();

            Poco::Data::Statement select_last_user_id(session);
            long last_user_id = 0;

            select_last_user_id << "SELECT MAX(last_id) FROM Last_user_id;-- sharding:0",
                into(last_user_id),
                now;

            // std::cout << "save_to_mysql_1" << select_last_user_id.toString() << std::endl;

            Poco::Data::Statement select_last_user_id_node0(session);
            long last_user_id_node0 = 0;

            select_last_user_id_node0 << "SELECT MAX(id) FROM User;-- sharding:0",
                into(last_user_id_node0),
                now;

            if (last_user_id_node0 > last_user_id)
                last_user_id = last_user_id_node0;

            last_user_id = last_user_id + 1;

            // std::cout << "last_user_id " << last_user_id << std::endl;

            Poco::Data::Statement insert(session);

            std::string sharding_user = database::Database::sharding_user(last_user_id);

            insert << "INSERT INTO User (id, first_name,last_name,email,title,login,password) VALUES(?, ?, ?, ?, ?, ?, ?);" + sharding_user,
                use(last_user_id), use(_first_name), use(_last_name), use(_email),
                use(_title), use(_login), use(_password);

            // std::cout << "save_to_mysql_2" << insert.toString() << std::endl;

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID();" + sharding_user, into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;

            Poco::Data::Statement insert_last_user_id(session);

            insert_last_user_id << "INSERT INTO `Last_user_id` (last_id) VALUES (?);-- sharding:0",
                use(last_user_id);

            // std::cout << "save_to_mysql_2" << insert_last_user_id.toString() << std::endl;

            insert_last_user_id.execute();
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    const std::string &User::get_login() const { return _login; }

    const std::string &User::get_password() const { return _password; }

    std::string &User::login() { return _login; }

    std::string &User::password() { return _password; }

    long User::get_id() const { return _id; }

    const std::string &User::get_first_name() const { return _first_name; }

    const std::string &User::get_last_name() const { return _last_name; }

    const std::string &User::get_email() const { return _email; }

    const std::string &User::get_title() const { return _title; }

    long &User::id() { return _id; }

    std::string &User::first_name() { return _first_name; }

    std::string &User::last_name() { return _last_name; }

    std::string &User::email() { return _email; }

    std::string &User::title() { return _title; }
} // namespace database