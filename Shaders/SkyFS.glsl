#version 330
 
uniform samplerCube texture_1;

in vec3 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function

	out_color = texture(texture_1,texcoord);
}
