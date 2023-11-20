#ifndef VISIT_H
#define VISIT_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Visit{
        private:
            long _id;
            long _user_id;
            std::string _datetime;

        public:

            static Visit fromJSON(const std::string & str);

            long get_id() const;
            long get_user_id() const;
            const std::string &get_datetime() const;

            long &id();
            long &user_id();
            std::string &datetime();

            static void init();
            std::vector<database::Visit> get_visits();
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif