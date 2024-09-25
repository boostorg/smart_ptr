//  This header intentionally has no include guards.
//
//  Copyright (c) 2001-2009, 2012 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

    explicit operator bool () const BOOST_SP_NOEXCEPT
    {
        return px != 0;
    }

    // operator! is redundant, but some compilers need it
    bool operator! () const BOOST_SP_NOEXCEPT
    {
        return px == 0;
    }
