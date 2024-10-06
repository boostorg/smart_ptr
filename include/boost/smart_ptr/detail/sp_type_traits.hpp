#ifndef BOOST_SMART_PTR_DETAIL_SP_TYPE_TRAITS_HPP_INCLUDED
#define BOOST_SMART_PTR_DETAIL_SP_TYPE_TRAITS_HPP_INCLUDED

// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <type_traits>

namespace boost
{
namespace detail
{

// std::is_bounded_array (C++20)

template<class T> struct sp_is_bounded_array: std::false_type
{
};

template<class T, std::size_t N> struct sp_is_bounded_array< T[N] >: std::true_type
{
};

} // namespace detail
} // namespace boost

#endif  // #ifndef BOOST_SMART_PTR_DETAIL_SP_TYPE_TRAITS_HPP_INCLUDED
