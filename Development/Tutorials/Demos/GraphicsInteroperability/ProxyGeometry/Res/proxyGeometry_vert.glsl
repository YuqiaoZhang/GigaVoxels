////////////////////////////////////////////////////////////////////////////////
//
// VERTEX SHADER
//
// Proxy Geometry
//
// - Generate a depth map of a mesh faces
// - Depending of the fixed pipeline configuration, it will be based on closests or farthest faces
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// VERSION
////////////////////////////////////////////////////////////////////////////////

#version 400

////////////////////////////////////////////////////////////////////////////////
// INPUT
////////////////////////////////////////////////////////////////////////////////

layout (location = 0) in vec3 VertexPosition;

////////////////////////////////////////////////////////////////////////////////
// UNIFORM
////////////////////////////////////////////////////////////////////////////////

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
//uniform mat4 uModelViewProjectionMatrix;

////////////////////////////////////////////////////////////////////////////////
// OUTPUT
////////////////////////////////////////////////////////////////////////////////

// Vertex position in Eye space
out vec4 EyeVertexPosition;

////////////////////////////////////////////////////////////////////////////////
// PROGRAM
////////////////////////////////////////////////////////////////////////////////
void main()
{
	// Output eye space position
	EyeVertexPosition = uModelViewMatrix * vec4( VertexPosition, 1.0 );

	// Send vertex to clip space
	gl_Position = uProjectionMatrix * EyeVertexPosition;
}
