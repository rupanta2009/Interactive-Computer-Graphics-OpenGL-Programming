/* 
File Name: "fshader_fireworks.glsl":
           Fragment Shader
*/

// #version 150 


in  vec4 color;
in float yPosition;
out vec4 fColor;

void main() 
{ 
	if (yPosition < 0.1)
		discard;
	else
		fColor = color;
} 

