#include "R3BIOFiles.h"
#include <boost/format.hpp>
#include <regex>

namespace R3B::yml
{
    namespace fs = std::filesystem;
    void R3BIOManager::Process()
    {
        is_processed = true;
        auto input_root_files = GetRegexFiles(input_root_regex_);
        auto input_par_files = GetRegexFiles(input_par_regex_);

        for (auto& [key, input_root_file] : input_root_files)
        {
            if (auto iter = input_par_files.find(key); iter != input_par_files.end())
            {
                auto file = Files{ std::move(input_root_file), std::move(iter->second), "" };
                files_.insert({ key, std::move(file) });
            }
        }

        for (auto& [key, value] : files_)
        {
            value.output_root = (boost::format(output_format_) % key).str();
            std::cout << "input: " << value.input_root << "\t par: " << value.input_par
                      << "\t output: " << value.output_root << std::endl;
            ;
        }
    }

    auto R3BIOManager::GetRegexFiles(std::string_view regex) -> std::map<std::string, std::string>
    {
        const auto regex_path = fs::path(regex);
        const auto rgx = std::regex{ regex_path.filename().string() };
        const auto regex_dir = fs::absolute(regex_path.parent_path());

        auto filenames = std::map<std::string, std::string>{};
        auto matches = std::smatch();

        for (auto const& file : fs::directory_iterator(regex_dir))
        {
            auto filename = file.path().filename().string();
            if (std::regex_match(filename, matches, rgx))
            {
                std::cout << "matched: " << matches[1] << std::endl;
                // TODO exception:
                filenames.insert({ matches[1], filename });
            }
        }
        return filenames;
    }
} // namespace R3B::yml
