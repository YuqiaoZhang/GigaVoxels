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

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// OpenGL
#include <GsGL/glew.h>
#include <GL/freeglut.h>

// Qt
#include <QApplication>

// Project
#include "SampleViewer.h"

// GigaVoxels
#include <GvCore/GvVersion.h>
#include <GvCore/GvDeviceManager.h>

// STL
#include <iostream>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GvCore;

// STL
using namespace std;

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
 * Main entry program
 *
 * @param pArgc number of arguments
 * @param pArgv list of arguments
 *
 * @return code
 ******************************************************************************/
int main( int pArgc, char* pArgv[] )
{
	// Exit code
	int result = 0;

	// GLUT initialization
	glutInit( &pArgc, pArgv );

	// Qt main application
	QApplication app( pArgc, pArgv );

	// GigaVoxels API's version
	cout << "GigaVoxels API's version : " << GvVersion::getVersion() << endl;

	// Test client architecture
	// If harware is compliant with le GigaVoxels Engine, launch the demo
	SampleViewer* sampleViewer = NULL;
	if ( GvDeviceManager::get().initialize() )
	{
		// Create your QGLViewer custom widget
		sampleViewer = new SampleViewer();
		if ( sampleViewer != NULL )
		{
			sampleViewer->setWindowTitle( "Cache Management : Inspector view" );
			sampleViewer->show();

			// Enter Qt main event loop
			result = app.exec();
		}
	}
	else
	{
		cout << "\nThe program will now exit" << endl;
	}
	
	// Release memory
	delete sampleViewer;
	GvDeviceManager::get().finalize();

	// CUDA tip: clean up to ensure correct profiling
	cudaError_t error = cudaDeviceReset();
	
	// Return exit code
	return result;
}
