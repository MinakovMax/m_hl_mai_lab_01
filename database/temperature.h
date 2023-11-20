#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Temperature{
        private:
            long _id;
            long _user_id;
            float _value;
            std::string _datetime;
            

        public:

            static Temperature fromJSON(const std::string & str);

            long get_id() const;
            long get_user_id() const;
            float get_value() const;
            const std::string &get_datetime() const;

            long &id();
            long &user_id();
            float &value();
            std::string &datetime();

            static void init();
            std::vector<database::Temperature> get_temperatures(std::string start_date, std::string end_date);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif