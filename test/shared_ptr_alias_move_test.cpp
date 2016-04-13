#include <boost/config.hpp>

//  shared_ptr_alias_move_test.cpp
//
//  Copyright (c) 2007 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/detail/lightweight_test.hpp>
#include <boost/shared_ptr.hpp>
#include <utility>

#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

struct X
{
    static long instances;

    int v_;

    explicit X( int v ): v_( v )
    {
        ++instances;
    }

    ~X()
    {
        v_ = 0;
        --instances;
    }

private:
    X( X const & );
    X & operator=( X const & );
};

long X::instances = 0;

int main()
{
    BOOST_TEST( X::instances == 0 );

    {
        boost::shared_ptr< X > p( new X( 5 ) );
        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( p.unique() );
        BOOST_TEST( p->v_ == 5 );

        boost::shared_ptr< X > p2( std::move( p ), p.get() );
        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( p.get() == 0 );
        BOOST_TEST( p2.unique() );
        BOOST_TEST( p2->v_ == 5 );

        boost::shared_ptr< int const > p3( std::move( p2 ), &p2->v_ );
        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( p2.get() == 0 );
        BOOST_TEST( p3.unique() );
        BOOST_TEST( *p3 == 5 );

        p3.reset();
        BOOST_TEST( X::instances == 0 );
    }

    {
        boost::shared_ptr< X > p( new X( 5 ) );
        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( p.unique() );
        BOOST_TEST( p->v_ == 5 );

        {
            boost::shared_ptr< X > p2(p);
            BOOST_TEST( X::instances == 1 );
            BOOST_TEST( p.get() == p2.get() );
            BOOST_TEST( p.use_count() == 2 );
            BOOST_TEST( p2.use_count() == 2 );

            boost::shared_ptr< int const > p3( std::move( p2 ), &p2->v_ );
            BOOST_TEST( X::instances == 1 );
            BOOST_TEST( p.use_count() == 2 );
            BOOST_TEST( p2.use_count() == 0 );
            BOOST_TEST( p2.get() == 0 );
            BOOST_TEST( p3.use_count() == 2 );
            BOOST_TEST( p3.get() == &p->v_ );
        }

        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( p.unique() );
        BOOST_TEST( p->v_ == 5 );
    }

    return boost::report_errors();
}

#else // defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

int main()
{
    return 0;
}

#endif
