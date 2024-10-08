//
// scoped_array_eq_fail.cpp - a negative test for "p == q"
//
// Copyright (c) 2008 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/scoped_array.hpp>

int main()
{
    boost::scoped_array<int> p, q;
    p == q; // must fail
    return 0;
}
