/*
Copyright 2019 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/smart_ptr/allocation_ptr.hpp>
#include <boost/core/default_allocator.hpp>
#include <boost/core/lightweight_test_trait.hpp>

struct scalar { };
struct unbounded_array { };
struct bounded_array { };

void test_pointer(scalar)
{
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<int,
        boost::default_allocator<void> >::pointer, int*);
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<const int,
        boost::default_allocator<void> >::pointer, int*);
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<const volatile int,
        boost::default_allocator<void> >::pointer, int*);
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<volatile int,
        boost::default_allocator<void> >::pointer, int*);
}

void test_pointer(unbounded_array)
{
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<int[],
        boost::default_allocator<void> >::pointer, int*);
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<const int[],
        boost::default_allocator<void> >::pointer, int*);
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<const volatile int[],
        boost::default_allocator<void> >::pointer, int*);
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<volatile int[],
        boost::default_allocator<void> >::pointer, int*);
}

void test_pointer(bounded_array)
{
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<int[5],
        boost::default_allocator<void> >::pointer, int*);
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<const int[5],
        boost::default_allocator<void> >::pointer, int*);
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<const volatile int[5],
        boost::default_allocator<void> >::pointer, int*);
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<volatile int[5],
        boost::default_allocator<void> >::pointer, int*);
}

void test_element_type(scalar)
{
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<int,
        boost::default_allocator<void> >::element_type, int);
}

void test_element_type(unbounded_array)
{
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<int[],
        boost::default_allocator<void> >::element_type, int);
}

void test_element_type(bounded_array)
{
    BOOST_TEST_TRAIT_SAME(boost::allocation_ptr<int[5],
        boost::default_allocator<void> >::element_type, int);
}

void test_default_construct(scalar)
{
    BOOST_TEST_NOT((boost::allocation_ptr<int,
        boost::default_allocator<void> >().get()));
}

void test_default_construct(unbounded_array)
{
    BOOST_TEST_NOT((boost::allocation_ptr<int[],
        boost::default_allocator<void> >().get()));
}

void test_default_construct(bounded_array)
{
    BOOST_TEST_NOT((boost::allocation_ptr<int[5],
        boost::default_allocator<void> >().get()));
}

#if !defined(BOOST_NO_CXX11_NULLPTR)
void test_nullptr_construct(scalar)
{
    BOOST_TEST_NOT((boost::allocation_ptr<int,
        boost::default_allocator<void> >(nullptr).get()));
}

void test_nullptr_construct(unbounded_array)
{
    BOOST_TEST_NOT((boost::allocation_ptr<int[],
        boost::default_allocator<void> >(nullptr).get()));
}

void test_nullptr_construct(bounded_array)
{
    BOOST_TEST_NOT((boost::allocation_ptr<int[5],
        boost::default_allocator<void> >(nullptr).get()));
}
#endif

void test_access(scalar)
{
    boost::default_allocator<int> a;
    int* v = a.allocate(1);
    ::new(v) int(5);
    BOOST_TEST_EQ((*boost::allocation_ptr<int,
        boost::default_allocator<void> >(v)), 5);
    BOOST_TEST_EQ((boost::allocation_ptr<int,
        boost::default_allocator<void> >(v).operator->()), v);
    a.deallocate(v, 1);
}

void test_access(unbounded_array)
{
    boost::default_allocator<int> a;
    int* v = a.allocate(3);
    ::new(v) int(5);
    boost::allocation_ptr<int[], boost::default_allocator<void> > p(v, 3);
    BOOST_TEST_EQ(p[0], 5);
    BOOST_TEST_EQ(p.size(), 3);
    a.deallocate(v, 3);
}

void test_access(bounded_array)
{
    boost::default_allocator<int> a;
    int* v = a.allocate(1);
    ::new(v) int(5);
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p(v);
    BOOST_TEST_EQ(p[0], 5);
    BOOST_TEST_EQ(p.size(), 3);
    a.deallocate(v, 1);
}

#if !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
void test_bool(scalar)
{
    BOOST_TEST_NOT((boost::allocation_ptr<int,
        boost::default_allocator<void> >()));
    boost::default_allocator<int> a;
    int* v = a.allocate(1);
    BOOST_TEST((boost::allocation_ptr<int,
        boost::default_allocator<void> >(v)));
    a.deallocate(v, 1);
}

void test_bool(unbounded_array)
{
    BOOST_TEST_NOT((boost::allocation_ptr<int[],
        boost::default_allocator<void> >()));
    boost::default_allocator<int> a;
    int* v = a.allocate(3);
    BOOST_TEST((boost::allocation_ptr<int[],
        boost::default_allocator<void> >(v, 3)));
    a.deallocate(v, 3);
}

void test_bool(bounded_array)
{
    BOOST_TEST_NOT((boost::allocation_ptr<int[3],
        boost::default_allocator<void> >()));
    boost::default_allocator<int> a;
    int* v = a.allocate(1);
    BOOST_TEST((boost::allocation_ptr<int[3],
        boost::default_allocator<void> >(v)));
    a.deallocate(v, 1);
}
#endif

void test_not(scalar)
{
    BOOST_TEST((!boost::allocation_ptr<int,
        boost::default_allocator<void> >()));
    boost::default_allocator<int> a;
    int* v = a.allocate(1);
    BOOST_TEST_NOT((!boost::allocation_ptr<int,
        boost::default_allocator<void> >(v)));
    a.deallocate(v, 1);
}

void test_not(unbounded_array)
{
    BOOST_TEST((!boost::allocation_ptr<int[],
        boost::default_allocator<void> >()));
    boost::default_allocator<int> a;
    int* v = a.allocate(3);
    BOOST_TEST_NOT((!boost::allocation_ptr<int[],
        boost::default_allocator<void> >(v, 3)));
    a.deallocate(v, 3);
}

void test_not(bounded_array)
{
    BOOST_TEST((!boost::allocation_ptr<int[3],
        boost::default_allocator<void> >()));
    boost::default_allocator<int> a;
    int* v = a.allocate(1);
    BOOST_TEST_NOT((!boost::allocation_ptr<int[3],
        boost::default_allocator<void> >(v)));
    a.deallocate(v, 1);
}

void test_equal(scalar)
{
    boost::allocation_ptr<int, boost::default_allocator<void> > p1;
    BOOST_TEST(p1 == p1);
    boost::default_allocator<int> a;
    int* v1 = a.allocate(1);
    boost::allocation_ptr<int, boost::default_allocator<void> > p2(v1);
    BOOST_TEST(p2 == p2);
    BOOST_TEST_NOT(p1 == p2);
    BOOST_TEST_NOT(p2 == p1);
    boost::allocation_ptr<int, boost::default_allocator<void> > p3(v1);
    BOOST_TEST(p3 == p2);
    BOOST_TEST(p2 == p3);
    int* v2 = a.allocate(1);
    boost::allocation_ptr<int, boost::default_allocator<void> > p4(v2);
    BOOST_TEST_NOT(p2 == p4);
    BOOST_TEST_NOT(p4 == p2);
    a.deallocate(v2, 1);
    a.deallocate(v1, 1);
}

void test_equal(unbounded_array)
{
    boost::allocation_ptr<int[], boost::default_allocator<void> > p1;
    BOOST_TEST(p1 == p1);
    boost::default_allocator<int> a;
    int* v1 = a.allocate(3);
    boost::allocation_ptr<int[], boost::default_allocator<void> > p2(v1, 3);
    BOOST_TEST(p2 == p2);
    BOOST_TEST_NOT(p1 == p2);
    BOOST_TEST_NOT(p2 == p1);
    boost::allocation_ptr<int[], boost::default_allocator<void> > p3(v1, 3);
    BOOST_TEST(p3 == p2);
    BOOST_TEST(p2 == p3);
    int* v2 = a.allocate(3);
    boost::allocation_ptr<int[], boost::default_allocator<void> > p4(v2, 3);
    BOOST_TEST_NOT(p2 == p4);
    BOOST_TEST_NOT(p4 == p2);
    a.deallocate(v2, 3);
    a.deallocate(v1, 3);
}

void test_equal(bounded_array)
{
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p1;
    BOOST_TEST(p1 == p1);
    boost::default_allocator<int> a;
    int* v1 = a.allocate(1);
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p2(v1);
    BOOST_TEST(p2 == p2);
    BOOST_TEST_NOT(p1 == p2);
    BOOST_TEST_NOT(p2 == p1);
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p3(v1);
    BOOST_TEST(p3 == p2);
    BOOST_TEST(p2 == p3);
    int* v2 = a.allocate(1);
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p4(v2);
    BOOST_TEST_NOT(p2 == p4);
    BOOST_TEST_NOT(p4 == p2);
    a.deallocate(v2, 1);
    a.deallocate(v1, 1);
}

void test_not_equal(scalar)
{
    boost::allocation_ptr<int, boost::default_allocator<void> > p1;
    BOOST_TEST_NOT(p1 != p1);
    boost::default_allocator<int> a;
    int* v1 = a.allocate(1);
    boost::allocation_ptr<int, boost::default_allocator<void> > p2(v1);
    BOOST_TEST_NOT(p2 != p2);
    BOOST_TEST(p1 != p2);
    BOOST_TEST(p2 != p1);
    boost::allocation_ptr<int, boost::default_allocator<void> > p3(v1);
    BOOST_TEST_NOT(p3 != p2);
    BOOST_TEST_NOT(p2 != p3);
    int* v2 = a.allocate(1);
    boost::allocation_ptr<int, boost::default_allocator<void> > p4(v2);
    BOOST_TEST(p2 != p4);
    BOOST_TEST(p4 != p2);
    a.deallocate(v2, 1);
    a.deallocate(v1, 1);
}

void test_not_equal(unbounded_array)
{
    boost::allocation_ptr<int[], boost::default_allocator<void> > p1;
    BOOST_TEST_NOT(p1 != p1);
    boost::default_allocator<int> a;
    int* v1 = a.allocate(3);
    boost::allocation_ptr<int[], boost::default_allocator<void> > p2(v1, 3);
    BOOST_TEST_NOT(p2 != p2);
    BOOST_TEST(p1 != p2);
    BOOST_TEST(p2 != p1);
    boost::allocation_ptr<int[], boost::default_allocator<void> > p3(v1, 3);
    BOOST_TEST_NOT(p3 != p2);
    BOOST_TEST_NOT(p2 != p3);
    int* v2 = a.allocate(3);
    boost::allocation_ptr<int[], boost::default_allocator<void> > p4(v2, 3);
    BOOST_TEST(p2 != p4);
    BOOST_TEST(p4 != p2);
    a.deallocate(v2, 3);
    a.deallocate(v1, 3);
}

void test_not_equal(bounded_array)
{
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p1;
    BOOST_TEST_NOT(p1 != p1);
    boost::default_allocator<int> a;
    int* v1 = a.allocate(1);
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p2(v1);
    BOOST_TEST_NOT(p2 != p2);
    BOOST_TEST(p1 != p2);
    BOOST_TEST(p2 != p1);
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p3(v1);
    BOOST_TEST_NOT(p3 != p2);
    BOOST_TEST_NOT(p2 != p3);
    int* v2 = a.allocate(1);
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p4(v2);
    BOOST_TEST(p2 != p4);
    BOOST_TEST(p4 != p2);
    a.deallocate(v2, 1);
    a.deallocate(v1, 1);
}

#if !defined(BOOST_NO_CXX11_NULLPTR)
void test_equal_nullptr(scalar)
{
    boost::allocation_ptr<int, boost::default_allocator<void> > p1;
    BOOST_TEST(p1 == nullptr);
    BOOST_TEST(nullptr == p1);
    boost::default_allocator<int> a;
    int* v = a.allocate(1);
    boost::allocation_ptr<int, boost::default_allocator<void> > p2(v);
    BOOST_TEST_NOT(p2 == nullptr);
    BOOST_TEST_NOT(nullptr == p2);
    a.deallocate(v, 1);
}

void test_equal_nullptr(unbounded_array)
{
    boost::allocation_ptr<int[], boost::default_allocator<void> > p1;
    BOOST_TEST(p1 == nullptr);
    BOOST_TEST(nullptr == p1);
    boost::default_allocator<int> a;
    int* v = a.allocate(3);
    boost::allocation_ptr<int[], boost::default_allocator<void> > p2(v, 3);
    BOOST_TEST_NOT(p2 == nullptr);
    BOOST_TEST_NOT(nullptr == p2);
    a.deallocate(v, 1);
}

void test_equal_nullptr(bounded_array)
{
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p1;
    BOOST_TEST(p1 == nullptr);
    BOOST_TEST(nullptr == p1);
    boost::default_allocator<int> a;
    int* v = a.allocate(1);
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p2(v);
    BOOST_TEST_NOT(p2 == nullptr);
    BOOST_TEST_NOT(nullptr == p2);
    a.deallocate(v, 1);
}

void test_not_equal_nullptr(scalar)
{
    boost::allocation_ptr<int, boost::default_allocator<void> > p1;
    BOOST_TEST_NOT(p1 != nullptr);
    BOOST_TEST_NOT(nullptr != p1);
    boost::default_allocator<int> a;
    int* v = a.allocate(1);
    boost::allocation_ptr<int, boost::default_allocator<void> > p2(v);
    BOOST_TEST(p2 != nullptr);
    BOOST_TEST(nullptr != p2);
    a.deallocate(v, 1);
}

void test_not_equal_nullptr(unbounded_array)
{
    boost::allocation_ptr<int[], boost::default_allocator<void> > p1;
    BOOST_TEST_NOT(p1 != nullptr);
    BOOST_TEST_NOT(nullptr != p1);
    boost::default_allocator<int> a;
    int* v = a.allocate(3);
    boost::allocation_ptr<int[], boost::default_allocator<void> > p2(v, 3);
    BOOST_TEST(p2 != nullptr);
    BOOST_TEST(nullptr != p2);
    a.deallocate(v, 1);
}

void test_not_equal_nullptr(bounded_array)
{
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p1;
    BOOST_TEST_NOT(p1 != nullptr);
    BOOST_TEST_NOT(nullptr != p1);
    boost::default_allocator<int> a;
    int* v = a.allocate(1);
    boost::allocation_ptr<int[3], boost::default_allocator<void> > p2(v);
    BOOST_TEST(p2 != nullptr);
    BOOST_TEST(nullptr != p2);
    a.deallocate(v, 1);
}
#endif

template<class T>
void test(T tag)
{
    test_pointer(tag);
    test_element_type(tag);
    test_default_construct(tag);
#if !defined(BOOST_NO_CXX11_NULLPTR)
    test_nullptr_construct(tag);
#endif
    test_access(tag);
#if !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
    test_bool(tag);
#endif
    test_not(tag);
    test_equal(tag);
    test_not_equal(tag);
#if !defined(BOOST_NO_CXX11_NULLPTR)
    test_equal_nullptr(tag);
    test_not_equal_nullptr(tag);
#endif
}

int main()
{
    test(scalar());
    test(unbounded_array());
    test(bounded_array());
    return boost::report_errors();
}
