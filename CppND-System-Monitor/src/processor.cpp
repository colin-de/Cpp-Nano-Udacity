#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  totalJiffiesStart = LinuxParser::Jiffies();
  activeJiffiesStart = LinuxParser::ActiveJiffies();
  
  usleep(100000);//microseconds --> 100 millseconds
  
  totalJiffiesEnd = LinuxParser::Jiffies();
  activeJiffiesEnd = LinuxParser::ActiveJiffes();
  
  long totalDelta = totalJiffiesEnd - totalJiffiesStart;
  long activeDelta = activejiffiesEnd - activeJiffiesStart;
  
  if (totalDelta == 0){
    return 0.0;
  }
  return float(activeDelta) / float(totalDelta); 
}