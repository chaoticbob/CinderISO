/*
 Copyright (c) 2010, Hai Nguyen

 This code is intended to be used with the Cinder C++ library, http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/Cinder.h"
#include "cinder/CinderMath.h"
#include "cinder/Color.h"
#include "cinder/Vector.h"

#include <algorithm>
#include <exception>
#include <map>
#include <set>
#include <vector>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>
using boost::addressof;

// -------------------------------------------------------------------------
// FORWARD DECLARE
// -------------------------------------------------------------------------
// Declares the following
//    Class       - Class
//    Ptr         - Shared pointer - 
//    ConstPtr    - Const shared pointer
//    RawPtr      - Raw pointer
//    ConstRawPtr - Const shared pointer
// Note: We use boost::shared_ptr for now until the C++0x gets 
//       further implemented in the various compilers.
#define CINDER_HAI_FWD_DECL( _CLASS_ )                              \
    class _CLASS_;                                                  \
    typedef boost::shared_ptr<_CLASS_> _CLASS_##Ptr;                \
    typedef boost::shared_ptr<const _CLASS_> Const##_CLASS_##Ptr;   \
    typedef _CLASS_ * _CLASS_##RawPtr;                              \
    typedef const _CLASS_ * Const##_CLASS_##RawPtr;

// -------------------------------------------------------------------------
// INLINE DECLARE
// -------------------------------------------------------------------------
template <typename T> 
struct CiIsoTypeDecl {
	typedef boost::shared_ptr<T>		ptr;
	typedef boost::shared_ptr<const T>	const_ptr;
	typedef T*							raw_ptr;
	typedef const T*					const_raw_ptr;
	typedef T&							ref;
	typedef const T&					const_ref;
};

// -------------------------------------------------------------------------
// TYPEDEF
// -------------------------------------------------------------------------
// Remaps a type
#define CINDER_HAI_TYPEDEF( _SRC_TYPE_, _DST_TYPE_ )                    \
    typedef _SRC_TYPE_ _DST_TYPE_;                                      \
    typedef boost::shared_ptr<_DST_TYPE_> _DST_TYPE_##Ptr;              \
    typedef boost::shared_ptr<const _DST_TYPE_> Const##_DST_TYPE_##Ptr; \
    typedef _DST_TYPE_ * _DST_TYPE_##RawPtr;                            \
    typedef const _DST_TYPE_ * Const##_DST_TYPE_##RawPtr;


// -------------------------------------------------------------------------
// PIMPL 
// -------------------------------------------------------------------------
#define CINDER_HAI_PIMPL_DECL                               \
    private:                                                \
        struct Impl;                                        \
        boost::shared_ptr<Impl> m_pimpl;                    \
        boost::shared_ptr<const Impl> getImpl() const {     \
            return m_pimpl;                                 \
        }                                                   \
        boost::shared_ptr<Impl> getImpl() {                 \
            return m_pimpl;                                 \
        }                                                   \
        const Impl & getImplRef() const {                   \
            return *m_pimpl;                                \
        }                                                   \
        Impl & geImplRef() const {                          \
            return *m_pimpl;                                \
        }

#define CINDER_HAI_IMPL \
	Impl

#define CINDER_HAI_PIMPL_BEGIN( _CLASS_ ) \
	struct _CLASS_::Impl

#define CINDER_HAI_PIMPL_BEGIN_TEMPLATED_1( _CLASS_, _PARAMS_1_DECL_, _PARAMS_1_ ) \
	template <_PARAMS_1_DECL_> struct _CLASS_<_PARAMS_1_>::Impl   

#define CINDER_HAI_PIMPL_BEGIN_TEMPLATED_2( _CLASS_, _PARAMS_1_DECL_, _PARAMS_2_DECL_, _PARAMS_1_, _PARAMS_2_ ) \
	template <_PARAMS_1_DECL_, _PARAMS_2_DECL_> struct _CLASS_<_PARAMS_1_,_PARAMS_2_>::Impl   

#define CINDER_HAI_PIMPL_CTOR_0_DECL()                     Impl()
#define CINDER_HAI_PIMPL_CTOR_1_DECL( A0 )                 Impl( A0 )
#define CINDER_HAI_PIMPL_CTOR_2_DECL( A0, A1 )             Impl( A0, A1 )
#define CINDER_HAI_PIMPL_CTOR_3_DECL( A0, A1, A2 )         Impl( A0, A1, A2  )
#define CINDER_HAI_PIMPL_CTOR_4_DECL( A0, A1, A2, A3 )     Impl( A0, A1, A2, A3 )

#define CINDER_HAI_PIMPL_CTOR_0_CALL()                      m_pimpl( boost::make_shared<Impl>() )
#define CINDER_HAI_PIMPL_CTOR_1_CALL( A0 )                  m_pimpl( boost::make_shared<Impl>( A0 ) )
#define CINDER_HAI_PIMPL_CTOR_2_CALL( A0, A1 )              m_pimpl( boost::make_shared<Impl>( A0, A1 ) )
#define CINDER_HAI_PIMPL_CTOR_3_CALL( A0, A1, A2 )          m_pimpl( boost::make_shared<Impl>( A0, A1, A2 ) )
#define CINDER_HAI_PIMPL_CTOR_4_CALL( A0, A1, A2, A3 )      m_pimpl( boost::make_shared<Impl>( A0, A1, A2, A3 ) ) 

// Base exception
class CiHaiExc : public std::exception
{
public:
    CiHaiExc( const std::string &msg = "" ) throw() : m_msg( msg ) {}
    virtual ~CiHaiExc() throw() {}
    virtual const char * what() const throw() { return m_msg.c_str(); }
private:
    std::string m_msg;
};
