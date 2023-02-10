#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::ifstream;
using std::istringstream;
using std::to_string;
using std::vector;

// Find the Operating System //
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Find the Kernel Identifier //
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization //
float LinuxParser::MemoryUtilization() { 
  string key, value, kb, line;
  ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (!filestream.is_open()) { return 0.0; }

  string memtotal, memfree;
  while (getline(filestream, line)) {
    istringstream ss(line);
    while (ss >> key >> value >> kb) {
      if (key == "MemTotal:") { memtotal = value; continue; }
      if (key == "MemFree:") { memfree = value; }
    }
    if (!memtotal.empty() && !memfree.empty()) { break; }
  }
  // Total used memory = MemTotal - MemFree
  return (stof(memtotal) - stof(memfree)) / stof(memtotal); 
}

// Read and return the system uptime //
long LinuxParser::UpTime() { 
  string uptime, idle, line;
  ifstream filestream(kProcDirectory + kUptimeFilename);
  
  if (!filestream.is_open()) { return 0; }
  std::getline(filestream, line);
  istringstream(line) >> uptime >> idle;
  return stol(uptime); 
}

// Read and return the total number of processes //
int LinuxParser::TotalProcesses() { 
  string key, value, line;
  ifstream filestream(kProcDirectory + kStatFilename);

  if (!filestream.is_open()) { return 0; }
  while (std::getline(filestream, line)) {
    istringstream ss(line);
    while (ss >> key >> value) {
      if (key == "processes") {
        return stoi(value);
      }
    }
  }
  return 0;  
}

// Read and return the number of running processes //
int LinuxParser::RunningProcesses() { 
  string key, value, line;
  ifstream filestream(kProcDirectory + kStatFilename);

  if (!filestream.is_open()) { return 0; }
  while (std::getline(filestream, line)) {
    istringstream ss(line);
    while (ss >> key >> value) {
      if (key == "procs_running") {
        return stoi(value);
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process //
string LinuxParser::Command(int pid) { 
  string cmd, line;
  ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (!filestream.is_open()) { return string(); }
  getline(filestream, line);
  istringstream(line) >> cmd;
  return cmd; 
}

// Read and return the user ID associated with a process //
string LinuxParser::Uid(int pid) { 
  string key, val1, line;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (!filestream.is_open()) { return string(); }

  while (getline(filestream, line)) {
    istringstream ss(line);
    while (ss >> key >> val1) {
      if (key == "Uid:") {
        return val1;
      }
    }
  }
  return string(); 
}

// Read and return the user associated with a uid //
string LinuxParser::User(int uid) { 
  string user, x, user_id, line;
  ifstream filestream(kPasswordPath);
  if (!filestream.is_open()) { return string(); }

  while (getline(filestream, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    istringstream ss(line);
    while (ss >> user >> x >> user_id) {
      if (user_id == to_string(uid)) {
        return user;
      }
    }
  }
  return string(); 
}

// Read and return the memory used by a process //
string LinuxParser::Ram(int pid) { 
  string key, vmsize_in_kb, line;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (!filestream.is_open()) { return string(); }

  while (getline(filestream, line)) {
    istringstream ss(line);
    while (ss >> key >> vmsize_in_kb) {
      if (key == "VmSize:") {
        return to_string((int)stoi(vmsize_in_kb) / 1000).substr(0, 7);
      }
    }
  }
  return string();
}

// Read and return the uptime of a process //
long LinuxParser::UpTime(int pid) {
  // access the 22nd entry
  string starttime, line;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (!filestream.is_open()) { return 0; }

  getline(filestream, line);
  istringstream ss(line);
  for (int i = 0; i < 22; i ++) {
    ss >> starttime;
  }
  return LinuxParser::UpTime() - (stol(starttime) / sysconf(_SC_CLK_TCK)); 
}

// Return the cpu utilization for a process //
float LinuxParser::CpuUtilization(int pid) {
  // /proc/uptime:
  // uptime
  // /proc/[PID]/stat:
  // utime (14)
  // stime (15)
  // cutime (16)
  // sctime (17)
  // starttime (22)

  // total_time = utime + stime + cutime + cstime;
  // seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
  // cpu_usage = total_time / sysconf(_SC_CLK_TCK) / seconds;
  
  string utime, stime, cutime, sctime, line;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (!filestream.is_open()) { return 0; }

  getline(filestream, line);
  istringstream ss(line);

  for (int i = 0; i < 14; i ++) {
    ss >> utime;
  }
  ss >> stime >> cutime >> sctime;

  float total_time = stol(utime) + stol(stime) + stol(cutime) + stol(sctime);
  float seconds = LinuxParser::UpTime(pid);
  float cpu_usage = total_time / sysconf(_SC_CLK_TCK) / seconds;
  return cpu_usage;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpu(10, "");
  string key, line;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (!filestream.is_open()) { return {}; }

  getline(filestream, line);
  istringstream ss(line);
  ss >> key >> cpu[0] >> cpu[1] >> cpu[2] >> cpu[3] >> cpu[4] >> cpu[5] >> cpu[6] >> cpu[7] >> cpu[8] >> cpu[9];

  return cpu; 
}
