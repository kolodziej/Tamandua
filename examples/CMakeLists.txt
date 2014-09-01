find_package(Threads)
find_package(OpenSSL REQUIRED)

link_directories(
	"../lib/Debug"
)

include_directories(
	"${Boost_INCLUDE_DIR}"
	"${OPENSSL_INCLUDE_DIR}"
)

set(SERVER_NAME tamandua_server)
set(CLIENT_NAME tamandua_client)

add_executable(${SERVER_NAME} server_main.cpp)
add_executable(${CLIENT_NAME} client_main.cpp)
target_link_libraries(${SERVER_NAME} "${Boost_LIBRARIES}" "${OPENSSL_LIBRARIES}" "${LIB_NAME_FULL}" "${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(${CLIENT_NAME} "${Boost_LIBRARIES}" "${OPENSSL_LIBRARIES}" "${LIB_NAME_FULL}" "${CMAKE_THREAD_LIBS_INIT}")
