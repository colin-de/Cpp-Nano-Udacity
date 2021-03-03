#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float MemTotal, MemFree;
  string key, value, kb, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        while (linestream >> key >> value >> kb) {
          if (key == "MemTotal") {
            MemTotal = std::stof(value);
          }                             //std::stof change the string into float
          if (key == "MemFree"){
            MemFree = std::stof(value);
          }
        }
        return (MemTotal - MemFree) / MemTotal;
  }
  return 0.0; 
}
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    //std::stol change the string into long
    return stol(uptime); 
  }
  return 0; 
}
  
/**
The global variable jiffies holds the number of ticks that have occurred since the system booted. On boot, the kernel initializes the variable to zero, and it is incremented by one during each timer interrupt. Thus, because there are HZ timer interrupts in a second, there are HZ jiffies in a second. The system uptime is therefore jiffies/HZ seconds.
**/
  
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long total = 0;
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  for (int i = kUser_; i <= kSteal_; i++){
    total += stol(cpuUtilization[i]);
  }
  return total;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long total = 0;
  string value, line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> value){
      if (i >=13 && i <= 16) {
        total += stol(value);
      }
      i++;
    }
  }
  
  return total; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies(); 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long total = 0;
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  for (int i = kIdle_; i <= kIOwait_; i++){
    total += stol(cpuUtilization[i]);
  }
  return total; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string value, line;
  vector<string> cpuValues;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value){
      if (value != "cpu"){
        cpuValues.push_back(value);
      }
    }
  }
  return cpuValues; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int totalprocesses;
  string key, t_process, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> t_process){
        if (key == "processes"){
          totalprocesses = std::stoi(t_process);//change string into int
          return totalprocesses;
        }
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int runningprocesses;
  string key, r_process, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> r_process){
        if (key == "processes")
        {
          runningprocesses = std::stoi(r_process);//change string into int
          return runningprocesses;
        }
      }
    }
  }  
  return 0; 
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string cmd;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, cmd);
    return cmd;
  }
  return string(); 
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid[[maybe_unused]]) { 
  string line, key, value;
  std::stringstream ram;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize"){
        ram << std::fixed << std::setprecision(3) << stof(value) / 1000;
        return ram.str();
      }
        
    }
  }
  return "0"; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:"){
        return value;
      }
    }
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string line, key, x, value;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(),line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> x >> value;
      if (value == uid){
        return key;
      }
    }
  
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> value){
      if (i == 21){
        return stol(value) / sysconf(_SC_CLK_TCK);
      }
      i++;
    }
  }
  return 0; 
}
