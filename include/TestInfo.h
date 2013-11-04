#ifndef H_TEST_INFO
#define H_TEST_INFO

#include <string>

class TestInfo {
  
  public:

  TestInfo();

  explicit TestInfo(const std::string &src);

  operator std::string() const;

  bool status() const;

  TestInfo status(bool status_val) const;

  const std::string &filename() const;

  TestInfo filename(const std::string &filename_val) const;

  int lineno() const;

  TestInfo lineno(int lineno_val) const;

  const std::string &test_case() const;

  TestInfo test_case(const std::string &test_case_val) const;

  const std::string &suite() const;

  TestInfo suite(const std::string &suite_val) const;

  const std::string &report() const;

  TestInfo report(const std::string &report_val) const;

  private:

  bool status_val;

  std::string filename_val;

  int lineno_val;

  std::string test_case_val;

  std::string suite_val;

  std::string report_val;
};

#endif
