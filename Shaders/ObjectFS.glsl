#version 330

uniform sampler2D textura;
uniform int do_tex;

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;
in vec3 object_color;

in vec2 tex_coord;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform int spot;

uniform float cut_off;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 world_norm = normalize(world_normal);

	vec3 L = normalize( light_position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );

	// TODO: define ambient light component
	float ambient_light = 0.5;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * max ( dot(world_norm,L), 0);

	float primesteLumina;
	if( dot(world_normal,L) > 0 )
		primesteLumina = 1;
	else 
		primesteLumina = 0;

	// TODO: compute specular light component
	float specular_light = 0;
	

	float d = distance(world_position,light_position);
	float atenuation = 1/(d*d);

	float spot_light = dot(-L, light_direction);
	float spot_light_limit = cos(cut_off);

	
	float light=ambient_light;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * primesteLumina * pow(max(dot(world_norm, H), 0), material_shininess);
	}

	float cut_offf = radians(60);
	
	if(spot==0)
	{
		light += atenuation*(diffuse_light+specular_light);
	}

	if (spot_light > cos(cut_off) && spot==1)	
	{
		// Quadratic attenuation
		float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
		float light_att_factor = pow(linear_att, 2);

	// fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
	// se calculeaza atenuarea luminii
		light+=light_att_factor*(diffuse_light+specular_light);
	}

	if(do_tex==1)
		out_color = texture2D(textura,tex_coord)*light;
	else
		out_color = vec4(object_color,1)*light;

}