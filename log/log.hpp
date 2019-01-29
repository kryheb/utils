/*
 * file   log.hpp
 * author Krystian Heberlein
 * email  krystianheberlein@gmail.com
 * date   29.01.2019
 *
 * Logger based on boost::log with channel and severity support
 */

#include <map>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/utility/formatting_ostream.hpp>


namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;


using Channel =  src::severity_channel_logger<boost::log::trivial::severity_level>;

class Logger {

  std::map<std::string, Channel> m_channels;

private:
  Logger(){}
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;


public:
  static Logger& getInstance();
  void init();
  static void formatter(logging::record_view const& aRec, logging::formatting_ostream& aStrm);
  Channel& addChannel(const std::string& aName);

private:
  void configureConsoleSink();
  void configureFileSink();

};

