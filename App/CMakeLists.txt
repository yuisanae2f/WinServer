cmake_minimum_required(VERSION 3.12)

project("App" VERSION 1.0 LANGUAGES CXX)

set(Log_Console ON CACHE BOOL "Log Console")
set(Log_Trial ON CACHE BOOL "Log Trial")
set(Log_Save ON CACHE BOOL "Log Save")
set(Endpoint "localhost" CACHE STRING "Endpoint for your server")
set(Log_Path "log.txt" CACHE STRING "log path for debug. only for Log_Save")
set(Port_Num "5000" CACHE STRING "Port of the server")

string(LENGTH "${Endpoint}" Endpoint_len)
string(LENGTH "${Port_Num}" Port_Num_Len)

# 이 프로젝트의 실행 파일에 소스를 추가합니다.
add_executable (${PROJECT_NAME} "WinServer.cpp" "WinServer.h" "__config.h" "ErrStream.cpp" "jsonMgr.cpp" "multibit.h" "ThreadFile.cpp" "main.cpp" "HeapCounter.cpp")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	target_compile_definitions(${PROJECT_NAME} PUBLIC -D_Debug=1)
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC -D_Debug=0)
endif()

configure_file("${PROJECT_SOURCE_DIR}/__config.h" "${PROJECT_SOURCE_DIR}/config.h")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_options(${PROJECT_NAME} PUBLIC /std:c++20 /W4)

if(MSVC_VERSION GREATER_EQUAL 1922) # Visual Studio 2022 17.2 or later
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "1.0")
  target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
endif()


add_library("${PROJECT_NAME}_lib" "blank.cpp" "ThreadFile.cpp" "main.cpp" "HeapCounter.cpp")
target_include_directories("${PROJECT_NAME}_lib" PUBLIC "/")