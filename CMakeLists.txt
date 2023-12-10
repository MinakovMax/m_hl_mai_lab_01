cmake_minimum_required(VERSION 3.2)

project(hl_server C CXX)

SET (EXAMPLE_BINARY "m_hl_mai_lab_01")
SET (VISIT_SERVICE "visit_service")
SET (TEMPERATURE_SERVICE "temperature_service")

SET (STD_CXX "c++17")
SET (REDISCPP_FLAGS "-DREDISCPP_HEADER_ONLY=ON")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=${STD_CXX} ${REDISCPP_FLAGS}")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -std=${STD_CXX} -Wall -DNDEBUG ${REDISCPP_FLAGS}")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)


if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${EXAMPLE_BINARY} main.cpp 
                                 config/config.cpp
                                 redis-cpp/src/redis-cpp/stream.cpp 
                                 database/cache.cpp                                 
                                 database/database.cpp
                                 database/user.cpp
                                 database/visit.cpp)

add_executable(load_data load_data.cpp)

target_include_directories(${EXAMPLE_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${EXAMPLE_BINARY} PRIVATE -Wall -Wextra -pedantic -Werror )


target_link_libraries(${EXAMPLE_BINARY} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)

target_link_libraries(load_data PRIVATE 
                          ${CMAKE_THREAD_LIBS_INIT} 
                          ${Poco_LIBRARIES}
                          "PocoData"
                          "PocoDataMySQL"                      
                          ZLIB::ZLIB)

set_target_properties(${EXAMPLE_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${EXAMPLE_BINARY} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

add_executable(${VISIT_SERVICE} visit_main.cpp 
                                config/config.cpp
                                redis-cpp/src/redis-cpp/stream.cpp
                                database/database.cpp
                                database/cache.cpp 
                                database/user.cpp                                
                                database/visit.cpp)

target_include_directories(${VISIT_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${VISIT_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )


target_link_libraries(${VISIT_SERVICE} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)

set_target_properties(${VISIT_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${VISIT_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
                             
add_executable(${TEMPERATURE_SERVICE} 
                             temperature_main.cpp 
                             config/config.cpp 
                             redis-cpp/src/redis-cpp/stream.cpp
                             database/cache.cpp
                             database/database.cpp 
                             database/user.cpp 
                             database/temperature.cpp)

target_include_directories(${TEMPERATURE_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${TEMPERATURE_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )


target_link_libraries(${TEMPERATURE_SERVICE} PRIVATE 
                          ${CMAKE_THREAD_LIBS_INIT} 
                          ${Poco_LIBRARIES}
                          "PocoData"
                          "PocoDataMySQL"
                          "mysqlclient"
                          ZLIB::ZLIB)


set_target_properties(${TEMPERATURE_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${TEMPERATURE_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
                          




