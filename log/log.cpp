/*
 * file   log.cpp
 * author Krystian Heberlein
 * email  krystianheberlein@gmail.com
 * date   29.01.2019
 *
 * Logger based on boost::log with channel and severity support
 */

#include "log.hpp"

#include <iomanip>
#include <sstream>
#include <functional>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/value_extraction.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>


Logger& Logger::getInstance()
{
  static Logger logger;
  return logger;
}


void Logger::init()
{
  configureConsoleSink();
  configureFileSink();
  logging::add_common_attributes();
}


void Logger::formatter(logging::record_view const& aRec, logging::formatting_ostream& aStrm)
{
  std::stringstream ss;
  std::string timestamp;
  std::string name;
  std::string severity;

  if (auto nameAttr = logging::extract<std::string>("Name", aRec)) {
    name = "[" + *nameAttr + "]";
  }

  if (auto timestampAttr = logging::extract<logging::attributes::local_clock::value_type>("TimeStamp", aRec)) {
    timestamp = "[" + boost::posix_time::to_iso_extended_string(*timestampAttr) + "]";
  }

  if (auto severityAttr = aRec[logging::trivial::severity]) {
    severity = "[" + std::string(boost::log::trivial::to_string(*severityAttr)) + "]";
  }

  ss  << std::left << std::setfill(' ')
      << std::setw(15)<< timestamp
      << std::setw(15) << name
      << std::setw(10) << severity
      << aRec[expr::smessage];

    aStrm << ss.str();
}


void Logger::configureConsoleSink()
{
  using text_sink = sinks::synchronous_sink<sinks::text_ostream_backend>;
  auto sink = boost::make_shared<text_sink>();
  boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());
  sink->locked_backend()->add_stream(stream);
  sink->set_formatter(&formatter);
  logging::core::get()->add_sink(sink);
}


void Logger::configureFileSink()
{
  using file_sink = sinks::synchronous_sink<sinks::text_file_backend>;
  auto fsink = boost::make_shared<file_sink>(
      keywords::file_name = "log_%Y-%m-%d_%H-%M-%S.%N.log"
  );
  fsink->set_formatter(&formatter);
  logging::core::get()->add_sink(fsink);
}


LoggerChannel& Logger::addChannel(const std::string& aName)
{
  m_channels.try_emplace(aName, LoggerChannel{keywords::channel = aName});
  auto& channel = m_channels.at(aName);
  channel.add_attribute("Name", boost::log::attributes::constant<std::string>(aName));

  return channel;
}
