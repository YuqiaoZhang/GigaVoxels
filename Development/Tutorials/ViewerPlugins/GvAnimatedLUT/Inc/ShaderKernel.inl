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

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include <GvRendering/GvRendererContext.h>

/******************************************************************************
 ****************************** KERNEL DEFINITION *****************************
 ******************************************************************************/

/******************************************************************************
 * This method is called for each sample. For example, shading or secondary rays
 * should be done here.
 *
 * @param pBrickSampler brick sampler
 * @param pSamplePosScene position of the sample in the scene
 * @param pRayDir ray direction
 * @param pRayStep ray step
 * @param pConeAperture cone aperture
 ******************************************************************************/
template< typename SamplerType >
__device__
inline void ShaderKernel::runImpl( const SamplerType& brickSampler, const float3 samplePosScene,
										const float3 rayDir, float& rayStep, const float coneAperture )
{
	// Retrieve first channel element : color
	float4 color = brickSampler.template getValue< 0 >( coneAperture );

	// Fetch data from transfer function
	//float colorCyclingIndex = ( 0.5f * ( cosf( 2.f * 3.141592f / 256.f * k_currentTime * samplePosScene.x ) + 1.f ) );
	float temp = 0.5f/*half amplitude*/ * ( 0.5f * ( sinf( 2.f * 3.141592f / 128.f * ( 0.5f * k_currentTime ) * samplePosScene.y / 10.f + 13564.25f ) + 1.f ) );
	float colorCyclingIndex = ( 0.5f * ( cosf( 2.f * 3.141592f / 256.f * k_currentTime * samplePosScene.x * temp / 10.f + 64.18f ) + 1.f ) );
	color = tex1D( colorCyclingLookupTable, colorCyclingIndex );
	/*float4 animatedColor = tex1D( colorCyclingLookupTable, colorCyclingIndex );
	color.x = animatedColor.x;
	color.y = animatedColor.y;
	color.z = animatedColor.z;*/

	// Test opacity
	if ( color.w > 0.0f )
	{
		// Retrieve second channel element : normal
		float4 normal = brickSampler.template getValue< 1 >( coneAperture );

		float3 normalVec = normalize( make_float3( normal.x, normal.y, normal.z ) );
		float3 lightVec = normalize( cLightPosition );

		// Lambertian lighting
		float3 rgb = make_float3( color.x, color.y, color.z ) * max( 0.0f, dot( normalVec, lightVec ) );

		// Due to alpha pre-multiplication
		color.x = rgb.x / color.w;
		color.y = rgb.y / color.w;
		color.z = rgb.z / color.w;
		
		// -- [ Opacity correction ] --
		// The standard equation :
		//		_accColor = _accColor + ( 1.0f - _accColor.w ) * color;
		// must take alpha correction into account
		// NOTE : if ( color.w == 0 ) then alphaCorrection equals 0.f
		float alphaCorrection = ( 1.0f -_accColor.w ) * ( 1.0f - powf( 1.0f - color.w, rayStep * cShaderMaterialProperty ) );

		// Accumulate the color
		_accColor.x += alphaCorrection * color.x;
		_accColor.y += alphaCorrection * color.y;
		_accColor.z += alphaCorrection * color.z;
		_accColor.w += alphaCorrection;
	}	
}
