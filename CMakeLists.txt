cmake_minimum_required(VERSION 2.8.4)
project(heartbeat_server)

include_directories("${PROJECT_SOURCE_DIR}/extlibs/SFML-2.2/include")

link_directories("${PROJECT_SOURCE_DIR}/extlibs/SFML-2.2/bin/lib")

include_directories("${PROJECT_SOURCE_DIR}/extlibs/chaiscript/include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp GandiXmlRpc.cpp sha256.cpp NetworkManager.cpp)
set(CHAISCRIPT_NO_THREADS)

add_executable(heartbeat_server ${SOURCE_FILES})

target_link_libraries(heartbeat_server sfml-network)
target_link_libraries(heartbeat_server sfml-system)

target_link_libraries(heartbeat_server xmlrpc++)
target_link_libraries(heartbeat_server xmlrpc_packetsocket)
target_link_libraries(heartbeat_server xmlrpc_client++)
target_link_libraries(heartbeat_server xmlrpc_xmlparse)
target_link_libraries(heartbeat_server xmlrpc_xmltok)
target_link_libraries(heartbeat_server xmlrpc)
target_link_libraries(heartbeat_server xmlrpc_util)

target_link_libraries(heartbeat_server dl)

