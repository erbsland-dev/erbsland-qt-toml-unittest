
cmake_minimum_required(VERSION 3.25)

target_sources(unittest PRIVATE
        main.cpp
        CharTest.hpp
        ErrorTest.hpp
        InputStreamTest.hpp
        LocationTest.hpp
        TokenizerTest.hpp
        ParserTest.hpp
        ValueTest.hpp)

