#ifndef BOOST_SMART_PTR_MAKE_LOCAL_SHARED_OBJECT_HPP_INCLUDED
#define BOOST_SMART_PTR_MAKE_LOCAL_SHARED_OBJECT_HPP_INCLUDED

//  make_local_shared_object.hpp
//
//  Copyright 2017 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  See http://www.boost.org/libs/smart_ptr/ for documentation.

#include <boost/smart_ptr/local_shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/config.hpp>
#include <utility>
#include <cstddef>

namespace boost
{

namespace detail
{

template<class T> struct lsp_if_not_array
{
    typedef boost::local_shared_ptr<T> type;
};

template<class T> struct lsp_if_not_array<T[]>
{
};

template<class T, std::size_t N> struct lsp_if_not_array<T[N]>
{
};

} // namespace detail

template<class T, class... Args> typename boost::detail::lsp_if_not_array<T>::type make_local_shared( Args&&... args )
{
    return boost::make_shared<T>( std::forward<Args>(args)... );
}

template<class T> typename boost::detail::lsp_if_not_array<T>::type make_local_shared_noinit()
{
    return boost::make_shared_noinit<T>();
}

template<class T, class A, class... Args> typename boost::detail::lsp_if_not_array<T>::type allocate_local_shared( A const & a, Args&&... args )
{
    return boost::allocate_shared<T>( a, std::forward<Args>(args)... );
}

template<class T, class A> typename boost::detail::lsp_if_not_array<T>::type allocate_local_shared_noinit( A const & a )
{
    return boost::allocate_shared_noinit<T>( a );
}

} // namespace boost

#endif // #ifndef BOOST_SMART_PTR_MAKE_SHARED_OBJECT_HPP_INCLUDED
