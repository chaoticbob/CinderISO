#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "cinder/Text.h"
#include "cinder/TriMesh.h"
#include "cinder/Utilities.h"
using namespace ci;
using namespace ci::app;
using namespace std;

#include "Resources.h"

#include "CiIso.h"

class SimpleExampleApp : public AppBasic {
  public:
	void setup();
	void resize( ResizeEvent event );
	void update();
	void draw();
	
  private:
	CameraPersp				mCam;
	
	TriMesh					mTriMesh;
	gl::GlslProg			mShader;	
	
	CiIsoParticleSysf		mParticles;
	CiIsoPolygonizerf		mPolygonizer;
};

void SimpleExampleApp::setup()
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

	// Framerate
	setFrameRate( 60.0f );

	// Camera
	mCam.lookAt( Vec3f( 0, 0, 10 ), Vec3f::zero() );
	
	// Shader
	mShader = gl::GlslProg( loadResource( RES_SHADER_VERT ), loadResource( RES_SHADER_FRAG ) );	
	
	// Metaballs
	mParticles.addParticle( Vec3f( -1.0f, 0.0f, 0.0f ), 2.0f, Color( 1, 0, 0 ) );
	mParticles.addParticle( Vec3f(  1.0f, 0.0f, 0.0f ), 2.0f, Color( 0, 1, 0 ) );
	mParticles.addParticle( Vec3f(  0.0f, 1.0f, 0.0f ), 2.0f, Color( 0, 0, 1 ) );
	
	int gridSize = 64;
	mPolygonizer.setGridSize( gridSize );
	mPolygonizer.setParticles( &mParticles );
	mPolygonizer.setFieldFunc( CINDER_ISO_FIELDFUNC_BLINN );
	
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

void SimpleExampleApp::resize( ResizeEvent event )
{
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	gl::setMatrices( mCam );	
}

void SimpleExampleApp::update()
{
	// Update particles
	float t = getElapsedSeconds();
	mParticles.getPositionAt( 0 ).x = sin( t )*4.0f;
	mParticles.getPositionAt( 1 ).x = cos( t )*4.0f;
	mParticles.getPositionAt( 2 ).y = sin( t*2.0f )*4.0f;
	
	// Polygonize
	mTriMesh.clear();
	mPolygonizer.polygonize( &mTriMesh );	
}

void SimpleExampleApp::draw()
{
	gl::setMatrices( mCam );

	gl::clear( Color( 0, 0, 0 ) ); 

	gl::disableAlphaBlending();	
	if( mTriMesh.getNumTriangles() ) {
		gl::pushMatrices();

		mShader.bind();
		mShader.uniform( "eyeDir", mCam.getViewDirection().normalized() );
		gl::draw( mTriMesh );
		mShader.unbind();

		gl::popMatrices();
	}
	
	gl::enableAlphaBlending();

	// Simple FPS display
	gl::setMatricesWindow( getWindowSize() );
	gl::drawString( toString( getAverageFps() ) + " fps", Vec2i( 5, 5 ), ColorA( 1, 1, 1, 0.5f ), Font( "Arial", 18 ) );
}


CINDER_APP_BASIC( SimpleExampleApp, RendererGl )
