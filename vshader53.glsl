/*
File Name: "vshader53.glsl";
Vertex shader:
  - Per vertex shading for light source.
  - Entire shading computation is done in the Eye Frame.
*/

// #version 150  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

in  vec3 vPosition;
in  vec3 vNormal;
in  vec2 vtexCoord;
in  vec4 vColor;
out vec4 color;
out vec4 point;
out vec2 texCoord;
out float sphereTexture;
out vec2 sphereTexture1;
out vec2 sphereLattice;

uniform int colorFlag;
uniform vec4 globalAmbientProduct, AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 PositionalAmbientProduct, PositionalDiffuseProduct, PositionalSpecularProduct;
uniform mat4 model_view;
uniform mat4 projection;
uniform mat3 Normal_Matrix;
uniform vec4 LightPosition;
uniform vec4 LightPoint;
uniform vec4 LightDirection;
uniform float Shininess;
uniform int pointSpotFlag;
uniform int verticalSlantedFlag;
uniform int objectEyeFlag;
uniform int textureSphereFlag;
uniform int uprightTiltedFlag;

uniform float ConstAtt;		// Constant Attenuation
uniform float LinearAtt;	// Linear Attenuation
uniform float QuadAtt;		// Quadratic Attenuation

uniform float lightAngle;
uniform float lightExponent;

vec4 globalSource()
{
	vec4 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
	
	//Transform vertex position into eye coordinates
	vec3 pos = (model_view * vPosition4).xyz;

	vec3 L = normalize( -LightDirection );
	vec3 E = normalize( -pos );
	vec3 H = normalize( L + E );
	
	//Transform vertex normal into eye coordinates
	vec3 N = normalize(Normal_Matrix * vNormal);

 	float attenuation = 1.0;

	// Compute terms in the illumination equation
	vec4 ambient = AmbientProduct;

	float d = max( dot(L, N), 0.0 );
	vec4  diffuse = d * DiffuseProduct;

	float s = pow( max(dot(N, H), 0.0), Shininess );
	vec4  specular = s * SpecularProduct;

	if( dot(L, N) < 0.0 ) {
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}
	return attenuation * (ambient + diffuse + specular);
}

vec4 pointSource()
{
	vec4 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
	
	//Transform vertex position into eye coordinates
	vec3 pos = (model_view * vPosition4).xyz;

	vec3 L = normalize( LightPosition.xyz - pos );
	vec3 E = normalize( -pos );
	vec3 H = normalize( L + E );
	
	//Transform vertex normal into eye coordinates
	vec3 N = normalize(Normal_Matrix * vNormal);
	
	float dist = sqrt(pow((pos.x - LightPosition.x), 2) + pow((pos.y - LightPosition.y), 2) + pow((pos.z - LightPosition.z), 2));
 	float attenuation = (float) 1.0 / (ConstAtt + (LinearAtt * dist) + (QuadAtt * dist * dist));

	// Compute terms in the illumination equation
	vec4 ambient = PositionalAmbientProduct;

	float d = max( dot(L, N), 0.0 );
	vec4  diffuse = d * PositionalDiffuseProduct;

	float s = pow( max(dot(N, H), 0.0), Shininess );
	vec4  specular = s * PositionalSpecularProduct;

	if( dot(L, N) < 0.0 ) {
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}
	return attenuation * (ambient + diffuse + specular);
}

vec4 spotSource()
{
	vec4 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
	
	float cutOffAngle = cos(lightAngle);
	
	//Transform vertex position into eye coordinates
	vec3 pos = (model_view * vPosition4).xyz;

	vec3 Lf = normalize(LightPoint.xyz - LightPosition.xyz);
	float spotlight_attenuation = 0;
	vec3 l = normalize (pos.xyz - LightPosition.xyz);
	
	if (dot(Lf, l) < cutOffAngle)
	{
		spotlight_attenuation = 0;
	}
	else
	{
		spotlight_attenuation = pow(dot(Lf, l), lightExponent);
	}
	
	vec3 L = normalize( LightPosition.xyz - pos );
	vec3 E = normalize( -pos );
	vec3 H = normalize( L + E );
	
	// Transform vertex normal into eye coordinates
	vec3 N = normalize(Normal_Matrix * vNormal);
	
	float dist = sqrt(pow((pos.x - LightPosition.x), 2) + pow((pos.y - LightPosition.y), 2) + pow((pos.z - LightPosition.z), 2));
 	float attenuation = (float) spotlight_attenuation / (ConstAtt + (LinearAtt * dist) + (QuadAtt * dist * dist));

	// Compute terms in the illumination equation
	vec4 ambient = PositionalAmbientProduct;

	float d = max( dot(L, N), 0.0 );
	vec4  diffuse = d * PositionalDiffuseProduct;

	float s = pow( max(dot(N, H), 0.0), Shininess );
	vec4  specular = s * PositionalSpecularProduct;

	if( dot(L, N) < 0.0 ) {
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}
	return attenuation * (ambient + diffuse + specular);
}

void main()
{
	vec4 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
	vec3 pos = (model_view * vPosition4).xyz;
	gl_Position = projection * model_view * vPosition4;
	vtexCoord.x = vtexCoord.x * 6.0;
	vtexCoord.y = vtexCoord.y * 5.0;
	texCoord = vtexCoord;
	point = projection * model_view * vPosition4;
	if (colorFlag == 0)
	{
		color = vColor;
	}
	if (colorFlag == 1)
	{
		if (pointSpotFlag == 0)
		{
			color = globalAmbientProduct + globalSource() + spotSource();
		}
		if (pointSpotFlag == 1)
		{
			color = globalAmbientProduct + globalSource() + pointSource();
		}
	}
	if(textureSphereFlag == 1)
	{
		if (verticalSlantedFlag == 1)
		{
			if (objectEyeFlag == 1)
				sphereTexture = 2.5 * vPosition4.x;
			if (objectEyeFlag == 2)
				sphereTexture = 2.5 * pos.x;
		}
		if (verticalSlantedFlag == 2)
		{
			if (objectEyeFlag == 1)
				sphereTexture = 1.5 * (vPosition4.x + vPosition4.y + vPosition4.z);
			if (objectEyeFlag == 2)
				sphereTexture = 1.5 * (pos.x + pos.y + pos.z);
		}
	}
	if(textureSphereFlag == 2)
	{
		if (verticalSlantedFlag == 1)
		{
			if (objectEyeFlag == 1)
			{
				sphereTexture1.x = 0.75 * (vPosition4.x + 1);
				sphereTexture1.y = 0.75 * (vPosition4.y + 1);
			}
			if (objectEyeFlag == 2)
			{
				sphereTexture1.x = 0.75 * (pos.x + 1);
				sphereTexture1.y = 0.75 * (pos.y + 1);
			}
		}
		if (verticalSlantedFlag == 2)
		{
			if (objectEyeFlag == 1)
			{
				sphereTexture1.x = 0.45 * (vPosition4.x + vPosition4.y + vPosition4.z);
				sphereTexture1.y = 0.45 * (vPosition4.x - vPosition4.y + vPosition4.z);
			}
			if (objectEyeFlag == 2)
			{
				sphereTexture1.x = 0.45 * (pos.x + pos.y + pos.z);
				sphereTexture1.y = 0.45 * (pos.x - pos.y + pos.z);
			}
		}
	}	
	if(uprightTiltedFlag == 1)
		{
		sphereLattice.x = 0.5 * (vPosition4.x +1);
		sphereLattice.y = 0.5 * (vPosition4.y +1);
		}
	if(uprightTiltedFlag == 2)
		{
		sphereLattice.x = 0.3 * (vPosition4.x + vPosition4.y + vPosition4.z);
		sphereLattice.y = 0.3 * (vPosition4.x - vPosition4.y + vPosition4.z);
		}
}
