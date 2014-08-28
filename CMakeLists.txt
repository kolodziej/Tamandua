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

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.49 COMPONENTS system REQUIRED)

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
	src/user_message_interpreter.cpp
	src/message_composer.cpp
)

set(HEADERS_FILES
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
	headers/message_interpreter.hpp
	headers/user_message_interpreter.hpp
	headers/message_composer.hpp
	headers/tamandua.hpp
	headers/types.hpp
	headers/utility.hpp
	headers/version.hpp
)

add_library(${LIB_NAME_FULL} STATIC
	${HEADERS_FILES}
	${SOURCES_FILES}
)

add_subdirectory(examples examples)

if (UNIX)
	install(TARGETS ${LIB_NAME_FULL}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
	)

	install(CODE "EXECUTE_PROCESS(COMMAND ln -sf ${CMAKE_INSTALL_FULL_LIBDIR}/lib${LIB_NAME_FULL}.a ${CMAKE_INSTALL_FULL_LIBDIR}/lib${LIB_NAME}.a)")

	install(FILES
		${HEADERS_FILES}
		DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/tamandua)

endif(UNIX)