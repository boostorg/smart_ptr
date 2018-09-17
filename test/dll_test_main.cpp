// Copyright 2018 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/shared_ptr.hpp>
#include <boost/core/lightweight_test.hpp>

boost::shared_ptr<int> dll_test();

int main()
{
    boost::shared_ptr<int> p1 = dll_test();

    BOOST_TEST_EQ( *p1, 42 );

    return boost::report_errors();
}
