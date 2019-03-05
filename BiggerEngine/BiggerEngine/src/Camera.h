#ifndef _H_CAMERA_H_
#define _H_CAMERA_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UPWARD,
	DOWNWARD
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
	// Camera Attributes
	glm::vec3 m_vec3Position;
	glm::vec3 m_vec3Front;
	glm::vec3 m_vec3Up;
	glm::vec3 m_vec3Right;
	glm::vec3 m_vec3WorldUp;
	// Euler Angles
	float m_fYaw;
	float m_fPitch;
	// Camera options
	float m_fMovementSpeed;
	float m_fMouseSensitivity;
	float m_fZoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : m_vec3Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_fMovementSpeed(SPEED), m_fMouseSensitivity(SENSITIVITY), m_fZoom(ZOOM)
	{
		m_vec3Position = position;
		m_vec3WorldUp = up;
		m_fYaw = yaw;
		m_fPitch = pitch;
		updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_vec3Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_fMovementSpeed(SPEED), m_fMouseSensitivity(SENSITIVITY), m_fZoom(ZOOM)
	{
		m_vec3Position = glm::vec3(posX, posY, posZ);
		m_vec3WorldUp = glm::vec3(upX, upY, upZ);
		m_fYaw = yaw;
		m_fPitch = pitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(m_vec3Position, m_vec3Position + m_vec3Front, m_vec3Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = m_fMovementSpeed * deltaTime;
		if (direction == FORWARD)
			m_vec3Position += m_vec3Front * velocity;
		if (direction == BACKWARD)
			m_vec3Position -= m_vec3Front * velocity;
		if (direction == LEFT)
			m_vec3Position -= m_vec3Right * velocity;
		if (direction == RIGHT)
			m_vec3Position += m_vec3Right * velocity;
		if (direction == UPWARD)
		{
			m_vec3Position += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
		}
		if (direction == DOWNWARD)
		{
			m_vec3Position -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
		}
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= m_fMouseSensitivity;
		yoffset *= m_fMouseSensitivity;

		m_fYaw += xoffset;
		m_fPitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch) {
			if (m_fPitch > 89.0f)
				m_fPitch = 89.0f;
			if (m_fPitch < -89.0f)
				m_fPitch = -89.0f;
		}

		// Update m_vec3Front, m_vec3Right and m_vec3Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (m_fZoom >= 1.0f && m_fZoom <= 45.0f)
			m_fZoom -= yoffset;
		if (m_fZoom <= 1.0f)
			m_fZoom = 1.0f;
		if (m_fZoom >= 45.0f)
			m_fZoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new m_vec3Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
		front.y = sin(glm::radians(m_fPitch));
		front.z = sin(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
		m_vec3Front = glm::normalize(front);
		// Also re-calculate the m_vec3Right and m_vec3Up vector
		m_vec3Right = glm::normalize(glm::cross(m_vec3Front, m_vec3WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_vec3Up = glm::normalize(glm::cross(m_vec3Right, m_vec3Front));
	}
};

#endif
