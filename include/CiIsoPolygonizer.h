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

//
// !!! IMPORTANT !!!!
//
// The properties of the polygonizer must be set before first call of polygonize(). 
// If you try to call any of the set functions after polygonize() has executed,
// the results are undefined. These functions include, but are not limited to:
//     setGridSize()
//     setParticles()
//     setNumThreads()
//
// -- Grid Sizes -- 
// Since CinderISO is currently 32-bit, the maximum gridsize is about 200. If 
// you go too much over that, you will get a memory alocation error.
//
// -- Radii --
// The safe range for radii of a particle is [1, 2.5]. If you go outside that 
// range, you might get a result may not look very pleasing.
//
// -- Field Functions --
// Currently, CinderISO only supports the Blinn field function. There are 
// plans to add several field field functions that will produce different.
//
// -- Threads --
// By default, multi-threading is turned off. To turn on the threading:
//     setNumThreads( numThreads );
// If you wish to use the maximum of threads on your system, use the following:
//    setNumThreads( ci::System::getNumCores() );
// This should not immediately peg your CPU unless
// you have a high grid size or a high particle 
// count.
//

#pragma once

#include "CiIsoTypes.h"
#include "CiIsoParticleSys.h"

#include "cinder/TriMesh.h"

enum CiIsoFieldFunc {
	CINDER_ISO_FIELDFUNC_BLINN,
	CINDER_ISO_FIELDFUNC_METABALL,
	CINDER_ISO_FIELDFUNC_RADIUS,
	CINDER_ISO_FIELDFUNC_SOFTOBJECT,
	CINDER_ISO_FIELDFUNC_WYVILL
};

enum CiIsoMesher {
	CINDER_ISO_MESHER_MARCH_CUBES,
	CINDER_ISO_MESHER_CELL_DISTORT,
	CINDER_ISO_MESHER_MARCH_TETS,
	CINDER_ISO_MESHER_DUAL_MARCH_CUBES
};

template <typename T>
class CiIsoPolygonizer
{
	CINDER_HAI_PIMPL_DECL
public:

	CiIsoPolygonizer( int gridSize = 0 );
	~CiIsoPolygonizer();
	
	int						getNumThreads() const;
	void					setNumThreads( int numThreads );
	
	CiIsoFieldFunc			getFieldFunc() const;
	void					setFieldFunc( CiIsoFieldFunc fieldFunc );
	
	T						getFieldFuncParamA() const;
	void					setFieldFuncParamA( T A );
	T						getFieldFuncParamB() const;
	void					setFieldFuncParamB( T B );
	void					getFieldFuncParams( T* A, T* B ) const;
	void					setFieldFuncParams( T A, T B );
	
	// Sets max influence dist
	T						getMaxDist() const;
	void					setMaxDist( T maxDist );
	
	CiIsoMesher				getMesher() const;
	void					setMesher( CiIsoMesher mesher );
	
	int						getGridSize() const;
	void					setGridSize( int gridSize );
	
	void					setParticles( CiIsoParticleSys<T>* particles );
	void					updateBounds();
		
	// By default, polygonize will call the Pimpl's initForPolygonize.
	// If you change the fieldFunction, grid size, or number of threads
	// initForPolygonize needs to be called after any or all of those
	// fields are changed.
	void					initForPolygonize();
	void					polygonize( ci::TriMesh *mesh, bool autoUpdateBounds = true );
	void					clearPolyonizeData();
	
	// This will force the polygonizer to operate in multithreaded
	// behavior - even if there is only one thread. This is primary
	// used to debug the threaded behavior. There is no real reason
	// to use this under normal operations.
	void					forceMultiThread( bool force = true );
	
	// Enables/disables output of some vars to the console
	void					setPrintVars( bool val );
	bool					getPrintVars() const;
};

typedef CiIsoPolygonizer<float>		CiIsoPolygonizerf;
typedef CiIsoPolygonizer<double>	CiIsoPolygonizerd;
