////////////////////////////////////////////////////////////////////////////////
//
// VERTEX SHADER
//
// ...
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// VERSION
////////////////////////////////////////////////////////////////////////////////

#version 420

////////////////////////////////////////////////////////////////////////////////
// INPUT
////////////////////////////////////////////////////////////////////////////////

// Vertex position
in vec3 iPosition;

// Vertex normal
in vec3 iNormal;

////////////////////////////////////////////////////////////////////////////////
// UNIFORM
////////////////////////////////////////////////////////////////////////////////

// Projection matrix
uniform mat4 uProjectionMatrix;

// ModelView matrix
uniform mat4 uModelViewMatrix;

////////////////////////////////////////////////////////////////////////////////
// OUTPUT
////////////////////////////////////////////////////////////////////////////////

// Output data structure
out Data
{
	// ...
	float distMin;
	// ...
	vec3 normal;

} oData;

////////////////////////////////////////////////////////////////////////////////
// PROGRAM
////////////////////////////////////////////////////////////////////////////////
void main()
{
	// Compute position in eye-space coordinates system
	vec4 positionInView = uModelViewMatrix * vec4( iPosition, 1 );

	// Output distance along z axis in eye-space coordinates system
	oData.distMin = positionInView.z;

	// Modify z position to see all fragments
	positionInView.z = - abs( positionInView.z );

	// Output untransformed normal (in object coordinates system)
	oData.normal = iNormal;

	// Transform vertex to clip space
	gl_Position =  uProjectionMatrix * positionInView;
}