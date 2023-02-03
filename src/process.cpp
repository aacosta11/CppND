#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
    pid_ = pid;
    user_ = LinuxParser::User(std::stoi(LinuxParser::Uid(pid))).substr(0, 6);
    uptime_ = LinuxParser::UpTime(pid);
    command_ = LinuxParser::Command(pid);
    ram_ = LinuxParser::Ram(pid);
    cpu_utilization_ = LinuxParser::CpuUtilization(pid);
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return cpu_utilization_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return uptime_; }

// overload (less than)
bool Process::operator<(Process const& a) const { 
    return stoi(this->ram_) > stoi(a.ram_);
}