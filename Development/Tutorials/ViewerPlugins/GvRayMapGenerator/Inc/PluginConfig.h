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

/**
 * ...
 */
#ifndef GVRAYMAPGENERATOR_CONFIG_H
#define GVRAYMAPGENERATOR_CONFIG_H

//*** Plugin Library 

// Static or dynamic link configuration
#ifdef WIN32
#	ifdef GVRAYMAPGENERATOR_MAKELIB	// Create a static library.
#		define GVRAYMAPGENERATOR_EXPORT
#		define GVRAYMAPGENERATOR_TEMPLATE_EXPORT
#	elif defined GVRAYMAPGENERATOR_USELIB	// Use a static library.
#		define GVRAYMAPGENERATOR_EXPORT
#		define GVRAYMAPGENERATOR_TEMPLATE_EXPORT

#	elif defined GVRAYMAPGENERATOR_MAKEDLL	// Create a DLL library.
#		define GVRAYMAPGENERATOR_EXPORT	__declspec(dllexport)
#		define GVRAYMAPGENERATOR_TEMPLATE_EXPORT
#	else	// Use DLL library
#		define GVRAYMAPGENERATOR_EXPORT	__declspec(dllimport)
#		define GVRAYMAPGENERATOR_TEMPLATE_EXPORT	extern
#	endif
#else
#	 if defined(GVRAYMAPGENERATOR_MAKEDLL) || defined(GVRAYMAPGENERATOR_MAKELIB)
#		define GVRAYMAPGENERATOR_EXPORT
#		define GVRAYMAPGENERATOR_TEMPLATE_EXPORT
#	else
#		define GVRAYMAPGENERATOR_EXPORT
#		define GVRAYMAPGENERATOR_TEMPLATE_EXPORT	extern
#	endif
#endif

#endif
