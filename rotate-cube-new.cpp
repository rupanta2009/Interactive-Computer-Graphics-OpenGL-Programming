
/*Created by		: RUPANTA RWITEEJ DUTTA							*/							*/
/*Date of Creation	: 18th December 2015							*/                                                           */

#include "Angel-yjc.h"
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>
#define pi 3.14159265359

typedef Angel::vec4  color4;
typedef Angel::vec3  point3;
typedef Angel::vec4  point4;

GLuint Angel::InitShader(const char* vShaderFile, const char* fShaderFile);

GLuint program;							/* shader program object id for Version 53*/
GLuint program_fireworks;				/* shader program object id for Version fireworks*/

GLuint sphere_buffer;						/* vertex buffer object id for sphere */
GLuint shadow_buffer;						/* vertex buffer object id for shadow */
GLuint floor_buffer;						/* vertex buffer object id for floor */
GLuint axes_buffer;							/* axes buffer object id for axes */
GLuint fireworks_buffer;					/* fireworks buffer object id for fireworks */

// global definitions for constants and global image arrays
#define ImageWidth  32
#define ImageHeight 32
GLubyte Image[ImageHeight][ImageWidth][4];

static GLuint texName;
static GLuint texName1;
static GLuint texName2;

#define	stripeImageWidth 32
GLubyte stripeImage[4 * stripeImageWidth];

// global variables for fireworks
const int fireworks_num = 300;
point3 fireworks_vertices[fireworks_num];
color4 fireworks_colors[fireworks_num];
point3 fireworks_velocity[fireworks_num];
point3 fireworks_initial = point3(0.0, 0.1, 0.0);
float t = 0, tsub = 0, tmax = 10000.0;

int num;
float a1, b1, c1, a2, b2, c2, a3, b3, c3;
char filename[20];

// Data Structure: Linked List to store file data dynamically
struct dataList {
	float a1value;
	float b1value;
	float c1value;
	float a2value;
	float b2value;
	float c2value;
	float a3value;
	float b3value;
	float c3value;
	struct dataList *next;
};
struct dataList *head = NULL;
struct dataList *curr = NULL;

//Function to create linked list
struct dataList *createList(float a1value, float b1value, float c1value, float a2value, float b2value, float c2value, float a3value, float b3value, float c3value)
{
	struct dataList *ptr = (struct dataList *)malloc(sizeof(struct dataList));
	if (NULL == ptr) {
		printf("Node Creation Failed");
		return NULL;
	}
	ptr->a1value = a1value;
	ptr->b1value = b1value;
	ptr->c1value = c1value;
	ptr->a2value = a2value;
	ptr->b2value = b2value;
	ptr->c2value = c2value;
	ptr->a3value = a3value;
	ptr->b3value = b3value;
	ptr->c3value = c3value;
	ptr->next = NULL;
	head = curr = ptr;
	return ptr;
}

//Function to add items to the linked list
struct dataList* addToList(float a1value, float b1value, float c1value, float a2value, float b2value, float c2value, float a3value, float b3value, float c3value)
{
	if (NULL == head) {
		return(createList(a1value, b1value, c1value, a2value, b2value, c2value, a3value, b3value, c3value));
	}
	struct dataList *ptr = (struct dataList *)malloc(sizeof(struct dataList));
	if (NULL == ptr) {
		printf("Node Creation Failed");
		return NULL;
	}
	ptr->a1value = a1value;
	ptr->b1value = b1value;
	ptr->c1value = c1value;
	ptr->a2value = a2value;
	ptr->b2value = b2value;
	ptr->c2value = c2value;
	ptr->a3value = a3value;
	ptr->b3value = b3value;
	ptr->c3value = c3value;
	ptr->next = NULL;
	curr->next = ptr;
	curr = ptr;
	return ptr;
}

void quad(int, int, int);
void shad(int, int, int);

point3 vertices[10] = {
	point3(-5.0,  0.0, -4.0),
	point3(-5.0,  0.0,  8.0),
	point3(5.0,  0.0, -4.0),
	point3(5.0,  0.0,  8.0)
};

vec2 floor_texCoord[6] = {
	vec2(1.0, 0.0),  
	vec2(0.0, 0.0),  
	vec2(0.0, 1.0),  

	vec2(1.0, 0.0),  
	vec2(1.0, 1.0),  
	vec2(0.0, 1.0),  
};

//Function to get items from the linked list
void getData(void)
{
	struct dataList * ptr = head;
	while (ptr != NULL) {
		a1 = ptr->a1value;
		b1 = ptr->b1value;
		c1 = ptr->c1value;
		a2 = ptr->a2value;
		b2 = ptr->b2value;
		c2 = ptr->c2value;
		a3 = ptr->a3value;
		b3 = ptr->b3value;
		c3 = ptr->c3value;
		ptr = ptr->next;
		vertices[4] = point3(a1, b1, c1);
		vertices[5] = point3(a2, b2, c2);
		vertices[6] = point3(a3, b3, c3);
		vertices[7] = point3(a1, b1, c1);
		vertices[8] = point3(a2, b2, c2);
		vertices[9] = point3(a3, b3, c3);
		quad(4, 5, 6);
		shad(7, 8, 9);
	}
	return;
}

//file_in(): file input dunction
void file_in(void)
{
	FILE *ifp;
	int i, n;
	float av1, bv1, cv1, av2, bv2, cv2, av3, bv3, cv3;
	char *mode = "r";
	ifp = fopen(filename, mode);              
	if (ifp == NULL) {
		fprintf(stderr, "Cant Open Input File!!");
		_getch();
		exit(1);
	}
	else {
		fscanf(ifp, "%d", &num);
		for (i = 0; i < num; i++) {
			fscanf(ifp, "%d", &n);
			fscanf(ifp, "%f %f %f", &av1, &bv1, &cv1);
			fscanf(ifp, "%f %f %f", &av2, &bv2, &cv2);
			fscanf(ifp, "%f %f %f", &av3, &bv3, &cv3);
			addToList(av1, bv1, cv1, av2, bv2, cv2, av3, bv3, cv3);
		}
		printf("Input File Name: %s\n", filename);
		printf("Done Reading File Content. Check Output Window!!!\n\n");
	}
	fclose(ifp);
}

// Projection transformation parameters
GLfloat  fovy = 60.0;						// Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;							// Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 12.0;

GLfloat angle = 0.0;						// rotation angle in degrees
vec4 init_eye(7.0, 3.0, -10.0, 1.0);		// initial viewer position
vec4 eye = init_eye;						// current viewer position

int colorFlag = 0;							// 1: pass normals; 0: pass colors
int animationFlag = 0;						// 1: animation; 0: non-animation. Toggled by key 'b' or 'B'
int rightClickFlag = 1;						// 1: animation; 0: non-animation. Toggled by mouse right click'
int sphereFillFlag = 1;						// 1: filled sphere; 0: wire-framed sphere. Toggeled from mouse menu
int shadowFlag = 1;							// 1: shadow; 0: no shadow. Toggled from mouse menu
int lightingFlag = 0;						// 1: lighting enabled; 0: lighting disabled. Toggeled from mouse menu
int shadingFlag = 0;						// 1: smooth shading; 0: flat shading. Toggeled from mouse menu
int sourceFlag = 0;							// 1: point source; 0: spot light. Toggeled from mouse menu
int fogFlag = 0;							// 1: linear; 2: exponential; 3: exponential square; 0: no fog. Toggeled from mouse menu
int blendFlag = 0;							// 1: with blending; 0: without blending. Toggeled from mouse menu
int textureGroundFlag = 0;					// 1: ground texture; 0: no ground texture. Toggeled from mouse menu
int textureSphereFlag = 0;					// 1: sphere texture contour lines; 2: sphere texture checkerboard; 0: no sphere texture
int verticalSlantedFlag = 1;				// 1: vertical sphere texture; 2: slanted sphere texture
int objectEyeFlag = 1;						// 1: object space; 2: eye space
int latticeFlag = 0;						// 1: lattice sphere effect ON; 0: lattice sphere effect OFF
int uprightTiltedFlag = 1;					// 1: upright lattice sphere; 2: tilted lattice sphere

int fireworksFlag = 0;						// 1: enable fireworks; 0: disable fireworks. Toggeled from mouse menu

int sphere_NumVertices = 0;					//in case of 256 triangles: 768, in case of 1024 triangles: 3072
int shadow_NumVertices = 0;					//in case of 256 triangles: 768, in case of 1024 triangles: 3072

#if 1
point3 sphere_points[3500];
color4 sphere_colors[3500];
vec3 sphere_flat_normals[3500];
vec3 sphere_smooth_normals[3500];

point3 shadow_points[3500];
color4 shadow_colors[3500];
#endif

const int floor_NumVertices = 6;			//(1 face)*(2 triangles/face)*(3 vertices/triangle)
point3 floor_points[floor_NumVertices];		// positions for all vertices
color4 floor_colors[floor_NumVertices];		// colors for all vertices
vec3 floor_flat_normals[floor_NumVertices];
vec3 floor_smooth_normals[floor_NumVertices];

const int axes_NumVertices = 6;				//(1 axis)*(3 axes)*(2 vertices/axis)
point3 axes_points[axes_NumVertices];		// positions for all axes
color4 axes_colors[axes_NumVertices];		// colors for all axes
																				
color4 vertex_colors[6] = {						// RGBA colors
	color4(0.0, 1.0, 0.0, 1.0),					// green
	color4(1.0, 0.84, 0.0, 1.0),                // goldern yellow
	color4(1.0, 0.0, 0.0, 1.0),					// red
	color4(1.0, 0.0, 1.0, 1.0),					// magenta
	color4(0.0, 0.0, 1.0, 1.0),					// blue
	color4(0.25, 0.25, 0.25, 0.65)				// gray
};

//----------------------------------------------------------------------------
point4 light_position = point4(-14.0, 12.0, -3.0, 1.0);	// light position
point4 light_point = point4(-6.0, 0.0, -4.5, 1.0);		// light pointing towards point

color4 global_ambient(1.0, 1.0, 1.0, 1.0);		// global ambient light color

color4 light_ambient(0.0, 0.0, 0.0, 1.0);		// directional (distant) light source color
color4 light_diffuse(0.8, 0.8, 0.8, 1.0);		// light diffuse color
color4 light_specular(0.2, 0.2, 0.2, 1.0);		// light specular color

point4 light_direction(0.1, 0.0, -1.0, 0.0);	// light direction

color4 ground_ambient(0.2, 0.2, 0.2, 1.0);		// ground ambient color
color4 ground_diffuse(0.0, 1.0, 0.0, 1.0);		// ground diffuse color
color4 ground_specular(0.0, 0.0, 0.0, 1.0);		// ground specular color

color4 sphere_ambient(0.2, 0.2, 0.2, 1.0);		// sphere ambient color
color4 sphere_diffuse(1.0, 0.84, 0.0, 1.0);		// sphere diffuse color
color4 sphere_specular(1.0, 0.84, 0.0, 1.0);	// sphere specular color

float sphere_shininess = 125.0;					// sphere shininess coefficient

color4 light_positional_diffuse(1.0, 1.0, 1.0, 1.0);	// positional light diffuse color
color4 light_positional_specular(1.0, 1.0, 1.0, 1.0);	// positional light specular color
color4 light_positional_ambient(0.0, 0.0, 0.0, 1.0);	// positional light ambient color

float const_att = 2.0;							// constant attenuation
float linear_att = 0.01;						// linear attenuation
float quad_att = 0.001;							// quadratic attenuation

float light_exponent = 15.0;					// exponent value
float light_angle = (float) (20.0 * M_PI) / 180.0;			// cut off angle value

color4 sphere_ambient_product = sphere_ambient * light_ambient;
color4 sphere_diffuse_product = sphere_diffuse * light_diffuse;
color4 sphere_specular_product = sphere_specular * light_specular;

color4 ground_ambient_product = ground_ambient * light_ambient;
color4 ground_diffuse_product = ground_diffuse *  light_diffuse;
color4 ground_specular_product = ground_specular * light_specular;

color4 sphere_positional_ambient_product = sphere_ambient * light_positional_ambient;
color4 sphere_positional_diffuse_product = sphere_diffuse * light_positional_diffuse;
color4 sphere_positional_specular_product = sphere_specular * light_positional_specular;

color4 ground_positional_ambient_product = ground_ambient * light_positional_ambient;
color4 ground_positional_diffuse_product = ground_diffuse *  light_positional_diffuse;
color4 ground_positional_specular_product = ground_specular * light_positional_specular;


color4 global_sphere_ambient = sphere_ambient * global_ambient;
color4 global_ground_ambient = ground_ambient * global_ambient;
//----------------------------------------------------------------------------

void SetUp_Lighting_Uniform_Vars_sphere(mat4 mv1)
{
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, sphere_ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, sphere_diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, sphere_specular_product);

	glUniform4fv(glGetUniformLocation(program, "PositionalAmbientProduct"), 1, sphere_positional_ambient_product);
	glUniform4fv(glGetUniformLocation(program, "PositionalDiffuseProduct"), 1, sphere_positional_diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "PositionalSpecularProduct"), 1, sphere_positional_specular_product);

	glUniform4fv(glGetUniformLocation(program, "globalAmbientProduct"), 1, global_sphere_ambient);
	glUniform4fv(glGetUniformLocation(program, "LightDirection"), 1, light_direction);
	glUniform1i(glGetUniformLocation(program, "pointSpotFlag"), sourceFlag);
	
	// The Light Position in Eye Frame
	vec4 light_position_eyeFrame = mv1 * light_position;
	glUniform4fv(glGetUniformLocation(program, "LightPosition"),1, light_position_eyeFrame);
	
	vec4 light_point_eyeFrame = mv1 * light_point;
	glUniform4fv(glGetUniformLocation(program, "LightPoint"), 1, light_point_eyeFrame);

	glUniform1f(glGetUniformLocation(program, "ConstAtt"), const_att);
	glUniform1f(glGetUniformLocation(program, "LinearAtt"), linear_att);
	glUniform1f(glGetUniformLocation(program, "QuadAtt"), quad_att);

	glUniform1f(glGetUniformLocation(program, "Shininess"), sphere_shininess);

	glUniform1f(glGetUniformLocation(program, "lightAngle"), light_angle);
	glUniform1f(glGetUniformLocation(program, "lightExponent"), light_exponent);
}

void SetUp_Lighting_Uniform_Vars_ground(mat4 mv1)
{
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),	1, ground_ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),	1, ground_diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, ground_specular_product);
	
	glUniform4fv(glGetUniformLocation(program, "PositionalAmbientProduct"), 1, ground_positional_ambient_product);
	glUniform4fv(glGetUniformLocation(program, "PositionalDiffuseProduct"), 1, ground_positional_diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "PositionalSpecularProduct"), 1, ground_positional_specular_product);
	
	glUniform4fv(glGetUniformLocation(program, "globalAmbientProduct"), 1, global_ground_ambient);
	glUniform4fv(glGetUniformLocation(program, "LightDirection"), 1, light_direction);
	glUniform1i(glGetUniformLocation(program, "pointSpotFlag"), sourceFlag);

	// The Light Position in Eye Frame
	vec4 light_position_eyeFrame = mv1 * light_position;
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position_eyeFrame);

	vec4 light_point_eyeFrame = mv1 * light_point;
	glUniform4fv(glGetUniformLocation(program, "LightPoint"), 1, light_point_eyeFrame);

	glUniform1f(glGetUniformLocation(program, "ConstAtt"), const_att);
	glUniform1f(glGetUniformLocation(program, "LinearAtt"), linear_att);
	glUniform1f(glGetUniformLocation(program, "QuadAtt"), quad_att);
	
	glUniform1f(glGetUniformLocation(program, "lightAngle"), light_angle);
	glUniform1f(glGetUniformLocation(program, "lightExponent"), light_exponent);
}
//----------------------------------------------------------------------------

/*************************************************************
void image_set_up(void):
generate checkerboard and stripe images.

* Inside init(), call this function and set up texture objects
for texture mapping.
(init() is called from main() before calling glutMainLoop().)
***************************************************************/
void image_set_up()
{
	int i, j, c;

	/* --- Generate checkerboard image to the image array ---*/
	for (i = 0; i < ImageHeight; i++)
		for (j = 0; j < ImageWidth; j++)
		{
			c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0));

			if (c == 1)		/* green */
			{
				Image[i][j][0] = (GLubyte)0;
				Image[i][j][1] = (GLubyte)150;
				Image[i][j][2] = (GLubyte)0;
			}
			else			/* white */
			{
				c = 255;
				Image[i][j][0] = (GLubyte)c;
				Image[i][j][1] = (GLubyte)c;
				Image[i][j][2] = (GLubyte)c;
			}
			Image[i][j][3] = (GLubyte)255;
		}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*--- Generate 1D stripe image to array stripeImage[] ---*/
	for (j = 0; j < stripeImageWidth; j++) {
		/* When j <= 4, the color is (255, 0, 0),   i.e., red stripe/line.
		When j > 4,  the color is (255, 255, 0), i.e., yellow remaining texture
		*/
		stripeImage[4 * j] = (GLubyte)255;
		stripeImage[4 * j + 1] = (GLubyte)((j>4) ? 255 : 0);
		stripeImage[4 * j + 2] = (GLubyte)0;
		stripeImage[4 * j + 3] = (GLubyte)255;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/*----------- End 1D stripe image ----------------*/

	/*--- texture mapping set-up is to be done in
	init() (set up texture objects),
	display() (activate the texture object to be used, etc.)
	and in shaders.
	---*/

}
//----------------------------------------------------------------------------

color4 fogColor = color4(0.7, 0.7, 0.7, 0.5);
float starting = 0.0;
float ending = 18.0;
float density = 0.09;

void SetUp_Fog_Uniform_Vars()
{
	glUniform1i(glGetUniformLocation(program, "fogFlag"), fogFlag);
	glUniform1f(glGetUniformLocation(program, "starting"), starting);
	glUniform1f(glGetUniformLocation(program, "ending"), ending);
	glUniform1f(glGetUniformLocation(program, "density"), density);
	glUniform4fv(glGetUniformLocation(program, "fogColor"), 1, fogColor);
}
//----------------------------------------------------------------------------

void setUp_latticeEffect()
{
	glUniform1i(glGetUniformLocation(program, "latticeFlag"), latticeFlag);
	glUniform1i(glGetUniformLocation(program, "uprightTiltedFlag"), uprightTiltedFlag);
}
//----------------------------------------------------------------------------
void setUp_fireworks()

{
	for (int i = 0; i < fireworks_num; i++)
	{
		fireworks_colors[i].x = (rand()%256) / 256.0;
		fireworks_colors[i].y = (rand() % 256) / 256.0;
		fireworks_colors[i].z = (rand() % 256) / 256.0;
		fireworks_colors[i].w = 1.0;

		fireworks_vertices[i].x = fireworks_initial.x;
		fireworks_vertices[i].y = fireworks_initial.y;
		fireworks_vertices[i].z = fireworks_initial.z;

		fireworks_velocity[i].x = 2.0 * ((rand() % 256) / 256.0 - 0.5);
		fireworks_velocity[i].y = 1.2 * 2.0 * ((rand() % 256) / 256.0);
		fireworks_velocity[i].z = 2.0 * ((rand() % 256) / 256.0 - 0.5);
	}
}

//----------------------------------------------------------------------------
int Index = 0; // YJC: This must be a global variable since quad() is called
			   //      multiple times and Index should then go up to the total number of
			   //      vertices and 1 color
void quad(int a, int b, int c)
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];

	vec3 normal = normalize(cross(u, v));

	sphere_colors[Index] = vertex_colors[1]; sphere_points[Index] = vertices[a]; sphere_smooth_normals[Index] = normalize(vertices[a]); sphere_flat_normals[Index] = normal; Index++;
	sphere_colors[Index] = vertex_colors[1]; sphere_points[Index] = vertices[b]; sphere_smooth_normals[Index] = normalize(vertices[b]); sphere_flat_normals[Index] = normal; Index++;
	sphere_colors[Index] = vertex_colors[1]; sphere_points[Index] = vertices[c]; sphere_smooth_normals[Index] = normalize(vertices[c]); sphere_flat_normals[Index] = normal; Index++;
}
//----------------------------------------------------------------------------

int Index1 = 0; // YJC: This must be a global variable since shad() is called
				//      multiple times and Index should then go up to the total number of
				//      vertices and 1 color
void shad(int d, int e, int f)
{
	shadow_colors[Index1] = vertex_colors[5]; shadow_points[Index1] = vertices[d]; Index1++;
	shadow_colors[Index1] = vertex_colors[5]; shadow_points[Index1] = vertices[e]; Index1++;
	shadow_colors[Index1] = vertex_colors[5]; shadow_points[Index1] = vertices[f]; Index1++;
}

// generate triangles of the sphere
void colorsphere()
{
	getData();
}
//-------------------------------
// generate 2 triangles: 6 vertices and 1 color
void floor()
{
	vec4 u = vertices[1] - vertices[0];
	vec4 v = vertices[2] - vertices[0];

	vec3 normal = normalize(cross(u, v));

	floor_colors[0] = vertex_colors[0]; floor_points[0] = vertices[0]; floor_flat_normals[0] = normal; floor_smooth_normals[0] = normal;
	floor_colors[1] = vertex_colors[0]; floor_points[1] = vertices[1]; floor_flat_normals[1] = normal; floor_smooth_normals[1] = normal;
	floor_colors[2] = vertex_colors[0]; floor_points[2] = vertices[3]; floor_flat_normals[2] = normal; floor_smooth_normals[2] = normal;

	floor_colors[3] = vertex_colors[0]; floor_points[3] = vertices[0]; floor_flat_normals[3] = normal; floor_smooth_normals[3] = normal;
	floor_colors[4] = vertex_colors[0]; floor_points[4] = vertices[2]; floor_flat_normals[4] = normal; floor_smooth_normals[4] = normal;
	floor_colors[5] = vertex_colors[0]; floor_points[5] = vertices[3]; floor_flat_normals[5] = normal; floor_smooth_normals[5] = normal;
}
//----------------------------------------------------------------------------
point3 axisVertices[4] = {
	point3(0.0,  0.0, 0.0),
	point3(6.0,  0.0,  0.0),
	point3(0.0,  5.0, 0.0),
	point3(0.0,  0.0,  10.0)
};
// generate 2 triangles: 6 vertices and 1 color
int indexAxes = 0;							// YJC: This must be a global variable since axes() is called
											//      multiple times and indexAxes should then go up to 6 for
											//      the 6 vertices and 1 color
int colorAxes = 2;
int axesID = 1;

void axes()
{
	axes_colors[indexAxes] = vertex_colors[colorAxes]; axes_points[indexAxes] = axisVertices[0]; indexAxes++;
	axes_colors[indexAxes] = vertex_colors[colorAxes]; axes_points[indexAxes] = axisVertices[axesID]; indexAxes++;
	colorAxes++;
	axesID++;
}
//----------------------------------------------------------------------------

// OpenGL initialization
void init()
{
	image_set_up();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*--- Create and Initialize a texture object ---*/
	glGenTextures(1, &texName1);      // Generate texture obj name(s)

	glActiveTexture(GL_TEXTURE1);  // Set the active texture unit to be 0 
	glBindTexture(GL_TEXTURE_1D, texName1); // Bind the texture to this texture unit

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, ImageWidth,
		0, GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);

	/*--- Create and Initialize a texture object ---*/
	glGenTextures(1, &texName2);      // Generate texture obj name(s)

	glActiveTexture(GL_TEXTURE2);  // Set the active texture unit to be 0 
	glBindTexture(GL_TEXTURE_2D, texName2); // Bind the texture to this texture unit

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageWidth, ImageHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, Image);

	colorsphere();

	// Create and initialize a vertex buffer object for sphere, to be used in display()
	glGenBuffers(1, &sphere_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer);

#if 1
	glBufferData(GL_ARRAY_BUFFER, sizeof(point3) * sphere_NumVertices + sizeof(color4) * sphere_NumVertices + sizeof(vec3) * sphere_NumVertices + sizeof(vec3) * sphere_NumVertices,
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3) * sphere_NumVertices, sphere_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3) * sphere_NumVertices, sizeof(color4) * sphere_NumVertices, sphere_colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3) * sphere_NumVertices + sizeof(color4) * sphere_NumVertices, sizeof(vec3) * sphere_NumVertices, sphere_flat_normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3) * sphere_NumVertices + sizeof(color4) * sphere_NumVertices + sizeof(vec3) * sphere_NumVertices, sizeof(vec3) * sphere_NumVertices, sphere_smooth_normals);
#endif

	// Create and initialize a vertex buffer object for shadow, to be used in display()
	glGenBuffers(1, &shadow_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, shadow_buffer);
#if 1
	glBufferData(GL_ARRAY_BUFFER, sizeof(point3) * shadow_NumVertices + sizeof(color4) * shadow_NumVertices,
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3) * shadow_NumVertices, shadow_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3) * shadow_NumVertices, sizeof(color4) * shadow_NumVertices,
		shadow_colors);
#endif
	
	/*--- Create and Initialize a texture object ---*/
	glGenTextures(1, &texName);      // Generate texture obj name(s)

	glActiveTexture(GL_TEXTURE0);  // Set the active texture unit to be 0 
	glBindTexture(GL_TEXTURE_2D, texName); // Bind the texture to this texture unit

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageWidth, ImageHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, Image);
	floor();
	axes();
	axes();
	axes();
	
	// Create and initialize a vertex buffer object for floor, to be used in display()
	glGenBuffers(1, &floor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors) + sizeof(floor_flat_normals) + sizeof(floor_smooth_normals) + sizeof(floor_texCoord),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_colors), floor_colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors), sizeof(floor_flat_normals), floor_flat_normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors) + sizeof(floor_flat_normals), sizeof(floor_smooth_normals), floor_smooth_normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors) + sizeof(floor_flat_normals) + sizeof(floor_smooth_normals), sizeof(floor_texCoord), floor_texCoord);

	// Create and initialize a vertex buffer object for axes, to be used in display()
	glGenBuffers(1, &axes_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, axes_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes_points) + sizeof(axes_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(axes_points), axes_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(axes_points), sizeof(axes_colors),
		axes_colors);

	// Create and initialize a vertex buffer object for fireworks, to be used in display()
	setUp_fireworks();
	glGenBuffers(1, &fireworks_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, fireworks_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point3) * fireworks_num + sizeof(color4) * fireworks_num + sizeof(point3) * fireworks_num,
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3) * fireworks_num, fireworks_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3) * fireworks_num, sizeof(color4) * fireworks_num,
		fireworks_colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3) * fireworks_num + sizeof(color4) * fireworks_num, sizeof(point3) * fireworks_num,
		fireworks_velocity);

	// Load shaders and create a shader program (to be used in display())
	program = InitShader("vshader53.glsl", "fshader53.glsl");
	program_fireworks = InitShader("vshader_fireworks.glsl", "fshader_fireworks.glsl");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.529, 0.807, 0.92, 0.0);
	glLineWidth(2.0);
}
//----------------------------------------------------------------------------
// drawObj(buffer, num_vertices)/drawObj1(buffer, num_vertices):
//   draw the object that is associated with the vertex buffer object "buffer"
//   and has "num_vertices" vertices.
//
void drawObj(GLuint buffer, int num_vertices)							// draw sphere
{
	GLuint vTexCoord;
	if(sphereFillFlag == 1)
		setUp_latticeEffect();
	else
		glUniform1i(glGetUniformLocation(program, "latticeFlag"), 0);
	if (lightingFlag == 0)
	{
		//--- Activate the vertex buffer object to be drawn ---//
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		/*----- Set up vertex attribute arrays for each vertex attribute -----*/
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

		GLuint vColor = glGetAttribLocation(program, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(point3) * num_vertices));
		// the offset is the (total) size of the previous vertex attribute array(s)

		if (textureGroundFlag == 1)
		{
			vTexCoord = glGetAttribLocation(program, "vtexCoord");
			glEnableVertexAttribArray(vTexCoord);
			glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(sizeof(floor_points) + sizeof(floor_colors) + sizeof(floor_flat_normals) + sizeof(floor_smooth_normals)));
		}

		/* Draw a sequence of geometric objs (triangles) from the vertex buffer
		(using the attributes specified in each enabled vertex attribute array) */
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);

		/*--- Disable each vertex attribute array being enabled ---*/
		glDisableVertexAttribArray(vPosition);
		glDisableVertexAttribArray(vColor);
		if (textureGroundFlag == 1)
		{
			glDisableVertexAttribArray(vTexCoord);
		}
	}
	if (lightingFlag == 1)
	{
		if (sphereFillFlag == 0)
		{
			//--- Activate the vertex buffer object to be drawn ---//
			glBindBuffer(GL_ARRAY_BUFFER, buffer);

			/*----- Set up vertex attribute arrays for each vertex attribute -----*/
			GLuint vPosition = glGetAttribLocation(program, "vPosition");
			glEnableVertexAttribArray(vPosition);
			glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(0));

			GLuint vColor = glGetAttribLocation(program, "vColor");
			glEnableVertexAttribArray(vColor);
			glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(sizeof(point3) * num_vertices));
			// the offset is the (total) size of the previous vertex attribute array(s)

			if (textureGroundFlag == 1)
			{
				vTexCoord = glGetAttribLocation(program, "vtexCoord");
				glEnableVertexAttribArray(vTexCoord);
				glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
					BUFFER_OFFSET(sizeof(floor_points) + sizeof(floor_colors) + sizeof(floor_flat_normals) + sizeof(floor_smooth_normals)));
			}

			/* Draw a sequence of geometric objs (triangles) from the vertex buffer
			(using the attributes specified in each enabled vertex attribute array) */
			glDrawArrays(GL_TRIANGLES, 0, num_vertices);

			/*--- Disable each vertex attribute array being enabled ---*/
			glDisableVertexAttribArray(vPosition);
			glDisableVertexAttribArray(vColor);
			if (textureGroundFlag == 1)
			{
				glDisableVertexAttribArray(vTexCoord);
			}
		}
		else
		{
			//--- Activate the vertex buffer object to be drawn ---//
			glBindBuffer(GL_ARRAY_BUFFER, buffer);

			/*----- Set up vertex attribute arrays for each vertex attribute -----*/
			GLuint vPosition = glGetAttribLocation(program, "vPosition");
			glEnableVertexAttribArray(vPosition);
			glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(0));


			GLuint vNormal = glGetAttribLocation(program, "vNormal");
			glEnableVertexAttribArray(vNormal);

			if (shadingFlag == 0)
			{
				glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
					BUFFER_OFFSET(sizeof(point3) * num_vertices + sizeof(color4) * num_vertices));
			}
			if (shadingFlag == 1)
			{
				glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
					BUFFER_OFFSET(sizeof(point3) * num_vertices + sizeof(color4) * num_vertices + sizeof(vec3) * num_vertices));
			}

			if (textureGroundFlag == 1)
			{
				vTexCoord = glGetAttribLocation(program, "vtexCoord");
				glEnableVertexAttribArray(vTexCoord);
				glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
					BUFFER_OFFSET(sizeof(floor_points) + sizeof(floor_colors) + sizeof(floor_flat_normals) + sizeof(floor_smooth_normals)));
			}

			/* Draw a sequence of geometric objs (triangles) from the vertex buffer
			(using the attributes specified in each enabled vertex attribute array) */
			glDrawArrays(GL_TRIANGLES, 0, num_vertices);

			/*--- Disable each vertex attribute array being enabled ---*/
			glDisableVertexAttribArray(vPosition);
			glDisableVertexAttribArray(vNormal);
			if (textureGroundFlag == 1)
			{
				glDisableVertexAttribArray(vTexCoord);
			}
		}
	}
}

void drawObj1(GLuint buffer, int num_vertices)							// draw axes
{
	glUniform1i(glGetUniformLocation(program, "latticeFlag"), 0);
	//--- Activate the vertex buffer object to be drawn ---//
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	/*----- Set up vertex attribute arrays for each vertex attribute -----*/
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point3) * num_vertices));
	// the offset is the (total) size of the previous vertex attribute array(s)

	/* Draw a sequence of geometric objs (triangles) from the vertex buffer
	(using the attributes specified in each enabled vertex attribute array) */
	glDrawArrays(GL_LINE_STRIP, 0, num_vertices);

	/*--- Disable each vertex attribute array being enabled ---*/
	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vColor);
}

void drawObj2(GLuint buffer, int num_vertices)							// draw shadow
{
	if (sphereFillFlag == 1)
		setUp_latticeEffect();
	else
		glUniform1i(glGetUniformLocation(program, "latticeFlag"), 0);
	//--- Activate the vertex buffer object to be drawn ---//
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	/*----- Set up vertex attribute arrays for each vertex attribute -----*/
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point3) * num_vertices));
	// the offset is the (total) size of the previous vertex attribute array(s)

	/* Draw a sequence of geometric objs (triangles) from the vertex buffer
	(using the attributes specified in each enabled vertex attribute array) */
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	/*--- Disable each vertex attribute array being enabled ---*/
	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vColor);
}

void drawObj3(GLuint buffer, int num_vertices)							// draw floor
{
	glUniform1i(glGetUniformLocation(program, "latticeFlag"), 0);
	GLuint vTexCoord;
	if (lightingFlag == 0)
	{
		//--- Activate the vertex buffer object to be drawn ---//
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		/*----- Set up vertex attribute arrays for each vertex attribute -----*/
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

		GLuint vColor = glGetAttribLocation(program, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(point3) * num_vertices));
		// the offset is the (total) size of the previous vertex attribute array(s)

		if (textureGroundFlag == 1)
		{
			vTexCoord = glGetAttribLocation(program, "vtexCoord");
			glEnableVertexAttribArray(vTexCoord);
			glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(sizeof(floor_points) + sizeof(floor_colors) + sizeof(floor_flat_normals) + sizeof(floor_smooth_normals)));
		}

		/* Draw a sequence of geometric objs (triangles) from the vertex buffer
		(using the attributes specified in each enabled vertex attribute array) */
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);

		/*--- Disable each vertex attribute array being enabled ---*/
		glDisableVertexAttribArray(vPosition);
		glDisableVertexAttribArray(vColor);
		if (textureGroundFlag == 1)
		{
			glDisableVertexAttribArray(vTexCoord);
		}
	}
	if (lightingFlag == 1)
	{

		//--- Activate the vertex buffer object to be drawn ---//
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		/*----- Set up vertex attribute arrays for each vertex attribute -----*/
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));


		GLuint vNormal = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormal);

		if (shadingFlag == 0)
		{
			glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(sizeof(point3) * num_vertices + sizeof(color4) * num_vertices));
		}
		if (shadingFlag == 1)
		{
			glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(sizeof(point3) * num_vertices + sizeof(color4) * num_vertices + sizeof(vec3) * num_vertices));
		}

		if (textureGroundFlag == 1)
		{
			vTexCoord = glGetAttribLocation(program, "vtexCoord");
			glEnableVertexAttribArray(vTexCoord);
			glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(sizeof(floor_points) + sizeof(floor_colors) + sizeof(floor_flat_normals) + sizeof(floor_smooth_normals)));
		}

		/* Draw a sequence of geometric objs (triangles) from the vertex buffer
		(using the attributes specified in each enabled vertex attribute array) */
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);

		/*--- Disable each vertex attribute array being enabled ---*/
		glDisableVertexAttribArray(vPosition);
		glDisableVertexAttribArray(vNormal);
		if (textureGroundFlag == 1)
		{
			glDisableVertexAttribArray(vTexCoord);
		}
	}
}
void drawObj4(GLuint buffer, int num_vertices)							// draw firework particles
{
	//--- Activate the vertex buffer object to be drawn ---//
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	/*----- Set up vertex attribute arrays for each vertex attribute -----*/
	GLuint vPosition = glGetAttribLocation(program_fireworks, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program_fireworks, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point3) * num_vertices));
	
	GLuint vVelocity = glGetAttribLocation(program_fireworks, "vVelocity");
	glEnableVertexAttribArray(vVelocity);
	glVertexAttribPointer(vVelocity, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point3) * num_vertices + sizeof(color4) * num_vertices));

	/* Draw a sequence of geometric objs (triangles) from the vertex buffer
	(using the attributes specified in each enabled vertex attribute array) */
	glPointSize(3.0);
	glDrawArrays(GL_POINTS, 0, num_vertices);

	/*--- Disable each vertex attribute array being enabled ---*/
	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vColor);
	glDisableVertexAttribArray(vVelocity);
}

//----------------------------------------------------------------------------
GLuint direction = 0;
mat4 last_rotate = mat4 (vec4 (1,0,0,0),
					vec4 (0,1,0,0),
					vec4 (0,0,1,0),
					vec4 (0,0,0,1));
mat4 shadow = mat4 (vec4 (12,14,0,0),
				vec4(0,0,0,0),
				vec4(0,3,12,0),
				vec4(0,-1,0,12));
point3 OY = point3(0.0, 1.0, 0.0);
point3 AB1 = point3(-4.0, 1.0, 4.0);
point3 AB2 = point3(-1.0, 1.0, -4.0);
point3 AB3 = point3(3.0, 1.0, 5.0);
//----------------------------------------------------------------------------
void display(void)
{
	GLuint  model_view;						// model-view matrix uniform shader variable location for Version 53
	GLuint  projection;						// projection matrix uniform shader variable location for Version 53

	mat4  p = Perspective(fovy, aspect, zNear, zFar);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);					// Use the shader program for Version 42
	model_view = glGetUniformLocation(program, "model_view");
	projection = glGetUniformLocation(program, "projection");

	/*---  Set up and pass on Projection matrix to the shader ---*/
	glUniformMatrix4fv(projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major
	
												   /*---  Set up and pass on Model-View matrix to the shader ---*/
												   // eye is a global variable of vec4 set to init_eye and updated by keyboard()
	vec4    at(0.0, 0.0, 0.0, 1.0);
	vec4    up(0.0, 1.0, 0.0, 0.0);

	mat4  mv  = LookAt(eye, at, up);
	mat4 mv1 = LookAt(eye, at, up);

	SetUp_Fog_Uniform_Vars();

#if 1 // The following is to verify that Rotate() about (0,2,0) is RotateY():
	// Commenting out Rotate() and un-commenting RotateY()
	// gives the same result.
		if (direction == 0) {
			float distance = angle * pi / 180;
			point3 rotate_amount = AB2 - AB1;
			point3 translation(AB1[0] + distance * rotate_amount[0] / sqrt(73), AB1[1] + distance * rotate_amount[1] / sqrt(73),
				AB1[2] + distance * rotate_amount[2] / sqrt(73));
			point3 rotate_axis=cross(OY, rotate_amount);
			mv = mv * Translate(translation) * Scale(1.0, 1.0, 1.0)
				* Rotate(angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]) * last_rotate;
		}
		else if (direction == 1) {
			float distance = angle * pi / 180;
			point3 rotate_amount = AB3 - AB2;
			point3 translation(AB2[0] + distance * rotate_amount[0] / sqrt(97), AB2[1] + distance * rotate_amount[1] / sqrt(97),
				AB2[2] + distance * rotate_amount[2] / sqrt(97));
			point3 rotate_axis=cross(OY, rotate_amount);
			mv = mv * Translate(translation) * Scale(1.0, 1.0, 1.0)
				* Rotate(angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]) * last_rotate;
		}
		else if (direction == 2) {
			float distance = angle * pi / 180;
			point3 rotate_amount = AB1 - AB3;
			point3 translation(AB3[0] + distance * rotate_amount[0] / sqrt(50), AB3[1] + distance * rotate_amount[1] / sqrt(50),
				AB3[2] + distance * rotate_amount[2] / sqrt(50));
			point3 rotate_axis=cross(OY, rotate_amount);
			mv = mv * Translate(translation) * Scale(1.0, 1.0, 1.0)
				* Rotate(angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]) * last_rotate;
		}

#endif
	
	if (sphereFillFlag == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (textureSphereFlag == 1)
		{
			glUniform1i(glGetUniformLocation(program, "verticalSlantedFlag"), verticalSlantedFlag);
			glUniform1i(glGetUniformLocation(program, "objectEyeFlag"), objectEyeFlag);
			glUniform1i(glGetUniformLocation(program, "sphere_1D"), 1);
			glUniform1i(glGetUniformLocation(program, "textureSphereFlag"), textureSphereFlag);
			glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 1);
		}
		if (textureSphereFlag == 2)
		{
			glUniform1i(glGetUniformLocation(program, "verticalSlantedFlag"), verticalSlantedFlag);
			glUniform1i(glGetUniformLocation(program, "objectEyeFlag"), objectEyeFlag);
			glUniform1i(glGetUniformLocation(program, "sphere_2D"), 2);
			glUniform1i(glGetUniformLocation(program, "textureSphereFlag"), textureSphereFlag);
			glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 2);
		}
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (lightingFlag == 0)
	{
		glUniform1i(glGetUniformLocation(program, "colorFlag"), 0);
		glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);					// GL_TRUE: matrix is row-major
		glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 0);

		drawObj(sphere_buffer, sphere_NumVertices);						// draw the sphere
	}
		
	if (lightingFlag == 1)
	{
		if (sphereFillFlag == 1)
		{
			glUniform1i(glGetUniformLocation(program, "colorFlag"), 1);
			SetUp_Lighting_Uniform_Vars_sphere(mv1);
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);					// GL_TRUE: matrix is row-major
			mat3 normal_matrix = NormalMatrix(mv, 1);

			glUniformMatrix3fv(glGetUniformLocation(program, "Normal_Matrix"), 1, GL_TRUE, normal_matrix);


			glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
			glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 0);
			drawObj(sphere_buffer, sphere_NumVertices);						// draw the sphere
		}
		else
		{
			glUniform1i(glGetUniformLocation(program, "colorFlag"), 0);
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);					// GL_TRUE: matrix is row-major
			glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 0);
			drawObj(sphere_buffer, sphere_NumVertices);						// draw the sphere
		}
		
	}
	mv = LookAt(eye, at, up);
#if 1 // The following is to verify that Rotate() about (0,2,0) is RotateY():
																	// Commenting out Rotate() and un-commenting RotateY()
																	// gives the same result.
	if (direction == 0) {
		float distance = angle * pi / 180;
		point3 rotate_amount = AB2 - AB1;
		point3 translation(AB1[0] + distance * rotate_amount[0] / sqrt(73), AB1[1] + distance * rotate_amount[1] / sqrt(73),
			AB1[2] + distance * rotate_amount[2] / sqrt(73));
		point3 rotate_axis = cross(OY, rotate_amount);
		mv = mv * shadow * Translate(translation) * Scale(1.0, 1.0, 1.0)
			* Rotate(angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]) * last_rotate;
	}
	else if (direction == 1) {
		float distance = angle * pi / 180;
		point3 rotate_amount = AB3 - AB2;
		point3 translation(AB2[0] + distance * rotate_amount[0] / sqrt(97), AB2[1] + distance * rotate_amount[1] / sqrt(97),
			AB2[2] + distance * rotate_amount[2] / sqrt(97));
		point3 rotate_axis = cross(OY, rotate_amount);
		mv = mv * shadow * Translate(translation) * Scale(1.0, 1.0, 1.0)
			* Rotate(angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]) * last_rotate;
	}
	else if (direction == 2) {
		float distance = angle * pi / 180;
		point3 rotate_amount = AB1 - AB3;
		point3 translation(AB3[0] + distance * rotate_amount[0] / sqrt(50), AB3[1] + distance * rotate_amount[1] / sqrt(50),
			AB3[2] + distance * rotate_amount[2] / sqrt(50));
		point3 rotate_axis = cross(OY, rotate_amount);
		mv = mv * shadow * Translate(translation) * Scale(1.0, 1.0, 1.0)
			* Rotate(angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]) * last_rotate;
	}
	
#endif
	if (shadowFlag == 1)
	{
		if (blendFlag == 0)
		{
			if (lightingFlag == 0)
			{
				glUniform1i(glGetUniformLocation(program, "colorFlag"), 0);
				glDepthMask(GL_FALSE);												// disable z-buffer writing
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);					// GL_TRUE: matrix is row-major
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				if (textureGroundFlag == 1)
				{
					glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 1);
				}
				glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
				drawObj3(floor_buffer, floor_NumVertices);							// draw the floor

				glDepthMask(GL_TRUE);												// enable z-buffer writing
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);						// GL_TRUE: matrix is row-major
				if (sphereFillFlag == 1)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				if (eye[1] > 0.1)
				{
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 0);
					glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
					drawObj2(shadow_buffer, shadow_NumVertices);					// draw shadow
				}
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);				// disable color buffer
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);					// GL_TRUE: matrix is row-major
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				if (textureGroundFlag == 1)
				{
					glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 1);
				}
				glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
				drawObj3(floor_buffer, floor_NumVertices);							// draw the floor
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);					// enable color buffer
			}
			if (lightingFlag == 1)
			{
				glUniform1i(glGetUniformLocation(program, "colorFlag"), 1);
				SetUp_Lighting_Uniform_Vars_ground(mv1);
				mat3 normal_matrix = NormalMatrix(mv1, 1);
				glUniformMatrix3fv(glGetUniformLocation(program, "Normal_Matrix"), 1, GL_TRUE, normal_matrix);
				glDepthMask(GL_FALSE);												// disable z-buffer writing
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);					// GL_TRUE: matrix is row-major
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				if (textureGroundFlag == 1)
				{
					glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 1);
				}
				glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
				drawObj3(floor_buffer, floor_NumVertices);							// draw the floor

				glUniform1i(glGetUniformLocation(program, "colorFlag"), 0);
				glDepthMask(GL_TRUE);												// enable z-buffer writing
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);						// GL_TRUE: matrix is row-major
				if (sphereFillFlag == 1)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				if (eye[1] > 0.1)
				{
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 0);
					drawObj2(shadow_buffer, shadow_NumVertices);					// draw shadow
				}
				glUniform1i(glGetUniformLocation(program, "colorFlag"), 1);
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);				// disable color buffer
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);					// GL_TRUE: matrix is row-major
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				if (textureGroundFlag == 1)
				{
					glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 1);
				}
				glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
				drawObj3(floor_buffer, floor_NumVertices);							// draw the floor
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);					// enable color buffer
			}
		}
		if (blendFlag == 1)
		{
			if (lightingFlag == 0)
			{
				glUniform1i(glGetUniformLocation(program, "colorFlag"), 0);
				glDepthMask(GL_FALSE);												// disable z-buffer writing
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);					// GL_TRUE: matrix is row-major
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				if (textureGroundFlag == 1)
				{
					glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 1);
				}
				glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
				drawObj3(floor_buffer, floor_NumVertices);							// draw the floor
				
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);						// GL_TRUE: matrix is row-major
				if (sphereFillFlag == 1)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				if (eye[1] > 0.0)
				{
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 0);
					glEnable(GL_BLEND);												// enable blending
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);				// blending over function
					drawObj2(shadow_buffer, shadow_NumVertices);					// draw shadow
					glDisable(GL_BLEND);											// disable blending
				}
				glDepthMask(GL_TRUE);												// enable z-buffer writing
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);				// disable color buffer
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);					// GL_TRUE: matrix is row-major
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				if (textureGroundFlag == 1)
				{
					glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 1);
				}
				glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
				drawObj3(floor_buffer, floor_NumVertices);							// draw the floor
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);					// enable color buffer
			}
			if (lightingFlag == 1)
			{
				glUniform1i(glGetUniformLocation(program, "colorFlag"), 1);
				SetUp_Lighting_Uniform_Vars_ground(mv1);
				mat3 normal_matrix = NormalMatrix(mv1, 1);
				glUniformMatrix3fv(glGetUniformLocation(program, "Normal_Matrix"), 1, GL_TRUE, normal_matrix);
				glDepthMask(GL_FALSE);												// disable z-buffer writing
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);					// GL_TRUE: matrix is row-major
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				if (textureGroundFlag == 1)
				{
					glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 1);
				}
				glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
				drawObj3(floor_buffer, floor_NumVertices);							// draw the floor

				glUniform1i(glGetUniformLocation(program, "colorFlag"), 0);
				
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);						// GL_TRUE: matrix is row-major
				if (sphereFillFlag == 1)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				if (eye[1] > 0.0)
				{
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 0);
					glEnable(GL_BLEND);												// enable blending
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);				// blending over function
					drawObj2(shadow_buffer, shadow_NumVertices);					// draw shadow
					glDisable(GL_BLEND);											// disable blending
				}
				glDepthMask(GL_TRUE);												// enable z-buffer writing
				glUniform1i(glGetUniformLocation(program, "colorFlag"), 1);
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);				// disable color buffer
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);					// GL_TRUE: matrix is row-major
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				if (textureGroundFlag == 1)
				{
					glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
					glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 1);
				}
				glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
				drawObj3(floor_buffer, floor_NumVertices);							// draw the floor
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);					// enable color buffer
			}
		}
	}
	if (shadowFlag == 0)
	{
		if (lightingFlag == 0)
		{
			glUniform1i(glGetUniformLocation(program, "colorFlag"), 0);
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);					// GL_TRUE: matrix is row-major
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (textureGroundFlag == 1)
			{
				glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
				glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 1);
			}
			glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
			drawObj3(floor_buffer, floor_NumVertices);							// draw the floor
		}
		if (lightingFlag == 1)
		{
			glUniform1i(glGetUniformLocation(program, "colorFlag"), 1);
			SetUp_Lighting_Uniform_Vars_ground(mv1);
			mat3 normal_matrix = NormalMatrix(mv1, 1);
			glUniformMatrix3fv(glGetUniformLocation(program, "Normal_Matrix"), 1, GL_TRUE, normal_matrix);
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);					// GL_TRUE: matrix is row-major
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (textureGroundFlag == 1)
			{
				glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
				glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 1);
			}
			glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
			drawObj3(floor_buffer, floor_NumVertices);							// draw the floor
		}
	}
	
	mv = LookAt(eye, at, up);
	glUniform1i(glGetUniformLocation(program, "colorFlag"), 0);
	glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);								// GL_TRUE: matrix is row-major
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniform1i(glGetUniformLocation(program, "Texture_ground_flag"), 0);
	glUniform1i(glGetUniformLocation(program, "sphereGroundFlag"), 0);
	drawObj1(axes_buffer, axes_NumVertices);									// draw the axes

	if (fireworksFlag == 1)
	{
		glUseProgram(program_fireworks);
		model_view = glGetUniformLocation(program_fireworks, "model_view");
		projection = glGetUniformLocation(program_fireworks, "projection");
		glUniformMatrix4fv(projection, 1, GL_TRUE, p);							// GL_TRUE: matrix is row-major
		glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);						// GL_TRUE: matrix is row-major
		glUniform1f(glGetUniformLocation(program_fireworks, "time"), t);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		drawObj4(fireworks_buffer, fireworks_num);
	}

	glutSwapBuffers();
}
//---------------------------------------------------------------------------
void idle(void)
{
	if (fireworksFlag == 1)
	{
		t = (float) fmod(((glutGet(GLUT_ELAPSED_TIME)) - tsub), (tmax));
	}
	angle += 1.5;																	//YJC: change this value to adjust the cube rotation speed.
	if (direction == 0 && angle >= ((sqrt(73) * 180) / pi)) {
		direction = (direction + 1) % 3;
		point3 rotate_amount = AB2 - AB1;
		point3 rotate_axis = cross(OY, rotate_amount);
		last_rotate = Rotate(angle, rotate_axis[0], rotate_axis[1], rotate_axis[2])*last_rotate;
		angle = 0;
	}
	else if (direction == 1 && angle >= ((sqrt(97) * 180) / pi)) {
		direction = (direction + 1) % 3;
		point3 rotate_amount = AB3 - AB2;
		point3 rotate_axis = cross(OY, rotate_amount);
		last_rotate = Rotate(angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]) * last_rotate;
		angle = 0;
	}
	else if (direction == 2 && angle >= ((sqrt(50) * 180) / pi)) {
		direction = (direction + 1) % 3;
		point3 rotate_amount = AB1 - AB3;
		point3 rotate_axis = cross(OY, rotate_amount);
		last_rotate = Rotate(angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]) * last_rotate;
		angle = 0;
	}
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)						// Keyboard Functions
{
	switch (key) {
	case 033: // Escape Key
	case 'X': eye[0] += 1.0; break;
	case 'x': eye[0] -= 1.0; break;
	case 'Y': eye[1] += 1.0; break;
	case 'y': eye[1] -= 1.0; break;
	case 'Z': eye[2] += 1.0; break;
	case 'z': eye[2] -= 1.0; break;

	case 'b': case 'B':												// Toggle between animation and non-animation
		glutIdleFunc(idle);
		animationFlag = 1;
		break;

	case 'v': case 'V':												// Toggle to vertical sphere texture
		verticalSlantedFlag = 1;
		break;

	case 's': case 'S':												// Toggle to slanted sphere texture
		verticalSlantedFlag = 2;
		break;

	case 'o': case 'O':												// Toggle to object space
		objectEyeFlag = 1;
		break;

	case 'e': case 'E':												// Toggle to eye space
		objectEyeFlag = 2;
		break;

	case 'l': case 'L':												// Toggle lattice effect
		latticeFlag = 1 - latticeFlag;
		break;

	case 'u': case 'U':												// Toggle to upright lattice effect
		uprightTiltedFlag = 1;
		break;

	case 't': case 'T':												// Toggle to tilted lattice effect
		uprightTiltedFlag = 2;
		break;
	}
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
void demo_menu(int id)												// Mouse Menu Functions
{
	switch (id)
	{
	case 1:
		eye = init_eye;
		break;
	case 2:
		sphereFillFlag = 0;
		break;
	case 24:
		exit(0);
		break;
	}
	glutPostRedisplay();
}
void shadow_menu(int id)												
{
	switch (id)
	{
	case 3:
		shadowFlag = 0;
		break;
	case 4:
		shadowFlag = 1;
		break;
	}
	glutPostRedisplay();
}
void lighting_menu(int id)
{
	switch (id)
	{
	case 5:
		lightingFlag = 0;
		break;
	case 6:
		lightingFlag = 1;
		break;
	}
	glutPostRedisplay();
}
void source_menu(int id)
{
	switch (id)
	{
	case 7:
		sourceFlag = 0;
		break;
	case 8:
		sourceFlag = 1;
		break;
	}
	glutPostRedisplay();
}
void shading_menu(int id)
{
	switch (id)
	{
	case 9:
		shadingFlag = 0;
		sphereFillFlag = 1;
		break;
	case 10:
		shadingFlag = 1;
		sphereFillFlag = 1;
		break;
	}
	glutPostRedisplay();
}
void fog_menu(int id)
{
	switch (id)
	{
	case 11:
		fogFlag = 0;
		break;
	case 12:
		fogFlag = 1;
		break;
	case 13:
		fogFlag = 2;
		break;
	case 14:
		fogFlag = 3;
		break;
	}
	glutPostRedisplay();
}
void blending_menu(int id)
{
	switch (id)
	{
	case 15:
		blendFlag = 0;
		break;
	case 16:
		blendFlag = 1;
		break;
	}
	glutPostRedisplay();
}
void textureGround_menu(int id)
{
	switch (id)
	{
	case 17:
		textureGroundFlag = 0;
		break;
	case 18:
		textureGroundFlag = 1;
		break;
	}
	glutPostRedisplay();
}
void textureSphere_menu(int id)
{
	switch (id)
	{
	case 19:
		textureSphereFlag = 0;
		break;
	case 20:
		textureSphereFlag = 1;
		break;
	case 21:
		textureSphereFlag = 2;
		break;
	}
	glutPostRedisplay();
}
void fireworks_menu(int id)
{
	switch (id)
	{
	case 22:
		fireworksFlag = 0;
		break;
	case 23:
		fireworksFlag = 1;
		tsub = (float) glutGet(GLUT_ELAPSED_TIME);
		break;
	}
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		rightClickFlag = 1 - rightClickFlag;
		if (rightClickFlag == 1 && animationFlag == 1)
			glutIdleFunc(idle);
		else if (rightClickFlag == 0 && animationFlag == 1)
			glutIdleFunc(NULL);
	}
}
//----------------------------------------------------------------------------
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	aspect = (GLfloat)width / (GLfloat)height;
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int err;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Color Cube");

	/* Call glewInit() and error checking */
	err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Error: glewInit failed: %s\n", (char*)glewGetErrorString(err));
		exit(1);
	}
	system("cls");
	printf("--------------\n");
	printf("Rolling Sphere\n");
	printf("--------------\n");
	printf("Enter Input File Name (with extension): ");
	scanf("%s", filename);
	printf("\n\n\n");
	file_in();
	sphere_NumVertices = num * 3;
	shadow_NumVertices = num * 3;
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(NULL);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	int subMenuShadow = glutCreateMenu(shadow_menu);
	glutAddMenuEntry("No", 3);
	glutAddMenuEntry("Yes", 4);

	int subMenuLighting = glutCreateMenu(lighting_menu);
	glutAddMenuEntry("No", 5);
	glutAddMenuEntry("Yes", 6);

	int subMenuSource = glutCreateMenu(source_menu);
	glutAddMenuEntry("Spot Light", 7);
	glutAddMenuEntry("Point Source", 8);

	int subMenuShading = glutCreateMenu(shading_menu);
	glutAddMenuEntry("Flat Shading", 9);
	glutAddMenuEntry("Smooth Shading", 10);

	int subMenuFog = glutCreateMenu(fog_menu);
	glutAddMenuEntry("No Fog", 11);
	glutAddMenuEntry("Linear Fog", 12);
	glutAddMenuEntry("Exponential Fog", 13);
	glutAddMenuEntry("Exponential Square Fog", 14);

	int subMenuBlend = glutCreateMenu(blending_menu);
	glutAddMenuEntry("No", 15);
	glutAddMenuEntry("Yes", 16);

	int subMenuTextureGround = glutCreateMenu(textureGround_menu);
	glutAddMenuEntry("No", 17);
	glutAddMenuEntry("Yes", 18);

	int subMenuTextureSphere = glutCreateMenu(textureSphere_menu);
	glutAddMenuEntry("No", 19);
	glutAddMenuEntry("Yes - Contour Lines", 20);
	glutAddMenuEntry("Yes - Checkerboard", 21);

	int subMenuFireworks = glutCreateMenu(fireworks_menu);
	glutAddMenuEntry("No", 22);
	glutAddMenuEntry("Yes", 23);

	glutCreateMenu(demo_menu);
	glutAddMenuEntry("Default View Point", 1);
	glutAddMenuEntry("Wire Frame Sphere", 2);
	glutAddSubMenu("Shadow", subMenuShadow);
	glutAddSubMenu("Enable Lighting", subMenuLighting);
	glutAddSubMenu("Light Source", subMenuSource);
	glutAddSubMenu("Shading", subMenuShading);
	glutAddSubMenu("Fog Options", subMenuFog);
	glutAddSubMenu("Blending Shadow", subMenuBlend);
	glutAddSubMenu("Texture Mapped Ground", subMenuTextureGround);
	glutAddSubMenu("Texture Mapped Sphere", subMenuTextureSphere);
	glutAddSubMenu("Firework", subMenuFireworks);
	glutAddMenuEntry("Quit", 24);

	glutAttachMenu(GLUT_LEFT_BUTTON);

	init();
	glutMainLoop();
	return 0;
}
