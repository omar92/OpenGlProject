#version 330

in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 vertex_uv;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

float ka=0.5;
float kd=0.8;
float ks=5;
float shineness=130;
vec3 lightDir=vec3(3.0,0.0,0.0);
uniform vec3 material_color=vec3(1,1,1);
vec3 light_color=vec3(1,1,1);
vec3 camPos=vec3(0,0,3);

out vec3 color;
out vec2 frag_uv;
out vec3 light;
void main()
{	
	//position

	vec4 pos=projMat*viewMat*modelMat*vec4(vertex_position,1);
	pos.y +=sin(pos.z);
	gl_Position = pos;
	//lighting
	vec3 norm=normalize(modelMat*vec4(vertex_normal,0)).xyz;
	

	vec3 l=normalize(lightDir);
	vec3 v=normalize(camPos-(modelMat*vec4(vertex_position,1)).xyz);
	vec3 h=normalize(l+v);

	vec3 lightInt=ka*light_color+
	kd*max(dot(l,norm),0)*light_color+
	ks*max(pow(dot(h,norm),shineness),0)*vec3(1,1,1);

	light=lightInt;
	color=material_color;
	frag_uv=vertex_uv;
}

