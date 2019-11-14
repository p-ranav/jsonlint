#pragma once
#include <jsonlint/lexer.hpp>

namespace jsonlint {

enum class Element {
  STRING,
  NUMBER,
  TRUE,
  FALSE,
  NULL_,
  ARRAY,
  OBJECT
};

  
}
