#include "unistd.h"

#include "ReportStream.h"

using namespace std;
using namespace __gnu_cxx;

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
  info = TestInfo::deserialize(buf);
  return *this;
}
