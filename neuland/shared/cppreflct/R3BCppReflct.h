#pragma once

#include "R3BCppReflct_boilerplate.h"

namespace R3B::CppReflection
{
    enum class Options : unsigned char
    {
        none = 0U,
        aggregate = 1U << 0U,
        constant_visitor = 1U << 1U,
    };

    inline constexpr auto operator|(Options lhs, Options rhs) -> Options
    {
        using true_type = std::underlying_type_t<Options>;
        return static_cast<Options>(static_cast<true_type>(lhs) | static_cast<true_type>(rhs));
    }

    inline constexpr auto operator|=(Options& lhs, Options rhs) -> Options&
    {
        lhs = lhs | rhs;
        return lhs;
    }

    inline constexpr auto operator~(Options option) -> Options
    {
        using true_type = std::underlying_type_t<Options>;
        return static_cast<Options>(~static_cast<true_type>(option));
    }

    // Use De Morgan's law such that logic AND is consistent with logic OR
    inline constexpr auto operator&(Options lhs, Options rhs) -> Options { return ~((~lhs) | (~rhs)); }

    inline constexpr auto operator&=(Options& lhs, Options rhs) -> Options&
    {
        lhs = lhs & rhs;
        return lhs;
    }

    template <Options option, typename Visitor>
    inline auto DoMany(Visitor&& visitor)
    {
    }
    template <Options option, typename Visitor, typename First, typename... Args>
    constexpr auto DoMany(Visitor&& visitor, First&& first, Args&&... args);

    template <Options option = Options::none, typename DataType, typename Visitor>
    constexpr void Visit_members(DataType&& data, Visitor&& visitor)
    {
        using Raw_type = std::remove_reference_t<decltype(data)>;
        static_assert(std::is_class_v<Raw_type>, "Can not visit a non-class data structure!");

        // make sure data structure is non-const with a non-const visitor
        if constexpr (Options::none != (option & Options::constant_visitor))
        {
            static_assert(!std::is_const_v<Raw_type>, "data structure should not contain constant variables!");
        }
        Members_caller(std::forward<DataType>(data), [&](auto&&... args)
                       { DoMany<option>(std::forward<Visitor>(visitor), std::forward<decltype(args)>(args)...); });
    }

    template <Options option, typename Visitor, typename First, typename... Args>
    constexpr auto DoMany(Visitor&& visitor, First&& first, Args&&... args)
    {
        if constexpr (std::is_compound_v<remove_cvref<First>> && (option & Options::aggregate) != Options::none)
        {
            Visit_members<option>(std::forward<First>(first), std::forward<Visitor>(visitor));
        }
        else
        {
            visitor(std::forward<First>(first));
        }
        DoMany<option>(std::forward<Visitor>(visitor), std::forward<Args>(args)...);
    }
} // namespace R3B::CppReflection
