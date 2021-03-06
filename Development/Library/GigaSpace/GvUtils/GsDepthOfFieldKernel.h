/*
 * GigaVoxels - GigaSpace
 *
 * Website: http://gigavoxels.inrialpes.fr/
 *
 * Contributors: GigaVoxels Team
 *
 * BSD 3-Clause License:
 *
 * Copyright (C) 2007-2015 INRIA - LJK (CNRS - Grenoble University), All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the organization nor the names  of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * @version 1.0
 */

#ifndef _GV_DEPTH_OF_FIELD_KERNEL_H_
#define _GV_DEPTH_OF_FIELD_KERNEL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// Cuda
#include <cuda_runtime.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/
 
/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

 namespace GvUtils
 {
 
/** 
 * @class GvDepthOfFieldKernel
 *
 * @brief The GvDepthOfFieldKernel class provides basic interface to create depth of field
 *
 * It is based on CoC computation, the circle of confusion
 * (depth of field is used to enhance realism for visual effects)
 */
class GvDepthOfFieldKernel
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Get the CoC (circle of confusion) for the world-space distance
	 * from the camera-object distance calculated from camera parameters
	 *
	 * Object distance can be calculated from the z values in the z-buffer:
	 * objectdistance = -zfar * znear / (z * (zfar - znear) - zfar)
	 *
	 * @param pAperture camera lens aperture
	 * @param pFocalLength camera focal length
	 * @param pPlaneInFocus distance from the lens to the plane in focus
	 * @param pObjectDistance object distance from the lens
	 *
	 * @return the circle of confusion
	 */
	__device__
	static __forceinline__ float getCoC( const float pAperture, const float pFocalLength, const float pPlaneInFocus, const float pObjectDistance );

	/**
	 * Get the CoC (circle of confusion) calculated from the z-buffer values,
	 * with the camera parameters lumped into scale and bias terms :
	 * CoC = abs( z * CoCScale + CoCBias )
	 *
	 * @param pAperture camera lens aperture
	 * @param pFocalLength camera focal length
	 * @param pPlaneInFocus distance from the lens to the plane in focus
	 * @param pZNear camera z-near plane distance
	 * @param pZFar camera z-far plane distance
	 * @param pObjectDistance object distance from the lens
	 *
	 * @return the circle of confusion
	 */
	__device__
	static __forceinline__ float getCoC( const float pAperture, const float pFocalLength, const float pPlaneInFocus, const float pZNear, const float pZFar, const float pObjectDistance );
	
	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/
	
	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Compute the scale term of the CoC (circle of confusion) given camera parameters
	 *
	 * @param pAperture camera lens aperture
	 * @param pFocalLength camera focal length
	 * @param pPlaneInFocus distance from the lens to the plane in focus
	 * @param pZNear camera z-near plane distance
	 * @param pZFar camera z-far plane distance
	 *
	 * @return the the scale term of the circle of confusion
	 */
	__device__
	static __forceinline__ float getCoCScale( const float pAperture, const float pFocalLength, const float pPlaneInFocus, const float pZNear, const float pZFar );

	/**
	 * Compute the bias term of the CoC (circle of confusion) given camera parameters
	 *
	 * @param pAperture camera lens aperture
	 * @param pFocalLength camera focal length
	 * @param pPlaneInFocus distance from the lens to the plane in focus
	 * @param pZNear camera z-near plane distance
	 *
	 * @return the the bias term of the circle of confusion
	 */
	__device__
	static __forceinline__ float getCoCBias( const float pAperture, const float pFocalLength, const float pPlaneInFocus, const float pZNear );

};

} // namespace GvUtils

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GvDepthOfFieldKernel.inl"

#endif // !_GV_DEPTH_OF_FIELD_KERNEL_H_
