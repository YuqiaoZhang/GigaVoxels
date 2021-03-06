/*
 * GigaVoxels - GigaSpace
 *
 * Website: http://gigavoxels.inrialpes.fr/
 *
 * Contributors: GigaVoxels Team
 *
 * Copyright (C) 2007-2015 INRIA - LJK (CNRS - Grenoble University), All rights reserved.
 */

/** 
 * @version 1.0
 */

#include "SampleViewer.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include <GsGraphics/GsGraphicsCore.h>
#include <GvUtils/GsEnvironment.h>

// System
#include <cstdio>
#include <cstdlib>

// Cuda
#include <vector_types.h>
#include <vector_functions.h>

// QGLViewer
#include <QGLViewer/manipulatedFrame.h>

// Qt
#include <QDir>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Constructor
 ******************************************************************************/
SampleViewer::SampleViewer()
:	QGLViewer()
,	_sampleCore( NULL )
,	_light1( NULL )
,	_lightManipulation( false )
{
	// Makes Qt delete this widget when the widget has accepted the close event.
	// After been deleted, the Qt main event loop finishes.
	// Now, if Help window is visible, closing the viewer quits the application.
	setAttribute( Qt::WA_DeleteOnClose );

	// Window title
	setWindowTitle( tr( "Depth of Field example" ) );
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
SampleViewer::~SampleViewer()
{
	//** Setups connection
	QObject::disconnect( _light1, SIGNAL( manipulated() ), this, SLOT( onLightFrameManipulated() ) );

	// Free memory
	delete _sampleCore;
	delete _light1;
}

/******************************************************************************
 * Initialize the viewer
 ******************************************************************************/
void SampleViewer::init()
{
	// GLEW initialization
	GLenum error = glewInit();
	if ( error != GLEW_OK )
	{
		// Problem : glewInit failed
		fprintf( stderr, "Error: %s\n", glewGetErrorString( error ) );

		// Exit program
		exit( 1 );
	}

	// LOG associated Graphics Core library properties/capabilities (i.e. OpenGL)
	GsGraphics::GsGraphicsCore::printInfo();

	// Initialize the GigaVoxels pipeline
	_sampleCore = new SampleCore();
	_sampleCore->init();

	// Modify QGLViewer state filename
	QString stateFilename = GvUtils::GsEnvironment::getUserProfilePath().c_str();
	stateFilename += QDir::separator();
	stateFilename += "qglviewer.xml";
	setStateFileName( stateFilename );

	// Read QGLViewer XML settings file if any
	restoreStateFromFile();

	// Viewer settings :
	// - sets the backgroundColor() of the viewer and calls qglClearColor()
	setBackgroundColor( QColor( 51, 51, 51 ) );
	// Update GigaVoxels clear color
	_sampleCore->setClearColor( 51, 51, 51, 255 );

	// Light initialization
	_light1 = new qglviewer::ManipulatedFrame();
	_light1->setPosition( 1.0f, 1.0f, 1.0f );
	glEnable( GL_LIGHT1 );
	const GLfloat ambient[]  = { 0.2f, 0.2f, 2.0f, 1.0f };
	const GLfloat diffuse[]  = { 0.8f, 0.8f, 1.0f, 1.0f };
	const GLfloat specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glLightfv( GL_LIGHT1, GL_AMBIENT,  ambient );
	glLightfv( GL_LIGHT1, GL_SPECULAR, specular );
	glLightfv( GL_LIGHT1, GL_DIFFUSE,  diffuse );
	glDisable( GL_LIGHTING );
	// Update GigaVoxels light position
	_sampleCore->setLightPosition( make_float3( 1.0f, 1.0f, 1.0f ) );

	// Viewer initialization
	setSceneRadius( 4.0f );
	setMouseTracking( false );
		// QGLViewer uses a timer to redarw scene, this enables the maximum refreshing rate.
	setAnimationPeriod( 0 );
	startAnimation();

	// Camera settings : default point of view
	camera()->setPosition( qglviewer::Vec( 1.6554, 0.424961, 4.61722 ) );
	camera()->setOrientation( qglviewer::Quaternion( -0.0461286, 0.241959, 0.0613746, 0.967244 ) );

	//** Setups connection
	QObject::connect( _light1, SIGNAL( manipulated() ), this, SLOT( onLightFrameManipulated() ) );
}

/******************************************************************************
 * Draw function called each frame
 ******************************************************************************/
void SampleViewer::draw()
{
	// Clear default frame buffer
	// glClearColor( 0.0f, 0.1f, 0.3f, 0.0f );					// already done by SampleViewr::setBackgroundColor()
	// glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// already done in QGLViewr::preDraw() method

	// Render the GigaVoxels scene
	_sampleCore->draw();

	// Draw light if its manipulation is activated
	if ( _lightManipulation )
	{
		float pos[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
		qreal x,y,z;
		_light1->getPosition( x, y, z );
		pos[ 0 ] = x;
		pos[ 1 ] = y;
		pos[ 2 ] = z;

		glEnable( GL_LIGHT1 ); // must be enabled for drawLight()
		glLightfv( GL_LIGHT1, GL_POSITION, pos );
		glEnable( GL_DEPTH_TEST );
		if ( _light1->grabsMouse() )
		{
			drawLight( GL_LIGHT1, 1.2f );
		}
		else
		{
			drawLight( GL_LIGHT1 );
		}
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_LIGHT1 );
	}
}

/******************************************************************************
 * Resize GL event handler
 *
 * @param width the new width
 * @param height the new height
 ******************************************************************************/
void SampleViewer::resizeGL( int pWidth, int pHeight )
{
	// Handle QGLViewer resize
	QGLViewer::resizeGL( pWidth, pHeight );

	// Handle GigaVoxels resize
	_sampleCore->resize( pWidth, pHeight );
}

/******************************************************************************
 * Get the viewer size hint
 *
 * @return the viewer size hint
 ******************************************************************************/
QSize SampleViewer::sizeHint() const
{
	// Default size
	return QSize( 512, 512 );
}

/******************************************************************************
 * Key press event handler
 *
 * @param pEvent the event
 ******************************************************************************/
void SampleViewer::keyPressEvent( QKeyEvent* pEvent )
{
	switch ( pEvent->key() )
	{
		case Qt::Key_Plus:
			_sampleCore->incMaxVolTreeDepth();
			break;

		case Qt::Key_Minus:
			_sampleCore->decMaxVolTreeDepth();
			break;

		case Qt::Key_C:
			// Tell GigaVoxels to clear its cache
			_sampleCore->clearCache();
			break;

		case Qt::Key_D:
			_sampleCore->toggleDynamicUpdate();
			break;

		case Qt::Key_I:
			// Toggle GigaVoxels performance monitor mechanism (if it has been activated during GigaVoxels compilation)
			_sampleCore->togglePerfmonDisplay( 1 );
			break;

		case Qt::Key_T:
			// Toggle the display of the GigaVoxels space partitioning structure
			_sampleCore->toggleDisplayOctree();
			break;

		case Qt::Key_U:
			// Toggle GigaVoxels performance monitor mechanism (if it has been activated during GigaVoxels compilation)
			_sampleCore->togglePerfmonDisplay( 2 );
			break;

		case Qt::Key_Insert:
			// Modify DOF aperture
			_sampleCore->setAperture( _sampleCore->getAperture() + 0.1f );
			break;

		case Qt::Key_Delete:
			// Modify DOF aperture
			_sampleCore->setAperture( _sampleCore->getAperture() - 0.1f );
			break;

		case Qt::Key_Home:
			// Modify DOF focal length
			_sampleCore->setFocalLength( _sampleCore->getFocalLength() + 0.1f );
			break;

		case Qt::Key_End:
			// Modify DOF focal length
			_sampleCore->setFocalLength( _sampleCore->getFocalLength() - 0.1f );
			break;

		case Qt::Key_PageUp:
			// Modify DOF plane in focus
			_sampleCore->setPlaneInFocus( _sampleCore->getPlaneInFocus() + 0.1f );
			break;

		case Qt::Key_PageDown:
			// Modify DOF plane in focus
			_sampleCore->setPlaneInFocus( _sampleCore->getPlaneInFocus() - 0.1f );
			break;

		case Qt::Key_L:
			// Toggle light manipulation mechanism
			setLightManipulation( ! getLightManipulation() );
			break;

		case Qt::Key_R:
			// Reset light position
			_light1->setPosition( 1.0f, 1.0f, 1.0f );
			// Update GigaVoxels light position
			_sampleCore->setLightPosition( make_float3( 1.0f, 1.0f, 1.0f ) );
			break;

		default:
			QGLViewer::keyPressEvent( pEvent );
			break;
	}
}

/******************************************************************************
 * Get the flag to tell wheter or not light manipulation is activated
 *
 * @return the light manipulation flag
 ******************************************************************************/
bool SampleViewer::getLightManipulation() const
{
	return _lightManipulation;
}

/******************************************************************************
 * Set the flag to tell wheter or not light manipulation is activated
 *
 * @param pFlag the light manipulation flag
 ******************************************************************************/
void SampleViewer::setLightManipulation( bool pFlag )
{
	_lightManipulation = pFlag;

	// Modify mouse tracking state to enable real-time light manipulation
	setMouseTracking( pFlag );
}

/******************************************************************************
 * Slot called when the light ManipulatedFrame has been modified
 ******************************************************************************/
void SampleViewer::onLightFrameManipulated()
{
	if ( _sampleCore != NULL )
	{
		qreal pos[ 4 ] = { 1.f, 1.f, 1.f, 1.f };
		qreal x,y,z;
		_light1->getPosition( x, y, z );
		pos[ 0 ] = x;
		pos[ 1 ] = y;
		pos[ 2 ] = z;

		// Update GigaVoxels light position
		_sampleCore->setLightPosition( make_float3( pos[ 0 ], pos[ 1 ], pos[ 2 ] ) );
	}
}
