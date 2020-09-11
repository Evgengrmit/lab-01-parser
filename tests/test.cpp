// Copyright 2020 Evgenij Grigorev evgengrmit@icloud.com

#include <gtest/gtest.h>

#include "parser.hpp"

TEST(Parser, EmptyParser) {
  Parser a;
  ASSERT_TRUE(a.emptyJSONobject());
  Parser b;
  b.setJSONstring(R"({
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
  EXPECT_THROW(Parser().setJSONstring(R"({
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
  EXPECT_THROW(Parser().setJSONstring(R"({
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
TEST(Parser, CorrectTypesOfFieldsNull) {
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
TEST(Parser, CorrectTypesOfFieldsString) {
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
TEST(Parser, CorrectTypesOfFieldsArray) {
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
TEST(Test6, ThrowStudent) {
  ASSERT_THROW(Student{json::parse(R"({})")}, std::invalid_argument);
}
TEST(Test7, From_json) {
  Student s;
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
}
