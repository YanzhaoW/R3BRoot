#include "R3BException.h"
#include <filesystem>
#include <fmt/color.h>
#include <fmt/core.h>

namespace fs = std::filesystem;

namespace
{

    std::string Print_msg_with_loc(const std::string& err, const boost::source_location& loc)
    {
        auto filepath = fs::path{ loc.file_name() };
        return fmt::format(fmt::emphasis::bold | fg(fmt::color::red),
                           "{}:{}:{}: {}",
                           filepath.filename().string(),
                           loc.line(),
                           loc.function_name(),
                           err);
    }

} // namespace
namespace R3B
{

    runtime_error::runtime_error(const std::string& err, const boost::source_location& loc)
        : std::runtime_error{ Print_msg_with_loc(err, loc) }
    {
    }

    logic_error::logic_error(const std::string& err, const boost::source_location& loc)
        : std::logic_error{ Print_msg_with_loc(err, loc) }
    {
    }
} // namespace R3B
