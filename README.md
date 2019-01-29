author: Krystian Heberlein
email: krystianheberlein@gmail.com

# utils
reusable utilities

## log
Logger class based on boost::log.

#### Example
```
int main(int, char*[])
{
  auto& logger = Logger::getInstance();
  logger.init();

  auto mainChannel = logger.addChannel("MAIN_CHANNEL");
  BOOST_LOG_SEV(mainChannel, boost::log::trivial::severity_level::trace) << "Logging to main channel";
  BOOST_LOG_TRIVIAL(trace) << "Logging to general";

  return 0;
}
```

Output:
```
[2019-01-29T00:27:54.983423][MAIN_CHANNEL] [trace]    Logging to main channel
[2019-01-29T00:27:54.984467]               [trace]    Logging to general
```
#### compilation hints:
```
-DBOOST_LOG_DYN_LINK \
-lpthread \
-lboost_log \
-lboost_log_setup \
-lboost_system \
-lboost_thread \
-lboost_date_time \
-std=c++17
```
