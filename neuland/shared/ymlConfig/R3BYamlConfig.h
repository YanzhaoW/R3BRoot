#pragma once
#include "R3BYamlExceptions.h"
#include <R3BCppReflct.h>
#include <R3BYamlConfigNode.h>
#include <filesystem>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace R3B::yml
{
    namespace fs = std::filesystem;
    using CppReflection::is_among_types_v;
    using CppReflection::is_vector;
    using CppReflection::remove_cvref;
    using CppReflection::Visit_members;

    template <typename Type>
    constexpr bool is_basic_types_v =
        is_among_types_v<Type, std::string, int, bool> || std::is_floating_point_v<Type> || is_vector<Type>::value ||
        is_MPValue<Type>::value;

    class NodeVisitor
    {
      public:
        template <typename ValueType>
        void operator()(ValueType&& member)
        {
            static_assert(!std::is_const_v<std::remove_reference_t<decltype(member)>>,
                          "visitor can not visit a data structure with constant member variables!");
            using raw_type = typename remove_cvref<decltype(member)>::type;
            const auto& label = member.label();
            if constexpr (is_basic_types_v<raw_type>)
            {
                member.set_labelChain(label_chain_);
                member.set(GetValueFromNode<raw_type>(label, yaml_node_));
            }
            else
            {
                auto new_visitor = NodeVisitor{ YAML::Clone(yaml_node_[label]) };
                new_visitor.SetLabel_list(label_chain_);
                new_visitor.Append_label(label);
                Visit_members(member.mu_value(), new_visitor);
            }
        }

        template <typename RawType>
        auto GetValueFromNode(const std::string& label, const YAML::Node& node) -> std::optional<RawType>
        {
            try
            {
                if constexpr (is_MPValue<RawType>::value)
                {
                    return GetValueFromMPNode<RawType>(label, node);
                }
                else
                {
                    return std::optional<RawType>{ node[label].template as<RawType>() };
                }
            }
            catch (const YAML::InvalidNode& ex)
            {
                return std::optional<RawType>{};
            }
            catch (const YAML::BadConversion& ex)
            {
                auto chains = label_chain_;
                chains.push_back(label);
                throw Bad_conversion{ chains };
            }
        }

        template <typename MPType>
        inline auto GetValueFromMPNode(const std::string& label, const YAML::Node& node) -> std::optional<MPType>
        {
            using Type = typename MPType::type;
            return std::optional<MPType>{ node[label].template as<std::vector<Type>>() };
        }

        explicit NodeVisitor(const YAML::Node& node)
            : yaml_node_{ node }
        {
        }

        template <typename NodeType>
        constexpr void SetNode(const NodeType& node)
        {
            yaml_node_ = YAML::Clone(node);
        }

        template <typename String>
        constexpr void Append_label(const String& label)
        {
            label_chain_.push_back(label);
        }

        void SetLabel_list(std::vector<std::string_view> list) { label_chain_ = std::move(list); }

      private:
        const YAML::Node yaml_node_;
        std::vector<std::string_view> label_chain_;
    };

    class Manager
    {
      public:
        Manager() = default;
        explicit Manager(const std::vector<fs::path>& filenames) { AddFiles(filenames); }

        explicit Manager(std::string_view filename)
            : Manager(std::vector<fs::path>{ filename })
        {
        }

        template <typename ConfigNode>
        void Parse(ConfigNode& configNode) const
        {
            for (const auto& node : root_nodes_)
            {
                auto visitor = NodeVisitor{ YAML::Clone(node) };
                visitor(configNode);
            }
        }

        void AddFiles(const std::vector<fs::path>& files)
        {
            auto nodes = std::vector<YAML::Node>{};
            nodes.reserve(files.size());
            for (const auto& file : files)
            {
                AddFile(file);
            }
        }

        void AddFile(fs::path filename)
        {
            using namespace std::string_literals;
            try
            {
                if (!filename.empty())
                {
                    root_nodes_.push_back(YAML::LoadFile(filename.string()));
                    yamlFiles_.emplace_back(filename);
                }
            }
            catch (...)
            {
                auto msg = "Following YAML config path \"" + filename.string() + "\" cannot be read by yaml-cpp!"s;
                throw std::runtime_error(msg);
            }
        }

      private:
        std::vector<fs::path> yamlFiles_;
        std::vector<YAML::Node> root_nodes_;
    };

} // namespace R3B::yml
