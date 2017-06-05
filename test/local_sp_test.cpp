//
// local_sp_test.cpp
//
// Copyright 2002, 2003, 2017 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/smart_ptr/local_shared_ptr.hpp>
#include <boost/core/lightweight_test.hpp>

struct X
{
    static long instances;

    X()
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }

private:

    X( X const & );
    X & operator=( X const & );
};

long X::instances = 0;

class incomplete;

template<class T> static void test_empty( boost::local_shared_ptr<T> const & p )
{
    BOOST_TEST( p? false: true );
    BOOST_TEST( !p );
    BOOST_TEST( p.get() == 0 );
    BOOST_TEST( p.local_use_count() == 0 );
}

static void default_constructor()
{
    {
        boost::local_shared_ptr<int> p;
        test_empty( p );
    }

    {
        boost::local_shared_ptr<void> p;
        test_empty( p );
    }

    {
        boost::local_shared_ptr<incomplete> p;
        test_empty( p );
    }

    BOOST_TEST( X::instances == 0 );

    {
        boost::local_shared_ptr<X> p;

        BOOST_TEST( X::instances == 0 );

        test_empty( p );
    }
}

static void nullptr_constructor()
{
#if !defined( BOOST_NO_CXX11_NULLPTR )

    {
        boost::local_shared_ptr<int> p( nullptr );
        test_empty( p );
    }

    {
        boost::local_shared_ptr<void> p( nullptr );
        test_empty( p );
    }

    {
        boost::local_shared_ptr<incomplete> p( nullptr );
        test_empty( p );
    }

    BOOST_TEST( X::instances == 0 );

    {
        boost::local_shared_ptr<X> p( nullptr );

        BOOST_TEST( X::instances == 0 );

        test_empty( p );
    }

#endif
}

template<class T, class U> static void pc0_test_()
{
    boost::local_shared_ptr<T> p( static_cast<U*>( 0 ) );

    BOOST_TEST( p? false: true );
    BOOST_TEST( !p );
    BOOST_TEST( p.get() == 0 );
    BOOST_TEST( p.local_use_count() == 1 );
}

template<class T> static void pc0_test()
{
    pc0_test_<T, T>();
    pc0_test_<T const, T const>();
    pc0_test_<T volatile, T volatile>();
    pc0_test_<T const volatile, T const volatile>();

    pc0_test_<T const, T>();
    pc0_test_<T volatile, T>();
    pc0_test_<T const volatile, T>();

    pc0_test_<void, T>();
    pc0_test_<void const, T>();
    pc0_test_<void volatile, T>();
    pc0_test_<void const volatile, T>();
}

template<class T, class U> static void pc1_test_()
{
    boost::local_shared_ptr<T> p( new U() );

    BOOST_TEST( p? true: false );
    BOOST_TEST( !!p );
    BOOST_TEST( p.get() != 0 );
    BOOST_TEST( p.local_use_count() == 1 );
}

template<class T> static void pc1_test()
{
    pc1_test_<T, T>();
    pc1_test_<T const, T const>();
    pc1_test_<T volatile, T volatile>();
    pc1_test_<T const volatile, T const volatile>();

    pc1_test_<T const, T>();
    pc1_test_<T volatile, T>();
    pc1_test_<T const volatile, T>();

    pc1_test_<void, T>();
    pc1_test_<void const, T>();
    pc1_test_<void volatile, T>();
    pc1_test_<void const volatile, T>();
}

static void pointer_constructor()
{
    pc0_test<int>();
    pc0_test<X>();

    pc1_test<int>();

    BOOST_TEST( X::instances == 0 );

    pc1_test<X>();

    BOOST_TEST( X::instances == 0 );
}

int m = 0;

void deleter2( int * p )
{
    BOOST_TEST( p == &m );
    ++*p;
}

template<class T> static void deleter2_test_()
{
    {
        m = 0;
        boost::local_shared_ptr<T> p( &m, deleter2 );

        BOOST_TEST( p? true: false );
        BOOST_TEST( !!p );
        BOOST_TEST( p.get() == &m );
        BOOST_TEST( p.local_use_count() == 1 );
    }

    BOOST_TEST( m == 1 );
}

static void deleter_constructor()
{
    deleter2_test_<int>();
    deleter2_test_<int const>();
    deleter2_test_<int volatile>();
    deleter2_test_<int const volatile>();

    deleter2_test_<void>();
    deleter2_test_<void const>();
    deleter2_test_<void volatile>();
    deleter2_test_<void const volatile>();
}

template<class T> static void empty_copy_test()
{
    boost::local_shared_ptr<T> p1;
    test_empty( p1 );

    boost::local_shared_ptr<T> p2( p1 );
    test_empty( p2 );

    boost::local_shared_ptr<T const> p3( p1 );
    test_empty( p3 );

    boost::local_shared_ptr<void> p4( p1 );
    test_empty( p4 );

    boost::local_shared_ptr<void const> p5( p3 );
    test_empty( p5 );
}

template<class T, class U> static void test_nonempty_copy( boost::local_shared_ptr<U> const & p1 )
{
    long k = p1.local_use_count();

    {
        boost::local_shared_ptr<T> p2( p1 );

        BOOST_TEST( p2.get() == p1.get() );
        BOOST_TEST( p2.local_use_count() == p1.local_use_count() );
        BOOST_TEST( p2.local_use_count() == k + 1 );
    }

    BOOST_TEST( p1.local_use_count() == k );
}

template<class T> static void null_copy_test()
{
    boost::local_shared_ptr<T> p1( static_cast<T*>(0) );

    test_nonempty_copy<T>( p1 );
    test_nonempty_copy<T const>( p1 );
    test_nonempty_copy<T volatile>( p1 );
    test_nonempty_copy<T const volatile>( p1 );
    test_nonempty_copy<void>( p1 );
    test_nonempty_copy<void const>( p1 );
    test_nonempty_copy<void volatile>( p1 );
    test_nonempty_copy<void const volatile>( p1 );
}

template<class T> static void new_copy_test()
{
    boost::local_shared_ptr<T> p1( new T() );

    test_nonempty_copy<T>( p1 );
    test_nonempty_copy<T const>( p1 );
    test_nonempty_copy<T volatile>( p1 );
    test_nonempty_copy<T const volatile>( p1 );
    test_nonempty_copy<void>( p1 );
    test_nonempty_copy<void const>( p1 );
    test_nonempty_copy<void volatile>( p1 );
    test_nonempty_copy<void const volatile>( p1 );
}

static void copy_constructor()
{
    empty_copy_test<int>();
    empty_copy_test<incomplete>();
    empty_copy_test<X>();
    BOOST_TEST( X::instances == 0 );

    null_copy_test<int>();
    null_copy_test<X>();
    BOOST_TEST( X::instances == 0 );

    new_copy_test<int>();
    new_copy_test<X>();
    BOOST_TEST( X::instances == 0 );
}

template<class T, class U> void test_aliasing_( boost::local_shared_ptr<T> const & p1, U * p2 )
{
    boost::local_shared_ptr<U> p3( p1, p2 );

    BOOST_TEST( p3.get() == p2 );
    BOOST_TEST( p3.local_use_count() == p1.local_use_count() );
    BOOST_TEST( !p3.owner_before( p1 ) && !p1.owner_before( p3 ) );
}

template<class T, class U> void test_01_aliasing_()
{
    U u;
    boost::local_shared_ptr<T> p1;

    test_aliasing_( p1, &u );
}

template<class T, class U> void test_01_aliasing()
{
    test_01_aliasing_<T, U>();
    test_01_aliasing_<T const, U>();
    test_01_aliasing_<T volatile, U>();
    test_01_aliasing_<T const volatile, U>();

    test_01_aliasing_<T, U volatile>();
    test_01_aliasing_<T const, U volatile>();
    test_01_aliasing_<T volatile, U volatile>();
    test_01_aliasing_<T const volatile, U volatile>();
}

template<class T, class U> void test_10_aliasing_()
{
    boost::local_shared_ptr<T> p1( new T() );
    test_aliasing_( p1, static_cast<U*>(0) );
}

template<class T, class U> void test_10_aliasing()
{
    test_10_aliasing_<T, U>();
    test_10_aliasing_<T const, U>();
    test_10_aliasing_<T volatile, U>();
    test_10_aliasing_<T const volatile, U>();

    test_10_aliasing_<T, U const>();
    test_10_aliasing_<T const, U const>();
    test_10_aliasing_<T volatile, U const>();
    test_10_aliasing_<T const volatile, U const>();

    test_10_aliasing_<T, U volatile>();
    test_10_aliasing_<T const, U volatile>();
    test_10_aliasing_<T volatile, U volatile>();
    test_10_aliasing_<T const volatile, U volatile>();

    test_10_aliasing_<T, U const volatile>();
    test_10_aliasing_<T const, U const volatile>();
    test_10_aliasing_<T volatile, U const volatile>();
    test_10_aliasing_<T const volatile, U const volatile>();
}

template<class T, class U> void test_11_aliasing_()
{
    U u;
    boost::local_shared_ptr<T> p1( new T() );

    test_aliasing_( p1, &u );
}

template<class T, class U> void test_11_aliasing()
{
    test_11_aliasing_<T, U>();
    test_11_aliasing_<T const, U>();
    test_11_aliasing_<T volatile, U>();
    test_11_aliasing_<T const volatile, U>();

    test_11_aliasing_<T, U volatile>();
    test_11_aliasing_<T const, U volatile>();
    test_11_aliasing_<T volatile, U volatile>();
    test_11_aliasing_<T const volatile, U volatile>();
}

static void aliasing_constructor()
{
    test_01_aliasing<int, int>();
    test_10_aliasing<int, int>();
    test_11_aliasing<int, int>();

    test_01_aliasing<void, int>();

    test_10_aliasing<int, void>();

    test_10_aliasing<int, incomplete>();

    test_01_aliasing<X, X>();
    BOOST_TEST( X::instances == 0 );

    test_10_aliasing<X, X>();
    BOOST_TEST( X::instances == 0 );

    test_11_aliasing<X, X>();
    BOOST_TEST( X::instances == 0 );

    test_01_aliasing<int, X>();
    BOOST_TEST( X::instances == 0 );

    test_10_aliasing<int, X>();
    BOOST_TEST( X::instances == 0 );

    test_11_aliasing<int, X>();
    BOOST_TEST( X::instances == 0 );

    test_01_aliasing<X, int>();
    BOOST_TEST( X::instances == 0 );

    test_10_aliasing<X, int>();
    BOOST_TEST( X::instances == 0 );

    test_11_aliasing<X, int>();
    BOOST_TEST( X::instances == 0 );

    test_01_aliasing<void, X>();
    BOOST_TEST( X::instances == 0 );

    test_10_aliasing<X, void>();
    BOOST_TEST( X::instances == 0 );

    test_10_aliasing<X, incomplete>();
    BOOST_TEST( X::instances == 0 );
}

int main()
{
    default_constructor();
    nullptr_constructor();
    pointer_constructor();
    deleter_constructor();
    copy_constructor();
    aliasing_constructor();

    return boost::report_errors();
}
