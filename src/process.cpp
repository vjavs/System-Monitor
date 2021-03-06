#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

// Returns this process's ID.
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization.
float Process::CpuUtilization() const {
  // Process CPU utilization = spent seconds / total seconds
  return (LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK)) /
         static_cast<float>(LinuxParser::UpTime() - LinuxParser::UpTime(pid_));
}

// Returns the command that generated this process.
string Process::Command() { return LinuxParser::Command(pid_); }

// Returns this process's memory utilization.
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Returns the user (name) that generated this process.
string Process::User() { return LinuxParser::User(pid_); }

// Returns the age of this process (in seconds).
long Process::UpTime() {
  return LinuxParser::UpTime() - LinuxParser::UpTime(pid_);
}

// Overload the "less than" comparison operator for Process objects.
bool Process::operator<(Process const& a) const {
  // Higher CPU on top
  return CpuUtilization() > a.CpuUtilization();
}
