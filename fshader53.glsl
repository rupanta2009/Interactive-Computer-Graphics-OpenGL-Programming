/* 
File Name: "fshader53.glsl":
           Fragment Shader
*/

// #version 150 


in  vec4 color;
in  vec4 point;
in  float sphereTexture;
in  vec2 texCoord;
in  vec2 sphereTexture1;
in  vec2 sphereLattice;
out vec4 fColor;

uniform vec4 fogColor;
uniform float starting;
uniform float ending;
uniform float density;
uniform int fogFlag;

uniform int Texture_ground_flag;
uniform sampler2D texture_2D;
uniform sampler1D sphere_1D;
uniform sampler2D sphere_2D;
uniform int sphereGroundFlag;
uniform int latticeFlag;

float f;
float z_distance = point.z;
vec4 tempColor = color;

void main() 
{ 
	if (sphereGroundFlag == 1)
	{
		tempColor = color * texture(sphere_1D, sphereTexture);
	}
	if (sphereGroundFlag == 2)
	{
		if ((texture(sphere_2D, sphereTexture1)).x < 0.5)
		{
			tempColor = color * vec4(0.9,0.1,0.1,1.0);
		}
		else
			tempColor = color * texture(sphere_2D, sphereTexture1);
	}
	if (latticeFlag == 1)
	{
		if (fract(4 * sphereLattice.x) < 0.35 && fract(4 * sphereLattice.y) < 0.35)
			discard;
	}
	if (Texture_ground_flag == 1)
	{
		tempColor = color * texture(texture_2D, texCoord);
	}
	if (fogFlag == 0)
	{
		fColor = tempColor;
	}
	if (fogFlag == 1)
	{
		f = (ending - z_distance) / (ending - starting);
		f = clamp(f, 0.0, 1.0);
		fColor = mix(fogColor, tempColor, f);
	}
	if (fogFlag == 2)
	{
		f = exp(-1 * (density * z_distance));
		f = clamp(f, 0.0, 1.0);
		fColor = mix(fogColor, tempColor, f);
	}
	if (fogFlag == 3)
	{
		f = exp(-1 * (density * z_distance) * (density * z_distance));
		f = clamp(f, 0.0, 1.0);
		fColor = mix(fogColor, tempColor, f);
	}
}