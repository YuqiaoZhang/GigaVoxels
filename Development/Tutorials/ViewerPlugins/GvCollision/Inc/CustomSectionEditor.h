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

#ifndef _CUSTOM_SECTION_EDITOR_H_
#define _CUSTOM_SECTION_EDITOR_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GvViewer
#include <GvvSectionEditor.h>

// Qt
#include <QBrush>

// Project
#include "UI_GvQCustomEditor.h"

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

// GvViewer
namespace GvViewerCore
{
	class GvvBrowsable;
}

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/**
 * @class CustomSectionEditor
 *
 * @brief The CustomSectionEditor class provides the user custom editor to
 * this GigaVoxels pipeline effect.
 *
 * This editor is a child of the parent CustomEditor. It holds a user interface
 * to edit the GigaVoxels pipeline parameters.
 * It is a GvvSectionEditor, so it is reprenseted by a separate page of parameters
 * in the main viewer user interface.
 */
class CustomSectionEditor : public GvViewerGui::GvvSectionEditor, public Ui::GvQCustomEditor
{
	// Qt Macro
	Q_OBJECT

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Default constructor
	 *
	 * @param pParent parent widget
	 * @param pFlags the window flags
	 */
	CustomSectionEditor( QWidget* pParent = 0, Qt::WindowFlags pFlags = 0 );

	/**
	 * Destructor
	 */
	virtual ~CustomSectionEditor();

	/**
	 *
	 */
	void drawPoint( std::pair< float, unsigned int> point, QBrush brush );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Populates this editor with the specified browsable
	 *
	 * @param pBrowsable specifies the browsable to be edited
	 */
	virtual void populate( GvViewerCore::GvvBrowsable* pBrowsable );

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Variables used to store the time of production.
	 */
	static const unsigned int _size = 20;
	unsigned int _lastNodeProductionTimes[ _size ];
	unsigned int _next;
	float _total;

	/******************************** METHODS *********************************/

private slots:

	/**
	 * Slot called when noise first frequency value has changed
	 */
	void on__noiseFirstFrequencySpinBox_valueChanged( int value );

	/**
	 * Slot called when noise last frequency value has changed
	 */
	void on__noiseMaxFrequencySpinBox_valueChanged( int value );

	/**
	 * Slot called when noise strength value has changed
	 */
	void on__noiseStrengthSpinBox_valueChanged( double value );

	/**
	 * Slot called when noise type value has changed
	 */
	void on__noiseTypeComboBox_currentIndexChanged( int value );

	/**
	 * Slot called when light type has changed
	 */
	void on__lightTypeComboBox_currentIndexChanged( int value );

	/**
	 * Slot called when brightness value has changed
	 */
	void on__brightnessSpinBox_valueChanged( double value );

	/**
	 * Slot called when light position X value has changed
	 */
	void on__lightXDoubleSpinBox_valueChanged( double value );

	/**
	 * Slot called when light position Y value has changed
	 */
	void on__lightYDoubleSpinBox_valueChanged( double value );

	/**
	 * Slot called when light position Z value has changed
	 */
	void on__lightZDoubleSpinBox_valueChanged( double value );

	/**
	 * Slot called when the ambient reflection has changed.
	 */
	void on__ambientDoubleSpinBox_valueChanged( double value );

	/**
	 * Slot called when the diffuse reflection has changed.
	 */
	void on__diffuseDoubleSpinBox_valueChanged( double value );

	/**
	 * Slot called when the specular reflection has changed.
	 */
	void on__specularDoubleSpinBox_valueChanged( double value );

	/**
	 * Slot called each second to keep the graphics view up to date.
 	 */
	void updateTimerGraphicsView();

	/**
	 * Slot called when the torus radius has changed.
	 */
	void on__torusRadiusDoubleSpinBox_valueChanged( double value );

	/**
	 * Slot called when the tube radius has changed.
	 */
	void on__tubeRadiusDoubleSpinBox_valueChanged( double value );

	/**
	 * Slot called when the animation must run/stop.
	 */
	void on__runAnimationCheckBox_toggled( bool checked );

	/**
	 * Slot called when the number of particles has changed
	 */
	void on__nParticlesSpinBox_valueChanged( int value );

	/**
	 * Slot called when the animation must be restarted
	 */
	void on__resetAnimationPushButton_clicked();

	/**
	 * Slot called when the gravity has changed
	 */
	void on__gravityDoubleSpinBox_valueChanged( double value );

	/**
	 * Slot called when the rebound has changed
	 */
	void on__reboundDoubleSpinBox_valueChanged( double value );
};

#endif
