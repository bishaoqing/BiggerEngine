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

US_BIGGER;

GLFWwindow* createWindow();
void initOpenGL();
unsigned int createVAO();
unsigned int createVBO();
Shader createShader(const char *vertPath, const char *fragPath);
void processKeyBoard(GLFWwindow* window, float deltaTime);
void onSetFrameBuffer(GLFWwindow* window, int width, int height);
void onSetCursorPos(GLFWwindow* window, double xpos, double ypos);
void onSetScroll(GLFWwindow* window, double xoffset, double yoffset);
void setupObjectMVP(Shader shader);
void setupLightMVP(Shader shader);
// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float vertices[] = {
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



int main() {
	auto window = createWindow();
	if (window == nullptr)
	{
		return -1;
	}
	initOpenGL();


	// Create cube vao
	unsigned int vao = createVAO();
	glBindVertexArray(vao);
	unsigned int vbo = createVBO();
	glBindVertexArray(0);


	// Create ligh vao
	unsigned int lightVAO = createVAO();
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	Shader shader = createShader("res/shader/object.vert", "res/shader/object.frag");
	shader.Use();
	shader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.SetVec3("objectColor", glm::vec3(0.0f, 0.5f, 0.0f));

	Shader lightshader = createShader("res/shader/light.vert", "res/shader/light.frag");
	lightshader.Use();


	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		
		processKeyBoard(window, deltaTime);
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glBindVertexArray(vao);
		shader.Use();
		setupObjectMVP(shader);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(lightVAO);
		lightshader.Use();
		setupLightMVP(lightshader);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}
	
	glfwTerminate();
	return 0;
}

GLFWwindow* createWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// PolygonMode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

unsigned int createVAO() {
	// Create vao
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	glBindVertexArray(0);

	return vao;
}

unsigned int createVBO() {
	// Create vbo and bind vertex data
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Assign the format of vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return vbo;
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

	glm::mat4 model = glm::mat4(1.0f);
	shader.SetMat4("model", model);
}

void setupLightMVP(Shader shader) {
	shader.Use();

	setupVP(shader);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.SetMat4("model", model);
}

void processKeyBoard(GLFWwindow* window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWNWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyboard(UPWARD, deltaTime);
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