#version 150

// Scene info
uniform mat4 WorldViewProj;
uniform mat4 World;
uniform mat3 WorldIT;
uniform vec3 ViewDir;

// Light Info
uniform vec4 LightAmbient;
uniform vec4 LightSpecular;
uniform vec4 LightDiffuse;
uniform vec3 LightPos;
uniform vec3 LightAttenuation;
uniform float LightRange;

// Material info
uniform vec4 MatAmbient;
uniform vec4 MatSpecular;
uniform vec4 MatDiffuse;

// Texture(s)
uniform sampler2D texture;

// Varyings
in vec3 v_normal;
in vec4 v_pos;
in vec2 v_uv1;

out vec4 PixelColor;

void main()
{
	vec3 n = normalize( v_normal );

	vec3 lightDir = LightPos - v_pos.xyz;
	float distance = length(lightDir);

	if( distance > LightRange )
	{
		PixelColor = MatAmbient * LightAmbient;
	}
	else
	{
		// normalize it
		lightDir /= distance;

		// Ambient
		vec4 ambient = MatAmbient * LightAmbient;

		// Diffuse
		vec4 diffuse = MatDiffuse * LightDiffuse * max(0.0,dot(n, lightDir));

		// Specular
		vec3 R = reflect(-lightDir,n);
		float specFactor = pow(max(dot(R, ViewDir), 0.0), MatSpecular.w);
		vec4 specular = specFactor * MatSpecular * LightSpecular;

		// Combine the components
		vec4 light = clamp(ambient + diffuse + specular, 0, 1);

		// Attenuate the light
		float att = 1.0 / dot(LightAttenuation, vec3(1.0, distance, distance*distance));
		light *= att;

		PixelColor = texture(texture, v_uv1) * vec4( light.rgb, 1.0);
	}
}


