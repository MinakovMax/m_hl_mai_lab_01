#include "visit.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Visit::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            // CREATE TABLE IF NOT EXISTS Visit (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,user_id INT,datetime DATETIME);
            create_stmt << "CREATE TABLE IF NOT EXISTS Visit (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                        << "user_id INT,datetime DATETIME);",
                now;
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

    Poco::JSON::Object::Ptr Visit::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("user_id", _user_id);
        root->set("datetime", _datetime);

        return root;
    }

    Visit Visit::fromJSON(const std::string &str)
    {
        Visit Visit;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        Visit.id() = object->getValue<long>("id");
        Visit.user_id() = object->getValue<long>("user_id");
        Visit.datetime() = object->getValue<std::string>("datetime");

        return Visit;
    }

    std::vector<Visit> Visit::get_visits()
    {
        std::cout << "___1" << std::endl;
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Visit> result;

            std::cout << "___2 " << session.isConnected() << std::endl;

            select << "SELECT id, user_id, datetime FROM Visit;";

            std::cout << "___3 " << select.subTotalRowCount() << std::endl;
            // std::cout << "___4 " << select.execute() << std::endl;

            // create a RecordSet
            select.execute();
            Poco::Data::RecordSet rs(select);
            std::size_t cols = rs.columnCount();
            // print all column names
            for (std::size_t col = 0; col < cols; ++col)
            {
                std::cout << rs.columnName(col) << std::endl;
            }
            // iterate over all rows and columns
            bool more = rs.moveFirst();
            while (more)
            {
                Visit v;
                for (std::size_t col = 0; col < cols; ++col)
                {
                    if (col == 0)
                        v.id() = rs[col].convert<long>();
                    if (col == 1)
                        v.user_id() = rs[col].convert<long>();
                    if (col == 2)
                        v.datetime() = rs[col].convert<std::string>();
                }
                std::cout << std::endl;
                more = rs.moveNext();
                result.push_back(v);
            }

            return result;
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

    void Visit::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Visit (user_id,datetime) VALUES(?, ?)",
                use(_user_id),
                use(_datetime),

                insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
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

    long Visit::get_id() const
    {
        return _id;
    }

    long Visit::get_user_id() const
    {
        return _user_id;
    }

    long &Visit::id()
    {
        return _id;
    }

    long &Visit::user_id()
    {
        return _user_id;
    }

    const std::string &Visit::get_datetime() const
    {
        return _datetime;
    }

    std::string &Visit::datetime()
    {
        return _datetime;
    }
}