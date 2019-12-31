#version 330
 
uniform sampler2D terrain_tex;


uniform sampler2D heightmap;
uniform vec2 heightmapsize;

in vec2 tex_coord;
in vec3 world_position;

uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform mat4 Model;

layout(location = 0) out vec4 out_color;

uniform vec3 light_direction_sp1;
uniform vec3 light_position_sp1;
uniform vec3 light_direction_sp2;
uniform vec3 light_position_sp2;

uniform int spot_proj;
uniform vec3 light_direction_sp3;
uniform vec3 light_position_sp3;

void main()
{
	vec2 texelSize = vec2( 1/255,1/255);
	float height = 20*texture2D(heightmap,tex_coord).r;
	float heightRight = texture2D(heightmap, vec2(tex_coord.x + texelSize.x, tex_coord.y)).r;
	float heightUp = texture2D(heightmap, vec2(tex_coord.x,tex_coord.y+texelSize.y)).r;
	float heightLeft = texture2D(heightmap, vec2(tex_coord.x - texelSize.x, tex_coord.y)).r;
	float heightDown = texture2D(heightmap, vec2(tex_coord.x,tex_coord.y-texelSize.y)).r;
	float Hx =  (height - heightRight)/60;
	float Hz = (height - heightUp)/60;
	vec3 normal = normalize(vec3(Hx,1,Hz));
	

	vec3 world_norm = normalize( mat3(Model)*normal);

	vec3 L = normalize( light_position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );

	// TODO: define ambient light component
	float ambient_light = 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * max ( dot(world_norm,L), 0);

	float primesteLumina;
	if( dot(world_norm,L) > 0 )
		primesteLumina = 1;
	else 
		primesteLumina = 0;

	// TODO: compute specular light component
	float specular_light = 0;

	

	float d = distance(world_position,light_position);
	float atenuation = 1/(3+5*d+d*d);

	float light=ambient_light;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * primesteLumina * pow(max(dot(world_norm, H), 0), material_shininess);
	}

	light += atenuation*(diffuse_light+specular_light);

	float cut_off = radians(60);

	vec3 L1 = normalize( light_position_sp1 - world_position );
	vec3 H1 = normalize( L1 + V );

	float spot_light = dot(-L1, light_direction_sp1);
	float spot_light_limit = cos(cut_off);
	float diffuse2 = material_kd * max ( dot(world_norm,L1), 0)/1000;
	float primesteLumina2;
	if( dot(world_norm,L1) > 0 )
		primesteLumina2 = 1;
	else 
		primesteLumina2 = 0;
	float sp2 = material_ks * primesteLumina2 * pow(max(dot(world_norm, H1), 0), material_shininess);
	if (spot_light > cos(cut_off))	
	{
		// Quadratic attenuation
		float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
		float light_att_factor = pow(linear_att, 2);

	// fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
	// se calculeaza atenuarea luminii
		light+=light_att_factor*(diffuse2+sp2);
	}

	L1 = normalize( light_position_sp2 - world_position );
	H1 = normalize( L1 + V );

	spot_light = dot(-L1, light_direction_sp2);
	spot_light_limit = cos(cut_off);
	diffuse2 = material_kd * max ( dot(world_norm,L1), 0)/1000;
	primesteLumina2;
	if( dot(world_norm,L1) > 0 )
		primesteLumina2 = 1;
	else 
		primesteLumina2 = 0;
	sp2 = material_ks * primesteLumina2 * pow(max(dot(world_norm, H1), 0), material_shininess);
	if (spot_light > cos(cut_off))	
	{
		// Quadratic attenuation
		float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
		float light_att_factor = pow(linear_att, 2);

	// fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
	// se calculeaza atenuarea luminii
		light+=light_att_factor*(diffuse2+sp2);
	}

	if(spot_proj==1)
	{
		L1 = normalize( light_position_sp3 - world_position );
		H1 = normalize( L1 + V );

		spot_light = dot(-L1, light_direction_sp3);
		spot_light_limit = cos(cut_off);
		diffuse2 = material_kd * max ( dot(world_norm,L1), 0)/1000;
		primesteLumina2;
		if( dot(world_norm,L1) > 0 )
			primesteLumina2 = 1;
		else 
			primesteLumina2 = 0;
		sp2 = material_ks * primesteLumina2 * pow(max(dot(world_norm, H1), 0), material_shininess);
		if (spot_light > cos(cut_off))	
		{
			// Quadratic attenuation
			float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
			float light_att_factor = pow(linear_att, 1.5);

		// fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
		// se calculeaza atenuarea luminii
			light+=light_att_factor*(diffuse2+sp2);
		}
	}
	
	out_color = texture2D(terrain_tex,tex_coord)*light;;
}
