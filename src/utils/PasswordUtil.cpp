#include "../../include/utils/PasswordUtil.h"
#include <iomanip>
#include <sstream>

using namespace std;

// Key dùng cho mã hóa XOR đơn giản
const string key = "60f6c885-20be-4c68-a0e1-ff3e41238636";

string xorEncrypt(const string &input) {
  string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    output[i] ^= key[i % key.length()];
  }
  return output;
}

string toHex(const string &str) {
  ostringstream oss;
  for (unsigned char c : str) {
    oss << hex << setw(2) << setfill('0') << (int)c;
  }
  return oss.str();
}

string PasswordUtil::hashPassword(const string &password) {
  return toHex(xorEncrypt(password));
}

bool PasswordUtil::verifyPassword(const string &password, const string &hash) {
  return hash == hashPassword(password);
}
