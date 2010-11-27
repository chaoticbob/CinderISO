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

#include "CiIsoTypes.h"

#include <vector>

template <typename T>
class CiIsoParticleSys 
{
public:
	typedef T										value_type;
	typedef value_type								scalar_type;
	typedef ci::Vec4<T>								point_type;
	typedef ci::Color								color_type;
	typedef std::vector<scalar_type>				scalar_list;
	typedef std::vector<point_type>					point_list;
	typedef std::vector<color_type>					color_list;
	
	typedef typename scalar_list::const_iterator	const_scalar_iterator;
	typedef typename scalar_list::iterator			scalar_iterator;

	typedef typename point_list::const_iterator		const_point_iterator;
	typedef typename point_list::iterator			point_iterator;

	typedef typename color_list::const_iterator		const_color_iterator;
	typedef typename color_list::iterator			color_iterator;
	
	CiIsoParticleSys( size_t n = 0 );	
	CiIsoParticleSys( const CiIsoParticleSys &obj );
	
	CiIsoParticleSys & operator=( const CiIsoParticleSys &rhs );
	
	const_point_iterator	positionsBegin() const { return m_positions.begin(); }
	point_iterator			positionsBegin() { return m_positions.begin(); }

	const_point_iterator	positionsEnd() const { return m_positions.end(); }
	point_iterator			positionsEnd() { return m_positions.end(); }
	
	const_scalar_iterator	radiiBegin() const { return m_radii.begin(); }
	scalar_iterator			radiiBegin() { return m_radii.begin(); }

	const_scalar_iterator	radiiEnd() const { return m_radii.end(); }
	scalar_iterator			radiiEnd() { return m_radii.end(); }
	
	const_color_iterator	colorsBegin() const { return m_colors.begin(); }
	color_iterator			colorsBegin() { return m_colors.begin(); }
	
	const_color_iterator	colorsEnd() const { return m_colors.end(); }
	color_iterator			colorsEnd() { return m_colors.end(); }
		
	size_t					getNumParticles() const;
	void					setNumParticles( size_t n );
	
	void					clear();
	void					addParticle( const ci::Vec3<T> &P, T r, const color_type &color );
	
	const point_type &		getPositionAt( size_t n ) const { return m_positions[n]; }
	point_type &			getPositionAt( size_t n ) { return m_positions[n]; }

	const scalar_type &		getRadiusAt( size_t n ) const { return m_radii[n]; }
	scalar_type &			getRadiusAt( size_t n ) { return m_radii[n]; }
	
	const color_type &		getColorAt( size_t n ) const { return m_colors[n]; }
	color_type &			getColorAt( size_t n ) { return m_colors[n]; }
		
	void					generateRandomData( size_t n, T minRad, T maxRad, const point_type &P, T xSpan, T ySpan, T zSpan );

private:
	point_list				m_positions;
	scalar_list				m_radii;
	color_list				m_colors;
};

// Typedefs
typedef CiIsoParticleSys<float>		CiIsoParticleSysf;
typedef CiIsoParticleSys<double>	CiIsoParticleSysd;

