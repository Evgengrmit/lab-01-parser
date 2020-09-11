// Copyright 2020 Evgenij Grigorev evgengrmit@icloud.com

#ifndef INCLUDE_STUDENT_HPP_
#define INCLUDE_STUDENT_HPP_

#include <any>
#include <exception>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
class Student {
 public:
  explicit Student(const json &obj);
  Student();
  [[nodiscard]] const std::string &getName() const;
  void setName(const json &name);
  [[nodiscard]] const std::any &getGroup() const;
  void setGroup(const json &group);
  [[nodiscard]] double getAvg() const;
  void setAvg(const json &avg);
  [[nodiscard]] const std::any &getDebt() const;
  void setDebt(const json &debt);
  void from_json(const json &j);

 private:
  std::string Name;
  std::any Group;
  double Avg = 0;
  std::any Debt;
};
void from_json(const json &j, Student &s);

#endif  // INCLUDE_STUDENT_HPP_
