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
	const float lambertTerm = (dot( normalVec, lightVecNorm ));

	if ( lambertTerm > 0.0f )
	{
		// Diffuse component
		final_color += materialColor * diffuseTerm * lambertTerm ;

		// Specular component
		//const float3 halfVec = normalize( lightVecNorm + eyeVec );//*0.5f;
		//const float specular = __powf( max( dot( normalVec, halfVec ), 0.0f ), 64.0f );
		//final_color += make_float3(specular)*specularTerm;
	}

	return final_color;
}

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
		float3 grad = make_float3(0.0f);

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

		if ( length( grad ) > 0.0f )
		{
			grad =- grad;
			grad = normalize( grad );

			float vis = 1.0f;

			const float3 lightVec = normalize( cLightPosition - samplePosScene );
			const float3 viewVec = (-1.0f * rayDir);

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
			const float3 final_color = rgb * make_float3( 0.2f * vis );

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
