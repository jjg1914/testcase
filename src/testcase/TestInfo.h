#ifndef H_TESTCASE_TEST_INFO
#define H_TESTCASE_TEST_INFO

#include <string>

namespace testcase {

  class TestInfo {
    
    public:

    enum Status {
      PASSED = 0,
      FAILED,
      ERROR,
      TIMEOUT
    };

    static TestInfo passed();

    static TestInfo failed(const std::string &what, const std::string &file, int line);

    static TestInfo error(const std::string &what);

    static TestInfo timeout();

    static TestInfo deserialize(const std::string &src);

    TestInfo();

    std::string serialize() const;

    Status status() const;

    TestInfo status(Status status_val) const;

    const std::string &filename() const;

    TestInfo filename(const std::string &filename_val) const;

    int lineno() const;

    TestInfo lineno(int lineno_val) const;

    const std::string &test_case() const;

    TestInfo test_case(const std::string &test_case_val) const;

    const std::string &suite() const;

    TestInfo suite(const std::string &suite_val) const;

    const std::string &backtrace() const;

    TestInfo backtrace(const std::string &backtrace_val) const;

    const std::string &what() const;

    TestInfo what(const std::string &what_val) const;

    int asserts() const;

    TestInfo asserts(int asserts_val) const;

    const std::string &diff() const;

    TestInfo diff(const std::string &diff) const;

    private:

    Status status_val;

    std::string filename_val;

    int lineno_val;

    std::string test_case_val;

    std::string suite_val;

    std::string backtrace_val;

    std::string what_val;

    int asserts_val;

    std::string diff_val;
  };
}

#endif
