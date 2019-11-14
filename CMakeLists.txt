cmake_minimum_required(VERSION 3.6)
project(JSONLINT)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Disable deprecation for windows
if (WIN32)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# LEXER library
file(GLOB LEXER_SOURCES
  "src/lexer.cpp"
  )
ADD_LIBRARY(LEXER STATIC ${LEXER_SOURCES})
INCLUDE_DIRECTORIES("include")
set_target_properties(LEXER PROPERTIES OUTPUT_NAME lexer)
TARGET_LINK_LIBRARIES(LEXER)
set_property(TARGET LEXER PROPERTY CXX_STANDARD 11)

# make check - runs unit tests
if (UNIX)
  add_custom_target(check COMMAND ./tests)
endif(UNIX)
