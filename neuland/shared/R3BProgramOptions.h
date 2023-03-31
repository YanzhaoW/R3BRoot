#pragma once

#include <boost/program_options.hpp>
#include <unordered_map>

namespace r3b
{
    using std::runtime_error;

    namespace po = boost::program_options;

    template <typename>
    struct is_bool : std::false_type
    {
    };

    template <>
    struct is_bool<bool> : std::true_type
    {
    };

    class OptionConcept;
    template <typename Type>
    class Option;

    class ProgramOptions
    {
      public:
        ProgramOptions() = default;
        explicit ProgramOptions(const std::string& desc)
            : desc_{ desc }
        {
        }

        template <typename OptionType>
        auto Create_Option(const std::string& optionName, OptionType defaultValue = OptionType{});
        void Verify(int argc, char** argv);
        void Delete_Option(const std::string& optionName) { registries_.erase(optionName); }
        auto& Get_PosDescRef() { return pos_desc_; }
        auto& Get_DescRef() { return desc_; }

      private:
        std::unordered_map<std::string, OptionConcept*> registries_;
        po::positional_options_description pos_desc_ = {};
        po::options_description desc_ = {};
        po::variables_map varMap_ = {};
    };

    class OptionConcept
    {
      public:
        virtual ~OptionConcept() = default;
        OptionConcept() = default;
        OptionConcept(const OptionConcept&) = delete;
        OptionConcept(OptionConcept&&) = delete;
        OptionConcept& operator=(const OptionConcept&) = delete;
        OptionConcept& operator=(OptionConcept&&) = delete;
        virtual void Retrieve(const po::variables_map& varMap) = 0;
    };

    template <typename Type>
    class Option : public OptionConcept
    {
      public:
        using type = Type;
        explicit Option(std::string name, Type defaultValue, ProgramOptions* program)
            : name_{ std::move(name) }
            , value_{ std::move(defaultValue) }
            , program_{ program }
        {
            if (auto end = name_.find(','))
            {
                key_ = name_.substr(0, end);
            }
            else
            {
                key_ = name_;
            }
        }

        Option(const Option&) = delete;
        Option(Option&&) = delete;
        Option& operator=(const Option&) = delete;
        Option& operator=(Option&&) = delete;
        ~Option() override { program_->Delete_Option(name_); }

        void Add(const std::string& desc)
        {
            auto& po_desc = program_->Get_DescRef();
            if constexpr (is_bool<Type>::value)
            {
                po_desc.add_options()(name_.c_str(), desc.c_str());
            }
            else
            {
                po_desc.add_options()(name_.c_str(), po::value<Type>(), desc.c_str());
            }
        }

        void MakePositional(int option) { program_->Get_PosDescRef().add(name_.c_str(), option); }

        void Retrieve(const po::variables_map& varMap) override
        {
            if (varMap.count(key_) != 0U)
            {

                if constexpr (is_bool<Type>::value)
                {
                    value_ = true;
                }
                else
                {
                    value_ = varMap[key_].template as<Type>();
                }
            }
            else if (is_required)
            {
                const auto error_msg = "program option " + name_ + " is required.";
                throw runtime_error(error_msg);
            }
        }

        void Set_required(bool p_rq) { is_required = p_rq; }

        [[nodiscard]] auto Get() const { return value_; }

      private:
        std::string name_;
        std::string key_;
        std::string desc_;
        bool is_required = false;
        Type value_{};
        ProgramOptions* program_ = nullptr;
    };
}; // namespace r3b
