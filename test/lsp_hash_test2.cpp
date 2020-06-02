// Copyright 2011, 2020 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/smart_ptr/local_shared_ptr.hpp>
#include <boost/functional/hash.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config.hpp>
#include <functional>

#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)

int main() {}

#else

int main()
{
    {
        boost::hash< boost::local_shared_ptr<int> > hasher;
        std::hash< boost::local_shared_ptr<int> > hasher2;

        boost::local_shared_ptr<int> p1, p2( new int );

        BOOST_TEST_EQ( hasher( p1 ), hasher2( p1 ) );
        BOOST_TEST_EQ( hasher( p2 ), hasher2( p2 ) );
    }

    {
        boost::hash< boost::local_shared_ptr<int[]> > hasher;
        std::hash< boost::local_shared_ptr<int[]> > hasher2;

        boost::local_shared_ptr<int[]> p1, p2( new int[1] );

        BOOST_TEST_EQ( hasher( p1 ), hasher2( p1 ) );
        BOOST_TEST_EQ( hasher( p2 ), hasher2( p2 ) );
    }

    {
        boost::hash< boost::local_shared_ptr<int[1]> > hasher;
        std::hash< boost::local_shared_ptr<int[1]> > hasher2;

        boost::local_shared_ptr<int[1]> p1, p2( new int[1] );

        BOOST_TEST_EQ( hasher( p1 ), hasher2( p1 ) );
        BOOST_TEST_EQ( hasher( p2 ), hasher2( p2 ) );
    }

    return boost::report_errors();
}

#endif // #if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
