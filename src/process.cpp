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
    std::string uid = LinuxParser::Uid(pid);
    try 
    {
        user_ = LinuxParser::User(std::stoi(uid)).substr(0, 6);
    }
    catch (const std::invalid_argument& e) 
    {
        if ( uid.length() > 0 )
            user_ = uid;
        else
            user_ = "Unknown";
    }
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
bool Process::operator<(Process const& otherProcess) const { 
    try
    {
        return std::stoi(this->ram_) > std::stoi(otherProcess.ram_);
    }
    catch(const std::invalid_argument& e)
    {
        return true;
    }
}