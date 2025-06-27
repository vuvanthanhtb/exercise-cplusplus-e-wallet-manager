#include "../../include/utils/RandomUtil.h"
#include <cstdlib>
#include <ctime>

using namespace std;

string RandomUtil::generateRandomString(int length) {
  const string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%^&*()Ơ}|ơ]<>?/.,";
  string result;
  srand(time(nullptr));

  for (int i = 0; i < length; ++i) {
	result += chars[rand() % chars.length()];
  }
  return result;
}
