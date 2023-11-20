#include "web_server/http_web_server.h"
#include "database/visit.h"
#include "../web_server/handlers/visit_handler.h"



int main(int argc, char*argv[]) 
{
    HTTPWebServer app;
    return app.run(argc, argv);
}