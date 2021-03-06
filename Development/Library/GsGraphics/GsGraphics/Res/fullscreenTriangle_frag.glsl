////////////////////////////////////////////////////////////////////////////////
//
// FRAGMENT SHADER
//
// Fullscreen triangle rendering
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// VERSION
////////////////////////////////////////////////////////////////////////////////

#version 400 // compatibility

////////////////////////////////////////////////////////////////////////////////
// EXTENSION
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// DEFINE AND CONSTANT SECTION
////////////////////////////////////////////////////////////////////////////////

// Texture
#define GS_USE_TEXTURING

////////////////////////////////////////////////////////////////////////////////
// INPUT
////////////////////////////////////////////////////////////////////////////////

#ifdef GS_USE_TEXTURING
// Texture coordinate
in vec2 _texCoordV;
#endif

////////////////////////////////////////////////////////////////////////////////
// UNIFORM
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// OUTPUT
////////////////////////////////////////////////////////////////////////////////

// Output color
layout (location = 0) out vec4 _colorF;

////////////////////////////////////////////////////////////////////////////////
// PROGRAM
////////////////////////////////////////////////////////////////////////////////
void main()
{
#ifndef GS_USE_TEXTURING
	_colorF = vec4( 1.0, 0.0, 0.0, 1.0 );
#else
	_colorF = ;
#endif
}
