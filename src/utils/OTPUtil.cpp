#include "../../include/utils/OTPUtil.h"
#include <cstdlib>
#include <ctime>

using namespace std;

string OTPUtil::generate() {
  srand(time(0));
  int otp = 100000 + rand() % 900000;
  return to_string(otp);
}