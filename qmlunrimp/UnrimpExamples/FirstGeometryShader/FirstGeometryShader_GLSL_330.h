/*********************************************************\
 * Copyright (c) 2012-2013 Christian Ofenberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Shader start                                          ]
//[-------------------------------------------------------]
#ifndef RENDERER_NO_OPENGL
if (0 == strcmp(renderer->getName(), "OpenGL"))
{


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// TODO(co) Attribute less rendering (aka "drawing without data") possible with OpenGL? For me it appears not to work, I see nothing and also get no error...
// -> Tested with: "Radeon HD 6970M", driver "catalyst_12-7_beta_windows7_20120629.exe"
// -> According to http://renderingpipeline.com/2012/03/are-vertex-shaders-obsolete/ it should work
// -> Apparently there are currently some issues when using this approach: http://www.opengl.org/discussion_boards/showthread.php/177372-Rendering-simple-shapes-without-passing-vertices
// One vertex shader invocation per vertex
vertexShaderSourceCode =
"#version 330 core\n"	// OpenGL 3.3
STRINGIFY(
// Attribute input/output
in float Position;	// Dummy

// Programs
void main()
{
	// Pass through the dummy
	gl_Position = vec4(Position, 0.0, 0.0, 1.0);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Geometry shader source code                           ]
//[-------------------------------------------------------]
// One geometry shader invocation per primitive
geometryShaderSourceCode =
"#version 330 core\n"	// OpenGL 3.3
STRINGIFY(
// Attribute input/output
layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

// Programs
void main()
{
	//				Vertex ID	Triangle on screen
	//  0.0f, 1.0f,	// 0			0
	//  1.0f, 0.0f,	// 1		   .   .
	// -0.5f, 0.0f	// 2		  2.......1

	// Emit vertex 0 clip space position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(0.0, 1.0, 0.0, 1.0);
	EmitVertex();

	// Emit vertex 1 clip space position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(1.0, 0.0, 0.0, 1.0);
	EmitVertex();

	// Emit vertex 2 clip space position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(-0.5, 0.0, 0.0, 1.0);
	EmitVertex();

	// Done
	EndPrimitive();
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode =
"#version 330 core\n"	// OpenGL 3.3
STRINGIFY(
// Attribute input/output
layout(location = 0, index = 0) out vec4 Color0;

// Programs
void main()
{
	// Return white
	Color0 = vec4(1.0, 1.0, 1.0, 1.0);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
#endif
