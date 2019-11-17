#version 150

//uniform sampler2D texture1;
//uniform sampler2D texture2;
//uniform float fade;
//uniform mat3 WorldIT;
//uniform vec3 ViewDir;

// Light
//uniform vec4 LightAmbient;
//uniform vec4 LightSpecular;
//uniform vec4 LightDiffuse;
//uniform vec3 LightDir;

in vec2 v_uv1;
//in vec3 v_normal;
out vec4 PixelColor;

void main()
{
	//vec3 n = normalize( v_normal );

	// Ambient
	//vec4 ambient = LightAmbient;

	// Diffuse
	//vec4 diffuse = (LightDiffuse * max(0.0,dot(n, -LightDir)));

	// Specular
	//float specFactor = pow(max(dot(reflect(LightDir,n), ViewDir), 0.0), 1.0);
	//vec4 specular = (specFactor * texture(texture2, v_uv1) * LightSpecular);
    
	//vec4 light = clamp(ambient + diffuse + specular, 0, 1);

    //PixelColor = texture(texture1, v_uv1) * vec4( light.rgb, 1.0);
	PixelColor = vec4(v_uv1[0], v_uv1[1], 1.0, 1.0);
}
