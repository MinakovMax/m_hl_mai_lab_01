#include "temperature.h"
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

    void Temperature::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            // CREATE TABLE IF NOT EXISTS Temperature (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,user_id INT,datetime DATETIME);
            create_stmt << "CREATE TABLE IF NOT EXISTS Temperature (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                        << "user_id INT,value DECIMAL(6,2),datetime DATETIME);",
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
    
    Poco::JSON::Object::Ptr Temperature::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("user_id", _user_id);
        root->set("value", _value);   
        root->set("datetime", _datetime);  
           

        return root;
    }

    Temperature Temperature::fromJSON(const std::string &str)
    {
        Temperature Temperature;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        Temperature.id() = object->getValue<long>("id");
        Temperature.user_id() = object->getValue<long>("user_id");
        Temperature.value() = object->getValue<float>("value");        
        Temperature.datetime() = object->getValue<std::string>("datetime");

        return Temperature;
    }
    /*
    std::optional<long> Temperature::auth(std::string &login, std::string &password)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            long id;
            select << "SELECT id FROM Temperature where login=? and password=?",
                into(id),
                use(login),
                use(password),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return id;
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
    */
    
    std::vector<Temperature> Temperature::get_temperatures(std::string start_date, std::string end_date)
    {
        std::cout << "___1" << std::endl;
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Temperature> result;
            
                 

            std::cout << "___2 " << session.isConnected() << std::endl;
            
            select << "SELECT id, user_id, value, datetime FROM Temperature WHERE datetime >= ? AND datetime <= ?;",
            use(start_date), 
            use(end_date)
            ;   

            std::cout << "___3 " << select.subTotalRowCount() << std::endl; 
            //std::cout << "___4 " << select.execute() << std::endl;
            
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
                Temperature t;
                for (std::size_t col = 0; col < cols; ++col)
                {
                    if(col == 0) t.id() = rs[col].convert<long>();
                    if(col == 1) t.user_id() = rs[col].convert<long>();
                    if(col == 2) t.value() = rs[col].convert<float>();
                    if(col == 3) t.datetime() = rs[col].convert<std::string>();
                }
                std::cout << std::endl;
                more = rs.moveNext();
                result.push_back(t);
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
/*
    std::vector<Temperature> Temperature::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Temperature> result;
            Temperature a;
            select << "SELECT id, first_name, last_name, email, title, login, password FROM Temperature",
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._title),
                into(a._login),
                into(a._password),
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
*/

/*
    std::vector<Temperature> Temperature::search(std::string first_name, std::string last_name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Temperature> result;
            Temperature a;
            first_name += "%";
            last_name += "%";
            select << "SELECT id, first_name, last_name, email, title, login, password FROM Temperature where first_name LIKE ? and last_name LIKE ?",
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._title),
                into(a._login),
                into(a._password),
                use(first_name),
                use(last_name),
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
*//*
    std::vector<Temperature> Temperature::search_login(std::string login)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Temperature> result;
            Temperature a;
            login += "%";            
            select << "SELECT id, first_name, last_name, email, title, login, password FROM Temperature where login LIKE ?",
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._title),
                into(a._login),
                into(a._password),
                use(login),                
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
*/
    void Temperature::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Temperature (user_id,datetime,value) VALUES(?, ?, ?)",
                use(_user_id),
                use(_datetime),
                use(_value);

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

    long Temperature::get_id() const
    {
        return _id;
    }

    long Temperature::get_user_id() const
    {
        return _user_id;
    }

    float Temperature::get_value() const
    {
        return _value;
    }

    long &Temperature::id()
    {
        return _id;
    }

    long &Temperature::user_id()
    {
        return _user_id;
    }

    float &Temperature::value()
    {
        return _value;
    }

    const std::string &Temperature::get_datetime() const
    {
        return _datetime;
    }

    std::string &Temperature::datetime()
    {
        return _datetime;
    }
}