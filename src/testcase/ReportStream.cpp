#include "unistd.h"

#include "testcase/ReportStream.h"

using namespace std;
using namespace __gnu_cxx;
using namespace testcase;

ReportStream::EOFException::EOFException()
  : logic_error("EOF reached without a report")
{}

ReportStream::ReportStream()
  : p(),
    inbuff(p.in, ios::in),
    outbuff(p.out, ios::out),
    in(&inbuff),
    out(&outbuff)
{}

ReportStream::_pipe::_pipe()
  : in(fileds[0]),
    out(fileds[1])
{
  pipe(fileds);
}

ReportStream &ReportStream::operator<<(const TestInfo &info)
{
  out << info.serialize() << endl;
  return *this;
}

ReportStream &ReportStream::operator>>(TestInfo &info)
{
  string buf, line;
  while (getline(in, line), !in.eof() && line != "") {
    buf += line + "\n";
  }
  if (buf.empty()) {
    throw EOFException();
  }
  info = TestInfo::deserialize(buf);
  return *this;
}

void ReportStream::close_in()
{
  inbuff.close();
}

void ReportStream::close_out()
{
  outbuff.close();
}
