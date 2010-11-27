#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"
#include "cinder/TriMesh.h"
#include "cinder/Text.h"
#include "cinder/System.h"
#include "cinder/Utilities.h"
using namespace ci;
using namespace ci::app;
using namespace std;

#include <boost/lexical_cast.hpp>

#include "Resources.h"

#include "CiIso.h"

class AdvancedExampleApp : public AppBasic {
public:
	void setup();
	void keyDown( KeyEvent event );
	void resize( ResizeEvent event );
	void update();
	void draw();
	
private:
	CameraPersp				mCam;
	
	TriMesh					mTriMesh;
	gl::GlslProg			mShader;	
	Matrix44f				mRotation;	

	CiIsoParticleSysf		mBaseParticles;
	CiIsoParticleSysf		mParticles;
	CiIsoFieldFunc			mFieldFunc;
	
	bool					mMultiThread;
	CiIsoPolygonizerf		mPolygonizerMT;
	CiIsoPolygonizerf		mPolygonizerST;
	
	float					mRadiusScale;
	bool					mDrawWireframe;
};

void AdvancedExampleApp::setup()
{
	//
	// !!! IMPORTANT !!!!
	//
	// The grid size, particles and number of threads in a CiIsoPolygonizer object
	// must be set before first call of polygonize(). 
	// If you try to call any of the set functions after polygonize() has been called
	// the results are undefined. These functions include, but are not limited to:
	//     setGridSize()
	//     setParticles()
	//     setNumThreads()
	//
	// -- Grid Sizes -- 
	// Since CinderISO is currently 32-bit, the maximum gridsize is about 200. If 
	// you go too much over that, you will get a memory allocation error.
	//
	// -- Radii --
	// The safe range for radii of a particle is [1, 2.5]. If you venture out side
	// that [1, 2.5] you might get results which may seem very unpleasant.
	//
	// -- Field Functions --
	// Currently, CinderISO only supports the Blinn field function. There are 
	// plans to add several field field functions that will produce different
	// results.
	//
	// -- Threads --
	// By default, multi-threading is turned off. To turn on the threading:
	//     setNumThreads( numThreads );
	// If you wish to use the maximum of threads on your system, use the following:
	//    setNumThreads( ci::System::getNumCores() );
	// This should not immediately peg your CPU unless you have a high grid 
	// size or a high particle count.
	//
	
	// Camera
	mCam.lookAt( Vec3f( 0, 5, 20 ), Vec3f::zero() );
	
	// Shader
	mShader = gl::GlslProg( loadResource( RES_SHADER_VERT ), loadResource( RES_SHADER_FRAG ) );	
	
	// Rotation
	mRotation.setToIdentity();	
	
	// Control vars
	mFieldFunc = CINDER_ISO_FIELDFUNC_BLINN;
	
	// Metaballs
	int numParticles = 40;
	
	// Generate some random particles
	Rand::randSeed( 0x42 );
	mBaseParticles.generateRandomData( numParticles, 1.5f, 2.25f, Vec4f::zero(), 10.0f, 10.0f, 10.0f );
	mParticles = mBaseParticles;	
	
	// Turn on multithreading by default
	mMultiThread = true;
	
	// Set polygonizer params - multithread
	int gridSize = 64;
	mPolygonizerMT.setGridSize( gridSize );
	mPolygonizerMT.setParticles( &mParticles );
	mPolygonizerMT.setNumThreads( ci::System::getNumCores() );
	
	// Set polygonizer params - single thread
	mPolygonizerST.setGridSize( gridSize );
	mPolygonizerST.setParticles( &mParticles );
	mPolygonizerST.setNumThreads( 1 );
	
	// Radius scale
	mRadiusScale = 1.0f;

	// Controls the drawing of the wire frame
	mDrawWireframe = false;
	
	// GL Stuff
	GLfloat LightAmbient[]  = { 0.5f, 0.5f,  0.5f, 1.0f };
	GLfloat LightDiffuse[]  = { 1.0f, 1.0f,  1.0f, 1.0f };
	GLfloat LightPosition[] = { 10.0f, 15.0f, 15.0f, 1.0f };	
	glLightfv( GL_LIGHT0, GL_AMBIENT,  LightAmbient );
	glLightfv( GL_LIGHT0, GL_DIFFUSE,  LightDiffuse );	
	glLightfv( GL_LIGHT0, GL_POSITION, LightPosition );
	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHTING ); 	
	gl::enableDepthWrite();
	gl::enableDepthRead();	
}

void AdvancedExampleApp::keyDown( KeyEvent event )
{
	switch( event.getChar() ) {
		case '1': 
			mFieldFunc = CINDER_ISO_FIELDFUNC_BLINN;
			mPolygonizerMT.setFieldFuncParams( 0.25f, 3.0f );
			mPolygonizerST.setFieldFuncParams( 0.25f, 3.0f );
			mRadiusScale = 1.0f;
			break;
		case 'w':
			mDrawWireframe = ! mDrawWireframe;
		break;
		case 't':
			mMultiThread = !mMultiThread;
			break;
	}
}

void AdvancedExampleApp::resize( ResizeEvent event )
{
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	gl::setMatrices( mCam );	
}

void AdvancedExampleApp::update()
{
	// Update rotation mat
	mRotation.rotate( Vec3f( 0, 1, 0 ), 0.1f );

	// Update particle animation
	float t = (float)getElapsedSeconds() * 5;
	for( size_t i = 0; i < mBaseParticles.getNumParticles(); ++i ) {
		const Vec4f &srcParticle = mBaseParticles.getPositionAt( i );
		const float &srcRadius = mBaseParticles.getRadiusAt( i );
		
		Vec4f &dstParticle = mParticles.getPositionAt( i );
		float &dstRadius = mParticles.getRadiusAt( i );
		
		dstParticle.x = srcParticle.x + sin( t*srcRadius*0.25f )*3.0f;
		dstParticle.y = srcParticle.y + cos( t*srcRadius*0.35f )*4.0f;
		dstParticle.z = srcParticle.z + sin( t*srcRadius*0.25f )*2.5f;
		dstRadius = srcRadius*mRadiusScale;
	}
	
	// Polygonize
	mTriMesh.clear();
	if( mMultiThread ) {
		mPolygonizerMT.setFieldFunc( mFieldFunc );
		mPolygonizerMT.polygonize( &mTriMesh );
	}
	else {
		mPolygonizerST.setFieldFunc( mFieldFunc );
		mPolygonizerST.polygonize( &mTriMesh );	
	}
}

void AdvancedExampleApp::draw()
{
	gl::setMatrices( mCam );

	gl::clear( Color( 0, 0, 0 ) ); 

	gl::disableAlphaBlending();
	if( mDrawWireframe )
		gl::enableWireframe();
	
	if( mTriMesh.getNumTriangles() ) {
		gl::pushMatrices();
	
		mShader.bind();
		mShader.uniform( "eyeDir", mCam.getViewDirection().normalized() );
		gl::draw( mTriMesh );
		mShader.unbind();
	
		gl::popMatrices();
	}

	gl::disableWireframe();
	gl::enableAlphaBlending();

	// Simple FPS display
	gl::setMatricesWindow( getWindowSize() );
	gl::drawString( toString( getAverageFps() ) + " fps", Vec2i( 5, 5 ), ColorA( 1, 1, 1, 0.5f ), Font( "Arial", 18 ) );
}


CINDER_APP_BASIC( AdvancedExampleApp, RendererGl )
