#include "format.h"

#include <string>

using std::string;

// format seconds to HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string output("");
  int hours{0}, minutes{0};

  // find hours
  while (seconds - 3600 > 0) {
    seconds -= 3600;
    hours++;
  }
  // find minutes
  while (seconds - 60 > 0) {
    seconds -= 60;
    minutes++;
  }

  // add hours to string
  if (hours < 10) {
    output += "0";
  }
  output += std::to_string(hours) + ":";
  // add minutes to string
  if (minutes < 10) {
    output += "0";
  }
  output += std::to_string(minutes) + ":";
  // add seconds to string
  if (seconds < 10) {
    output += "0";
  }
  output += std::to_string(seconds);

  return output;
}