// Copyright 2020 Evgenij Grigorev evgengrmit@icloud.com

#include <algorithm>

#include "parser.hpp"

Parser::Parser() = default;

Parser::Parser(const std::vector<Student> &s) : students(s) {
  setLengths();
}
Parser::Parser(const std::string &path) {
  if (path.empty()) {
    throw std::invalid_argument("The file path cannot be empty!!!");
  }
  std::ifstream json_file(path);
  if (!json_file.is_open()) {
    throw std::out_of_range("The file with the specified name: " + path +
                            " does not exist!!!");
  }
  json data;
  json_file >> data;
  if (!data.at("items").is_array()) {
    throw std::invalid_argument("Items is not array!!!");
  }
  if (data.at("items").size() != data.at("_meta").at("count").get<size_t>()) {
    throw std::invalid_argument("Items length don't equal _meta.count!!!");
  }
  for (auto const &student : data.at("items")) {
    students.emplace_back(student);
  }
  setLengths();
}
const std::vector<Student> &Parser::getStudents() const { return students; }
const Lengths_of_fields &Parser::getL() const { return l; }

void Parser::printRow(const Student &student) const {
  std::cout << std::left << "|" << std::setw(l.length_1_field)
            << student.getName() << "|";
  if (std::any_cast<json>(student.getGroup()).is_number()) {
    std::cout << std::setw(l.length_2_field)
              << std::any_cast<json>(student.getGroup()).get<int>();
  } else if (std::any_cast<json>(student.getGroup()).is_string()) {
    std::cout << std::setw(l.length_2_field)
              << std::any_cast<json>(student.getGroup()).get<std::string>();
  } else {
    throw std::invalid_argument(
        "The type of the Group variable is undefined!!!");
  }
  std::cout << "|" << std::setprecision(3) << std::setw(l.length_3_field)
            << student.getAvg() << "|";
  if (std::any_cast<json>(student.getDebt()).is_null()) {
    std::cout << std::setw(l.length_4_field) << "null";
  } else if (std::any_cast<json>(student.getDebt()).is_array()) {
    std::string it = std::to_string(std::any_cast<json>(student.getDebt())
                                        .get<std::vector<std::string>>()
                                        .size()) +
                     " items";
    std::cout << std::setw(l.length_4_field) << it;
  } else if (std::any_cast<json>(student.getDebt()).is_string()) {
    std::cout << std::setw(l.length_4_field)
              << std::any_cast<json>(student.getDebt()).get<std::string>();
  } else {
    throw std::invalid_argument(
        "The type of the Debt variable is undefined!!!");
  }
  std::cout << "|";
}
std::string Parser::getSeparator() const {
  std::string sep = "|";
  for (size_t i = 0; i < l.length_1_field; ++i) {
    sep += "-";
  }
  sep += "|";
  for (size_t i = 0; i < l.length_2_field; ++i) {
    sep += "-";
  }
  sep += "|";
  for (size_t i = 0; i < l.length_3_field; ++i) {
    sep += "-";
  }
  sep += "|";
  for (size_t i = 0; i < l.length_4_field; ++i) {
    sep += "-";
  }
  sep += "|";
  return sep;
}
void Parser::parser(const std::string &path) {
  if (path.empty()) {
    throw std::invalid_argument("The file path cannot be empty!!!");
  }
  std::ifstream json_file(path);
  if (!json_file.is_open()) {
    throw std::out_of_range("The file with the specified name: " + path +
                            " does not exist!!!");
  }
  json data;
  json_file >> data;
  if (!data.at("items").is_array()) {
    throw std::invalid_argument("Items is not array!!!");
  }
  if (data.at("items").size() != data.at("_meta").at("count").get<size_t>()) {
    throw std::invalid_argument("Items length don't equal _meta.count!!!");
  }
  for (auto const &student : data.at("items")) {
    students.emplace_back(student);
  }
  setLengths();
  std::cout << std::left << "|" << std::setw(l.length_1_field) << "name"
            << "|" << std::setw(l.length_2_field) << "group"
            << "|" << std::setw(l.length_3_field) << "avg"
            << "|" << std::setw(l.length_4_field) << "debt"
            << "|" << '\n';
  std::string separator = getSeparator();
  std::cout << separator << "\n";
  for (const auto &student : students) {
    printRow(student);
    std::cout << '\n';
    std::cout << separator << "\n";
  }
}
void Parser::sortByName() {
  std::sort(students.begin(), students.end(),
            [](const Student &a, const Student &b) {
              return a.getName() < b.getName();
            });
}
void Parser::sortByAverageScore() {
  std::sort(students.begin(), students.end(),
            [](const Student &a, const Student &b) {
              return a.getAvg() < b.getAvg();
            });
}
bool Parser::emptyJSONobject() const { return students.empty(); }
void Parser::setJSONstring(const std::string &JSON) {
  std::stringstream json_string(JSON);
  json data;
  json_string >> data;
  if (!data.at("items").is_array()) {
    throw std::invalid_argument("Items is not array!!!");
  }
  if (data.at("items").size() != data.at("_meta").at("count").get<size_t>()) {
    throw std::invalid_argument("Items length don't equal _meta.count!!!");
  }
  for (auto const &student : data.at("items")) {
    students.emplace_back(student);
  }
}
void Parser::setLengths() {
  for (const auto &student : students) {
    if (student.getName().size() > l.length_1_field) {
      l.length_1_field = student.getName().size() + 1;
    }
    if (std::any_cast<json>(student.getGroup()).is_number()) {
      if (std::to_string(std::any_cast<json>(student.getGroup()).get<int>())
              .size() > l.length_2_field) {
        l.length_2_field =
            std::to_string(std::any_cast<json>(student.getGroup()).get<int>())
                .size() +
            1;
      }
    } else {
      if (std::any_cast<json>(student.getGroup()).get<std::string>().size() >
          l.length_2_field) {
        l.length_2_field =
            std::any_cast<json>(student.getGroup()).get<std::string>().size() +
            1;
      }
    }
    if (std::any_cast<json>(student.getDebt()).is_string() &&
        std::any_cast<json>(student.getDebt()).get<std::string>().size() >
            l.length_4_field) {
      l.length_4_field =
          std::any_cast<json>(student.getDebt()).get<std::string>().size() + 1;
    }
  }
}
Parser::~Parser() {}
