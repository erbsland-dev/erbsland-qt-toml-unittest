cmake_minimum_required(VERSION 3.25)

project(unittest-project)

add_executable(unittest)

target_include_directories(unittest PRIVATE ../erbsland-qt-toml/src)
target_link_libraries(unittest PRIVATE erbsland-qt-toml)
set_property(TARGET unittest PROPERTY CXX_STANDARD 17)
target_compile_features(unittest PRIVATE cxx_std_17)

add_subdirectory(src)

erbsland_unittest(
        TARGET unittest
        COPY_TEST_DATA data)

