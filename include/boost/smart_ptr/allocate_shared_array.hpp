/*
 * Copyright (c) 2012-2014 Glen Joseph Fernandes
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_SMART_PTR_ALLOCATE_SHARED_ARRAY_HPP
#define BOOST_SMART_PTR_ALLOCATE_SHARED_ARRAY_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/detail/allocate_array_helper.hpp>
#include <boost/smart_ptr/detail/array_deleter.hpp>
#include <boost/smart_ptr/detail/array_traits.hpp>
#include <boost/smart_ptr/detail/sp_if_array.hpp>

namespace boost {
    template<typename T, typename A>
    inline typename boost::detail::sp_if_array<T>::type
    allocate_shared(const A& allocator, std::size_t size) {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        T1* p1 = 0;
        T2* p2 = 0;
        std::size_t n1 = size * boost::detail::array_total<T1>::size;
        boost::detail::allocate_array_helper<A, T2[]> a1(allocator, n1, &p2);
        boost::detail::array_deleter<T2[]> d1(n1);
        boost::shared_ptr<T> s1(p1, d1, a1);
        typedef boost::detail::array_deleter<T2[]>* D2;
        p1 = reinterpret_cast<T1*>(p2);
        D2 d2 = static_cast<D2>(s1._internal_get_untyped_deleter());
        d2->init(p2);
        return boost::shared_ptr<T>(s1, p1);
    }

    template<typename T, typename A>
    inline typename boost::detail::sp_if_size_array<T>::type
    allocate_shared(const A& allocator) {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        enum {
            N = boost::detail::array_total<T>::size
        };
        T1* p1 = 0;
        T2* p2 = 0;
        boost::detail::allocate_array_helper<A, T2[N]> a1(allocator, &p2);
        boost::detail::array_deleter<T2[N]> d1;
        boost::shared_ptr<T> s1(p1, d1, a1);
        typedef boost::detail::array_deleter<T2[N]>* D2;
        p1 = reinterpret_cast<T1*>(p2);
        D2 d2 = static_cast<D2>(s1._internal_get_untyped_deleter());
        d2->init(p2);
        return boost::shared_ptr<T>(s1, p1);
    }

    template<typename T, typename A>
    inline typename boost::detail::sp_if_array<T>::type
    allocate_shared(const A& allocator, std::size_t size,
        const typename boost::detail::array_inner<T>::type& list) {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        typedef const T2 T3;
        enum {
            M = boost::detail::array_total<T1>::size
        };
        T1* p1 = 0;
        T2* p2 = 0;
        T3* p3 = 0;
        std::size_t n1 = M * size;
        boost::detail::allocate_array_helper<A, T2[]> a1(allocator, n1, &p2);
        boost::detail::array_deleter<T2[]> d1(n1);
        boost::shared_ptr<T> s1(p1, d1, a1);
        typedef boost::detail::array_deleter<T2[]>* D2;
        p3 = reinterpret_cast<T3*>(&list);
        p1 = reinterpret_cast<T1*>(p2);
        D2 d2 = static_cast<D2>(s1._internal_get_untyped_deleter());
        d2->template init<M>(p2, p3);
        return boost::shared_ptr<T>(s1, p1);
    }

    template<typename T, typename A>
    inline typename boost::detail::sp_if_size_array<T>::type
    allocate_shared(const A& allocator,
        const typename boost::detail::array_inner<T>::type& list) {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        typedef const T2 T3;
        enum {
            M = boost::detail::array_total<T1>::size,
            N = boost::detail::array_total<T>::size
        };
        T1* p1 = 0;
        T2* p2 = 0;
        T3* p3 = 0;
        boost::detail::allocate_array_helper<A, T2[N]> a1(allocator, &p2);
        boost::detail::array_deleter<T2[N]> d1;
        boost::shared_ptr<T> s1(p1, d1, a1);
        typedef boost::detail::array_deleter<T2[N]>* D2;
        p3 = reinterpret_cast<T3*>(&list);
        p1 = reinterpret_cast<T1*>(p2);
        D2 d2 = static_cast<D2>(s1._internal_get_untyped_deleter());
        d2->template init<M>(p2, p3);
        return boost::shared_ptr<T>(s1, p1);
    }

    template<typename T, typename A>
    inline typename boost::detail::sp_if_array<T>::type
    allocate_shared_noinit(const A& allocator, std::size_t size) {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        T1* p1 = 0;
        T2* p2 = 0;
        std::size_t n1 = size * boost::detail::array_total<T1>::size;
        boost::detail::allocate_array_helper<A, T2[]> a1(allocator, n1, &p2);
        boost::detail::array_deleter<T2[]> d1(n1);
        boost::shared_ptr<T> s1(p1, d1, a1);
        typedef boost::detail::array_deleter<T2[]>* D2;
        p1 = reinterpret_cast<T1*>(p2);
        D2 d2 = static_cast<D2>(s1._internal_get_untyped_deleter());
        d2->noinit(p2);
        return boost::shared_ptr<T>(s1, p1);
    }

    template<typename T, typename A>
    inline typename boost::detail::sp_if_size_array<T>::type
    allocate_shared_noinit(const A& allocator) {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        enum {
            N = boost::detail::array_total<T>::size
        };
        T1* p1 = 0;
        T2* p2 = 0;
        boost::detail::allocate_array_helper<A, T2[N]> a1(allocator, &p2);
        boost::detail::array_deleter<T2[N]> d1;
        boost::shared_ptr<T> s1(p1, d1, a1);
        typedef boost::detail::array_deleter<T2[N]>* D2;
        p1 = reinterpret_cast<T1*>(p2);
        D2 d2 = static_cast<D2>(s1._internal_get_untyped_deleter());
        d2->noinit(p2);
        return boost::shared_ptr<T>(s1, p1);
    }
}

#endif
