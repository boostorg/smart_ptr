/*
 * Copyright (c) 2012-2014 Glen Joseph Fernandes 
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License, 
 * Version 1.0. (See accompanying file LICENSE_1_0.txt 
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_SMART_PTR_DETAIL_ALLOCATE_ARRAY_HELPER_HPP
#define BOOST_SMART_PTR_DETAIL_ALLOCATE_ARRAY_HELPER_HPP

#include <boost/smart_ptr/detail/array_size_base.hpp>
#include <boost/smart_ptr/detail/array_traits.hpp>
#include <boost/type_traits/alignment_of.hpp>

namespace boost {
    namespace detail {
        template<typename A, typename T, typename Y = char>
        class allocate_array_helper
            : array_size_base<T> {
            using array_size_base<T>::size;

            template<typename A_, typename T_, typename Y_>
            friend class allocate_array_helper;

            typedef typename A::template rebind<Y>   ::other A2;
            typedef typename A::template rebind<char>::other A3;                   

        public:
            typedef typename array_inner<T>::type type;
            typedef typename A2::value_type value_type;
            typedef typename A2::pointer pointer;
            typedef typename A2::const_pointer const_pointer;
            typedef typename A2::reference reference;
            typedef typename A2::const_reference const_reference;
            typedef typename A2::size_type size_type;
            typedef typename A2::difference_type difference_type;

            template<typename U>
            struct rebind {
                typedef allocate_array_helper<A, T, U> other;
            };

            allocate_array_helper(const A& allocator_, type** data_)
                : allocator(allocator_),
                  data(data_) {
            }

            allocate_array_helper(const A& allocator_, std::size_t size_, type** data_)
                : array_size_base<T>(size_),
                  allocator(allocator_),
                  data(data_) {
            }

            template<class U>
            allocate_array_helper(const allocate_array_helper<A, T, U>& other) 
                : array_size_base<T>(other),
                  allocator(other.allocator),
                  data(other.data) {
            }

            pointer address(reference value) const {
                return allocator.address(value);
            }

            const_pointer address(const_reference value) const {
                return allocator.address(value);
            }

            size_type max_size() const {
                return allocator.max_size();
            }

            pointer allocate(size_type count, const void* value = 0) {
                std::size_t a1 = boost::alignment_of<type>::value;
                std::size_t n1 = count * sizeof(Y) + a1 - 1;
                char* p1 = A3(allocator).allocate(size + n1, value);
                char* p2 = p1 + n1;
                while (std::size_t(p2) % a1 != 0) {
                    p2--;
                }
                *data = reinterpret_cast<type*>(p2);
                return reinterpret_cast<Y*>(p1);
            }

            void deallocate(pointer memory, size_type count) {
                std::size_t a1 = boost::alignment_of<type>::value;
                std::size_t n1 = count * sizeof(Y) + a1 - 1;
                char* p1 = reinterpret_cast<char*>(memory);
                A3(allocator).deallocate(p1, size + n1);
            }

            void construct(pointer memory, const Y& value) {
                allocator.construct(memory, value);
            }

            void destroy(pointer memory) {
                allocator.destroy(memory);
            }

            template<typename U>
            bool operator==(const allocate_array_helper<A, T, U>& other) const {
                return allocator == other.allocator;
            }

            template<typename U>
            bool operator!=(const allocate_array_helper<A, T, U>& other) const {
                return !(*this == other); 
            }

        private:
            A2 allocator;
            type** data;
        };
    }
}

#endif
