cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(ostis-ui VERSION 0.1.0 LANGUAGES C CXX)

message(STATUS "Project: ${CMAKE_PROJECT_NAME} v${CMAKE_PROJECT_VERSION}")
message(STATUS "BUILD_TOKEN: ${BUILD_TOKEN}")

cmake_policy(SET CMP0048 NEW)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Отключаем ccache
set(AUTO_CCACHE OFF CACHE BOOL "" FORCE)
set(ENV{CCACHE_DISABLE} "1")

# Опции
option(SC_BUILD_TESTS "" OFF)
if(${SC_BUILD_TESTS})
    include(CTest)
    find_package(GTest REQUIRED)
endif()

# Поиск зависимостей
find_package(sc-machine REQUIRED)

# 👇 Подключаем сгенерированный заголовок с таймстампом
if(DEFINED GENERATED_INCLUDE_DIR AND EXISTS "${GENERATED_INCLUDE_DIR}/build_timestamp.h")
    include_directories("${GENERATED_INCLUDE_DIR}")
    add_compile_definitions(USE_BUILD_TIMESTAMP=1)
    message(STATUS "✅ Using generated timestamp header: ${GENERATED_INCLUDE_DIR}/build_timestamp.h")
endif()

# Глобальные дефайны
if(DEFINED BUILD_TOKEN)
    add_compile_definitions(BUILD_TOKEN="${BUILD_TOKEN}")
endif()

add_subdirectory(ui-lib)
add_subdirectory(action-interpreter)
add_subdirectory(model-generators)
add_subdirectory(translators)
add_subdirectory(template-agent)