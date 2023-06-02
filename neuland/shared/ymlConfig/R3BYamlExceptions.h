#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>

namespace R3B::yml
{
    class GeneralException : public std::exception
    {
      public:
        explicit GeneralException() = default;
        [[nodiscard]] auto what() const noexcept -> const char* override { return msg_.c_str(); }

        void Set_msg(std::string msg) { msg_ = std::string{ "\nError: " } + std::move(msg); }
        static auto GetLabel(const std::string& label, const std::vector<std::string_view>& list) -> std::string
        {
            auto chain = std::string{};
            for (const auto& it : list)
            {
                chain += std::string{ it } + std::string{ "." };
            }
            chain += label;
            return chain;
        }

      private:
        std::string msg_;
    };

    class No_definition : public GeneralException
    {
      public:
        explicit No_definition(const std::string& label, const std::vector<std::string_view>& list)
        {
            using namespace std::string_literals;
            auto msg = "Requesting a value with the label \""s + GetLabel(label, list) +
                       "\" which is not defined in the yaml config file!"s;
            Set_msg(msg);
        }
    };

    class Bad_conversion : public GeneralException
    {
      public:
        explicit Bad_conversion(const std::string& label, const std::vector<std::string_view>& list)
        {
            using namespace std::string_literals;
            auto msg = "Conversion fails with the label \""s + GetLabel(label, list) +
                       "\". Please check whether the type is defined correctly!"s;
            Set_msg(msg);
        }
    };
} // namespace R3B::yml
