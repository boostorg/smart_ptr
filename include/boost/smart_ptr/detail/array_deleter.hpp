/*
 * Copyright (c) 2012-2014 Glen Joseph Fernandes
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_SMART_PTR_DETAIL_ARRAY_DELETER_HPP
#define BOOST_SMART_PTR_DETAIL_ARRAY_DELETER_HPP

#include <boost/smart_ptr/detail/array_traits.hpp>
#include <boost/smart_ptr/detail/array_utility.hpp>

namespace boost {
    namespace detail {
        template<typename T>
        struct array_count_base;

        template<typename T>
        struct array_count_base<T[]> {
            array_count_base(std::size_t size_)
                : size(size_) {
            }

            std::size_t size;
        };

        template<typename T, std::size_t N>
        struct array_count_base<T[N]> {
            enum {
                size = N
            };
        };

        template<typename T>
        class array_deleter
            : array_count_base<T> {
            using array_count_base<T>::size;

        public:
            typedef typename array_inner<T>::type type;
        
            array_deleter()
                : object(0) {
            }

            array_deleter(std::size_t size_)
                : array_count_base<T>(size_),
                  object(0) {
            }

            ~array_deleter() {
                if (object) {
                    array_destroy(object, size);
                }
            }

            void init(type* memory) {
                array_init(memory, size);
                object = memory;
            }

            template<std::size_t N>
            void init(type* memory, const type* value) {
                array_init<type, N>(memory, size, value);
                object = memory;
            }

            void noinit(type* memory) {
                array_noinit(memory, size);
                object = memory;
            }

            void operator()(const void*) {
                if (object) {
                    array_destroy(object, size);
                    object = 0;
                }
            }

        private:
            type* object;
        };        
    }
}

#endif
