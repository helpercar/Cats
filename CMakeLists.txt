cmake_minimum_required(VERSION 3.10.0)
project(Cats VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.1.0
)
FetchContent_MakeAvailable(SFML)

add_executable(Cats main.cpp
    src/Cat.cpp
    src/Stats.cpp
    src/FileReader.cpp)

target_link_libraries(Cats PRIVATE sfml-graphics sfml-window sfml-system)
target_include_directories(Cats PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(WIN32)
    # Copy Windows .dll files on build
    add_custom_command(TARGET Cats POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE_DIR:Cats>
        COMMENT "Packaging SFML DLLs into the build directory..."
    )
elseif(APPLE)
    # Configure macOS rpath so the executable finds the bundled .dylib files
    set_target_properties(Cats PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
    set_target_properties(Cats PROPERTIES INSTALL_RPATH "@executable_path")

    add_custom_command(TARGET Cats POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE_DIR:Cats>
        COMMENT "Packaging SFML dylibs into the build directory..."
    )

add_custom_command(TARGET Cats POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/data"
        "$<TARGET_FILE_DIR:Cats>/data"
    COMMENT "Copying runtime data folder to build directory..."
)
endif()

install(TARGETS Cats DESTINATION bin)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/" DESTINATION bin/data)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
