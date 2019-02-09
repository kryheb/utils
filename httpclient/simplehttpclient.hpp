/*
 * file   simplehttpclient.hpp
 * author Krystian Heberlein
 * email  krystianheberlein@gmail.com
 * date   09.02.2019
 *
 * Simple http client based on lib curl
 */

#pragma once

#include <string>
#include "./../log/log.hpp"
#include <type_traits>

namespace http {

enum class Method {
  GET
};

enum class ErrorCode {
  HTTP_200_OK = 200,
  HTTP_403_FORBIDDEN = 403,
  HTTP_404_NOT_FOUND = 404,
  Unknown,
  First = HTTP_200_OK,
  Last = HTTP_404_NOT_FOUND
};

inline ErrorCode operator++(ErrorCode& aErr) {
  return (ErrorCode)(static_cast<std::underlying_type_t<ErrorCode>>(aErr) + 1);
}

std::optional<ErrorCode> errorCodeFromInt(uint aInt);

using Callback = std::function<void (const std::string& aData, ErrorCode aErrCode)>;

class SimpleHttpClient
{
  LoggerChannel mLoggerChannel;

  public:
  SimpleHttpClient();
  virtual ~SimpleHttpClient();
  void request(Method aMethod, const std::string& aUrl, Callback aCallback);

  private:
  void get(const std::string& aUrl, Callback aCallback);

};

} // http