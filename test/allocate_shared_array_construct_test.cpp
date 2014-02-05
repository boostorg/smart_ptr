/*
 * Copyright (c) 2012-2014 Glen Joseph Fernandes 
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License, 
 * Version 1.0. (See accompanying file LICENSE_1_0.txt 
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#include <boost/detail/lightweight_test.hpp>
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
#include <boost/smart_ptr/allocate_shared_array.hpp>

class type1 {
    friend class std::allocator<type1>;

public:
    static unsigned int instances;
    static const type1 object;

protected:
    explicit type1() {
        instances++;
    }

    type1(const type1&) {
        instances++;
    }

    ~type1() {
        instances--;
    }
};

unsigned int type1::instances;
const type1 type1::object;

template<typename T>
class creator {
public:
    typedef T value_type;

#if defined(BOOST_NO_CXX11_TEMPLATE_ALIASES)
    template<typename U>
    struct rebind {
        typedef creator<U> other;
    };
#endif

    creator() {
    }

    template<typename U>
    creator(const creator<U>&) {
    }

    T* allocate(std::size_t size) {
        void* p1 = ::operator new(size * sizeof(T));
        return static_cast<T*>(p1);
    }

    void deallocate(T* memory, std::size_t) {
        void* p1 = memory;
        ::operator delete(p1);
    }

    template<typename U, typename... Args>
    void construct(U* memory, Args&&... args) {
        void* p1 = memory;
        ::new(p1) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* memory) {
        memory->~U();
    }
};

class type2 {
    friend class creator<type2>;

public:
    static unsigned int instances;
    static const type2 object;

protected:
    explicit type2() {
        instances++;
    }

    type2(const type2&) {
        instances++;
    }

    ~type2() {
        instances--;
    }
};

unsigned int type2::instances;
const type2 type2::object;

int main() {
    BOOST_TEST(type1::instances == 1);
    {
        boost::shared_ptr<type1[]> a1 = boost::allocate_shared<type1[]>(std::allocator<void>(), 3);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(type1::instances == 4);
        a1.reset();
        BOOST_TEST(type1::instances == 1);
    }

    BOOST_TEST(type1::instances == 1);
    {
        boost::shared_ptr<type1[3]> a1 = boost::allocate_shared<type1[3]>(std::allocator<void>());
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(type1::instances == 4);
        a1.reset();
        BOOST_TEST(type1::instances == 1);
    }

    BOOST_TEST(type1::instances == 1);
    {
        boost::shared_ptr<type1[][2]> a1 = boost::allocate_shared<type1[][2]>(std::allocator<void>(), 2);
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type1::instances == 5);
        a1.reset();
        BOOST_TEST(type1::instances == 1);
    }

    BOOST_TEST(type1::instances == 1);
    {
        boost::shared_ptr<type1[2][2]> a1 = boost::allocate_shared<type1[2][2]>(std::allocator<void>());
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type1::instances == 5);
        a1.reset();
        BOOST_TEST(type1::instances == 1);
    }

    BOOST_TEST(type1::instances == 1);
    {
        boost::shared_ptr<const type1[]> a1 = boost::allocate_shared<const type1[]>(std::allocator<void>(), 3);
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type1::instances == 4);
        a1.reset();
        BOOST_TEST(type1::instances == 1);
    }

    BOOST_TEST(type1::instances == 1);
    {
        boost::shared_ptr<const type1[3]> a1 = boost::allocate_shared<const type1[3]>(std::allocator<void>());
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type1::instances == 4);
        a1.reset();
        BOOST_TEST(type1::instances == 1);
    }

    BOOST_TEST(type1::instances == 1);
    {
        boost::shared_ptr<const type1[][2]> a1 = boost::allocate_shared<const type1[][2]>(std::allocator<void>(), 2);
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type1::instances == 5);
        a1.reset();
        BOOST_TEST(type1::instances == 1);
    }

    BOOST_TEST(type1::instances == 1);
    {
        boost::shared_ptr<const type1[2][2]> a1 = boost::allocate_shared<const type1[2][2]>(std::allocator<void>());
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type1::instances == 5);
        a1.reset();
        BOOST_TEST(type1::instances == 1);
    }

    BOOST_TEST(type2::instances == 1);
    {
        boost::shared_ptr<type2[]> a1 = boost::allocate_shared<type2[]>(creator<void>(), 3);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(type2::instances == 4);
        a1.reset();
        BOOST_TEST(type2::instances == 1);
    }

    BOOST_TEST(type2::instances == 1);
    {
        boost::shared_ptr<type2[3]> a1 = boost::allocate_shared<type2[3]>(creator<void>());
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(type2::instances == 4);
        a1.reset();
        BOOST_TEST(type2::instances == 1);
    }

    BOOST_TEST(type2::instances == 1);
    {
        boost::shared_ptr<type2[][2]> a1 = boost::allocate_shared<type2[][2]>(creator<void>(), 2);
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type2::instances == 5);
        a1.reset();
        BOOST_TEST(type2::instances == 1);
    }

    BOOST_TEST(type2::instances == 1);
    {
        boost::shared_ptr<type2[2][2]> a1 = boost::allocate_shared<type2[2][2]>(creator<void>());
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type2::instances == 5);
        a1.reset();
        BOOST_TEST(type2::instances == 1);
    }

    BOOST_TEST(type2::instances == 1);
    {
        boost::shared_ptr<const type2[]> a1 = boost::allocate_shared<const type2[]>(creator<void>(), 3);
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type2::instances == 4);
        a1.reset();
        BOOST_TEST(type2::instances == 1);
    }

    BOOST_TEST(type2::instances == 1);
    {
        boost::shared_ptr<const type2[3]> a1 = boost::allocate_shared<const type2[3]>(creator<void>());
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type2::instances == 4);
        a1.reset();
        BOOST_TEST(type2::instances == 1);
    }

    BOOST_TEST(type2::instances == 1);
    {
        boost::shared_ptr<const type2[][2]> a1 = boost::allocate_shared<const type2[][2]>(creator<void>(), 2);
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type2::instances == 5);
        a1.reset();
        BOOST_TEST(type2::instances == 1);
    }

    BOOST_TEST(type2::instances == 1);
    {
        boost::shared_ptr<const type2[2][2]> a1 = boost::allocate_shared<const type2[2][2]>(creator<void>());
        BOOST_TEST(a1.get() != 0);
        BOOST_TEST(a1.use_count() == 1);
        BOOST_TEST(type2::instances == 5);
        a1.reset();
        BOOST_TEST(type2::instances == 1);
    }

    return boost::report_errors();
}
#else

int main() {
    return 0;
}

#endif
