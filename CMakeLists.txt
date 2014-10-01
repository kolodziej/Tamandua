cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(Tamandua)

include(./tamandua_cfg.cmake)

configure_file(
	"${CMAKE_SOURCE_DIR}/version_config.hpp.cmake"
	"${CMAKE_SOURCE_DIR}/headers/version_config.hpp"
)

include(GNUInstallDirs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGES_RELEASE} -DRELEASE")

find_package(Boost 1.49 COMPONENTS system regex program_options REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(
	"${Boost_INCLUDE_DIR}"
	"${PROJECT_SOURCE_DIR}/headers"
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG lib/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE lib/Release)

set(SOURCES_FILES
	src/group.cpp
	src/participant.cpp
	src/hidden_participant.cpp
	src/message_buffer.cpp
	src/message_composer.cpp
	src/private_room.cpp
	src/room.cpp
	src/root.cpp
	src/server.cpp
	src/user.cpp
	src/client.cpp
	src/command_interpreter.cpp
	src/message_composer.cpp
	src/module_base.cpp
	src/modules/base_user_module.cpp
	src/modules/help_module.cpp
	src/modules/history_module.cpp
	src/modules/registration_module.cpp
	src/modules/stat_module.cpp
	src/conference_room.cpp
	src/utility.cpp
	src/version.cpp
)

set(HEADERS_FILES
	headers/exception.hpp
	headers/group.hpp
	headers/participant.hpp
	headers/hidden_participant.hpp
	headers/logger.hpp
	headers/message_buffer.hpp
	headers/message.hpp
	headers/message_header.hpp
	headers/private_room.hpp
	headers/room.hpp
	headers/root.hpp
	headers/server.hpp
	headers/user.hpp
	headers/client.hpp
	headers/command_interpreter.hpp
	headers/module_base.hpp
	headers/modules/base_user_module.hpp
	headers/modules/help_module.hpp
	headers/modules/history_module.hpp
	headers/modules/registration_module.hpp
	headers/modules/stat_module.hpp
	headers/conference_room.hpp
	headers/message_composer.hpp
	headers/tamandua.hpp
	headers/types.hpp
	headers/server_config.hpp
	headers/utility.hpp
	headers/version.hpp
	headers/version_config.hpp
)

set(MSG_SOURCE_FILES
	src/message_buffer.cpp
	src/message_composer.cpp
	src/version.cpp
)

set(MSG_HEADERS_FILES
	headers/exception.hpp
	headers/message_buffer.hpp
	headers/message.hpp
	headers/message_header.hpp
	headers/message_composer.hpp
	headers/types.hpp
	headers/server_config.hpp
	headers/version.hpp
	headers/version_config.hpp
)

add_library(${LIB_NAME_FULL} STATIC
	${HEADERS_FILES}
	${SOURCES_FILES}
)

add_library(${LIB_MSG_NAME_FULL} STATIC
	${MSG_HEADERS_FILES}
	${MSG_SOURCE_FILES}
)

add_subdirectory(examples examples)

if (UNIX)
	install(TARGETS ${LIB_NAME_FULL} ${LIB_MSG_NAME_FULL}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
	)

	install(DIRECTORY headers/
		DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/tamandua)

	install(CODE "EXECUTE_PROCESS(COMMAND ln -sf ${CMAKE_INSTALL_FULL_LIBDIR}/lib${LIB_NAME_FULL}.a ${CMAKE_INSTALL_FULL_LIBDIR}/lib${LIB_NAME}.a)")
	install(CODE "EXECUTE_PROCESS(COMMAND ln -sf ${CMAKE_INSTALL_FULL_LIBDIR}/lib${LIB_MSG_NAME_FULL}.a ${CMAKE_INSTALL_FULL_LIBDIR}/lib${LIB_MSG_NAME}.a)")
	install(CODE "EXECUTE_PROCESS(COMMAND ln -sf ${CMAKE_INSTALL_FULL_INCLUDEDIR}/tamandua/tamandua.hpp ${CMAKE_INSTALL_FULL_INCLUDEDIR}/tamandua.hpp)")

endif(UNIX)
