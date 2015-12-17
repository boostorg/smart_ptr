//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_POINTER_CAST_HPP
#define BOOST_POINTER_CAST_HPP

namespace boost { 

//static_pointer_cast overload for raw pointers
template<class T, class U>
inline T* static_pointer_cast(U *ptr)
{  
   return static_cast<T*>(ptr);
}

//dynamic_pointer_cast overload for raw pointers
template<class T, class U>
inline T* dynamic_pointer_cast(U *ptr)
{  
   return dynamic_cast<T*>(ptr);
}

//const_pointer_cast overload for raw pointers
template<class T, class U>
inline T* const_pointer_cast(U *ptr)
{  
   return const_cast<T*>(ptr);
}

//reinterpret_pointer_cast overload for raw pointers
template<class T, class U>
inline T* reinterpret_pointer_cast(U *ptr)
{  
   return reinterpret_cast<T*>(ptr);
}

} // namespace boost

#if !defined( BOOST_NO_CXX11_SMART_PTR )

#include <memory>

namespace boost {

//static_pointer_cast overload for std::shared_ptr
using std::static_pointer_cast;

//dynamic_pointer_cast overload for std::shared_ptr
using std::dynamic_pointer_cast;

//const_pointer_cast overload for std::shared_ptr
using std::const_pointer_cast;

//reinterpret_pointer_cast overload for std::shared_ptr
template<class T, class U> std::shared_ptr<T> reinterpret_pointer_cast(const std::shared_ptr<U> & r ) BOOST_NOEXCEPT
{
   (void) reinterpret_cast< T* >( static_cast< U* >( 0 ) );

   typedef typename std::shared_ptr<T>::element_type E;

   E * p = reinterpret_cast< E* >( r.get() );
   return std::shared_ptr<T>( r, p );
}

} // namespace boost

#endif // #if !defined( BOOST_NO_CXX11_SMART_PTR )

#endif   //BOOST_POINTER_CAST_HPP
