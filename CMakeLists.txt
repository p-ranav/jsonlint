cmake_minimum_required(VERSION 3.6)
project(JSONLINT)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Disable deprecation for windows
if (WIN32)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# UTILS library
file(GLOB UTILS_SOURCES
  "src/string.cpp"
  "src/errors.cpp"
  )
ADD_LIBRARY(UTILS STATIC ${UTILS_SOURCES})
INCLUDE_DIRECTORIES("include")
set_target_properties(UTILS PROPERTIES OUTPUT_NAME utils)
TARGET_LINK_LIBRARIES(UTILS)
set_property(TARGET UTILS PROPERTY CXX_STANDARD 17)

# LEXER library
file(GLOB LEXER_SOURCES
  "src/lexer.cpp"
  )
ADD_LIBRARY(LEXER STATIC ${LEXER_SOURCES})
INCLUDE_DIRECTORIES("include")
set_target_properties(LEXER PROPERTIES OUTPUT_NAME lexer)
TARGET_LINK_LIBRARIES(LEXER UTILS)
set_property(TARGET LEXER PROPERTY CXX_STANDARD 17)

# PARSER library
file(GLOB PARSER_SOURCES 
  "src/parser.cpp"
  )
ADD_LIBRARY(PARSER STATIC ${PARSER_SOURCES})
INCLUDE_DIRECTORIES("include" "ext")
set_target_properties(PARSER PROPERTIES OUTPUT_NAME parser)
TARGET_LINK_LIBRARIES(PARSER LEXER) 
set_property(TARGET PARSER PROPERTY CXX_STANDARD 17)

# JSONLINT CLI
file(GLOB JSONLINT_SOURCES 
  "src/main.cpp"
  )
ADD_EXECUTABLE(JSONLINT ${JSONLINT_SOURCES})
INCLUDE_DIRECTORIES("include" "ext")
set_target_properties(JSONLINT PROPERTIES OUTPUT_NAME jsonlint)
TARGET_LINK_LIBRARIES(JSONLINT PARSER) 
set_property(TARGET JSONLINT PROPERTY CXX_STANDARD 17)

# Tests
file(GLOB TEST_SOURCES  
  "test/*.cpp" 
  "test/lexer/*.cpp"
  "test/parser/*.cpp"
)
ADD_EXECUTABLE(TESTS ${TEST_SOURCES})
INCLUDE_DIRECTORIES("ext")
set_target_properties(TESTS PROPERTIES OUTPUT_NAME tests)
TARGET_LINK_LIBRARIES(TESTS PARSER)
set_property(TARGET TESTS PROPERTY CXX_STANDARD 17)

# make check - runs unit tests
if (UNIX)
  add_custom_target(check COMMAND ./tests)
endif(UNIX)
