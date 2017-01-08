#pragma once
#include <glm/glm.hpp>
class ICameraControllerInput;

struct cameraConfiguration
{
	float x;
	float y;
	float z;
	float horizontalAngle;
	float verticalAngle;
	float fieldOfView;
	float range;
	float ratio;

	cameraConfiguration(float x=0, float y=0, float z=0, float horizontalAngle=0, float verticalAngle=0, float fieldOfView=60, float range=100, float ratio=4.0f/3.0f) :
		x(x), y(y), z(z), horizontalAngle(horizontalAngle), verticalAngle(verticalAngle), fieldOfView(fieldOfView), range(range), ratio(ratio) { }
};

class ICamera
{
protected:
	~ICamera() = default;
public:
	virtual glm::mat4 getViewMatrix() const = 0;
	virtual glm::mat4 getProjectionMatrix() const = 0;
	virtual glm::vec3 getPosition() const = 0;
	virtual float getVerticalAngle() const = 0;
	virtual float getHorizontalAngle() const = 0;
	virtual float getFieldOfView() const = 0;
	virtual float getFarDepth() const = 0;

	virtual void update(const ICameraControllerInput& controllerInput) = 0;
	virtual bool pointInCamera(float x, float y, float z) const = 0;
	virtual bool pointInCamera(const glm::vec3& pt) const = 0;
};

