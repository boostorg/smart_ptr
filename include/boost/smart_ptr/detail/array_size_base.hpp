/*
 * Copyright (c) 2014 Glen Joseph Fernandes 
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License, 
 * Version 1.0. (See accompanying file LICENSE_1_0.txt 
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_SMART_PTR_DETAIL_ARRAY_SIZE_BASE_HPP
#define BOOST_SMART_PTR_DETAIL_ARRAY_SIZE_BASE_HPP

#include <cstddef>

namespace boost {
    namespace detail {
        template<typename T>
        struct array_size_base;

        template<typename T>
        struct array_size_base<T[]> {
            array_size_base(std::size_t size_)
                : size(size_ * sizeof(T)) {
            }

            std::size_t size;
        };        

        template<typename T, std::size_t N>
        struct array_size_base<T[N]> {
            enum {
                size = N * sizeof(T)
            };
        };
    }
}

#endif
