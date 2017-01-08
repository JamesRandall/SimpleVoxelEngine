#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "ICamera.h"
class ICameraControllerInput;
class Camera : public ICamera
{
	glm::mat4 _viewMatrix;
	glm::mat4 _projectionMatrix;
	glm::vec3 _position;
	float _verticalAngle;
	float _horizontalAngle;	
	float _fieldOfView;
	float _farDepth;
	float _nearDepth;
	float _ratio;
	std::shared_ptr<ICameraControllerInput> _cameraControllerInput;

	float _frustrumWidth;
	float _frustrumHeight;
	glm::vec3 _frustrumX;
	glm::vec3 _frustrumZ;
	glm::vec3 _frustrumY;
	float _tang;

public:
	Camera();
	Camera(float x, float y, float z, float horizontalAngle, float verticalAngle, float fieldOfView, float range, float ratio= 4.0f / 3.0f);
	virtual ~Camera();

	glm::mat4 getViewMatrix() const override { return _viewMatrix; }
	glm::mat4 getProjectionMatrix() const override { return _projectionMatrix; }	
	glm::vec3 getPosition() const override { return _position; }
	float getVerticalAngle() const override { return _verticalAngle; }
	float getHorizontalAngle() const override { return _horizontalAngle; }
	float getFieldOfView() const override { return _fieldOfView; }
	float getFarDepth() const override { return _farDepth; }
	bool pointInCamera(float x, float y, float z) const override;
	bool pointInCamera(const glm::vec3& pt) const override;

	void update(const ICameraControllerInput& controllerInput) override;
};

