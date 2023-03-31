#include "R3BProgramOptions.h"

namespace r3b
{
    template <typename OptionType>
    auto ProgramOptions::Create_Option(const std::string& optionName, OptionType defaultValue)
    {
        if (auto search = registries_.find(optionName); search != registries_.end())
        {
            throw runtime_error("option has been already defined!");
        }
        auto option = Option<OptionType>{ optionName, std::move(defaultValue), this };
        registries_.emplace(optionName, &option);
        return option;
    }

    void ProgramOptions::Verify(int argc, char** argv)
    {

        po::store(po::command_line_parser(argc, argv).positional(pos_desc_).options(desc_).run(), varMap_);
        po::notify(varMap_);
        for (auto& registrie : registries_)
        {
            registrie.second->Retrieve(varMap_);
        }
    }
} // namespace r3b
