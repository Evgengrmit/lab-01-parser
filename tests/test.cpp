// Copyright 2020 Evgenij Grigorev evgengrmit@icloud.com

#include <gtest/gtest.h>

#include "parser.hpp"

TEST(Parser, EmptyParser) {
  Parser a;
  ASSERT_TRUE(a.emptyJSONobject());
  Parser b;
  b.setJSONString(R"({
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
})");
  ASSERT_FALSE(b.emptyJSONobject());
}
TEST(Parser, Throw) {
  EXPECT_THROW(Parser(""), std::invalid_argument);
  EXPECT_THROW(Parser("Wrong.json"), std::out_of_range);
  EXPECT_THROW(Parser().setJSONString(R"({
  "items":
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    }
}
)"),
               std::invalid_argument);
  EXPECT_THROW(Parser().setJSONString(R"({
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 4
  }
})"),
               std::invalid_argument);
}
TEST(Parser, LengthOfFields) {
  ASSERT_EQ(Parser().getL().length_1_field, 15);
  ASSERT_EQ(Parser().getL().length_2_field, 8);
  ASSERT_EQ(Parser().getL().length_3_field, 6);
  ASSERT_EQ(Parser().getL().length_4_field, 15);
  Parser b;
  b.setJSONString(R"({
  "items": [
    {
      "name": "Ivanov Petr Petrovich",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++ Java Python C#"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31-2019-2025",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
})");
  ASSERT_EQ(b.getL().length_1_field, 22); // 21+1
  ASSERT_EQ(b.getL().length_2_field, 17); // 16+1
  ASSERT_EQ(b.getL().length_3_field, 6);
  ASSERT_EQ(b.getL().length_4_field, 19); // 18+1
}
TEST(Parser, NumberOfStudents) {
  ASSERT_EQ(Parser().getStudents().size(), 0);
  Parser b;
  b.setJSONString(R"({
  "items": [
    {
      "name": "Ivanov Petr Petrovich",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++ Java Python C#"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31-2019-2025",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
})");
  ASSERT_EQ(b.getStudents().size(), 3); // 21+1
}
TEST(Parser, Separator) {
  std::string s = "|---------------|--------|------|---------------|";
  ASSERT_EQ(Parser().getSeparator(), s);
}

TEST(Student, CorrectTypesOfFieldsNull) {
  Student s{json::parse(R"({
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    })")};
  ASSERT_EQ(s.getName(), "Ivanov Petr");
  ASSERT_EQ(std::any_cast<json>(s.getGroup()).get<std::string>(), "1");
  ASSERT_DOUBLE_EQ(s.getAvg(), 4.25);
  ASSERT_TRUE(std::any_cast<json>(s.getDebt()).is_null());
}
TEST(Student, CorrectTypesOfFieldsString) {
  Student s{json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    })")};
  ASSERT_EQ(s.getName(), "Sidorov Ivan");
  ASSERT_EQ(std::any_cast<json>(s.getGroup()).get<int>(), 31);
  ASSERT_DOUBLE_EQ(s.getAvg(), 4.00);
  ASSERT_TRUE(std::any_cast<json>(s.getDebt()).is_string());
}
TEST(Student, CorrectTypesOfFieldsArray) {
  Student s{json::parse(R"({
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    })")};
  ASSERT_EQ(s.getName(), "Pertov Nikita");
  ASSERT_EQ(std::any_cast<json>(s.getGroup()).get<std::string>(), "IU8-31");
  ASSERT_DOUBLE_EQ(s.getAvg(), 3.33);
  ASSERT_TRUE(std::any_cast<json>(s.getDebt()).is_array());
}
TEST(Student, ThrowStudent) {
  ASSERT_THROW(Student{json::parse(R"({})")}, std::invalid_argument);
}
TEST(Student, From_json_methods) {
  Student s, m;
  s.from_json(json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    })"));
  ASSERT_EQ(s.getName(), "Sidorov Ivan");
  ASSERT_EQ(std::any_cast<json>(s.getGroup()).get<int>(), 31);
  ASSERT_DOUBLE_EQ(s.getAvg(), 4.00);
  ASSERT_EQ(std::any_cast<json>(s.getDebt()).get<std::string>(), "C++");
  m.from_json(json::parse(R"({
      "name": "Ivanov Petr Petrovich",
      "group": "1",
      "avg": "4.25",
      "debt": null
    })"));
  ASSERT_EQ(m.getName(), "Ivanov Petr Petrovich");
  ASSERT_EQ(std::any_cast<json>(m.getGroup()).get<std::string>(), "1");
  ASSERT_DOUBLE_EQ(m.getAvg(), 4.25);
  ASSERT_TRUE(std::any_cast<json>(m.getDebt()).is_null());
}
TEST(Student, From_json_function) {
  Student s;
  from_json(json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    })"),
            s);
  ASSERT_EQ(s.getName(), "Sidorov Ivan");
  ASSERT_EQ(std::any_cast<json>(s.getGroup()).get<int>(), 31);
  ASSERT_DOUBLE_EQ(s.getAvg(), 4.00);
  ASSERT_EQ(std::any_cast<json>(s.getDebt()).get<std::string>(), "C++");
  ASSERT_THROW(from_json(json::parse(R"({})"), s), std::invalid_argument);
}
TEST(Student, StudentSetMethods) {
  Student s;
  json j = json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    })");
  s.setName(j.at("name"));
  s.setGroup(j.at("group"));
  s.setAvg(j.at("avg"));
  s.setDebt(j.at("debt"));
  ASSERT_EQ(s.getName(), "Sidorov Ivan");
  ASSERT_EQ(std::any_cast<json>(s.getGroup()).get<int>(), 31);
  s.setGroup(json::parse("24"));
  ASSERT_EQ(std::any_cast<json>(s.getGroup()).get<int>(), 24);
  ASSERT_DOUBLE_EQ(s.getAvg(), 4.00);
  ASSERT_EQ(std::any_cast<json>(s.getDebt()).get<std::string>(), "C++");
}
TEST(Student, ThrowFromJsonMethod) {
  Student s;
  ASSERT_THROW(s.from_json(json::parse(R"({})")), std::invalid_argument);
  json j = json::parse(R"({
      "name": "Sidorov Ivan",
      "group": "31",
      "avg": [],
      "debt": "C++"
    })");
  ASSERT_THROW(s.from_json(j), std::invalid_argument);
}
TEST(Student, ThrowSetMethod) {
  Student s;
  json j = json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": [],
      "debt": "C++"
    })");

  ASSERT_THROW(s.setAvg("avg"), std::invalid_argument);
  ASSERT_THROW(s.setAvg(j.at("avg")), std::invalid_argument);
}
TEST(Student, ThrowConstructor) {
  ASSERT_THROW(Student(json::parse(R"({})")), std::invalid_argument);
  ASSERT_THROW(Student(json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": [],
      "debt": "C++"
    })")),
               std::invalid_argument);
}
