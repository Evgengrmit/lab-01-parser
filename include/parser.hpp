// Copyright 2020 Evgenij Grigorev evgengrmit@icloud.com

#ifndef INCLUDE_PARSER_HPP_
#define INCLUDE_PARSER_HPP_

#include "student.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct Lengths_of_fields {
  size_t length_1_field;
  size_t length_2_field;
  size_t length_3_field;
  size_t length_4_field;
};
class Parser {
 public:
  Parser();
  explicit Parser(const std::string &path);
  ~Parser();
  [[nodiscard]] const std::vector<Student> &getStudents() const;
  [[nodiscard]] const Lengths_of_fields &getL() const;
  void printRow(const Student &student) const;
  [[nodiscard]] std::string getSeparator() const;
  void parser(const std::string &path);
  void printData();
  [[nodiscard]] bool emptyJSONobject() const;
  void setJSONString(const std::string &JSON);

 private:
  std::vector<Student> students;
  Lengths_of_fields l{15, 8, 6, 15};
  void setLengths();
};

#endif  // INCLUDE_PARSER_HPP_
