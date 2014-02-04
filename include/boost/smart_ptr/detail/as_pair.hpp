/*
 * Copyright (c) 2014 Glen Joseph Fernandes 
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License, 
 * Version 1.0. (See accompanying file LICENSE_1_0.txt 
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_SMART_PTR_DETAIL_AS_PAIR_HPP
#define BOOST_SMART_PTR_DETAIL_AS_PAIR_HPP

#include <cstddef>

namespace boost {
    namespace detail {
        template<typename A, typename T>
        struct as_pair
            : A {
            as_pair(const A& allocator, const T& value)
                : A(allocator),
                  data(value) {
            }

            T data;
        };
    }
}

#endif
