#ifndef MAIL_UTIL_H
#define MAIL_UTIL_H

#include <string>

using namespace std;

class MailUtil {
public:
  static bool sendMail(const string &toEmail, const string &subject,
                       const string &body);
};

#endif
