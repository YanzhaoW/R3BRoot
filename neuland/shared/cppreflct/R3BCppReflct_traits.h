#pragma once
#include <type_traits>
#include <utility>
#include <vector>

namespace R3B::CppReflection
{
    struct Any
    {
        template <typename Type>
        operator Type(); // NOLINT
    };

    template <typename DataType, typename = void, typename... Args>
    struct can_construct_from_impl : std::false_type
    {
    };

    template <typename DataType, typename... Args>
    struct can_construct_from_impl<DataType, std::void_t<decltype(DataType{ std::declval<Args>()... })>, Args...>
        : std::true_type
    {
    };

    template <typename DataType, typename... Args>
    struct is_constructible_from : can_construct_from_impl<DataType, void, Args...>
    {
    };

    template <typename DataType, typename... Args>
    constexpr bool is_constructible_from_v = is_constructible_from<DataType, Args...>::value;

    template <typename Type>
    using remove_cvref = std::remove_cv_t<std::remove_reference_t<Type>>;

    template <typename Type, typename... AllowedTypes>
    constexpr bool is_among_types_v = (std::is_same_v<Type, AllowedTypes> || ...);

    template <typename Type>
    struct is_vector : std::false_type
    {
    };

    template <typename Type>
    struct is_vector<std::vector<Type>> : std::true_type
    {
    };
} // namespace R3B::CppReflection
