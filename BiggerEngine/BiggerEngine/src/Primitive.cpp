// #include "Primitive.h"
// #include <glad/glad.h> 
// #include <GLFW\glfw3.h>
// 
// NS_BIGGER_BEGIN
// 
// VAO Primitive::DrawTriangle(const Vec3 & p0, const Vec3 & p1, const Vec3 & p2)
// {
// 	const float vertices[] = {
// 		p0.x,p0.y,p0.z,
// 		p1.x,p1.y,p1.z,
// 		p2.x,p2.y,p2.z
// 	};
// 
// 	VAO vao;
// 	glGenVertexArrays(1, &vao);
// 	glBindVertexArray(vao);
// 
// 	VBO vbo;
// 	glGenBuffers(1, &vbo);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);
// 
// 	return vao;
// }
// 
// NS_BIGGER_END