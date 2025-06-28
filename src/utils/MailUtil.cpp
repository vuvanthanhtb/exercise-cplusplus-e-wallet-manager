#include "../../include/utils/MailUtil.h"
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Thông tin mail
#define FROM_EMAIL ""
#define APP_PASSWORD ""

struct UploadStatus {
  size_t linesRead;
  const vector<string> *payload;
};

// Hàm đọc nội dung từng dòng gửi cho libcurl
static size_t payloadSource(void *ptr, size_t size, size_t nmemb, void *userp) {
  UploadStatus *uploadCtx = static_cast<UploadStatus *>(userp);
  if (uploadCtx->linesRead >= uploadCtx->payload->size())
    return 0;

  const string &line = (*uploadCtx->payload)[uploadCtx->linesRead++];
  size_t len = line.length();
  memcpy(ptr, line.c_str(), len);
  return len;
}

bool MailUtil::sendMail(const string &toEmail, const string &subject,
                        const string &body) {
  CURL *curl;
  CURLcode res = CURLE_OK;

  const string fromEmail = FROM_EMAIL;
  const string appPassword = APP_PASSWORD;

  // Nội dung email chia dòng
  vector<string> payloadText = {
      "To: " + toEmail + "\r\n", "From: " + fromEmail + "\r\n",
      "Subject: " + subject + "\r\n", "\r\n", body + "\r\n"};

  UploadStatus uploadCtx = {0, &payloadText};
  struct curl_slist *recipients = NULL;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_USERNAME, fromEmail.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, appPassword.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");

    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, ("<" + fromEmail + ">").c_str());
    recipients = curl_slist_append(recipients, ("<" + toEmail + ">").c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payloadSource);
    curl_easy_setopt(curl, CURLOPT_READDATA, &uploadCtx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    // Bật log để debug nếu cần
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      cerr << "Gửi email thất bại: " << curl_easy_strerror(res) << endl;
      return false;
    } else
      cout << "Email đã được gửi đến: " << toEmail << endl;

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
  }

  return false;
}
