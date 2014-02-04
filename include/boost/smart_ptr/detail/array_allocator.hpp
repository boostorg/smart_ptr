/*
 * Copyright (c) 2012-2014 Glen Joseph Fernandes 
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License, 
 * Version 1.0. (See accompanying file LICENSE_1_0.txt 
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_SMART_PTR_DETAIL_ARRAY_ALLOCATOR_HPP
#define BOOST_SMART_PTR_DETAIL_ARRAY_ALLOCATOR_HPP

#include <boost/smart_ptr/detail/array_traits.hpp>
#include <boost/smart_ptr/detail/as_pair.hpp>
#include <boost/type_traits/alignment_of.hpp>
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
#include <memory>
#endif

namespace boost {
    namespace detail {
        template<typename T>
        struct ms_allocator_base;

        template<typename T>
        struct ms_allocator_base<T[]> {
            ms_allocator_base(std::size_t size_)
                : size(size_ * sizeof(T)) {
            }

            std::size_t size;
        };

        template<typename T, std::size_t N>
        struct ms_allocator_base<T[N]> {
            enum {
                size = N * sizeof(T)
            };
        };

        template<typename T, typename A, typename Y = char>
        class as_allocator
            : ms_allocator_base<T> {
            using ms_allocator_base<T>::size;

            template<typename T_, typename A_, typename Y_>
            friend class as_allocator;

#if !defined(BOOST_NO_CXX11_ALLOCATOR)
            typedef typename std::allocator_traits<A>::
                template rebind_alloc<Y> YA;
            typedef typename std::allocator_traits<A>::
                template rebind_alloc<char> CA;
            typedef typename std::allocator_traits<A>::
                template rebind_traits<Y> YT;
            typedef typename std::allocator_traits<A>::
                template rebind_traits<char> CT;
#else
            typedef typename A::
                template rebind<Y>::other YA;
            typedef typename A::
                template rebind<char>::other CA;
#endif

        public:
            typedef typename array_inner<T>::type type;

#if !defined(BOOST_NO_CXX11_ALLOCATOR)
            typedef typename YT::value_type value_type;
            typedef typename YT::pointer pointer;
            typedef typename YT::const_pointer const_pointer;
            typedef typename YT::size_type size_type;
            typedef typename YT::difference_type difference_type;
            typedef Y& reference;
            typedef const Y& const_reference;
#else
            typedef typename YA::value_type value_type;
            typedef typename YA::pointer pointer;
            typedef typename YA::const_pointer const_pointer;
            typedef typename YA::size_type size_type;
            typedef typename YA::difference_type difference_type;
            typedef typename YA::reference reference;
            typedef typename YA::const_reference const_reference;
#endif

            template<typename U>
            struct rebind {
                typedef as_allocator<T, A, U> other;
            };

            as_allocator(const A& allocator, type** data)
                : pair(allocator, data) {
            }

            as_allocator(const A& allocator, std::size_t size_, type** data)
                : ms_allocator_base<T>(size_),
                  pair(allocator, data) {
            }

            template<class U>
            as_allocator(const as_allocator<T, A, U>& other) 
                : ms_allocator_base<T>(other),
                  pair(other.pair, other.pair.data) {
            }

            pointer address(reference value) const {
                return pair.address(value);
            }

            const_pointer address(const_reference value) const {
                return pair.address(value);
            }

            size_type max_size() const {
                return pair.max_size();
            }

            pointer allocate(size_type count, const void* value = 0) {
                std::size_t a1 = boost::alignment_of<type>::value;
                std::size_t n1 = count * sizeof(Y) + a1 - 1;
                CA ca(pair);
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                char* p1 = CT::allocate(ca, size + n1, value);
#else
                char* p1 = ca.allocate(size + n1, value);
#endif
                char* p2 = p1 + n1;
                while (std::size_t(p2) % a1 != 0) {
                    p2--;
                }
                *pair.data = reinterpret_cast<type*>(p2);
                return reinterpret_cast<Y*>(p1);
            }

            void deallocate(pointer memory, size_type count) {
                std::size_t a1 = boost::alignment_of<type>::value;
                std::size_t n1 = count * sizeof(Y) + a1 - 1;
                char* p1 = reinterpret_cast<char*>(memory);
                CA ca(pair);
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                CT::deallocate(ca, p1, size + n1);
#else
                ca.deallocate(p1, size + n1);
#endif
            }

            void construct(pointer memory, const Y& value) {
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                YT::construct(pair, memory, value);
#else
                pair.construct(memory, value);
#endif
            }

            void destroy(pointer memory) {
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                YT::destroy(pair, memory);
#else
                pair.destroy(memory);
#endif
            }

            template<typename U>
            bool operator==(const as_allocator<T, A, U>& other) const {
                return pair == other.pair;
            }

            template<typename U>
            bool operator!=(const as_allocator<T, A, U>& other) const {
                return !(*this == other); 
            }

        private:
            as_pair<YA, type**> pair;
        };

        template<typename T, typename Y = char>
        class ms_allocator 
            : ms_allocator_base<T> {
            using ms_allocator_base<T>::size;

            template<typename T_, typename Y_>
            friend class ms_allocator;

        public:
            typedef typename array_inner<T>::type type;
            typedef Y value_type;
            typedef Y* pointer;
            typedef const Y* const_pointer;
            typedef std::size_t size_type;
            typedef ptrdiff_t difference_type;
            typedef Y& reference;
            typedef const Y& const_reference;

            template<typename U>
            struct rebind {
                typedef ms_allocator<T, U> other;
            };

            ms_allocator(type** data_)
                : data(data_) {
            }

            ms_allocator(std::size_t size_, type** data_)
                : ms_allocator_base<T>(size_),
                data(data_) {
            }

            template<class U>
            ms_allocator(const ms_allocator<T, U>& other)
                : ms_allocator_base<T>(other),
                data(other.data) {
            }

            pointer address(reference value) const {
                return &value;
            }

            const_pointer address(const_reference value) const {
                return &value;
            }

            size_type max_size() const {
                return static_cast<std::size_t>(-1) / sizeof(Y);
            }

            pointer allocate(size_type count, const void* = 0) {
                std::size_t a1 = boost::alignment_of<type>::value;
                std::size_t n1 = count * sizeof(Y)+a1 - 1;
                void* p1 = ::operator new(n1 + size);
                char* p2 = static_cast<char*>(p1)+n1;
                while (std::size_t(p2) % a1 != 0) {
                    p2--;
                }
                *data = reinterpret_cast<type*>(p2);
                return reinterpret_cast<Y*>(p1);
            }

            void deallocate(pointer memory, size_type) {
                void* p1 = memory;
                ::operator delete(p1);
            }

            void construct(pointer memory, const Y& value) {
                void* p1 = memory;
                ::new(p1) Y(value);
            }

            void destroy(pointer memory) {
                memory->~Y();
            }

            template<typename U>
            bool operator==(const ms_allocator<T, U>&) const {
                return true;
            }

            template<typename U>
            bool operator!=(const ms_allocator<T, U>& other) const {
                return !(*this == other);
            }

        private:
            type** data;
        };
    }
}

#endif
