#include "Camera.h"
#include "ICameraControllerInput.h"
#include <glm/gtc/matrix_transform.inl>

#include "glm/gtc/matrix_transform.hpp"

// default camera position is at world origin looking towards -z
Camera::Camera() : Camera(0.0f, 0.0f, 0.0f, 3.14f, 0.0f, 60.0f, 200.0f)
{
}

Camera::Camera(float x,
	float y,
	float z,
	float horizontalAngle,
	float verticalAngle,
	float fieldOfView,
	float range,
	float ratio) : _position(x, y, z),
		_verticalAngle(verticalAngle),
		_horizontalAngle(horizontalAngle),
		_fieldOfView(fieldOfView),
		_farDepth(range),
		_nearDepth(0.1f),
		_ratio(ratio),
		_frustrumWidth(0.0f),
		_frustrumHeight(0.0f),
		_tang(0.0f)
{
	
}

#define HALF_ANG2RAD 3.14159265358979323846/360.0 
#define ANG2RAD 3.14159265358979323846/180.0
void Camera::update(const ICameraControllerInput& controllerInput)
{
	_horizontalAngle += controllerInput.getLeftThumbstickDelta().xDelta;
	_verticalAngle += controllerInput.getLeftThumbstickDelta().yDelta;

	// Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(_verticalAngle) * sin(_horizontalAngle),
		sin(_verticalAngle),
		cos(_verticalAngle) * cos(_horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(_horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Apply right thumbstick
	_position += direction * controllerInput.getRightThumbstickDelta().yDelta;
	_position += right * controllerInput.getLeftThumbstickDelta().xDelta;

	// projection matrix: field of View, display ratio (4:3), display range
	_projectionMatrix = glm::perspective(glm::radians(_fieldOfView), _ratio, _nearDepth, _farDepth);
	// Camera matrix

	
	_viewMatrix = glm::lookAt(
		_position,           // Camera is here
		_position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// set up the frustrum
	//_tang = static_cast<float>(tan(glm::radians(_fieldOfView * 0.5)));
	_tang = static_cast<float>(tan(_fieldOfView * ANG2RAD));
	_frustrumHeight = _nearDepth * _tang;
	_frustrumWidth = _frustrumHeight * _ratio;
	glm::vec3 pointingAt = _position + direction;
	glm::vec3 frustrumZAxis = _position - pointingAt;
	_frustrumZ = glm::normalize(frustrumZAxis);
	glm::vec3 frustrumXAxis = up * _frustrumZ;
	_frustrumX = glm::normalize(frustrumXAxis);
	_frustrumY = _frustrumZ * _frustrumX;
}

bool Camera::pointInCamera(const glm::vec3& pt) const
{
	glm::vec3 v = pt - _position;
	float pcz = glm::dot(v, -_frustrumZ);
	if (pcz > _farDepth || pcz < _nearDepth) return false;

	float pcy = glm::dot(v, _frustrumY);
	float aux = pcz * _tang;
	if (pcy > aux || pcy < -aux) return false;

	float pcx = glm::dot(v, _frustrumX);
	aux = aux * _ratio;
	if (pcx > aux || pcx < -aux) return false;

	return true;
}

bool Camera::pointInCamera(float x, float y, float z) const
{
	return pointInCamera(glm::vec3(x, y, z));
}

Camera::~Camera()
{
}
