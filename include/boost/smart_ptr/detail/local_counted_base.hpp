#ifndef BOOST_SMART_PTR_DETAIL_LOCAL_COUNTED_BASE_HPP_INCLUDED
#define BOOST_SMART_PTR_DETAIL_LOCAL_COUNTED_BASE_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//  detail/local_counted_base.hpp
//
//  Copyright 2017 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/ for documentation.

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/config.hpp>
#include <utility>
#include <climits>

namespace boost
{

namespace detail
{

class local_counted_base
{
private:

    local_counted_base( local_counted_base const & );
    local_counted_base & operator= ( local_counted_base const & );

private:

    // not 'int' or 'unsigned' to avoid aliasing and enable optimizations
    enum count_type { min_ = 0, initial_ = 1, max_ = UINT_MAX };

    count_type local_use_count_;

public:

    BOOST_CONSTEXPR local_counted_base() BOOST_SP_NOEXCEPT: local_use_count_( initial_ )
    {
    }

    virtual ~local_counted_base() /*BOOST_NOEXCEPT*/
    {
    }

    void add_ref()
    {
#if defined( __has_builtin )
# if __has_builtin( __builtin_assume )

        __builtin_assume( local_use_count_ >= 1 );

# endif
#endif

        local_use_count_ = static_cast<count_type>( local_use_count_ + 1 );
    }

    void release()
    {
        local_use_count_ = static_cast<count_type>( local_use_count_ - 1 );

        if( local_use_count_ == 0 )
        {
            delete this;
        }
    }

    long local_use_count() const BOOST_SP_NOEXCEPT
    {
        return local_use_count_;
    }
};

class local_counted_impl: public local_counted_base
{
private:

    boost::shared_ptr<void const volatile> pn_;

public:

    template<class T> explicit local_counted_impl( boost::shared_ptr<T> const& pn ): pn_( pn )
    {
    }

#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

    template<class T> explicit local_counted_impl( boost::shared_ptr<T>&& pn ): pn_( std::move(pn) )
    {
    }

#endif
};

} // namespace detail

} // namespace boost

#endif  // #ifndef BOOST_SMART_PTR_DETAIL_LOCAL_COUNTED_BASE_HPP_INCLUDED
