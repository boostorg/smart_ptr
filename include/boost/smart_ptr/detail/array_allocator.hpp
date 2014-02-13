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
#include <boost/smart_ptr/detail/array_utility.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <algorithm>

namespace boost {
    namespace detail {
        template<class T, class A>
        struct as_size_base;

        template<class T, class A>
        struct as_size_base<T[], A> 
            : A {
            as_size_base(const A& allocator, std::size_t size_)
                : A(allocator),
                  size(size_ * array_total<T>::size) {
            }

            std::size_t size;
        };

        template<class T, std::size_t N, class A>
        struct as_size_base<T[N], A> 
            : A {
            as_size_base(const A& allocator)
                : A(allocator) {
            }

            enum {
                size = array_total<T[N]>::size
            };
        };

        struct ms_init_tag   { };
        struct ms_noinit_tag { };

        template<class T, class R, class A, class Y = char>
        class as_allocator
            : as_size_base<T, A> {
            using as_size_base<T, A>::size;

            template<class T_, class R_, class A_, class Y_>
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
            typedef typename A::template rebind<Y>::other YA;
            typedef typename A::template rebind<char>::other CA;
#endif

        public:
            typedef typename array_base<T>::type type;

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

            template<class U>
            struct rebind {
                typedef as_allocator<T, R, A, U> other;
            };

            as_allocator(const A& allocator)
                : as_size_base<T, A>(allocator),
                  object(0) {
            }

            as_allocator(const A& allocator, std::size_t size_)
                : as_size_base<T, A>(allocator, size_),
                  object(0) {
            }

            as_allocator(const as_allocator& other, void* memory,
                std::size_t offset)
                : as_size_base<T, A>(other) {
                enum {
                    M = boost::alignment_of<type>::value
                };
                std::size_t n1 = offset + M - 1;
                char* p1 = static_cast<char*>(memory) + n1;
                while (std::size_t(p1) % M != 0) {
                    p1--;
                }
                object = reinterpret_cast<type*>(p1);
            }

            template<class U>
            as_allocator(const as_allocator<T, R, A, U>& other)
                : as_size_base<T, A>(other),
                  object(other.object) {
            }

            pointer address(reference value) const {
                YA ya(*this);
                return ya.address(value);
            }

            const_pointer address(const_reference value) const {
                YA ya(*this);
                return ya.address(value);
            }

            size_type max_size() const {
                YA ya(*this);
                return ya.max_size();
            }

            pointer allocate(size_type count, const void* value = 0) {
                enum {
                    M = boost::alignment_of<type>::value
                };
                std::size_t n1 = count * sizeof(value_type) + M - 1;
                std::size_t n2 = size * sizeof(type);
                CA ca(*this);
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                void* p1 = CT::allocate(ca, n1 + n2, value);
#else
                void* p1 = ca.allocate(n1 + n2, value);
#endif
                return static_cast<value_type*>(p1);
            }

            void deallocate(pointer memory, size_type count) {
                enum {
                    M = boost::alignment_of<type>::value
                };
                std::size_t n1 = count * sizeof(value_type) + M - 1;
                char* p1 = reinterpret_cast<char*>(memory);
                CA ca(*this);
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                CT::deallocate(ca, p1, size + n1);
#else
                ca.deallocate(p1, size + n1);
#endif
            }

            template<class U>
            void construct(U* memory, const_reference value) {
                YA ya(*this);
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                YT::construct(ya, memory, value);
#else
                ya.construct(memory, value);
#endif
            }

            template<class U>
            void destroy(U* memory) {
                YA ya(*this);
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                YT::destroy(ya, memory);
#else
                ya.destroy(memory);
#endif
            }

#if !defined(BOOST_NO_CXX11_ALLOCATOR) && \
    !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) && \
    !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
            template<class U, class... Args>
            void construct(U* memory, Args&&... args) {
                YA ya(*this);
                YT::construct(ya, memory, std::forward<Args>(args)...);
            }
#endif

            template<class U>
            bool operator==(const as_allocator<T, R, A, U>& other) const {
                return true;
            }

            template<class U>
            bool operator!=(const as_allocator<T, R, A, U>& other) const {
                return !(*this == other);
            }

            void swap(type*& other) {
                std::swap(object, other);
            }

            void operator()() {
                if (object) {
                    R tag;
                    free(tag);
                }
            }

        private:
            void free(ms_init_tag) {
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                const A& a1(*this);
                as_destroy(a1, object, size);
#else
                ms_destroy(object, size);
#endif
            }

            void free(ms_noinit_tag) {
                ms_destroy(object, size);
            }

            type* object;
        };

        template<class T>
        struct ms_size_base;

        template<class T>
        struct ms_size_base<T[]> {
            ms_size_base(std::size_t size_)
                : size(size_ * array_total<T>::size) {
            }

            std::size_t size;
        };

        template<class T, std::size_t N>
        struct ms_size_base<T[N]> {
            enum {
                size = array_total<T[N]>::size
            };
        };

        template<class T, class R, class Y = char>
        class ms_allocator
            : ms_size_base<T> {
            using ms_size_base<T>::size;

            template<class T_, class R_, class Y_>
            friend class ms_allocator;

        public:
            typedef typename array_base<T>::type type;
            typedef Y value_type;
            typedef Y* pointer;
            typedef const Y* const_pointer;
            typedef std::size_t size_type;
            typedef ptrdiff_t difference_type;
            typedef Y& reference;
            typedef const Y& const_reference;

            template<class U>
            struct rebind {
                typedef ms_allocator<T, R, U> other;
            };

            ms_allocator()
                : object(0) {
            }

            ms_allocator(std::size_t size_)
                : ms_size_base<T>(size_),
                  object(0) {
            }

            ms_allocator(const ms_allocator& other, void* memory,
                std::size_t offset)
                : ms_size_base<T>(other) {
                enum {
                    M = boost::alignment_of<type>::value
                };
                std::size_t n1 = offset + M - 1;
                char* p1 = static_cast<char*>(memory) + n1;
                while (std::size_t(p1) % M != 0) {
                    p1--;
                }
                object = reinterpret_cast<type*>(p1);
            }

            template<class U>
            ms_allocator(const ms_allocator<T, R, U>& other)
                : ms_size_base<T>(other),
                  object(other.object) {
            }

            pointer address(reference value) const {
                return &value;
            }

            const_pointer address(const_reference value) const {
                return &value;
            }

            size_type max_size() const {
                enum {
                    N = static_cast<std::size_t>(-1) / sizeof(value_type)
                };
                return N;
            }

            pointer allocate(size_type count, const void* = 0) {
                enum {
                    M = boost::alignment_of<type>::value
                };
                std::size_t n1 = count * sizeof(value_type) + M - 1;
                std::size_t n2 = size * sizeof(type);
                void* p1 = ::operator new(n1 + n2);
                return static_cast<value_type*>(p1);
            }

            void deallocate(pointer memory, size_type) {
                void* p1 = memory;
                ::operator delete(p1);
            }

            template<class U>
            void construct(U* memory, const_reference value) {
                void* p1 = memory;
                ::new(p1) U(value);
            }

            template<class U>
            void destroy(U* memory) {
                memory->~U();
            }

#if !defined(BOOST_NO_CXX11_ALLOCATOR) && \
    !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) && \
    !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
            template<class U, class... Args>
            void construct(U* memory, Args&&... args) {
                void* p1 = memory;
                ::new(p1) U(std::forward<Args>(args)...);
            }
#endif

            template<class U>
            bool operator==(const ms_allocator<T, R, U>&) const {
                return true;
            }

            template<class U>
            bool operator!=(const ms_allocator<T, R, U>& other) const {
                return !(*this == other);
            }

            void swap(type*& other) {
                std::swap(object, other);
            }

            void operator()() {
                if (object) {
                    ms_destroy(object, size);
                }
            }

        private:
            type* object;
        };

        class ms_in_allocator_tag {
        public:
            void operator()(const void*) {
            }
        };
    }
}

#endif
