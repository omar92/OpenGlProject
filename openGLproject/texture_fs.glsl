#version 330

in vec3 color;
in vec3 light ;
in vec2 frag_uv;

uniform sampler2D colorMap;

void main()
{
	gl_FragColor=vec4(light*color,1)*texture(colorMap,frag_uv);
	//gl_FragColor=vec4(color,1);
}