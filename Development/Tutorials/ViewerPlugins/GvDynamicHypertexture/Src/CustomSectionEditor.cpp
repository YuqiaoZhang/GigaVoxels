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

#include "CustomSectionEditor.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// Qt
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QToolBar>

// GvViewer
#include "Gvv3DWindow.h"
#include "GvvPipelineInterfaceViewer.h"
#include "GvvPluginManager.h"

#include "GvvApplication.h"
#include "GvvMainWindow.h"
#include "Gvv3DWindow.h"
#include "GvvPipelineInterfaceViewer.h"
#include "GvvPipelineInterface.h"

// Project
#include "SampleCore.h"

// STL
#include <iostream>

// System
#include <cassert>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GvViewer
using namespace GvViewerCore;
using namespace GvViewerGui;

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
 * Default constructor
 *
 * @param pParent parent widget
 * @param pFlags the window flags
 ******************************************************************************/
CustomSectionEditor::CustomSectionEditor( QWidget* pParent, Qt::WindowFlags pFlags )
:	GvvSectionEditor( pParent, pFlags )
{
	setupUi( this );

	// Editor name
	setName( tr( "Dynamic Hypertexture" ) );
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
CustomSectionEditor::~CustomSectionEditor()
{
}

/******************************************************************************
 * Populates this editor with the specified browsable
 *
 * @param pBrowsable specifies the browsable to be edited
 ******************************************************************************/
void CustomSectionEditor::populate( GvViewerCore::GvvBrowsable* pBrowsable )
{
	assert( pBrowsable != NULL );
	SampleCore* pipeline = dynamic_cast< SampleCore* >( pBrowsable );
	assert( pipeline != NULL );
	if ( pipeline != NULL )
	{
		// Hypertexture parameters
		//
		// - noise type
		_noiseActionDisplacement->setCurrentIndex( pipeline->getDisplacement() );
		_noiseComboBox->setCurrentIndex( pipeline->getNoiseFunctionModifier() );
		// - noise parameters
		_noiseFirstFrequencySpinBox->setValue( pipeline->getNoiseFirstFrequency() );
		_noiseShellWidthUpperBoundSpinBox->setVisible( pipeline->getDisplacement() );
		_labelNoiseUpperBound->setVisible( pipeline->getDisplacement() );
		_noiseShellWidthSpinBox->setValue( pipeline->getNoiseShellWidth() );
		_noiseShellWidthUpperBoundSpinBox->setValue( pipeline->getNoiseShellWidthUpperBound() );
		// - fill the shape
		_noiseFillTheShapeCheckBox->setEnabled( pipeline->getDisplacement() );
		_noiseFillTheShapeCheckBox->setChecked( pipeline->getFillTheShape() != 1 );
		// - animation
		_animateCheckBox->setChecked( pipeline->hasAnimation() );

		// Light parameters
		float x;
		float y;
		float z;
		pipeline->getLightPosition( x, y, z );
		_lightXDoubleSpinBox->setValue( x );
		_lightYDoubleSpinBox->setValue( y );
		_lightZDoubleSpinBox->setValue( z );

		// Shader parameters
		_voxelSizeMultiplierDoubleSpinBox->setValue( pipeline->getVoxelSizeMultiplier() );
	}
}

/******************************************************************************
 * Slot called when noise first frequency value has changed
 ******************************************************************************/
void CustomSectionEditor::on__noiseFirstFrequencySpinBox_valueChanged( double value )
{
	GvvApplication& application = GvvApplication::get();
	GvvMainWindow* mainWindow = application.getMainWindow();
	Gvv3DWindow* window3D = mainWindow->get3DWindow();
	GvvPipelineInterfaceViewer* pipelineViewer = window3D->getPipelineViewer();
	GvViewerCore::GvvPipelineInterface* pipeline = pipelineViewer->editPipeline();

	SampleCore* sampleCore = dynamic_cast< SampleCore* >( pipeline );
	assert( sampleCore != NULL );

	sampleCore->setNoiseFirstFrequency( value );
}

/******************************************************************************
 * Slot called when noise Shell width value has changed
 ******************************************************************************/
void CustomSectionEditor::on__noiseShellWidthSpinBox_valueChanged( double value )
{
	GvvApplication& application = GvvApplication::get();
	GvvMainWindow* mainWindow = application.getMainWindow();
	Gvv3DWindow* window3D = mainWindow->get3DWindow();
	GvvPipelineInterfaceViewer* pipelineViewer = window3D->getPipelineViewer();
	GvViewerCore::GvvPipelineInterface* pipeline = pipelineViewer->editPipeline();

	SampleCore* sampleCore = dynamic_cast< SampleCore* >( pipeline );
	assert( sampleCore != NULL );

	sampleCore->setNoiseShellWidth( value );
}

/******************************************************************************
 * Slot called when noise shell width upper bound value has changed
 ******************************************************************************/
void CustomSectionEditor::on__noiseShellWidthUpperBoundSpinBox_valueChanged( double value )
{
	GvvApplication& application = GvvApplication::get();
	GvvMainWindow* mainWindow = application.getMainWindow();
	Gvv3DWindow* window3D = mainWindow->get3DWindow();
	GvvPipelineInterfaceViewer* pipelineViewer = window3D->getPipelineViewer();
	GvViewerCore::GvvPipelineInterface* pipeline = pipelineViewer->editPipeline();

	SampleCore* sampleCore = dynamic_cast< SampleCore* >( pipeline );
	assert( sampleCore != NULL );

	sampleCore->setNoiseShellWidthUpperBound( value );

	_noiseShellWidthSpinBox->setMaximum( value );
	if ( _noiseShellWidthSpinBox->value() > value )
	{
		_noiseShellWidthSpinBox->setValue( value );
	}
}


/******************************************************************************
 * Slot called when noise noise action displacement value has changed
 ******************************************************************************/
void CustomSectionEditor::on__noiseActionDisplacement_currentIndexChanged( int value )
{
	GvvApplication& application = GvvApplication::get();
	GvvMainWindow* mainWindow = application.getMainWindow();
	Gvv3DWindow* window3D = mainWindow->get3DWindow();
	GvvPipelineInterfaceViewer* pipelineViewer = window3D->getPipelineViewer();
	GvViewerCore::GvvPipelineInterface* pipeline = pipelineViewer->editPipeline();

	SampleCore* sampleCore = dynamic_cast< SampleCore* >( pipeline );
	assert( sampleCore != NULL );
	sampleCore->setDisplacement( value );

	if ( value == 0 )
	{
		// Color Displacement
		_noiseShellWidthSpinBox->setMaximum( 1.f );
		_noiseShellWidthUpperBoundSpinBox->setVisible( false );
		_labelNoiseUpperBound->setVisible( false );

		_noiseFillTheShapeCheckBox->setEnabled( false );
		sampleCore->setFillTheShape( 1 );
	}
	else
	{
		// Normal Displacement
		_noiseShellWidthSpinBox->setMaximum( _noiseShellWidthUpperBoundSpinBox->value() );
		if ( _noiseShellWidthSpinBox->value() > _noiseShellWidthUpperBoundSpinBox->value() )
		{
			_noiseShellWidthSpinBox->setValue( _noiseShellWidthUpperBoundSpinBox->value() );
		}
		_noiseShellWidthUpperBoundSpinBox->setVisible( true );
		_labelNoiseUpperBound->setVisible( true );

		_noiseFillTheShapeCheckBox->setEnabled( true );
		if ( _noiseFillTheShapeCheckBox->isChecked() )
		{
			sampleCore->setFillTheShape( 0 );
		}
		else
		{
			sampleCore->setFillTheShape( 2 );
		}
	}
}

/******************************************************************************
 * Slot called when FillTheShape value has changed
 ******************************************************************************/
void CustomSectionEditor::on__noiseFillTheShapeCheckBox_clicked( bool checked )
{
	GvvApplication& application = GvvApplication::get();
	GvvMainWindow* mainWindow = application.getMainWindow();
	Gvv3DWindow* window3D = mainWindow->get3DWindow();
	GvvPipelineInterfaceViewer* pipelineViewer = window3D->getPipelineViewer();
	GvViewerCore::GvvPipelineInterface* pipeline = pipelineViewer->editPipeline();

	SampleCore* sampleCore = dynamic_cast< SampleCore* >( pipeline );
	assert( sampleCore != NULL );

	if ( checked )
	{
		sampleCore->setFillTheShape( 0 );
	}
	else
	{
		sampleCore->setFillTheShape( 2 );
	}
}

/******************************************************************************
 * Slot called when light position X value has changed
 ******************************************************************************/
void CustomSectionEditor::on__lightXDoubleSpinBox_valueChanged( double value )
{
	GvvApplication& application = GvvApplication::get();
	GvvMainWindow* mainWindow = application.getMainWindow();
	Gvv3DWindow* window3D = mainWindow->get3DWindow();
	GvvPipelineInterfaceViewer* pipelineViewer = window3D->getPipelineViewer();
	GvViewerCore::GvvPipelineInterface* pipeline = pipelineViewer->editPipeline();

	SampleCore* sampleCore = dynamic_cast< SampleCore* >( pipeline );
	assert( sampleCore != NULL );

	float x;
	float y;
	float z;
	sampleCore->getLightPosition( x, y, z );
	x = value;
	sampleCore->setLightPosition( x, y, z );
}

/******************************************************************************
 * Slot called when light position Y value has changed
 ******************************************************************************/
void CustomSectionEditor::on__lightYDoubleSpinBox_valueChanged( double value )
{
	GvvApplication& application = GvvApplication::get();
	GvvMainWindow* mainWindow = application.getMainWindow();
	Gvv3DWindow* window3D = mainWindow->get3DWindow();
	GvvPipelineInterfaceViewer* pipelineViewer = window3D->getPipelineViewer();
	GvViewerCore::GvvPipelineInterface* pipeline = pipelineViewer->editPipeline();

	SampleCore* sampleCore = dynamic_cast< SampleCore* >( pipeline );
	assert( sampleCore != NULL );

	float x;
	float y;
	float z;
	sampleCore->getLightPosition( x, y, z );
	y = value;
	sampleCore->setLightPosition( x, y, z );
}

/******************************************************************************
 * Slot called when light position Y value has changed
 ******************************************************************************/
void CustomSectionEditor::on__lightZDoubleSpinBox_valueChanged( double value )
{
	GvvApplication& application = GvvApplication::get();
	GvvMainWindow* mainWindow = application.getMainWindow();
	Gvv3DWindow* window3D = mainWindow->get3DWindow();
	GvvPipelineInterfaceViewer* pipelineViewer = window3D->getPipelineViewer();
	GvViewerCore::GvvPipelineInterface* pipeline = pipelineViewer->editPipeline();

	SampleCore* sampleCore = dynamic_cast< SampleCore* >( pipeline );
	assert( sampleCore != NULL );

	float x;
	float y;
	float z;
	sampleCore->getLightPosition( x, y, z );
	z = value;
	sampleCore->setLightPosition( x, y, z );
}

/******************************************************************************
 *  * Slot called when shader's voxel size multiplier value has changed
 ******************************************************************************/
void CustomSectionEditor::on__voxelSizeMultiplierDoubleSpinBox_valueChanged( double value )
{
	GvvApplication& application = GvvApplication::get();
	GvvMainWindow* mainWindow = application.getMainWindow();
	Gvv3DWindow* window3D = mainWindow->get3DWindow();
	GvvPipelineInterfaceViewer* pipelineViewer = window3D->getPipelineViewer();
	GvViewerCore::GvvPipelineInterface* pipeline = pipelineViewer->editPipeline();

	SampleCore* sampleCore = dynamic_cast< SampleCore* >( pipeline );
	assert( sampleCore != NULL );

	sampleCore->setVoxelSizeMultiplier( value );
}

/******************************************************************************
 *  * Slot called when the noise function modifier is changed
 ******************************************************************************/
void CustomSectionEditor::on__noiseComboBox_currentIndexChanged( int value )
{
	GvvApplication& application = GvvApplication::get();
	GvvMainWindow* mainWindow = application.getMainWindow();
	Gvv3DWindow* window3D = mainWindow->get3DWindow();
	GvvPipelineInterfaceViewer* pipelineViewer = window3D->getPipelineViewer();
	GvViewerCore::GvvPipelineInterface* pipeline = pipelineViewer->editPipeline();

	SampleCore* sampleCore = dynamic_cast< SampleCore* >( pipeline );
	assert( sampleCore != NULL );

	sampleCore->setNoiseFunctionModifier( value );
}

/******************************************************************************
 * Slot called when the animate check box value is changed
 ******************************************************************************/
void CustomSectionEditor::on__animateCheckBox_clicked (bool checked ) 
{
	GvvApplication& application = GvvApplication::get();
	GvvMainWindow* mainWindow = application.getMainWindow();
	Gvv3DWindow* window3D = mainWindow->get3DWindow();
	GvvPipelineInterfaceViewer* pipelineViewer = window3D->getPipelineViewer();
	GvViewerCore::GvvPipelineInterface* pipeline = pipelineViewer->editPipeline();

	SampleCore* sampleCore = dynamic_cast< SampleCore* >( pipeline );
	assert( sampleCore != NULL );

	sampleCore->setAnimation( checked );
}
