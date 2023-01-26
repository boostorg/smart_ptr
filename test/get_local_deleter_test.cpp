//
// get_local_deleter_test.cpp
//
// Copyright 2002, 2017 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/smart_ptr/local_shared_ptr.hpp>
#include <boost/core/lightweight_test.hpp>

struct deleter
{
    int data;

    deleter(): data(0)
    {
    }

    void operator()(void *)
    {
        BOOST_TEST(data == 17041);
    }
};

struct deleter2
{
};

struct X
{
};

int main()
{
    {
        boost::local_shared_ptr<X> p;

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
        boost::local_shared_ptr<X> p(new X);

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
        X x;
        boost::local_shared_ptr<X> p(&x, deleter());

        BOOST_TEST(boost::get_deleter<void>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<void const>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<int>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<int const>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<X>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<X const>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<deleter2>(p) == BOOST_NULLPTR);
        BOOST_TEST(boost::get_deleter<deleter2 const>(p) == BOOST_NULLPTR);

        deleter * q = boost::get_deleter<deleter>(p);

        BOOST_TEST(q != BOOST_NULLPTR);
        BOOST_TEST(q->data == 0);

        q->data = 17041;

        deleter const * r = boost::get_deleter<deleter const>(p);

        BOOST_TEST(r == q);
        BOOST_TEST(r->data == 17041);
    }

    return boost::report_errors();
}
