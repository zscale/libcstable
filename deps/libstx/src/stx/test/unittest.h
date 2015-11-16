/**
 * This file is part of the "libstx" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * libstx is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _libstx_UTIL_UNITTEST_H
#define _libstx_UTIL_UNITTEST_H

#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <string.h>

#include "stx/exception.h"
#include "stx/inspect.h"
#include "stx/random.h"
#include "stx/io/fileutil.h"
#include "stx/io/inputstream.h"
#include "stx/io/outputstream.h"

const char kExpectationFailed[] = "ExpectationFailed";

#define UNIT_TEST(T) \
    static stx::test::UnitTest T(#T); \
    int main() { \
      auto& t = T; \
      return t.run(); \
    }

#define TEST_CASE(T, N, L) \
    static stx::test::UnitTest::TestCase __##T##__case__##N(&T, #N, (L));

#define TEST_INITIALIZER(T, N, L) \
    static stx::test::UnitTest::TestInitializer __##T##__case__##N( \
        &T, (L));

#define EXPECT(X) \
    if (!(X)) { \
      RAISE( \
          kExpectationFailed, \
          "expectation failed: %s", #X); \
    }


void EXPECT_TRUE(bool val) {
  if (!val) {
    RAISE(
        kExpectationFailed,
        "expectation failed: expected TRUE, got FALSE");
  }
}

void EXPECT_FALSE(bool val) {
  if (val) {
    RAISE(
        kExpectationFailed,
        "expectation failed: expected FALSE, got TRUE");
  }
}

template <typename T1, typename T2, typename T3>
void EXPECT_NEAR(T1 expected, T2 actual, T3 diff) {
  if (actual < (expected - diff) || actual > (expected + diff)) {
    RAISE(
        kExpectationFailed,
        "expectation failed: actual %s near expected %s, diff %s",
        stx::inspect<T2>(actual).c_str(),
        stx::inspect<T1>(expected).c_str(),
        stx::inspect<T3>(diff).c_str());
  }
}

template <typename T1, typename T2>
void EXPECT_EQ(T1 left, T2 right) {
  if (!(left == right)) {
    RAISE(
        kExpectationFailed,
        "expectation failed: %s == %s",
        stx::inspect<T1>(left).c_str(),
        stx::inspect<T2>(right).c_str());
  }
}

#define EXPECT_EXCEPTION(E, L) \
    { \
      bool raised = false; \
      try { \
        L(); \
      } catch (stx::Exception e) { \
        raised = true; \
        auto msg = e.getMessage().c_str(); \
        if (strcmp(msg, E) != 0) { \
          RAISE( \
              kExpectationFailed, \
              "excepted exception '%s' but got '%s'", E, msg); \
        } \
      } \
      if (!raised) { \
        RAISE( \
            kExpectationFailed, \
            "excepted exception '%s' but got no exception", E); \
      } \
    }

#define EXPECT_FILES_EQ(F1, F2) \
  { \
    auto one = stx::FileInputStream::openFile(F1); \
    auto two = stx::FileInputStream::openFile(F2); \
    std::string one_str; \
    std::string two_str; \
    one->readUntilEOF(&one_str); \
    two->readUntilEOF(&two_str); \
    if (one_str != two_str) { \
      std::string filename1(F1); \
      std::string filename2(F2); \
      RAISE( \
          kExpectationFailed, \
          "expected files '%s' and '%s' to be equal, but the differ", \
          filename1.c_str(), filename2.c_str()); \
    } \
  }


namespace stx {
namespace test {

class UnitTest {
public:

  static std::string tempFilePath() {
    return "/tmp/_libstx_test_tmp/";
  }

  static std::string testDataPath() {
    return "./";
  }

  class TestCase {
  public:
    TestCase(
        UnitTest* test,
        const char* name,
        std::function<void ()> lambda) :
        name_(name),
        lambda_(lambda) {
      test->addTestCase(this);
    }

    const char* name_;
    std::function<void ()> lambda_;
  };

  class TestInitializer {
  public:
    TestInitializer(
        UnitTest* test,
        std::function<void ()> lambda) :
        lambda_(lambda) {
      test->addInitializer(this);
    }

    std::function<void ()> lambda_;
  };

  UnitTest(const char* name) : name_(name) {}

  void addTestCase(const TestCase* test_case) {
    cases_.push_back(test_case);
  }

  void addInitializer(const TestInitializer* init) {
    initializers_.push_back(init);
  }

  int run() {
    stx::FileUtil::mkdir_p(UnitTest::tempFilePath());

    for (auto initializer : initializers_) {
      initializer->lambda_();
    }

    fprintf(stderr, "%s\n", name_);

    int num_tests_passed = 0;
    std::unordered_map<const TestCase*, stx::Exception> errors;

    for (auto test_case : cases_) {
      fprintf(stderr, "    %s::%s", name_, test_case->name_);
      fflush(stderr);

      try {
        test_case->lambda_();
      } catch (stx::Exception e) {
        fprintf(stderr, " \033[1;31m[FAIL]\e[0m\n");
        errors.emplace(test_case, e);
        continue;
      }

      num_tests_passed++;
      fprintf(stderr, " \033[1;32m[PASS]\e[0m\n");
    }

    if (num_tests_passed != cases_.size()) {
      for (auto test_case : cases_) {
        const auto& err = errors.find(test_case);

        if (err != errors.end()) {
          fprintf(
              stderr,
              "\n\033[1;31m[FAIL] %s::%s\e[0m\n",
              name_,
              test_case->name_);
          err->second.debugPrint();
        }
      }

      fprintf(stderr, 
          "\n\033[1;31m[FAIL] %i/%i tests failed :(\e[0m\n",
          (int) cases_.size() - num_tests_passed,
          (int) cases_.size());
      return 1;
    }

    return 0;
  }

protected:
  const char* name_;
  std::vector<const TestCase*> cases_;
  std::vector<const TestInitializer*> initializers_;
};

}
}
#endif
