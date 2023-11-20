#ifndef TemperatureHANDLER_H
#define TemperatureHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>
#include<sstream>

#include "../../database/temperature.h"
#include "../../helper.h"

static bool hasSubstrT(const std::string &str, const std::string &substr)
{
    if (str.size() < substr.size())
        return false;
    for (size_t i = 0; i <= str.size() - substr.size(); ++i)
    {
        bool ok{true};
        for (size_t j = 0; ok && (j < substr.size()); ++j)
            ok = (str[i + j] == substr[j]);
        if (ok)
            return true;
    }
    return false;
}


class TemperatureHandler : public HTTPRequestHandler
{
private:
    bool check_user_id(const long &user_id, std::string &reason)
    {
       

        if (user_id > 10)
        {
            reason = "Id can't this user";
            return false;
        }       

        return true;
    };

    bool check_userTemperature(std::vector<std::string> &results, std::string &reason)
    {
       reason = results[0];
       return true;
    }

    bool check_datetime(const std::string &datetime, std::string &reason)
    {       
        
        if (datetime.find(' ') != std::string::npos)
        {

            std::string str1 = "DateTime can't contain spaces ";            
            reason =  str1 + datetime;
            return false;
        }
        

        return true;
    };

public:
    TemperatureHandler(const std::string &format) : _format(format)
    {
    }

    Poco::JSON::Object::Ptr remove_password(Poco::JSON::Object::Ptr src)
    {
        if (src->has("password"))
            src->set("password", "*******");
        return src;
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());
        try
        {
            /*
            if (form.has("id") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                long id = atol(form.get("id").c_str());

                std::optional<database::User> result = database::User::read_by_id(id);
                if (result)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(remove_password(result->toJSON()), ostr);
                    return;
                }
                else
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_found");
                    root->set("title", "Internal exception");
                    root->set("status", "404");
                    root->set("detail", "user ot found");
                    root->set("instance", "/user");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }
            }*/ /*
            else if (hasSubstrV(request.getURI(), "/auth"))
            {

                std::string scheme;
                std::string info;
                request.getCredentials(scheme, info);
                std::cout << "scheme: " << scheme << " identity: " << info << std::endl;

                std::string login, password;
                if (scheme == "Basic")
                {
                    get_identity(info, login, password);
                    if (auto id = database::User::auth(login, password))
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << "{ \"id\" : \"" << *id << "\"}" << std::endl;
                        return;
                    }
                }

                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("type", "/errors/unauthorized");
                root->set("title", "Internal exception");
                root->set("status", "401");
                root->set("detail", "not authorized");
                root->set("instance", "/auth");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }*/
            /*
            // Выполнено. Поиск пользователя по маске имени и фамилии.
            else if (hasSubstrV(request.getURI(), "/fixedTemperature") && form.has("first_name") && form.has("last_name"))
            {

                std::string fn = form.get("first_name");
                std::string ln = form.get("last_name");
                auto results = database::User::search(fn, ln);
                Poco::JSON::Array arr;
                for (auto s : results)
                    arr.add(remove_password(s.toJSON()));
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(arr, ostr);

                return;
            }
            */
           
             // Массив поиска по датам
            if (hasSubstrT(request.getURI(), "/get_temperatures") && form.has("start_datetime") && form.has("end_datetime"))
            {             
                database::Temperature Temperature;
                std::vector<database::Temperature> results = Temperature.get_temperatures(form.get("start_datetime").c_str(), form.get("end_datetime").c_str());

                Poco::JSON::Array arr;
                for (auto t : results)
                    arr.add(t.toJSON());
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(arr, ostr);

                return;               
              
            }
            
            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) 
            {
                if (form.has("user_id") && form.has("value"))
                {
                    database::Temperature t;
                 
                    t.user_id() =  atol(form.get("user_id").c_str());   
                    t.value() = std::stof(form.get("value").c_str());   
                    time_t now = time(0);
    
                    // В таком формате мы получаем дату. Sat Nov 18 21:34:02 2023
                    std::string temp_value = ctime(&now);

                    // Создаем объект структуры tm для хранения времени
                    std::tm date = {};

                    // Парсим дату из строки в структуру tm
                    std::istringstream ss(temp_value);
                    ss >> std::get_time(&date, "%a %b %d %H:%M:%S %Y");
                    
                    if (ss.fail()) {
                        std::cout << "Ошибка при парсинге даты" << std::endl;                        
                    }
                    
                    // Преобразуем дату в нужный формат
                    std::ostringstream oss;
                    oss << std::put_time(&date, "%Y-%m-%d %H:%M:%S");
                    
                    std::string output_date = oss.str();
                                        
                    t.datetime() = output_date;

                    bool check_result = true;
                    std::string message;
                    std::string reason;

                   /*

                    if (!check_user_id(Temperature.get_user_id(), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }

                    if (!check_datetime(Temperature.datetime(), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }
                    */
                    /*

                    if (!check_email(user.get_email(), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }
                    */

                    if (check_result)
                    {
                        t.save_to_mysql();
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << t.get_id();
                        return;
                        
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        ostr << message;
                        response.send();
                        return;
                    }
                }
            }
        }
        catch (...)
        {
                std::string message;
                std::string reason;
                
                if (1 == 1)
                    {                        
                        message += "не стаботало!";
                        message += "<br>";
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        ostr << message;
                        response.send();
                        return;
                    }
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "request ot found");
        root->set("instance", "/Temperature");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
};
#endif