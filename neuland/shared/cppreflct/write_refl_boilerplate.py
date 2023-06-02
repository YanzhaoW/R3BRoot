
def Get_Any_str(num:int) -> str:
    any_str = 'Any'
    return ', '.join([any_str]*num)

def print_a(num:int) -> str:
    num_list = [str(i) for i in range(1, num+1)]
    a_list = ['a'+ i for i in num_list]
    return  ', '.join(a_list)


def Write_GetNMember(num:int, file):
    file.writelines(
'''
template <typename DataType>
constexpr auto %s()
{
''' % (GetN_FunName)
                )
    for i in reversed(range(num)):
        file.writelines(
'''
    if constexpr (is_constructible_from_v<DataType, %s>)
    {
        return %d;
    }
''' % (Get_Any_str(i+1), i+1)
        )

    file.writelines('''
    return 0;
}
''')


def Write_pretexts(file):
    file.writelines(
'''
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

#include "R3BCppReflct_traits.hh"


'''
        )

def Write_members_caller(num:int, file):
    file.writelines(
'''
template <typename DataType, typename Call_back>
constexpr auto Members_caller(DataType&& object, Call_back&& call_back)
{
    // constexpr auto count = Number_of_members<DataType>();
    constexpr auto count = Get_NumOfMembers<remove_cvref<DataType>>();
    if constexpr (count == 0)
    {
        return call_back();
    }
'''
            )
    for i in range(1, num + 1):
        a_list = print_a(i)
        file.writelines(
'''
    else if constexpr (count == %d)
    {
        auto&& [%s] = object;
        return call_back(%s);
    }
''' 
%(i, a_list, a_list)
            )
    file.writelines(
'''
}
'''
            )

def Write_ignore_begin(file):
    file.writelines(
'''
// NOLINTBEGIN(readability-function-cognitive-complexity,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
'''
                    )

def Write_ignore_end(file):
    file.writelines(
'''
// NOLINTEND(readability-function-cognitive-complexity,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
'''
                    )


if __name__ == "__main__":
    filename = 'reflection_boilerplate.hh'
    static_var_name = 'num'
    max_number = 100
    GetN_FunName = 'Get_NumOfMembers'
    with open(filename, 'w') as file:
        
        Write_pretexts( file);
        Write_ignore_begin(file);
        Write_GetNMember(max_number, file)
        Write_members_caller(max_number, file)
        Write_ignore_end(file);
