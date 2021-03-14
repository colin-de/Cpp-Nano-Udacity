#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>

//TODO: Return the aggregate CPU utilization
// float Processor::Utilization() { 
//   totalJiffiesStart = LinuxParser::Jiffies();
//   activeJiffiesStart = LinuxParser::ActiveJiffies();
  
//   usleep(100000);//microseconds --> 100 millseconds
  
//   totalJiffiesEnd = LinuxParser::Jiffies();
//   activeJiffiesEnd = LinuxParser::ActiveJiffies();
  
//   long totalDelta = totalJiffiesEnd - totalJiffiesStart;
//   long activeDelta = activeJiffiesEnd - activeJiffiesStart;
  
//   if (totalDelta == 0){
//     return 0.0;
//   }
//   return float(activeDelta) / float(totalDelta); 
// }


float Processor::Utilization() {
    std::vector<std::string> cpu_utilization = LinuxParser::CpuUtilization();
    static std::vector<std::string> cpu_utilization_last = cpu_utilization;
    // std::cout << "cpu_utilization[kUser_]" << cpu_utilization[LinuxParser::kUser_] << std::endl;
    // std::cout << "cpu_utilization[kNice_]" << cpu_utilization[LinuxParser::kNice_] << std::endl;
    // std::cout << "cpu_utilization[kSystem_]" << cpu_utilization[LinuxParser::kSystem_] << std::endl;
    // std::cout << "cpu_utilization[kIdle_]" << cpu_utilization[LinuxParser::kIdle_] << std::endl;

    const auto Idle = stof(cpu_utilization[LinuxParser::kIdle_]) + stof(cpu_utilization[LinuxParser::kIOwait_]);
    const auto PrevIdle = stof(cpu_utilization_last[LinuxParser::kIdle_]) + stof(cpu_utilization_last[LinuxParser::kIOwait_]);

    const auto nonidle = stof(cpu_utilization[LinuxParser::kUser_]) + stof(cpu_utilization[LinuxParser::kNice_]) + stof(cpu_utilization[LinuxParser::kSystem_]) +
                         stof(cpu_utilization[LinuxParser::kIRQ_]) + stof(cpu_utilization[LinuxParser::kSoftIRQ_]) + stof(cpu_utilization[LinuxParser::kSteal_]);
    const auto prev_nonidle = stof(cpu_utilization_last[LinuxParser::kUser_]) + stof(cpu_utilization_last[LinuxParser::kNice_]) + stof(cpu_utilization_last[LinuxParser::kSystem_]) +
                              stof(cpu_utilization_last[LinuxParser::kIRQ_]) + stof(cpu_utilization_last[LinuxParser::kSoftIRQ_]) + stof(cpu_utilization_last[LinuxParser::kSteal_]);

    const auto total = Idle + nonidle;
    const auto prev_total = PrevIdle + prev_nonidle;

    auto totald = total - prev_total;
    totald = totald < 0.0001f ? 0.0001f : totald;
    const auto idled = Idle - PrevIdle;
    const float precent = (totald - idled) / totald;
    // std::cout << "totald: " << totald << ", idled: " << idled << ", totald - idled: " << totald - idled << ", a: " << a <<std::endl;

    cpu_utilization_last = cpu_utilization;

    return precent;
}