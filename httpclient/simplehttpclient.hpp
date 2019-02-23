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
#include <array>

namespace http {

enum class Method {
  GET
};

enum class ErrorCode {
  HTTP_200_OK = 200,
  HTTP_403_FORBIDDEN = 403,
  HTTP_404_NOT_FOUND = 404,
  Unknown
};


struct Errors {
  const std::array<ErrorCode, 3> errorCodes;

  std::optional<ErrorCode> fromInt(uint aInt);
  static int toInt(ErrorCode);
};



using Callback = std::function<void (const std::string& aData, ErrorCode aErrCode)>;

class SimpleHttpClient
{
  LoggerChannel mLoggerChannel;

  Errors errors = {{
    ErrorCode::HTTP_200_OK,
    ErrorCode::HTTP_403_FORBIDDEN,
    ErrorCode::HTTP_404_NOT_FOUND
  }};

  public:
  SimpleHttpClient();
  virtual ~SimpleHttpClient();
  void request(Method aMethod, const std::string& aUrl, Callback aCallback);

  private:
  void get(const std::string& aUrl, Callback aCallback);

};

} // http