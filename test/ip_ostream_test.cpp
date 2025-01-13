// Copyright 2011, 2025 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/intrusive_ptr.hpp>
#include <boost/core/lightweight_test.hpp>
#include <sstream>

class base
{
private:

    int use_count_;

    base(base const &);
    base & operator=(base const &);

protected:

    base(): use_count_(0)
    {
    }

    virtual ~base()
    {
    }

public:

    long use_count() const
    {
        return use_count_;
    }

    inline friend void intrusive_ptr_add_ref(base * p)
    {
        ++p->use_count_;
    }

    inline friend void intrusive_ptr_release(base * p)
    {
        if(--p->use_count_ == 0) delete p;
    }
};

struct X: public base
{
};

template<class T> std::string to_string( T const& t )
{
    std::ostringstream os;
    os << t;
    return os.str();
}

template<class T> std::wstring to_wstring( T const& t )
{
    std::wostringstream os;
    os << t;
    return os.str();
}

int main()
{
    boost::intrusive_ptr<X> p1, p2( new X );

    BOOST_TEST_EQ( to_string( p1 ), to_string( p1.get() ) );
    BOOST_TEST_EQ( to_string( p2 ), to_string( p2.get() ) );

    BOOST_TEST( to_wstring( p1 ) == to_wstring( p1.get() ) );
    BOOST_TEST( to_wstring( p2 ) == to_wstring( p2.get() ) );

    return boost::report_errors();
}
