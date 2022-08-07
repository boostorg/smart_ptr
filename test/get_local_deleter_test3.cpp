//
// get_local_deleter_test3.cpp
//
// Copyright 2002, 2017 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>

#if defined( BOOST_NO_CXX11_RVALUE_REFERENCES ) || defined( BOOST_NO_CXX11_VARIADIC_TEMPLATES )

int main()
{
}

#else

#include <boost/smart_ptr/local_shared_ptr.hpp>
#include <boost/smart_ptr/make_local_shared.hpp>
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
        boost::local_shared_ptr<X> p = boost::make_local_shared<X>();

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

#endif
