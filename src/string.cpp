#include <jsonlint/string.hpp>

namespace jsonlint {

namespace string {

// std::string operations
size_t Copy(char *dst, const char *src, size_t destination_size) {
  size_t length = strlen(src);
  if (destination_size) {
    size_t bl = (length < destination_size - 1 ? length : destination_size - 1);
    ((char *)memcpy(dst, src, bl))[bl] = 0;
  }
  return length;
}

std::string Format(std::string format_string, ...) {
  int n =
      ((int)format_string.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
  std::unique_ptr<char[]> formatted;
  va_list ap;
  while (1) {
    formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
    Copy(&formatted[0], format_string.c_str(), format_string.length());
    va_start(ap, format_string);
    int final_n = vsnprintf(&formatted[0], n, format_string.c_str(), ap);
    va_end(ap);
    if (final_n < 0 || final_n >= n)
      n += abs(final_n - n + 1);
    else
      break;
  }
  return std::string(formatted.get());
}

// Python-style std::string slicing
std::string Slice(const std::string &input_string, int start_index, int end_index, int step) {
  std::string result;
  if (end_index == INT_MAX) { // if end index is default, i.e., till end of string
    if (start_index < 0) {    // if start index is negative, then start start_index
                              // characters from end of string
      for (int i = static_cast<int>(input_string.size()) + start_index;
           i < static_cast<int>(input_string.size()); i = i + step) {
        result += input_string[i];
      }
    } else { // start_index is not negative; iterate from start_index till end
             // of string
      for (int i = start_index; i < static_cast<int>(input_string.size()); i = i + step) {
        result += input_string[i];
      }
    }
  } else {               // end_index is not std::string::npos
    if (end_index < 0) { // if end_index is negative, start from start_index and
                         // go till (end_of_string + end_index)
      for (int i = start_index; i < static_cast<int>(input_string.size()) + end_index;
           i = i + step) {
        result += input_string[i];
      }
    } else { // if end_index is not negative either, then this is the trivial
             // case
      for (int i = start_index; i < end_index; i = i + step) {
        result += input_string[i];
      }
    }
  }
  return result;
}

// split string based on a delimiter string
// supports multi-character delimiter
// returns a vector of substrings after split
std::vector<std::string> Split(const std::string &input_string, const std::string &delimiter,
                               std::shared_ptr<std::vector<std::string>> result) {
  std::string current_string(input_string);
  size_t delimiter_position =
      current_string.find(delimiter);            // check if delimiter is in input string
  if (delimiter_position != std::string::npos) { // if delimiter position is not end_of_string
    size_t counter_position = 0;
    std::string split_string = current_string.substr(counter_position, delimiter_position);
    delimiter_position += delimiter.size();
    std::string split_remaining = current_string.erase(counter_position, delimiter_position);
    result->push_back(split_string);
    Split(split_remaining, delimiter, result);
  } else { // delimiter not in input string. Just add entire input string to
           // result vector
    result->push_back(current_string);
  }
  return *result;
}

// Check if string startswith some character
bool StartsWith(const std::string &input_string, char c, bool ignore_case) {
  bool result = false;
  if (input_string.size() > 0) {
    char first_character = input_string[0];
    if (ignore_case) {
      first_character = toupper(first_character);
      c = toupper(c);
    }
    if (first_character == c)
      result = true;
  }
  return result;
}

// Check if string startswith some substring
bool StartsWith(const std::string &input_string, const std::string &starter, bool ignore_case) {
  bool result = false;
  if (input_string.size() > 0 && input_string.size() > starter.size()) {
    std::string input_copy = input_string;
    for (size_t i = 0; i < starter.size(); i++) {
      char input_character = input_string[i];
      char starter_character = starter[i];
      if (ignore_case) {
        input_character = toupper(input_character);
        starter_character = toupper(starter_character);
      }
      if (input_character == starter_character) {
        result = true;
        continue;
      } else {
        result = false;
        break;
      }
    }
  }
  return result;
}

// Check if string endswith character
bool EndsWith(const std::string &input_string, char c, bool ignore_case) {
  bool result = false;
  if (input_string.size() > 0) {
    char last_character = input_string[input_string.size() - 1];
    if (ignore_case) {
      last_character = toupper(last_character);
      c = toupper(c);
    }
    if (last_character == c)
      result = true;
  }
  return result;
}

// std::string contains
bool Contains(std::string input, std::string search_string, bool ignore_case) {
  bool result = false;
  if (ignore_case) { // if case is to be ignored, convert both input and search
                     // string to all upper case
    std::transform(input.begin(), input.end(), input.begin(), ::toupper);
    std::transform(search_string.begin(), search_string.end(), search_string.begin(), ::toupper);
  }
  if (input.find(search_string) != std::string::npos) { // search using find algorithm
    result = true;
  }
  return result;
}

// Count number of times some search_string appears in input_string
size_t Count(std::string input_string, std::string search_string, bool ignore_case) {
  size_t result = 0;
  if (Contains(input_string, search_string, ignore_case)) {
    if (ignore_case) {
      std::transform(input_string.begin(), input_string.end(), input_string.begin(), ::toupper);
      std::transform(search_string.begin(), search_string.end(), search_string.begin(), ::toupper);
    }
    auto split_vector = Split(input_string, search_string);
    result = split_vector.size() - 1;
  }
  return result;
}

// repeat input string for count number of times, optionally with a connector
// string
std::string Repeat(const std::string &input_string, int count_value, const std::string &connector) {
  std::string result = "";
  while (count_value > 0) {
    result += input_string + connector;
    count_value--;
  }
  return result;
}

// returns copy of input string that is all upper case
std::string Upper(const std::string &input_string) {
  std::string result;
  result.resize(input_string.size());
  std::transform(input_string.begin(), input_string.end(), result.begin(), ::toupper);
  return result;
}

// returns copy of input string that is all lower case
std::string Lower(const std::string &input_string) {
  std::string result;
  result.resize(input_string.size());
  std::transform(input_string.begin(), input_string.end(), result.begin(), ::tolower);
  return result;
}

// find and replace substring in input string, optionally for a limited number
// of times by default, every occurrence of find_string is replaced by
// replace_string
std::string Replace(const std::string &input_string, const std::string &find_string,
                    const std::string &replace_string, int replace_count) {
  std::string result = "";
  auto split_vector = Split(input_string, find_string);
  auto split_size = split_vector.size();
  auto max_replace_count = split_size - 1;
  if (replace_count == -1) {
    for (auto &s : split_vector) {
      if (max_replace_count > 0)
        result += s + replace_string;
      else
        result += s;
      max_replace_count--;
    }
  } else {
    for (auto &s : split_vector) {
      if (replace_count > 0) {
        if (max_replace_count > 0)
          result += s + replace_string;
        else
          result += s;
        max_replace_count--;
      } else {
        if (max_replace_count > 0)
          result += s + find_string;
        else
          result += s;
        max_replace_count--;
      }
      replace_count--;
    }
  }
  return result;
}

// join a vector of strings into a single string
std::string Join(const std::vector<std::string> &input, const std::string &connector) {
  std::string result;
  size_t max_connector_count = input.size() - 1;
  for (auto &s : input) {
    if (max_connector_count > 0)
      result += s + connector;
    else
      result += s;
    max_connector_count--;
  }
  return result;
}

// bulk find and replace sub-string in input string using translation table
std::string Translate(const std::string &input_string,
                      const std::map<std::string, std::string> &translation_table) {
  std::string result = input_string;
  for (auto &pair : translation_table)
    result = Replace(result, pair.first, pair.second);
  return result;
}

// trim white spaces from the left end of an input string
std::string TrimLeft(const std::string &input_string) {
  std::string result = input_string;
  result.erase(result.begin(), std::find_if(result.begin(), result.end(),
                                            [](int ch) { return !std::isspace(ch); }));
  return result;
}

// trim white spaces from right end of an input string
std::string TrimRight(const std::string &input_string) {
  std::string result = input_string;
  result.erase(
      std::find_if(result.rbegin(), result.rend(), [](int ch) { return !std::isspace(ch); }).base(),
      result.end());
  return result;
}

// trim white spaces from either end of an input string
std::string Trim(const std::string &input_string) { return TrimLeft(TrimRight(input_string)); }

// Returns true if the two input strings are equal
bool Equal(const std::string &lhs, const std::string &rhs) { return (lhs.compare(rhs) == 0); }

// generic find function
// if sub-string is found, returns the index
// if sub-string is not found, returns std::string::npos
size_t Find(const std::string &input_string, const std::string &search_string) {
  return input_string.find(search_string);
}

// finds first occurrence of sub-string in input string
size_t FindFirst(const std::string &input_string, const std::string &search_string) {
  return input_string.find_first_of(search_string);
}

// finds last occurrence of sub-string in input string
size_t FindLast(const std::string &input_string, const std::string &search_string) {
  return input_string.find_last_of(search_string);
}

// performs std regex search and returns a vector of matched results
std::vector<std::string> FindRegex(const std::string &input_string,
                                   const std::string &regex_string) {
  std::string current_string(input_string);
  std::vector<std::string> results;
  std::smatch match_result;
  while (std::regex_search(current_string, match_result, std::regex(regex_string))) {
    for (auto &m : match_result)
      results.push_back(m);
    current_string = match_result.suffix();
  }
  return results;
}

} // namespace string

} // namespace jsonlint
