/*
 * Copyright (c) 2012-2014 Glen Joseph Fernandes
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_SMART_PTR_MAKE_SHARED_ARRAY_HPP
#define BOOST_SMART_PTR_MAKE_SHARED_ARRAY_HPP

#include <boost/smart_ptr/detail/array_allocator.hpp>
#include <boost/smart_ptr/detail/array_deleter.hpp>
#include <boost/smart_ptr/detail/sp_if_array.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost {
    template<typename T>
    inline typename boost::detail::sp_if_array<T>::type
    make_shared(std::size_t size) {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        typedef typename boost::remove_cv<T2>::type T3;
        typedef boost::detail::ms_allocator<T3[]> A1;
        typedef boost::detail::ms_deleter<T3[]> D1;
        T1* p1 = 0;
        T3* p2 = 0;
        std::size_t n1 = size * boost::detail::array_total<T1>::size;
        D1 d1(n1);
        A1 a1(n1, &p2);
        boost::shared_ptr<T> s1(p1, d1, a1);
        p1 = reinterpret_cast<T1*>(p2);
        boost::detail::ms_init(p2, n1);
        D1* d2 = static_cast<D1*>(s1._internal_get_untyped_deleter());
        d2->set(p2);
        return boost::shared_ptr<T>(s1, p1);
    }

    template<typename T>
    inline typename boost::detail::sp_if_size_array<T>::type
    make_shared() {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        typedef typename boost::remove_cv<T2>::type T3;
        enum {
            N = boost::detail::array_total<T>::size
        };
        typedef boost::detail::ms_allocator<T3[N]> A1;
        typedef boost::detail::ms_deleter<T3[N]> D1;
        T1* p1 = 0;
        T3* p2 = 0;
        D1 d1;
        A1 a1(&p2);
        boost::shared_ptr<T> s1(p1, d1, a1);
        p1 = reinterpret_cast<T1*>(p2);
        boost::detail::ms_init(p2, N);
        D1* d2 = static_cast<D1*>(s1._internal_get_untyped_deleter());
        d2->set(p2);
        return boost::shared_ptr<T>(s1, p1);
    }

    template<typename T>
    inline typename boost::detail::sp_if_array<T>::type
    make_shared(std::size_t size,
        const typename boost::detail::array_inner<T>::type& value) {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        typedef typename boost::remove_cv<T2>::type T3;
        typedef const T2 T4;
        typedef boost::detail::ms_allocator<T3[]> A1;
        typedef boost::detail::ms_deleter<T3[]> D1;
        enum {
            M = boost::detail::array_total<T1>::size
        };
        T1* p1 = 0;
        T3* p2 = 0;
        T4* p3 = reinterpret_cast<T4*>(&value);
        std::size_t n1 = M * size;
        D1 d1(n1);
        A1 a1(n1, &p2);
        boost::shared_ptr<T> s1(p1, d1, a1);
        p1 = reinterpret_cast<T1*>(p2);
        boost::detail::ms_init<T3, M>(p2, n1, p3);
        D1* d2 = static_cast<D1*>(s1._internal_get_untyped_deleter());
        d2->set(p2);
        return boost::shared_ptr<T>(s1, p1);
    }

    template<typename T>
    inline typename boost::detail::sp_if_size_array<T>::type
    make_shared(const typename boost::detail::array_inner<T>::type& value) {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        typedef typename boost::remove_cv<T2>::type T3;
        typedef const T2 T4;
        enum {
            M = boost::detail::array_total<T1>::size,
            N = boost::detail::array_total<T>::size
        };
        typedef boost::detail::ms_allocator<T3[N]> A1;
        typedef boost::detail::ms_deleter<T3[N]> D1;
        T1* p1 = 0;
        T3* p2 = 0;
        T4* p3 = reinterpret_cast<T4*>(&value);
        D1 d1;
        A1 a1(&p2);
        boost::shared_ptr<T> s1(p1, d1, a1);
        p1 = reinterpret_cast<T1*>(p2);
        boost::detail::ms_init<T3, M>(p2, N, p3);
        D1* d2 = static_cast<D1*>(s1._internal_get_untyped_deleter());
        d2->set(p2);
        return boost::shared_ptr<T>(s1, p1);
    }

    template<typename T>
    inline typename boost::detail::sp_if_array<T>::type
    make_shared_noinit(std::size_t size) {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        typedef typename boost::remove_cv<T2>::type T3;
        typedef boost::detail::ms_allocator<T3[]> A1;
        typedef boost::detail::ms_deleter<T3[]> D1;
        T1* p1 = 0;
        T3* p2 = 0;
        std::size_t n1 = size * boost::detail::array_total<T1>::size;
        D1 d1(n1);
        A1 a1(n1, &p2);
        boost::shared_ptr<T> s1(p1, d1, a1);
        p1 = reinterpret_cast<T1*>(p2);
        boost::detail::ms_noinit(p2, n1);
        D1* d2 = static_cast<D1*>(s1._internal_get_untyped_deleter());
        d2->set(p2);
        return boost::shared_ptr<T>(s1, p1);
    }

    template<typename T>
    inline typename boost::detail::sp_if_size_array<T>::type
    make_shared_noinit() {
        typedef typename boost::detail::array_inner<T>::type T1;
        typedef typename boost::detail::array_base<T1>::type T2;
        typedef typename boost::remove_cv<T2>::type T3;
        enum {
            N = boost::detail::array_total<T>::size
        };
        typedef boost::detail::ms_allocator<T3[N]> A1;
        typedef boost::detail::ms_deleter<T3[N]> D1;
        T1* p1 = 0;
        T3* p2 = 0;
        D1 d1;
        A1 a1(&p2);
        boost::shared_ptr<T> s1(p1, d1, a1);
        p1 = reinterpret_cast<T1*>(p2);
        boost::detail::ms_noinit(p2, N);
        D1* d2 = static_cast<D1*>(s1._internal_get_untyped_deleter());
        d2->set(p2);
        return boost::shared_ptr<T>(s1, p1);
    }
}

#endif
