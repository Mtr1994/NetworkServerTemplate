cmake_minimum_required(VERSION 3.16)

project(rtps)

set(CMAKE_CXX_STANDARD 11)

message(STATUS "start compile project rtps ...")

include_directories ("${PROJECT_SOURCE_DIR}/sdk/hpsocket/include")
include_directories ("${PROJECT_SOURCE_DIR}/sdk/crypto++/include")
include_directories ("${PROJECT_SOURCE_DIR}/sdk/mysqlclient/include/mysql")
include_directories ("${PROJECT_SOURCE_DIR}/sdk/mysqlclient/include")
include_directories ("${PROJECT_SOURCE_DIR}/sdk/spdlog/include")
include_directories ("${PROJECT_SOURCE_DIR}/")
include_directories ("${PROJECT_SOURCE_DIR}/log")
include_directories ("${PROJECT_SOURCE_DIR}/conf")
include_directories ("${PROJECT_SOURCE_DIR}/mysql")
include_directories ("${PROJECT_SOURCE_DIR}/net")
include_directories ("${PROJECT_SOURCE_DIR}/parser")

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/log/*.cpp ${PROJECT_SOURCE_DIR}/conf/*.cpp ${PROJECT_SOURCE_DIR}/mysql/*.cpp ${PROJECT_SOURCE_DIR}/parser/*.cpp ${PROJECT_SOURCE_DIR}/net/*.cpp)

add_executable(rtps ${SOURCES})

target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/sdk/crypto++/lib/libcrypto++.so.6.0.0)
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/sdk/hpsocket/lib/libhpsocket.so)
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/sdk/mysqlclient/lib/libmysqlclient.so.21.2.35)
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/sdk/spdlog/lib/libspdlog.so.1.5.0)

message(STATUS "done.")

message(STATUS "start copy shell files ...")

file(COPY ./script/copy.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./script/init.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./script/startup.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./script/shutdown.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

message(STATUS "done.")
