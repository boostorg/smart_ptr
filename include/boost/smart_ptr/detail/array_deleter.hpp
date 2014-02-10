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

#include <boost/smart_ptr/detail/allocator_pair.hpp>
#include <boost/smart_ptr/detail/array_traits.hpp>
#include <boost/smart_ptr/detail/array_utility.hpp>

namespace boost {
    namespace detail {
        template<typename T>
        struct ms_deleter_base;

        template<typename T>
        struct ms_deleter_base<T[]> {
            ms_deleter_base(std::size_t size_)
                : size(size_) {
            }

            std::size_t size;
        };

        template<typename T, std::size_t N>
        struct ms_deleter_base<T[N]> {
            enum {
                size = N
            };
        };

        template<typename T, typename A>
        class as_deleter
            : ms_deleter_base<T> {
            using ms_deleter_base<T>::size;

        public:
            typedef typename array_inner<T>::type type;

#if !defined(BOOST_NO_CXX11_ALLOCATOR)
            typedef typename std::allocator_traits<A>::
                template rebind_alloc<type> allocator;
#else
            typedef typename A::
                template rebind<type>::other allocator;
#endif

            as_deleter(const A& allocator_)
                : pair(allocator_, 0) {
            }

            as_deleter(const A& allocator_, std::size_t size_)
                : ms_deleter_base<T>(size_),
                  pair(allocator_, 0) {
            }

            void set(type* memory) {
                pair.data = memory;
            }

            void operator()(const void*) {
                if (pair.data) {
                    as_destroy<type, allocator>(pair, pair.data, size);
                }
            }

        private:
            as_pair<allocator, type*> pair;
        };

        template<typename T>
        class ms_deleter
            : ms_deleter_base<T> {
            using ms_deleter_base<T>::size;

        public:
            typedef typename array_inner<T>::type type;

            ms_deleter()
                : object(0) {
            }

            ms_deleter(std::size_t size_)
                : ms_deleter_base<T>(size_),
                  object(0) {
            }

            void set(type* memory) {
                object = memory;
            }

            void operator()(const void*) {
                if (object) {
                    ms_destroy(object, size);
                }
            }

        private:
            type* object;
        };
    }
}

#endif
