//
//  pointer_cast_test2.cpp - a test for unique_ptr casts
//
//  Copyright 2016 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/config.hpp>

#if defined( BOOST_NO_CXX11_SMART_PTR )

int main()
{
    return 0;
}

#else

#include <boost/pointer_cast.hpp>
#include <boost/core/lightweight_test.hpp>
#include <memory>

struct B
{
    virtual ~B()
    {
    }
};

struct D: B
{
};

static void test_static_cast()
{
    {
        std::unique_ptr<int> p1( new int );
        int * q1 = p1.get();

        std::unique_ptr<int> p2 = boost::static_pointer_cast<int>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }

    {
        std::unique_ptr<int> p1( new int );
        int * q1 = p1.get();

        std::unique_ptr<int const> p2 = boost::static_pointer_cast<int const>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }

    {
        std::unique_ptr<int[]> p1( new int[ 1 ] );
        int * q1 = p1.get();

        std::unique_ptr<int[]> p2 = boost::static_pointer_cast<int[]>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }

    {
        std::unique_ptr<int[]> p1( new int[ 1 ] );
        int * q1 = p1.get();

        std::unique_ptr<int const[]> p2 = boost::static_pointer_cast<int const[]>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }

    {
        std::unique_ptr<D> p1( new D );
        D * q1 = p1.get();

        std::unique_ptr<B> p2 = boost::static_pointer_cast<B>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }

    {
        std::unique_ptr<B> p1( new D );
        B * q1 = p1.get();

        std::unique_ptr<D> p2 = boost::static_pointer_cast<D>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }
}

static void test_const_cast()
{
    {
        std::unique_ptr<int const> p1( new int );
        int const * q1 = p1.get();

        std::unique_ptr<int> p2 = boost::const_pointer_cast<int>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }

    {
        std::unique_ptr<int> p1( new int );
        int * q1 = p1.get();

        std::unique_ptr<int const> p2 = boost::const_pointer_cast<int const>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }

#if !defined( BOOST_MSVC ) || BOOST_MSVC >= 1900

    {
        std::unique_ptr<int const[]> p1( new int[ 1 ] );
        int const * q1 = p1.get();

        std::unique_ptr<int[]> p2 = boost::const_pointer_cast<int[]>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }

#endif

	{
        std::unique_ptr<int[]> p1( new int[ 1 ] );
        int * q1 = p1.get();

        std::unique_ptr<int const[]> p2 = boost::const_pointer_cast<int const[]>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }
}

static void test_dynamic_cast()
{
    {
        std::unique_ptr<D> p1( new D );
        D * q1 = p1.get();

        std::unique_ptr<B> p2 = boost::dynamic_pointer_cast<B>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }

    {
        std::unique_ptr<B> p1( new D );
        B * q1 = p1.get();

        std::unique_ptr<D> p2 = boost::dynamic_pointer_cast<D>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }

    {
        std::unique_ptr<B> p1( new B );
        B * q1 = p1.get();

        std::unique_ptr<D> p2 = boost::dynamic_pointer_cast<D>( std::move( p1 ) );

        BOOST_TEST( p2.get() == 0 );
        BOOST_TEST_EQ( p1.get(), q1 );
    }
}

static void test_reinterpret_cast()
{
    {
        std::unique_ptr<int> p1( new int );
        void * q1 = p1.get();

        std::unique_ptr<char> p2 = boost::reinterpret_pointer_cast<char>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );

        p1 = boost::reinterpret_pointer_cast<int>( std::move( p2 ) );

        BOOST_TEST( p2.get() == 0 );
        BOOST_TEST_EQ( p1.get(), q1 );
    }

    {
        std::unique_ptr<int> p1( new int );
        void * q1 = p1.get();

        std::unique_ptr<char[]> p2 = boost::reinterpret_pointer_cast<char[]>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );

        p1 = boost::reinterpret_pointer_cast<int>( std::move( p2 ) );

        BOOST_TEST( p2.get() == 0 );
        BOOST_TEST_EQ( p1.get(), q1 );
    }

    {
        std::unique_ptr<int[]> p1( new int[ 1 ] );
        void * q1 = p1.get();

        std::unique_ptr<char[]> p2 = boost::reinterpret_pointer_cast<char[]>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );

        p1 = boost::reinterpret_pointer_cast<int[]>( std::move( p2 ) );

        BOOST_TEST( p2.get() == 0 );
        BOOST_TEST_EQ( p1.get(), q1 );
    }
}

int main()
{
    test_static_cast();
    test_const_cast();
    test_dynamic_cast();
    test_reinterpret_cast();

    return boost::report_errors();
}

#endif
