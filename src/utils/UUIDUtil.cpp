#include "../../include/utils/UUIDUtil.h"
#include <random>
#include <sstream>

using namespace std;

string UUIDUtil::generateUUID() {
  static random_device rd;
  static mt19937 gen(rd());
  static uniform_int_distribution<> dis(0, 15);
  static uniform_int_distribution<> dis2(8, 11);

  stringstream ss;
  int i;
  ss << hex;
  for (i = 0; i < 8; i++) {
    ss << dis(gen);
  }
  ss << "-";
  for (i = 0; i < 4; i++) {
    ss << dis(gen);
  }
  ss << "-4"; // version 4
  for (i = 0; i < 3; i++) {
    ss << dis(gen);
  }
  ss << "-";
  ss << dis2(gen); // variant 1
  for (i = 0; i < 3; i++) {
    ss << dis(gen);
  }
  ss << "-";
  for (i = 0; i < 12; i++) {
    ss << dis(gen);
  }

  return ss.str();
}
