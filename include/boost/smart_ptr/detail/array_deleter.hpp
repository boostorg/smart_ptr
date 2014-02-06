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

#include <boost/config.hpp>
#include <boost/smart_ptr/detail/array_traits.hpp>
#include <boost/smart_ptr/detail/as_pair.hpp>
#include <boost/type_traits/has_trivial_constructor.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
#include <memory>
#endif

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

            as_deleter(const A& allocator) 
                : pair(allocator, 0) {
            }

            as_deleter(const A& allocator, std::size_t size_)
                : ms_deleter_base<T>(size_),
                  pair(allocator, 0) {
            }

            void init(type* memory) {
                value_init(memory);
                pair.data = memory;
            }

            template<std::size_t N>
            void init(type* memory, const type* value) {
                value_init<N>(memory, value);
                pair.data = memory;
            }

            void operator()(const void*) {
                if (pair.data) {
                    destroy(pair.data, size);
                }
            }

        private:
#if !defined(BOOST_NO_CXX11_ALLOCATOR) && \
    !defined(BOOST_NO_CXX11_TEMPLATE_ALIASES)
            typedef typename std::allocator_traits<A>::
                template rebind_alloc<type> TA;
#else
            typedef typename A::
                template rebind<type>::other TA;
#endif

#if !defined(BOOST_NO_CXX11_ALLOCATOR)
            typedef std::allocator_traits<TA> TT;
#endif

            void destroy(type* memory, std::size_t count) {
                for (std::size_t i = count; i > 0;) {
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                    TT::destroy(pair, &memory[--i]);
#else
                    memory[--i].~type();
#endif
                }
            }

            void value_init(type* memory, boost::true_type) {
                for (std::size_t i = 0; i < size; i++) {
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                    TT::construct(pair, memory + i);
#else
                    void* p1 = memory + i;
                    ::new(p1) type();
#endif
                }
            }

            void value_init(type* memory, boost::false_type) {
#if !defined(BOOST_NO_EXCEPTIONS)
                std::size_t i = 0;
                try {
                    for (; i < size; i++) {
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                        TT::construct(pair, memory + i);
#else
                        void* p1 = memory + i;
                        ::new(p1) type();
#endif
                    }
                } catch (...) {
                    destroy(memory, i);
                    throw;
                }
#else
                for (std::size_t i = 0; i < size; i++) {
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                    TT::construct(pair, memory + i);
#else
                    void* p1 = memory + i;
                    ::new(p1) type();
#endif
                }
#endif
            }

            void value_init(type* memory) {
                boost::has_trivial_default_constructor<type> tag;
                value_init(memory, tag);
            }

            template<std::size_t N>
            void value_init(type* memory, const type* list) {
#if !defined(BOOST_NO_EXCEPTIONS)
                std::size_t i = 0;
                try {
                    for (; i < size; i++) {
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                        TT::construct(pair, memory + i, list[i % N]);
#else
                        void* p1 = memory + i;
                        ::new(p1) type(list[i % N]);
#endif
                    }
                } catch (...) {
                    destroy(memory, i);
                    throw;
                }
#else
                for (std::size_t i = 0; i < size; i++) {
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
                    TT::construct(allocator, memory + i, list[i % N]);
#else
                    void* p1 = memory + i;
                    ::new(p1) type(list[i % N]);
#endif
                }
#endif
            }

            as_pair<TA, type*> pair;
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

            void init(type* memory) {
                value_init(memory);
                object = memory;
            }

            template<std::size_t N>
            void init(type* memory, const type* value) {
                value_init<N>(memory, value);
                object = memory;
            }

            void noinit(type* memory) {
                default_init(memory);
                object = memory;
            }

            void operator()(const void*) {
                if (object) {
                    destroy(object, size);
                }
            }

        private:
            void destroy(type*, std::size_t, boost::true_type) {
            }

            void destroy(type* memory, std::size_t count, boost::false_type) {
                for (std::size_t i = count; i > 0;) {
                    memory[--i].~type();
                }
            }

            void destroy(type* memory, std::size_t count) {
                boost::has_trivial_destructor<type> tag;
                destroy(memory, count, tag);
            }

            void value_init(type* memory, boost::true_type) {
                for (std::size_t i = 0; i < size; i++) {
                    void* p1 = memory + i;
                    ::new(p1) type();
                }
            }

            void value_init(type* memory, boost::false_type) {
#if !defined(BOOST_NO_EXCEPTIONS)
                std::size_t i = 0;
                try {
                    for (; i < size; i++) {
                        void* p1 = memory + i;
                        ::new(p1) type();
                    }
                } catch (...) {
                    destroy(memory, i);
                    throw;
                }
#else
                for (std::size_t i = 0; i < size; i++) {
                    void* p1 = memory + i;
                    ::new(p1) type();
                }
#endif
            }

            void value_init(type* memory) {
                boost::has_trivial_default_constructor<type> tag;
                value_init(memory, tag);
            }

            template<std::size_t N>
            void value_init(type* memory, const type* list) {
#if !defined(BOOST_NO_EXCEPTIONS)
                std::size_t i = 0;
                try {
                    for (; i < size; i++) {
                        void* p1 = memory + i;
                        ::new(p1) type(list[i % N]);
                    }
                } catch (...) {
                    destroy(memory, i);
                    throw;
                }
#else
                for (std::size_t i = 0; i < size; i++) {
                    void* p1 = memory + i;
                    ::new(p1) type(list[i % N]);
                }
#endif
            }

            void default_init(type*, boost::true_type) {
            }

            void default_init(type* memory, boost::false_type) {
#if !defined(BOOST_NO_EXCEPTIONS)
                std::size_t i = 0;
                try {
                    for (; i < size; i++) {
                        void* p1 = memory + i;
                        ::new(p1) type;
                    }
                } catch (...) {
                    destroy(memory, i);
                    throw;
                }
#else
                for (std::size_t i = 0; i < size; i++) {
                    void* p1 = memory + i;
                    ::new(p1) type;
                }
#endif
            }

            void default_init(type* memory) {
                boost::has_trivial_default_constructor<type> tag;
                default_init(memory, tag);
            }

            type* object;
        };
    }
}

#endif
