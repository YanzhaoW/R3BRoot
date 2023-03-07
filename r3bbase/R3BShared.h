#pragma once
#include <type_traits>

class TF1;
class TH1;
class TTree;

namespace r3b
{
    // ROOT types owned by ROOT: TH1, TF1;
    template <typename... Types>
    struct TypeCollection
    {
    };

    using RootTypes = TypeCollection<TF1, TH1, TTree>;

    template <typename Type, typename>
    inline constexpr bool is_based_on = false;

    template <typename Type, typename... BaseTypes>
    inline constexpr bool is_based_on<Type, TypeCollection<BaseTypes...>> = (std::is_base_of_v<BaseTypes, Type> || ...);

    template <typename Type>
    inline constexpr bool is_root_owned = is_based_on<Type, RootTypes>;

    template <typename RootType, typename... Args>
    inline auto root_owned(Args&&... args)
    {
        static_assert(is_root_owned<RootType>, "root_owned: such type cannot be owned by ROOT!");
        return new RootType(std::forward<Args>(args)...); // NOLINT
    }
} // namespace r3b
