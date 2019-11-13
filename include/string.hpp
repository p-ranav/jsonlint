#pragma once
#include <algorithm>
#include <cctype>
#include <climits>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

namespace tsparse {

namespace string {

size_t Copy(char *dst, const char *src, size_t destination_size);

std::string Format(std::string format_string, ...);
std::string Slice(const std::string &input_string, int start_index, int end_index = INT_MAX,
                  int step = 1);

// split std::string based on a delimiter string
// supports multi-character delimiter
// returns a vector of substrings after split
std::vector<std::string> Split(const std::string &input_string, const std::string &delimiter,
                               std::shared_ptr<std::vector<std::string>> result =
                                   std::make_shared<std::vector<std::string>>());

// join a vector of strings into a single string
std::string Join(const std::vector<std::string> &input, const std::string &connector = "");

// Check if std::string startswith some character
bool StartsWith(const std::string &input_string, char c, bool ignore_case = false);

// Check if std::string startswith some substring
bool StartsWith(const std::string &input_string, const std::string &starter,
                bool ignore_case = false);

// Check if std::string endswith character
bool EndsWith(const std::string &input_string, char c, bool ignore_case = false);

// std::string contains
bool Contains(std::string input, std::string search_string, bool ignore_case = false);

// Count number of times some search_std::string appears in input_string
size_t Count(std::string input_string, std::string search_string, bool ignore_case = false);

// repeat input std::string for count number of times, optionally with a
// connector string
std::string Repeat(const std::string &input_string, int count_value,
                   const std::string &connector = "");

// returns copy of input std::string that is all upper case
std::string Upper(const std::string &input_string);

// returns copy of input std::string that is all lower case
std::string Lower(const std::string &input_string);

// find and replace substd::string in input string, optionally for a limited
// number of times by default, every occurrence of find_std::string is replaced
// by replace_string
std::string Replace(const std::string &input_string, const std::string &find_string,
                    const std::string &replace_string, int replace_count = -1);

// bulk find and replace sub-std::string in input std::string using translation
// table
std::string Translate(const std::string &input_string,
                      const std::map<std::string, std::string> &translation_table);

// trim white spaces from the left end of an input string
std::string TrimLeft(const std::string &input_string);

// trim white spaces from right end of an input string
std::string TrimRight(const std::string &input_string);

// trim white spaces from either end of an input string
std::string Trim(const std::string &input_string);

// Returns true if the two input strings are equal
bool Equal(const std::string &lhs, const std::string &rhs);

// generic find function
// if sub-std::string is found, returns the index
// if sub-std::string is not found, returns std::string::npos
size_t Find(const std::string &input_string, const std::string &search_string);

// finds first occurrence of sub-std::string in input string
size_t FindFirst(const std::string &input_string, const std::string &search_string);

// finds last occurrence of sub-std::string in input string
size_t FindLast(const std::string &input_string, const std::string &search_string);

// performs std regex search and returns a vector of matched results
std::vector<std::string> FindRegex(const std::string &input_string,
                                   const std::string &regex_string);

} // namespace string

} // namespace tsparse