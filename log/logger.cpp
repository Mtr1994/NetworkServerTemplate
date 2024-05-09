#include "logger.h"

#include "spdlog/sinks/rotating_file_sink.h"

Logger::Logger()
{

}

Logger::~Logger()
{
    spdlog::drop_all();
    spdlog::shutdown();
}

void Logger::init()
{
    auto logger = spdlog::rotating_logger_mt("sink_log", "./logs/sink_log.txt", 1024 * 1024 * 2, 0);
    logger->set_pattern("%Y-%m-%d %H:%M:%S.%e [%6t] <%s> {%#}: %v");
    logger->flush_on(spdlog::level::debug);

    spdlog::flush_every(std::chrono::seconds(60));
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);

    LOG_DEBUG("------------------------- sink log system start -------------------------");
}

