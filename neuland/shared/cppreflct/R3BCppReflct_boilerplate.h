#pragma once
/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019-2023 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

/**
 * Boilerplate code to achieve the reflection of C++ data structure.
 * It can be removed once the standard refelction is implemented (probably) in C++26
 * @author Yanzhao Wang
 * @since May 30th, 2023
 */

#include "R3BCppReflct_traits.h"

// NOLINTBEGIN(readability-function-cognitive-complexity,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

namespace R3B::CppReflection
{
    template <typename DataType>
    constexpr auto Get_NumOfMembers()
    {

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 100;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 99;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 98;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 97;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 96;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any>)
        {
            return 95;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any>)
        {
            return 94;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any>)
        {
            return 93;
        }

        if constexpr (is_constructible_from_v<
                          DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 92;
        }

        if constexpr (is_constructible_from_v<
                          DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 91;
        }

        if constexpr (is_constructible_from_v<
                          DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 90;
        }

        if constexpr (is_constructible_from_v<
                          DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 89;
        }

        if constexpr (is_constructible_from_v<
                          DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 88;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 87;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 86;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 85;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 84;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 83;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 82;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 81;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any>)
        {
            return 80;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any>)
        {
            return 79;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any>)
        {
            return 78;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any>)
        {
            return 77;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any>)
        {
            return 76;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any>)
        {
            return 75;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any>)
        {
            return 74;
        }

        if constexpr (is_constructible_from_v<
                          DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                          Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 73;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 72;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 71;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 70;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 69;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 68;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 67;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 66;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any>)
        {
            return 65;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any>)
        {
            return 64;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any>)
        {
            return 63;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any>)
        {
            return 62;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any>)
        {
            return 61;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any>)
        {
            return 60;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any>)
        {
            return 59;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any>)
        {
            return 58;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 57;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 56;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 55;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 54;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 53;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 52;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 51;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any>)
        {
            return 50;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any>)
        {
            return 49;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any>)
        {
            return 48;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any>)
        {
            return 47;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any>)
        {
            return 46;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any>)
        {
            return 45;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any>)
        {
            return 44;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any>)
        {
            return 43;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 42;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 41;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 40;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 39;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 38;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 37;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 36;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any>)
        {
            return 35;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any>)
        {
            return 34;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any>)
        {
            return 33;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any>)
        {
            return 32;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any>)
        {
            return 31;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any>)
        {
            return 30;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any>)
        {
            return 29;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any>)
        {
            return 28;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 27;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 26;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 25;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 24;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 23;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 22;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 21;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any, Any>)
        {
            return 20;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any, Any>)
        {
            return 19;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any, Any>)
        {
            return 18;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any, Any>)
        {
            return 17;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any, Any>)
        {
            return 16;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any, Any>)
        {
            return 15;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any>)
        {
            return 14;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any,
                                              Any>)
        {
            return 13;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 12;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 11;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 10;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 9;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 8;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any, Any>)
        {
            return 7;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any, Any>)
        {
            return 6;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any, Any>)
        {
            return 5;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any, Any>)
        {
            return 4;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any, Any>)
        {
            return 3;
        }

        if constexpr (is_constructible_from_v<DataType, Any, Any>)
        {
            return 2;
        }

        if constexpr (is_constructible_from_v<DataType, Any>)
        {
            return 1;
        }

        return 0;
    }

    template <typename DataType, typename Call_back>
    constexpr auto Members_caller(DataType&& object, Call_back&& call_back)
    {

        constexpr auto count = Get_NumOfMembers<remove_cvref<DataType>>();
        if constexpr (count == 0)
        {
            return call_back();
        }

        else if constexpr (count == 1)
        {
            auto&& [a1] = object;
            return call_back(a1);
        }

        else if constexpr (count == 2)
        {
            auto&& [a1, a2] = object;
            return call_back(a1, a2);
        }

        else if constexpr (count == 3)
        {
            auto&& [a1, a2, a3] = object;
            return call_back(a1, a2, a3);
        }

        else if constexpr (count == 4)
        {
            auto&& [a1, a2, a3, a4] = object;
            return call_back(a1, a2, a3, a4);
        }

        else if constexpr (count == 5)
        {
            auto&& [a1, a2, a3, a4, a5] = object;
            return call_back(a1, a2, a3, a4, a5);
        }

        else if constexpr (count == 6)
        {
            auto&& [a1, a2, a3, a4, a5, a6] = object;
            return call_back(a1, a2, a3, a4, a5, a6);
        }

        else if constexpr (count == 7)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7);
        }

        else if constexpr (count == 8)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8);
        }

        else if constexpr (count == 9)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9);
        }

        else if constexpr (count == 10)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
        }

        else if constexpr (count == 11)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
        }

        else if constexpr (count == 12)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
        }

        else if constexpr (count == 13)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
        }

        else if constexpr (count == 14)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
        }

        else if constexpr (count == 15)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
        }

        else if constexpr (count == 16)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
        }

        else if constexpr (count == 17)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
        }

        else if constexpr (count == 18)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
        }

        else if constexpr (count == 19)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
        }

        else if constexpr (count == 20)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
        }

        else if constexpr (count == 21)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21] =
                object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21);
        }

        else if constexpr (count == 22)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21,
                    a22] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22);
        }

        else if constexpr (count == 23)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23);
        }

        else if constexpr (count == 24)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24);
        }

        else if constexpr (count == 25)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25);
        }

        else if constexpr (count == 26)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26);
        }

        else if constexpr (count == 27)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27);
        }

        else if constexpr (count == 28)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28);
        }

        else if constexpr (count == 29)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29);
        }

        else if constexpr (count == 30)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30);
        }

        else if constexpr (count == 31)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
        }

        else if constexpr (count == 32)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32);
        }

        else if constexpr (count == 33)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33);
        }

        else if constexpr (count == 34)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34);
        }

        else if constexpr (count == 35)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35);
        }

        else if constexpr (count == 36)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36);
        }

        else if constexpr (count == 37)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37);
        }

        else if constexpr (count == 38)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38);
        }

        else if constexpr (count == 39)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39);
        }

        else if constexpr (count == 40)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40);
        }

        else if constexpr (count == 41)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41] =
                object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41);
        }

        else if constexpr (count == 42)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41,
                    a42] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42);
        }

        else if constexpr (count == 43)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43);
        }

        else if constexpr (count == 44)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44);
        }

        else if constexpr (count == 45)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45);
        }

        else if constexpr (count == 46)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46);
        }

        else if constexpr (count == 47)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47);
        }

        else if constexpr (count == 48)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48);
        }

        else if constexpr (count == 49)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49);
        }

        else if constexpr (count == 50)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50);
        }

        else if constexpr (count == 51)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51);
        }

        else if constexpr (count == 52)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52);
        }

        else if constexpr (count == 53)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53);
        }

        else if constexpr (count == 54)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54);
        }

        else if constexpr (count == 55)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55);
        }

        else if constexpr (count == 56)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56);
        }

        else if constexpr (count == 57)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57);
        }

        else if constexpr (count == 58)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58);
        }

        else if constexpr (count == 59)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59);
        }

        else if constexpr (count == 60)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60);
        }

        else if constexpr (count == 61)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61] =
                object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61);
        }

        else if constexpr (count == 62)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61,
                    a62] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62);
        }

        else if constexpr (count == 63)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63);
        }

        else if constexpr (count == 64)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64);
        }

        else if constexpr (count == 65)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65);
        }

        else if constexpr (count == 66)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66);
        }

        else if constexpr (count == 67)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67);
        }

        else if constexpr (count == 68)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68);
        }

        else if constexpr (count == 69)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69);
        }

        else if constexpr (count == 70)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70);
        }

        else if constexpr (count == 71)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71);
        }

        else if constexpr (count == 72)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72);
        }

        else if constexpr (count == 73)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73);
        }

        else if constexpr (count == 74)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74);
        }

        else if constexpr (count == 75)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75);
        }

        else if constexpr (count == 76)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76);
        }

        else if constexpr (count == 77)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77);
        }

        else if constexpr (count == 78)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78);
        }

        else if constexpr (count == 79)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79);
        }

        else if constexpr (count == 80)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80);
        }

        else if constexpr (count == 81)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81] =
                object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81);
        }

        else if constexpr (count == 82)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81,
                    a82] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82);
        }

        else if constexpr (count == 83)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83);
        }

        else if constexpr (count == 84)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84);
        }

        else if constexpr (count == 85)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85);
        }

        else if constexpr (count == 86)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86);
        }

        else if constexpr (count == 87)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87);
        }

        else if constexpr (count == 88)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88);
        }

        else if constexpr (count == 89)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89);
        }

        else if constexpr (count == 90)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89, a90] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90);
        }

        else if constexpr (count == 91)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89, a90, a91] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91);
        }

        else if constexpr (count == 92)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89, a90, a91, a92] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92);
        }

        else if constexpr (count == 93)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92,
                             a93);
        }

        else if constexpr (count == 94)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92,
                             a93, a94);
        }

        else if constexpr (count == 95)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92,
                             a93, a94, a95);
        }

        else if constexpr (count == 96)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92,
                             a93, a94, a95, a96);
        }

        else if constexpr (count == 97)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92,
                             a93, a94, a95, a96, a97);
        }

        else if constexpr (count == 98)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92,
                             a93, a94, a95, a96, a97, a98);
        }

        else if constexpr (count == 99)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92,
                             a93, a94, a95, a96, a97, a98, a99);
        }

        else if constexpr (count == 100)
        {
            auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22,
                    a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42,
                    a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62,
                    a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82,
                    a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100] = object;
            return call_back(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
                             a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38,
                             a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56,
                             a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74,
                             a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92,
                             a93, a94, a95, a96, a97, a98, a99, a100);
        }
    }

    // NOLINTEND(readability-function-cognitive-complexity,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
} // namespace R3B::CppReflection
