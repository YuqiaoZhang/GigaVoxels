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

#ifndef _SAMPLE_CORE_H_
#define _SAMPLE_CORE_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// Cuda
#include <cuda_runtime.h>
#include <vector_types.h>

// Cuda SDK
#include <helper_math.h>

// GL
#include <GsGL/glew.h>

// Loki
#include <loki/Typelist.h>

// GigaVoxels
#include <GvCore/GsVectorTypesExt.h>
#include <GvUtils/GsForwardDeclarationHelper.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

// GigaVoxels
namespace GvUtils
{
	class GsCommonGraphicsPass;
}
namespace GsGraphics
{
	class GsShaderProgram;
}

// Custom Producer
template< typename TDataStructureType >
class ProducerKernel;
template< typename TDataStructureType >
class /*ProducerKernel2*/ProducerTorusKernel;

// Custom Shader
class ShaderKernel;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

// Defines the type list representing the content of one voxel
typedef Loki::TL::MakeTypelist< uchar4, half4 >::Result DataType;

// Defines the size of a node tile
typedef GvCore::GsVec1D< 2 > NodeRes;

// Defines the size of a brick
typedef GvCore::GsVec1D< 8 > BrickRes;

// Defines the type of structure we want to use
typedef GvStructure::GsVolumeTree
<
	DataType,
	NodeRes, BrickRes
>
DataStructureType;

// Defines the type of the producer
typedef GvUtils::GsSimpleHostProducer
<
	ProducerKernel< DataStructureType >,
	DataStructureType
>
ProducerType;

// Defines the type of the producer
typedef GvUtils::GsSimpleHostProducer
<
	/*ProducerKernel2*/ProducerTorusKernel< DataStructureType >,
	DataStructureType
>
ProducerType2;

// Defines the type of the shader
typedef GvUtils::GsSimpleHostShader< ShaderKernel > ShaderType;

// Define the type of renderer
typedef GvStructure::GsDataProductionManager< DataStructureType > DataProductionManagerType;
typedef GvRendering::GsRendererCUDA< DataStructureType, DataProductionManagerType, ShaderType > RendererType;

/**
 * Custom pipeline
 */
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
class MyPipeline : public GvUtils::GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
{
	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	public:
		
		/**
		 * Constructor
		 */
		MyPipeline();

		/**
		 * Destructor
		 */
		virtual ~MyPipeline();

		/**
		 * Launch the main GigaSpace flow sequence
		 */
		virtual void execute( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport );

};

// Custom Pipeline
typedef MyPipeline
<
	ShaderType,
	DataStructureType,
	DataProductionManagerType
>
PipelineType;

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @class SampleCore
 *
 * @brief The SampleCore class provides a helper class containing a	GigaVoxels pipeline.
 *
 * A simple GigaVoxels pipeline consists of :
 * - a data structure
 * - a cache
 * - a custom producer
 * - a renderer
 *
 * The custom shader is pass as a template argument.
 *
 * Besides, this class enables the interoperability with OpenGL graphics library.
 */
class SampleCore
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Internal matrices
	 */
	float4x4 _modelMatrix;
	float4x4 _modelViewMatrix;
	float4x4 _projectionMatrix;
	int4 _viewport;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	SampleCore();

	/**
	 * Destructor
	 */
	~SampleCore();

	/**
	 * Initialize the GigaVoxels pipeline
	 */
	void init();

	/**
	 * Draw function called every frame
	 */
	void draw();

	/**
	 * Resize the frame
	 *
	 * @param width the new width
	 * @param height the new height
	 */
	void resize( int width, int height );

	/**
	 * Clear the GigaVoxels cache
	 */
	void clearCache();

	/**
	 * Toggle the display of the N-tree (octree) of the data structure
	 */
	void toggleDisplayOctree();

	/**
	 * Toggle the GigaVoxels dynamic update mode
	 */
	void toggleDynamicUpdate();

	/**
	 * Toggle the display of the performance monitor utility if
	 * GigaVoxels has been compiled with the Performance Monitor option
	 *
	 * @param mode The performance monitor mode (1 for CPU, 2 for DEVICE)
	 */
	void togglePerfmonDisplay( uint mode );

	/**
	 * Increment the max resolution of the data structure
	 */
	void incMaxVolTreeDepth();

	/**
	 * Decrement the max resolution of the data structure
	 */
	void decMaxVolTreeDepth();

	/**
	 * Specify color to clear the color buffer
	 *
	 * @param pRed red component
	 * @param pGreen green component
	 * @param pBlue blue component
	 * @param pAlpha alpha component
	 */
	void setClearColor( unsigned char pRed, unsigned char pGreen, unsigned char pBlue, unsigned char pAlpha );

	/**
	 * Set the light position
	 *
	 * @param pX the X light position
	 * @param pY the Y light position
	 * @param pZ the Z light position
	 */
	void setLightPosition( float pX, float pY, float pZ );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Reset graphics resources
	 */
	void resetGraphicsresources();

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/******************************* ATTRIBUTES *******************************/

	/**
	 * GigaSpace pipeline
	 */
	PipelineType* _pipeline;

	/**
	 * GigaSpace producer
	 */
	ProducerType* _producer;
	ProducerType2* _producer2;

	/**
	 * GigaSpace renderer
	 */
	RendererType* _renderer;

	/**
	 * Graphics environment
	 */
	GvUtils::GsCommonGraphicsPass* _graphicsEnvironment;

	/**
	 * Flag to tell wheter or not to display the N-tree (octree) of the data structure
	 */
	bool _displayOctree;

	/**
	 * Flag to tell wheter or not to display the performance monitor utility
	 */
	uint _displayPerfmon;

	/**
	 * Max resolution of the data structure
	 */
	uint _maxVolTreeDepth;

	/**
	 * Depth buffer
	 */
	GLuint _depthBuffer;

	/**
	 * Color texture
	 */
	GLuint _colorTex;

	/**
	 * Depth texture
	 */
	GLuint _depthTex;

	/**
	 * Frame buffer
	 */
	GLuint _frameBuffer;

	/**
	 * Frame width
	 */
	int _width;

	/**
	 * Frame height
	 */
	int _height;

	///**
	// * Internal matrices
	// */
	//float4x4 _modelViewMatrix;
	//float4x4 _projectionMatrix;
	//int4 _viewportMatrix;

	/**
	 * Shader program
	 */
	GsGraphics::GsShaderProgram* _shaderProgram;

	/**
	 * VAO for full-screen quad rendering (vertex array object)
	 */
	GLuint _fullscreenQuadVAO;

	/**
	 * VBO for full-screen quad rendering (vertex buffer object of positions)
	 */
	GLuint _fullscreenQuadVertexBuffer;

	/**
	 * Texture sampler location
	 */
	GLint _textureSamplerUniformLocation;

	/******************************** METHODS *********************************/

};

#endif // !_SAMPLE_CORE_H_
