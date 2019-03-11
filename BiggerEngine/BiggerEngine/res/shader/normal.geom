#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
} gs_in[];

void drawNormal(int index){
	
	gl_Position = gl_in[index].gl_Position;

	//�����ʲô������Ҫ�������ݸ�fs����ô��Ҫ��Emit֮ǰ�����úã�����uv����
	EmitVertex();

	gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 1.0f) * 0.4f;
	EmitVertex();

	EndPrimitive();
}

void main(){
	drawNormal(0);
	drawNormal(1);
	drawNormal(2);
}