/*
Copyright 2019 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef BOOST_SMART_PTR_ALLOCATION_PTR_HPP
#define BOOST_SMART_PTR_ALLOCATION_PTR_HPP

#include <boost/smart_ptr/detail/sp_noexcept.hpp>
#include <boost/smart_ptr/detail/sp_nullptr_t.hpp>
#include <boost/assert.hpp>
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
#include <memory>
#endif

namespace boost {
namespace detail {

template<class T, class A>
struct sp_allocation_ptr {
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
    typedef typename std::allocator_traits<A>::template
        rebind_traits<T>::pointer type;
#else
    typedef typename A::template rebind<T>::other::pointer type;
#endif
};

} /* detail */

template<class T, class A>
class allocation_ptr {
public:
    typedef typename detail::sp_allocation_ptr<T, A>::type pointer;
    typedef T element_type;

    allocation_ptr() BOOST_SP_NOEXCEPT
        : p_() { }

#if !defined(BOOST_NO_CXX11_NULLPTR)
    allocation_ptr(detail::sp_nullptr_t) BOOST_SP_NOEXCEPT
        : p_() { }
#endif

    explicit allocation_ptr(pointer p) BOOST_SP_NOEXCEPT
        : p_(p) { }

    T& operator*() const {
        return *p_;
    }

    pointer operator->() const BOOST_SP_NOEXCEPT {
        return p_;
    }

    pointer get() const BOOST_SP_NOEXCEPT {
        return p_;
    }

#if !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
    explicit operator bool() const BOOST_SP_NOEXCEPT {
        return static_cast<bool>(p_);
    }
#endif

    bool operator!() const BOOST_SP_NOEXCEPT {
        return !p_;
    }

private:
    pointer p_;
};

template<class T, class A>
class allocation_ptr<T[], A> {
public:
    typedef typename detail::sp_allocation_ptr<T, A>::type pointer;
    typedef T element_type;

    allocation_ptr() BOOST_SP_NOEXCEPT
        : p_()
        , n_() { }

#if !defined(BOOST_NO_CXX11_NULLPTR)
    allocation_ptr(detail::sp_nullptr_t) BOOST_SP_NOEXCEPT
        : p_()
        , n_() { }
#endif

    allocation_ptr(pointer p, std::size_t n) BOOST_SP_NOEXCEPT
        : p_(p)
        , n_(n) { }

    T& operator[](std::size_t i) const BOOST_SP_NOEXCEPT_WITH_ASSERT {
        BOOST_ASSERT(i < n_);
        return p_[i];
    }

    pointer get() const BOOST_SP_NOEXCEPT {
        return p_;
    }

    std::size_t size() const BOOST_SP_NOEXCEPT {
        return n_;
    }

#if !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
    explicit operator bool() const BOOST_SP_NOEXCEPT {
        return static_cast<bool>(p_);
    }
#endif

    bool operator!() const BOOST_SP_NOEXCEPT {
        return !p_;
    }

private:
    pointer p_;
    std::size_t n_;
};

template<class T, std::size_t N, class A>
class allocation_ptr<T[N], A> {
public:
    typedef typename detail::sp_allocation_ptr<T, A>::type pointer;
    typedef T element_type;

    allocation_ptr() BOOST_SP_NOEXCEPT
        : p_() { }

#if !defined(BOOST_NO_CXX11_NULLPTR)
    allocation_ptr(detail::sp_nullptr_t) BOOST_SP_NOEXCEPT
        : p_() { }
#endif

    explicit allocation_ptr(pointer p) BOOST_SP_NOEXCEPT
        : p_(p) { }

    T& operator[](std::size_t i) const BOOST_SP_NOEXCEPT_WITH_ASSERT {
        BOOST_ASSERT(i < N);
        return p_[i];
    }

    pointer get() const BOOST_SP_NOEXCEPT {
        return p_;
    }

    static std::size_t size() BOOST_SP_NOEXCEPT {
        return N;
    }

#if !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
    explicit operator bool() const BOOST_SP_NOEXCEPT {
        return static_cast<bool>(p_);
    }
#endif

    bool operator!() const BOOST_SP_NOEXCEPT {
        return !p_;
    }

private:
    pointer p_;
};

template<class T, class A>
inline bool
operator==(const allocation_ptr<T, A>& lhs, const allocation_ptr<T, A>& rhs)
{
    return lhs.get() == rhs.get();
}

template<class T, class A>
inline bool
operator!=(const allocation_ptr<T, A>& lhs, const allocation_ptr<T, A>& rhs)
{
    return !(lhs == rhs);
}

#if !defined(BOOST_NO_CXX11_NULLPTR)
template<class T, class A>
inline bool
operator==(const allocation_ptr<T, A>& lhs,
    detail::sp_nullptr_t) BOOST_SP_NOEXCEPT
{
    return !lhs.get();
}

template<class T, class A>
inline bool
operator==(detail::sp_nullptr_t,
    const allocation_ptr<T, A>& rhs) BOOST_SP_NOEXCEPT
{
    return !rhs.get();
}

template<class T, class A>
inline bool
operator!=(const allocation_ptr<T, A>& lhs,
    detail::sp_nullptr_t) BOOST_SP_NOEXCEPT
{
    return static_cast<bool>(lhs.get());
}

template<class T, class A>
inline bool
operator!=(detail::sp_nullptr_t,
    const allocation_ptr<T, A>& rhs) BOOST_SP_NOEXCEPT
{
    return static_cast<bool>(rhs.get());
}
#endif

} /* boost */

#endif
