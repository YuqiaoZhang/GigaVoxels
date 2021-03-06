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
#include <GvRendering/GsRendererContext.h>

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

/******************************************************************************
 ****************************** KERNEL DEFINITION *****************************
 ******************************************************************************/

/******************************************************************************
 * ...
 *
 * @param materialColor ...
 * @param normalVec ...
 * @param lightVec ...
 * @param eyeVec ...
 * @param ambientTerm ...
 * @param diffuseTerm ...
 * @param specularTerm ...
 *
 * @return ...
 ******************************************************************************/
__device__
inline float3 ShaderKernel::shadePointLight( float3 materialColor, float3 normalVec, float3 lightVec, float3 eyeVec,
	float3 ambientTerm, float3 diffuseTerm, float3 specularTerm )
{
	float3 final_color = materialColor * ambientTerm;

	//float lightDist=length(lightVec);
	float3 lightVecNorm = (lightVec);
	float lambertTerm = (dot( normalVec, lightVecNorm ));

	if ( lambertTerm > 0.0f )
	{
		//Diffuse
		final_color += materialColor * diffuseTerm * lambertTerm ;

	//	float3 halfVec = normalize( lightVecNorm + eyeVec );//*0.5f;
	//	float specular = __powf( max( dot( normalVec, halfVec ), 0.0f ), 64.0f );

		//Specular
		//final_color += make_float3(specular)*specularTerm;
	}

	return final_color;
}

/******************************************************************************
 * This method returns the cone aperture for a given distance.
 *
 * @param pTTree the current distance along the ray's direction.
 *
 * @return the cone aperture
 ******************************************************************************/
//__device__
//inline float ShaderKernel::getConeApertureImpl( const float tTree ) const
//{
//	// Overestimate to avoid aliasing
//	//const float scaleFactor = 1.333f;
//	//return k_renderViewContext.pixelSize.x * tTree * scaleFactor * k_renderViewContext.frustumNearINV;
//
//	return k_renderViewContext.pixelSize.x * tTree * /*GvRendering::k_voxelSizeMultiplier*/cLightPosition.x * k_renderViewContext.frustumNearINV;
//}

/******************************************************************************
 * ...
 *
 * @param brickSampler ...
 * @param samplePosScene ...
 * @param rayDir ...
 * @param rayStep ...
 * @param coneAperture ...
 ******************************************************************************/
template< typename BrickSamplerType >
__device__
inline void ShaderKernel::runImpl( const BrickSamplerType& brickSampler, const float3 samplePosScene,
					const float3 rayDir, float& rayStep, const float coneAperture )
{
	float4 col = brickSampler.template getValue< 0 >( coneAperture );

	if ( col.w > 0.0f )
	{
#ifdef _DATA_HAS_NORMALS_
	const float4 normal = brickSampler.template getValue< 1 >( coneAperture );
	//printf( "%f %f %f \n", normal.x, normal.y, normal.z );
#endif

#ifdef _DATA_HAS_NORMALS_
		float3 grad = make_float3( normal.x, normal.y, normal.z );
#else
		float3 grad = make_float3( 0.0f );

		float gradStep = rayStep * 0.25f;

		float4 v0, v1;

		v0 = brickSampler.template getValue< 0 >( coneAperture, make_float3( gradStep, 0.0f, 0.0f) );
		v1 = brickSampler.template getValue< 0 >( coneAperture, make_float3(-gradStep, 0.0f, 0.0f) );
		grad.x = v0.w - v1.w;

		v0 = brickSampler.template getValue< 0 >( coneAperture, make_float3(0.0f,  gradStep, 0.0f) );
		v1 = brickSampler.template getValue< 0 >( coneAperture, make_float3(0.0f, -gradStep, 0.0f) );
		grad.y = v0.w - v1.w;

		v0 = brickSampler.template getValue< 0 >( coneAperture, make_float3(0.0f, 0.0f,  gradStep) );
		v1 = brickSampler.template getValue< 0 >( coneAperture, make_float3(0.0f, 0.0f, -gradStep) );
		grad.z = v0.w - v1.w;
#endif
		if ( length( grad ) > 0.0f )
		{
#ifdef _DATA_HAS_NORMALS_
#else
			grad =- grad;
#endif
			grad = normalize( grad );

			const float vis = 1.0f;

			const float3 lightVec = normalize( cLightPosition - samplePosScene );
			const float3 viewVec = ( -1.0f * rayDir );

			float3 rgb;
			rgb.x = col.x;
			rgb.y = col.y;
			rgb.z = col.z;
			rgb = shadePointLight( rgb, grad, lightVec, viewVec, make_float3( 0.2f * vis ), make_float3( 1.0f * (0.3f + vis*0.7f) ), make_float3(0.9f) );
		
			// Due to alpha pre-multiplication
			//
			// Note : inspecting device SASS code, assembly langage seemed to compute (1.f / color.w) each time, that's why we use a constant and multiply
			const float alphaPremultiplyConstant = 1.f / col.w;
			col.x = rgb.x * alphaPremultiplyConstant;
			col.y = rgb.y * alphaPremultiplyConstant;
			col.z = rgb.z * alphaPremultiplyConstant;

			/*float eps =1.f/255.f;
			col.x = (col.x > 153.f/255.f -eps && col.x < 153.f/255.f +eps)? col.x : 1.f;
			col.y = (col.y > 153.f/255.f -eps && col.y < 153.f/255.f +eps)? col.y : 0.f;
			col.z = (col.z > 153.f/255.f -eps && col.z < 153.f/255.f +eps)? col.z : 0.f;*/
		}
		else
		{
			//col = make_float4( 0.0f );

			// Problem !
			// In this case, the normal generated by the gradient is null...
			// That generates visual artefacts...
			//col = make_float4( 0.0f );
			//color = make_float4( 1.0, 0.f, 0.f, 1.0f );
			// Ambient : no shading
			//float3 final_color = materialColor * ambientTerm;
			float vis = 1.0f;
			float3 rgb;
			rgb.x = col.x; rgb.y = col.y; rgb.z = col.z;
			float3 final_color = rgb * make_float3( 0.2f * vis );
#ifdef _DEBUG
			final_color = make_float3( 1.0, 0.f, 0.f );
#endif

			// Due to alpha pre-multiplication
			//
			// Note : inspecting device SASS code, assembly langage seemed to compute (1.f / color.w) each time, that's why we use a constant and multiply
			const float alphaPremultiplyConstant = 1.f / col.w;
			col.x = final_color.x * alphaPremultiplyConstant;
			col.y = final_color.y * alphaPremultiplyConstant;
			col.z = final_color.z * alphaPremultiplyConstant;
		}
	
		// -- [ Opacity correction ] --
		// The standard equation :
		//		_accColor = _accColor + ( 1.0f - _accColor.w ) * color;
		// must take alpha correction into account
		// NOTE : if ( color.w == 0 ) then alphaCorrection equals 0.f
		const float alphaCorrection = ( 1.0f -_accColor.w ) * ( 1.0f - __powf( 1.0f - col.w, rayStep * 512.f ) );

		// Accumulate the color
		_accColor.x += alphaCorrection * col.x;
		_accColor.y += alphaCorrection * col.y;
		_accColor.z += alphaCorrection * col.z;
		_accColor.w += alphaCorrection;
	}
}
