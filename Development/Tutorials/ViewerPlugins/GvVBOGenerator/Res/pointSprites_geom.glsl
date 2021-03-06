////////////////////////////////////////////////////////////////////////////////
//
// GEOMETRY SHADER
//
// Point Sprites (texture mapped quads)
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// VERSION
////////////////////////////////////////////////////////////////////////////////

#version 400

////////////////////////////////////////////////////////////////////////////////
// INPUT
////////////////////////////////////////////////////////////////////////////////

// Input are points
layout ( points ) in;

////////////////////////////////////////////////////////////////////////////////
// UNIFORM
////////////////////////////////////////////////////////////////////////////////

// Half width of the quad
uniform float HalfSize;

// Projection matrix
uniform mat4 ProjectionMatrix;

////////////////////////////////////////////////////////////////////////////////
// OUTPUT
////////////////////////////////////////////////////////////////////////////////

// Geometry shader will generate a quad for each input point
layout ( triangle_strip, max_vertices = 4 ) out;

// Generated quad texture coordinates
out vec2 TexCoord;

////////////////////////////////////////////////////////////////////////////////
// PROGRAM
////////////////////////////////////////////////////////////////////////////////
void main()
{
	// Input positions are in eye coordinate system

	// Bottom left point
	gl_Position = ProjectionMatrix * ( gl_in[ 0 ].gl_Position + vec4( -HalfSize, -HalfSize, 0.0, 0.0 ) );
	TexCoord = vec2( 0.0, 0.0 );
	EmitVertex();

	// Bottom right point
	gl_Position = ProjectionMatrix * ( gl_in[ 0 ].gl_Position + vec4( HalfSize, -HalfSize, 0.0, 0.0 ) );
	TexCoord = vec2( 1.0, 0.0 );
	EmitVertex();

	// Top right point
	gl_Position = ProjectionMatrix * ( gl_in[ 0 ].gl_Position + vec4( -HalfSize, HalfSize, 0.0, 0.0 ) );
	TexCoord = vec2( 0.0, 1.0 );
	EmitVertex();

	// Top left point
	gl_Position = ProjectionMatrix * ( gl_in[ 0 ].gl_Position + vec4( HalfSize, HalfSize, 0.0, 0.0 ) );
	TexCoord = vec2( 1.0, 1.0 );
	EmitVertex();

	EndPrimitive();	// Not useful...
}
