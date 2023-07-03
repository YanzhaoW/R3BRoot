#pragma once

#include "R3BMultiProcess.h"
#include "R3BYamlExceptions.h"
#include <R3BCppReflct.h>
#include <stdexcept>
#include <string>

namespace R3B::yml
{
    using CppReflection::remove_cvref;

    template <typename DType>
    class MPValue
    {
      public:
        MPValue() = default;
        using type = DType;
        explicit MPValue(std::vector<DType> data)
            : data_{ std::move(data) }
        {
        }
        auto size() const { return data_.size(); }
        auto at(size_t index) const -> const auto&
        {

            if (index >= size())
            {
                throw std::out_of_range("MPValue: out of bound reading!");
            }
            return data_[index];
        }

      private:
        std::vector<DType> data_;
    };

    template <typename Type>
    struct is_MPValue : std::false_type
    {
    };

    template <typename Type>
    struct is_MPValue<MPValue<Type>> : std::true_type
    {
    };

    template <typename DType>
    class DNode
    {
      public:
        DNode(std::string_view label) // NOLINT
            : label_{ label }
        {
        }
        using type = DType;
        [[nodiscard]] auto label() const -> const std::string& { return label_; }
        [[nodiscard]] auto mu_value() -> auto&
        {
            if (!data_.has_value())
            {
                data_.emplace();
            }

            return data_.value();
        }

        auto operator()() const
        {
            if (!data_.has_value())
            {
                throw No_definition{ label_chain_ };
            }

            if constexpr (is_MPValue<DType>::value)
            {
                static_assert(is_MPValue<DType>::value, "A non-MPValue cannot be accessed with an index!");
                auto index = GetGlobalIndices()->at(label_chain_);
                return (data_.value()).at(index);
            }
            else
            {
                return data_.value();
            }
        }

        void reset() { data_.reset(); }

        // TODO:
        // There is a global variable "MP_indices" used here for the multiprocesses purpose. Eliminating it is very
        // necessary!
        template <typename InputType>
        void set(InputType&& value)
        {
            static_assert(!std::is_const_v<std::remove_reference_t<decltype(data_)>>, "can not set a const variable!");
            static_assert(std::is_same_v<remove_cvref<InputType>, std::optional<DType>>,
                          "Cannot set with different types!");

            if (value.has_value())
            {
                data_ = std::forward<InputType>(value);
                has_set = true;
                if constexpr (is_MPValue<DType>::value)
                {
                    GetGlobalIndexManager()->AddEntry(label_chain_, data_.value().size());
                }
            }
            else
            {
                if (!has_set)
                {
                    data_.reset();
                }
            }
        };

        void set_labelChain(std::vector<std::string_view> chain)
        {
            chain.push_back(label_);
            label_chain_ = std::move(chain);
        }

      private:
        bool has_set = false;
        const std::string label_;
        std::vector<std::string_view> label_chain_;
        std::optional<DType> data_ = {};
    };

} // namespace R3B::yml
