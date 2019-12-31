#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform sampler2D heightmap;
uniform vec2 heightmapsize;

out vec2 tex_coord;

out vec3 world_position;

void main()
{
	// TODO : pass v_texture_coord as output to Fragment Shader
	vec3 pos = v_position;
	float height = texture2D(heightmap,v_texture_coord).r*20;
	pos.y = height;
	//if(pos.x==0 && pos.z==0) pos.y=74.75;
	gl_Position = Projection * View * Model * vec4(pos, 1.0);

	world_position = (Model * vec4(pos,1)).xyz;

	tex_coord = vec2(v_texture_coord.x*5,v_texture_coord.y*5);
}
