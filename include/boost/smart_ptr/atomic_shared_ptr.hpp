#ifndef BOOST_SMART_PTR_ATOMIC_SHARED_PTR_HPP_INCLUDED
#define BOOST_SMART_PTR_ATOMIC_SHARED_PTR_HPP_INCLUDED

//
//  atomic_shared_ptr.hpp
//
//  Copyright 2017 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/ for documentation.
//

#include <boost/smart_ptr/shared_ptr.hpp>

namespace boost
{

template<class T> class atomic_shared_ptr
{
private:

    boost::shared_ptr<T> p_;

    atomic_shared_ptr(const atomic_shared_ptr&);
    atomic_shared_ptr& operator=(const atomic_shared_ptr&);

public:

    atomic_shared_ptr() BOOST_SP_NOEXCEPT
    {
    }

    BOOST_CONSTEXPR atomic_shared_ptr( shared_ptr<T> p ) BOOST_SP_NOEXCEPT: p_( p )
    {
    }

    atomic_shared_ptr& operator=( shared_ptr<T> r ) BOOST_SP_NOEXCEPT
    {
#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

        atomic_store( &p_, std::move( r ) );

#else

        atomic_store( &p_, r );

#endif
        return *this;
    }

    BOOST_CONSTEXPR bool is_lock_free() const BOOST_SP_NOEXCEPT
    {
        return false;
    }

    shared_ptr<T> load( int = 0 ) const BOOST_SP_NOEXCEPT
    {
        return atomic_load( &p_ );
    }

    operator shared_ptr<T>() const BOOST_SP_NOEXCEPT
    {
        return atomic_load( &p_ );
    }

    void store( shared_ptr<T> r, int = 0 ) BOOST_SP_NOEXCEPT
    {
#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

        atomic_store( &p_, std::move( r ) );

#else

        atomic_store( &p_, r );

#endif
    }

    shared_ptr<T> exchange( shared_ptr<T> r, int = 0 ) BOOST_SP_NOEXCEPT
    {
#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

        return atomic_exchange( &p_, std::move( r ) );

#else

        return atomic_exchange( &p_, r );

#endif
    }

    bool compare_exchange_weak( shared_ptr<T>& v, const shared_ptr<T>& w, int, int ) BOOST_SP_NOEXCEPT
    {
        return atomic_compare_exchange( &p_, &v, w );
    }

    bool compare_exchange_weak( shared_ptr<T>& v, const shared_ptr<T>& w, int = 0 ) BOOST_SP_NOEXCEPT
    {
        return atomic_compare_exchange( &p_, &v, w );
    }

    bool compare_exchange_strong( shared_ptr<T>& v, const shared_ptr<T>& w, int, int ) BOOST_SP_NOEXCEPT
    {
        return atomic_compare_exchange( &p_, &v, w );
    }

    bool compare_exchange_strong( shared_ptr<T>& v, const shared_ptr<T>& w, int = 0 ) BOOST_SP_NOEXCEPT
    {
        return atomic_compare_exchange( &p_, &v, w );
    }

#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

    bool compare_exchange_weak( shared_ptr<T>& v, shared_ptr<T>&& w, int,  int ) BOOST_SP_NOEXCEPT
    {
        return atomic_compare_exchange( &p_, &v, std::move( w ) );
    }

    bool compare_exchange_weak( shared_ptr<T>& v, shared_ptr<T>&& w, int = 0 ) BOOST_SP_NOEXCEPT
    {
        return atomic_compare_exchange( &p_, &v, std::move( w ) );
    }

    bool compare_exchange_strong( shared_ptr<T>& v, shared_ptr<T>&& w, int, int ) BOOST_SP_NOEXCEPT
    {
        return atomic_compare_exchange( &p_, &v, std::move( w ) );
    }

    bool compare_exchange_strong( shared_ptr<T>& v, shared_ptr<T>&& w, int = 0 ) BOOST_SP_NOEXCEPT
    {
        return atomic_compare_exchange( &p_, &v, std::move( w ) );
    }

#endif
};

} // namespace boost

#endif  // #ifndef BOOST_SMART_PTR_ATOMIC_SHARED_PTR_HPP_INCLUDED
