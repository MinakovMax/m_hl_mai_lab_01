
#include <optional>
#include "temperature_server/http_temperature_other_server.h"

int main(int argc, char*argv[]) 
{
    HTTPTemperatureOtherWebServer app;
    return app.run(argc, argv);
}