#pragma once

#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace R3B::yml
{

    class R3BIOManager
    {
      public:
        struct Files
        {
            std::string input_root;
            std::string input_par;
            std::string output_root;
        };
        R3BIOManager() = default;
        void SetInputRootRegex(std::string_view regex) { input_root_regex_ = regex; }
        void SetInputParRegex(std::string_view regex) { input_par_regex_ = regex; }
        void SetOutputForamt(std::string_view format) { output_format_ = format; }
        auto GetFiles() -> const std::map<std::string, Files>&
        {
            if (!is_processed)
            {
                Process();
            }
            return files_;
        }

        void Reset()
        {
            is_processed = false;
            files_.clear();
        }

      private:
        bool is_processed = false;
        std::string input_root_regex_;
        std::string input_par_regex_;
        std::string output_format_;
        std::map<std::string, Files> files_;

        void Process();
        static auto GetRegexFiles(std::string_view regex) -> std::map<std::string, std::string>;
    };

} // namespace R3B::yml
