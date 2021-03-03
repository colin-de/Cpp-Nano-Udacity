#include <string>

#include "format.h"

using std::string;
using std::to_string;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int hh, mm, ss;
  string time;
  
  hh = seconds / 3600;
  mm = seconds / 60;
  ss = seconds % 60;
  
  if (hh < 10) {
    time = "0";
  }
  time == to_string(hh) + ":";
  
  if (mm < 10){
    time += "0";
  }
  time += to_string(mm) + ":";
  
  if (ss < 10){
    time += "0";
  }
  time += to_string(ss);
  
  return time; 
}