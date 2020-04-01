// Copyright 2020 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/enable_shared_from.hpp>

struct X: public boost::enable_shared_from
{
};

int main()
{
    boost::shared_ptr<X> px( new X );
    px->weak_from_this();
}
