#include <util/logger/logger.hpp>

#include <iomanip>
#include <thread>

#include <util/date.hpp>

namespace fast_chess {

std::atomic_bool Logger::should_log_ = false;
std::unique_ptr<std::ostream> Logger::log_ = nullptr;
std::mutex Logger::log_mutex_;
Logger::Level Logger::level_ = Logger::Level::WARN;

void Logger::openFile(const std::string &file) {
    if (file == "stdout") {
        Logger::log_ = std::make_unique<std::ostream>(std::cout.rdbuf());
    } else {
        Logger::log_ = std::make_unique<std::ofstream>(file, std::ios::app);
    }
    Logger::should_log_ = true;
}

void Logger::setLevel(Level level) { Logger::level_ = level; }

void Logger::writeToEngine(const std::string &msg, const std::string &name) {
    if (!should_log_) {
        return;
    }

    std::stringstream ss;
    ss << "[" << util::time::datetime("%H:%M:%S") << "] "
       << " <" << std::setw(3) << std::this_thread::get_id() << "> " << name << " <--- " << msg
       << std::endl;

    // Acquire the lock
    const std::lock_guard<std::mutex> lock(log_mutex_);
    *log_ << ss.str() << std::flush;
}

void Logger::readFromEngine(const std::string &msg, const std::string &name, bool err) {
    if (!should_log_) {
        return;
    }

    std::stringstream ss;
    ss << "[" << util::time::datetime("%H:%M:%S") << "] "
       << " <" << std::setw(3) << std::this_thread::get_id() << "> " << name
       << (err ? " 1 " : " 2 ") << "---> " << msg << std::endl;

    // Acquire the lock
    const std::lock_guard<std::mutex> lock(log_mutex_);
    *log_ << ss.str() << std::flush;
}

}  // namespace fast_chess
