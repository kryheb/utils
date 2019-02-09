/*
 * file   simplehttpclient.cpp
 * author Krystian Heberlein
 * email  krystianheberlein@gmail.com
 * date   09.02.2019
 *
 * Simple http client based on lib curl
 */



#include "simplehttpclient.hpp"
#include "curl/curl.h"

#define LOG(s) BOOST_LOG_SEV(mLoggerChannel, s)
using namespace boost::log::trivial;

using namespace http;


std::optional<ErrorCode> http::errorCodeFromInt(uint aInt)
{
  for (auto e = ErrorCode::First; e <= ErrorCode::Last; ++e) {
    if (static_cast<std::underlying_type_t<ErrorCode>>(e) == aInt) {
      return e;
    }
  }
  return std::nullopt;
}


SimpleHttpClient::SimpleHttpClient() :
  mLoggerChannel(Logger::getInstance().addChannel("HTTP_CLIENT"))
{
  curl_global_init(CURL_GLOBAL_DEFAULT);
}


SimpleHttpClient::~SimpleHttpClient()
{
  curl_global_cleanup();
}


void SimpleHttpClient::request(Method aMethod, const std::string& aUrl, Callback aCallback)
{
  switch(aMethod) {
    case Method::GET: get(aUrl, aCallback); break;
    default:  return;
  }
}


void SimpleHttpClient::get(const std::string& aUrl, Callback aCallback)
{
  LOG(severity_level::debug) << "HTTP GET request to " << aUrl;
  CURL *curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, aUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);

    if (auto result = curl_easy_perform(curl);
        result != CURLE_OK) {
      LOG(severity_level::error) << "HTTP request has failed";
    } else {
      long rCode;
      std::string rString;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &rCode);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rString);
      LOG(severity_level::debug) << "Response (" << rCode << ") " << rString;
      if (aCallback) aCallback(rString, errorCodeFromInt(rCode).value_or(ErrorCode::Unknown));
    }

  }
  curl_easy_cleanup(curl);
}