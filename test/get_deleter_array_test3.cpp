//
// get_deleter_test3.cpp
//
// Copyright 2002, 2017 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/core/lightweight_test.hpp>

struct deleter
{
};

struct deleter2;

struct X
{
};

int main()
{
    {
        boost::shared_ptr<X[]> p = boost::make_shared<X[]>( 1 );

        BOOST_TEST(boost::get_deleter<void>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<void const>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<int>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<int const>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<X>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<X const>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<deleter>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<deleter const>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<deleter2>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<deleter2 const>(p) == BOOST_NULLPTR);
    }

    {
        boost::shared_ptr<X[1]> p = boost::make_shared<X[1]>();

        BOOST_TEST(boost::get_deleter<void>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<void const>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<int>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<int const>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<X>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<X const>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<deleter>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<deleter const>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<deleter2>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<deleter2 const>(p) == BOOST_NULLPTR);
    }

    return boost::report_errors();
}

struct deleter2
{
};
