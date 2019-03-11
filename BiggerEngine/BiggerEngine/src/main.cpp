#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <stdio.h>
#include <iostream>

#include "Miscro.h"
#include "Primitive.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Texture2D.h"
#include "Model.h"
#include "FrameBuffer.h"
#include "Time.h"
#include "SkyBox.h"

GLFWwindow* createWindow();
void initOpenGL();
Shader createShader(const char *vertPath, const char *fragPath);
void processKeyBoard(GLFWwindow* window);
void onSetFrameBuffer(GLFWwindow* window, int width, int height);
void onSetCursorPos(GLFWwindow* window, double xpos, double ypos);
void onSetScroll(GLFWwindow* window, double xoffset, double yoffset);
void setupObjectMVP(Shader shader);
void setupLightMVP(Shader shader);
void drawObject(unsigned int vao, Shader shader);
void drawLight(unsigned int vao, Shader shader);
void setupVP(Shader shader);



// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float cubeVertices[] = {
	// positions          // texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

float lightVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float rectVertices[] = {
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,   // 右上
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // 右下
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // 左下
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f    // 左上
};
unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

vector<std::string> faces {
	"res/img/skybox/right.jpg",
	"res/img/skybox/left.jpg",
	"res/img/skybox/top.jpg",
	"res/img/skybox/bottom.jpg",
	"res/img/skybox/front.jpg",
	"res/img/skybox/back.jpg"
};

int main() {
	auto window = createWindow();
	if (window == nullptr)
	{
		return -1;
	}
	initOpenGL();

// 	SkyBox skybox(faces);
// 
// 
// 	//Create cube vao
// 	unsigned int skyboxvao;
// 	glGenVertexArrays(1, &skyboxvao);
// 	glBindVertexArray(skyboxvao);
// 
// 	unsigned int skyboxvbo;
// 	glGenBuffers(1, &skyboxvbo);
// 	glBindBuffer(GL_ARRAY_BUFFER, skyboxvbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
// 	
// 	// VBO only save the data of vertices, without any attribute of vertex !
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);
// 	
// 	glBindVertexArray(0);
// 	
// 	Shader skyboxshader("res/shader/skybox.vert", "res/shader/skybox.frag");
// 	skyboxshader.Use();
// 	skyboxshader.SetInt("skybox", 0);
// 
// 	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.m_iTexture);


// 	// Create light vao
// 	unsigned int lightVAO = createVAO();
// 	unsigned int lightVBO = createVBO(lightVertices, sizeof(lightVertices));
// 	glBindVertexArray(lightVAO);
// 	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
// 	
// 	// VBO only save the data of vertices, without any attribute of vertex !
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);
// 	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
// 	glEnableVertexAttribArray(1);
// 	
// 	glBindVertexArray(0);
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);

	

// 	Shader shader = createShader("res/shader/object.vert", "res/shader/object.frag");
// 	shader.Use();
// 
// 	shader.SetVec3("viewPos", camera.m_vec3Position);
// 
// 	shader.SetVec3("directLight.direction", -0.2f, -1.0f, -0.3f);
// 	shader.SetVec3("directLight.ambient", 0.8f, 0.2f, 0.2f);
// 	shader.SetVec3("directLight.diffuse", 0.8f, 0.5f, 0.5f);
// 	shader.SetVec3("directLight.specular", 1.0f, 1.0f, 1.0f);
// 
// 	shader.SetVec3("spotLight.position", camera.m_vec3Position);
// 	shader.SetVec3("spotLight.direction", camera.m_vec3Front);
// 	shader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
// 	shader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
// 
// 	shader.SetVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
// 	shader.SetVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
// 	shader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
// 
// 	shader.SetVec3("pointLight.position", lightPos);
// 	shader.SetVec3("pointLight.ambient", 0.2f, 0.2f, 0.8f);
// 	shader.SetVec3("pointLight.diffuse", 0.5f, 0.5f, 0.8f);
// 	shader.SetVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
// 	shader.SetFloat("pointLight.constant", 1.0f);
// 	shader.SetFloat("pointLight.linear", 0.09f);
// 	shader.SetFloat("pointLight.quadratic", 0.032f);
// 
// 
// 	shader.SetFloat("material.shininess", 32.0f);

// 	Texture2D diffuseTexture("res/img/container2.png", GL_RGBA);
// 	diffuseTexture.Use(GL_TEXTURE0);
// 	shader.SetInt("light.diffuse", 0);
// 	Texture2D::Clear();
// 
// 	Texture2D specularTexture("res/img/container2_specular.png", GL_RGBA);
// 	specularTexture.Use(GL_TEXTURE1);
// 	shader.SetInt("light.specular", 1);
// 	Texture2D::Clear();
// 
// 	Shader lightshader = createShader("res/shader/light.vert", "res/shader/light.frag");
// 	lightshader.Use();
// 	lightshader.SetInt("sampler0", 0);
// 
// 	Texture2D lightTexture2D("res/img/light.jpg", GL_RGB, true);
// 	lightTexture2D.Use();
// 	Texture2D::Clear();

// 	Model ourModel("res/mesh/nanosuit/nanosuit.obj");
// 
// 	Shader normalShader("res/shader/normal.vert", "res/shader/normal.frag", "res/shader/normal.geom");
// 
// 	Shader modelShader("res/shader/model.vert", "res/shader/model.frag");



// 	// cube VAO
// 	unsigned int cubeVAO, cubeVBO;
// 	glGenVertexArrays(1, &cubeVAO);
// 	glGenBuffers(1, &cubeVBO);
// 	glBindVertexArray(cubeVAO);
// 	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
// 	glEnableVertexAttribArray(0);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(1);
// 	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
// 	glBindVertexArray(0);
// 	Shader cubeShader("res/shader/refract.vert", "res/shader/refract.frag");
// 	cubeShader.Use();
// 	cubeShader.SetInt("skybox", 0);

// 	unsigned int cubeTexture;
// 	glGenTextures(1, &cubeTexture);
// 	glBindTexture(GL_TEXTURE_2D, cubeTexture);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// 	int width, height, channels;
// 	unsigned char* data = stbi_load("res/img/container.jpg", &width, &height, &channels, 0);
// 	if (data) {
// 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
// 		glGenerateMipmap(GL_TEXTURE_2D);
// 	}
// 	else {
// 		std::cout << "Load image error:" << std::endl;
// 	}
// 	stbi_image_free(data);
	

	

	

		unsigned int rectVAO, rectVBO, rectEBO;
		glGenVertexArrays(1, &rectVAO);
		glGenBuffers(1, &rectVBO);
		glGenBuffers(1, &rectEBO);
		glBindVertexArray(rectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), &rectVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
		glBindVertexArray(0);
	
	
		Shader rectShader("res/shader/simple.vert", "res/shader/simple.frag");
		rectShader.Use();
		rectShader.SetInt("screenTexture", 0);
	
	
// 		FrameBuffer famebuffer(SCR_WIDTH, SCR_HEIGHT);

	
Shader instanceShader("res/shader/instance.vert", "res/shader/instance.frag");

	glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }

    // store instance data in an array buffer
    // --------------------------------------
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float quadVertices[] = {
        // positions     // colors
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
         0.05f,  0.05f,  0.0f, 1.0f, 1.0f
    };
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    // also set instance data
	glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.


	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FrameBuffer error" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// configure second post-processing framebuffer
	unsigned int intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	// create a color attachment texture
	unsigned int screenTexture;
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		
		processKeyBoard(window);
		
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glEnable(GL_DEPTH_TEST);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// configure transformation matrices
		glm::mat4 projection = glm::perspective(glm::radians(camera.m_fZoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();;
		glm::mat4 model = glm::mat4(1.0f);

		instanceShader.Use();
		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
		glBindVertexArray(0);
		


		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);



// 		modelShader.Use();
// 		modelShader.SetMat4("projection", projection);
// 		modelShader.SetMat4("view", view);
// 		modelShader.SetMat4("model", model);
// 
// 		// draw model
// 		ourModel.Draw(modelShader);
// 
// 
// 		normalShader.Use();
// 		normalShader.SetMat4("projection", projection);
// 		normalShader.SetMat4("view", view);
// 		normalShader.SetMat4("model", model);
// 
// 		// draw model
// 		ourModel.Draw(normalShader);

// 		glBindVertexArray(cubeVAO);
// 		glActiveTexture(GL_TEXTURE0);
// 		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.m_iTexture);
// 
// 		cubeShader.Use();
// 		cubeShader.SetMat4("model", model);
// 		cubeShader.SetMat4("view", view);
// 		cubeShader.SetMat4("projection", projection);
// 		cubeShader.SetVec3("cameraPos", camera.m_vec3Position);
// 		glDrawArrays(GL_TRIANGLES, 0, 36);
// 		glBindVertexArray(0);
// 
// 
// 


// 		Texture2D::Clear();
// 
// 		diffuseTexture.Use(GL_TEXTURE0);
// 		specularTexture.Use(GL_TEXTURE1);

// 		for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
// 		{
// 			drawObject(vao, shader);
// 			glm::mat4 model = glm::mat4(1.0f);
// 			model = glm::translate(model, cubePositions[i]);
// 			shader.SetMat4("model", model);
// 		}
		

// 		lightTexture2D.Use(GL_TEXTURE0);
// 		drawLight(lightVAO, lightshader);





// 		glBindFramebuffer(GL_FRAMEBUFFER, famebuffer.m_iFBO);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 		glEnable(GL_DEPTH_TEST);
// 
// 		// don't forget to enable shader before setting uniforms
// 		ourShader.Use();
// 
// 
// 		setupVP(ourShader);
// 
// 		// render the loaded model
// 		glm::mat4 model = glm::mat4(1.0f);
// 		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
// 		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
// 		ourShader.SetMat4("model", model);
// 
// 		ourModel.Draw(ourShader);
// 
// 
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT );

		
		
		rectShader.Use();

		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTexture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

// 		// draw skybox as last
// 		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
// 		skyboxshader.Use();
// 		skyboxshader.SetMat4("view", glm::mat4(glm::mat3(view)));
// 		skyboxshader.SetMat4("projection", projection);
// 
// 		// skybox cube
// 		glBindVertexArray(skyboxvao);
// 		glActiveTexture(GL_TEXTURE0);
// 		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.m_iTexture);
// 		glDrawArrays(GL_TRIANGLES, 0, 36);
// 		glBindVertexArray(0);
// 		glDepthFunc(GL_LESS); // set depth function back to default

		glfwSwapBuffers(window);
		glfwPollEvents();

		float currentTime = glfwGetTime();
		Time::deltaTime = currentTime - Time::lastTime;
		Time::lastTime = currentTime;
	}
	
	glfwTerminate();
	return 0;
}

GLFWwindow* createWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BiggerEngine", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, onSetFrameBuffer);
	glfwSetCursorPosCallback(window, onSetCursorPos);
	glfwSetScrollCallback(window, onSetScroll);

	return window;
}

void initOpenGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// PolygonMode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_CULL_FACE);

}

Shader createShader(const char *vertPath, const char *fragPath) {
	// Create shader program
	Shader shaderProgram(vertPath, fragPath);


	return shaderProgram;
}

void setupVP(Shader shader) {
	glm::mat4 view = camera.GetViewMatrix();
	shader.SetMat4("view", view);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(camera.m_fZoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
	shader.SetMat4("projection", projection);
}

void setupObjectMVP(Shader shader) {
	shader.Use();

	setupVP(shader);

	
	
}

void setupLightMVP(Shader shader) {
	shader.Use();

	setupVP(shader);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.SetMat4("model", model);
	
}

void processKeyBoard(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT);
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWNWARD);
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyboard(UPWARD);
	}
}

void onSetFrameBuffer(GLFWwindow* window, int width, int height) {
	SCR_WIDTH = width; 
	SCR_HEIGHT = height; 
	glViewport(0, 0, width, height); 
}

void onSetCursorPos(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void onSetScroll(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void drawObject(unsigned int vao, Shader shader) {
	glBindVertexArray(vao);
	shader.Use();


	setupObjectMVP(shader);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void drawLight(unsigned int vao, Shader shader) {
	glBindVertexArray(vao);
	shader.Use();
	setupLightMVP(shader);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}