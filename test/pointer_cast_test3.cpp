//
//  pointer_cast_test3.cpp - a test for unique_ptr casts
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

// This test fails on msvc-10.0, 11.0, 12.0 because
// their unique_ptr implementation can't compile the
// initialization of p1

static void test_const_cast()
{
    {
        std::unique_ptr<int const[]> p1( new int[ 1 ] );
        int const * q1 = p1.get();

        std::unique_ptr<int[]> p2 = boost::const_pointer_cast<int[]>( std::move( p1 ) );

        BOOST_TEST( p1.get() == 0 );
        BOOST_TEST_EQ( p2.get(), q1 );
    }
}

int main()
{
    test_const_cast();
    return boost::report_errors();
}

#endif
