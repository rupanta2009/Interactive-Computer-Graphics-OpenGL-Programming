/***************************
 * File: vshader_fireworks.glsl:
 *   A simple vertex shader.
 *
 * - Vertex attributes (positions & colors) for all vertices are sent
 *   to the GPU via a vertex buffer object created in the OpenGL program.
 *
 * - This vertex shader uses the Model-View and Projection matrices passed
 *   on from the OpenGL program as uniform variables of type mat4.
 ***************************/

 #version 150  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

in  vec3 vPosition;
in  vec4 vColor;
in  vec3 vVelocity;
out vec4 color;
out float yPosition;

uniform mat4 model_view;
uniform mat4 projection;
uniform float time;

void main() 
{
	
    float x_position = vPosition.x + (vVelocity.x * time * 0.001);
	float y_position = vPosition.y + (vVelocity.y * time * 0.001) + (0.5* (-0.00000049) * time * time);
	float z_position = vPosition.z + (vVelocity.z * time * 0.001);
	
	vec4 vPosition4 = vec4(x_position, y_position, z_position, 1.0);
	gl_Position = projection * model_view * vPosition4;

	yPosition = y_position;
    color = vColor;
} 
