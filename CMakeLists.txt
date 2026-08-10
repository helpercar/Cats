cmake_minimum_required(VERSION 3.10.0)
project(Cats VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Cats main.cpp
    src/Cat.cpp
    src/Stats.cpp
    src/FileReader.cpp)

target_include_directories(Cats PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_custom_command(TARGET Cats POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/data"
        "$<TARGET_FILE_DIR:Cats>/data"
    COMMENT "Copying runtime data folder to build directory..."
)

install(TARGETS Cats DESTINATION bin)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/" DESTINATION bin/data)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
