#include "Camera.hpp"

void Camera::pan(float x, float y)
{
	position = position - zoom*panningSpeedScale*glm::vec2(x, y);
}
void Camera::changeZoom(float increment)
{
	zoom = zoom + increment;
	if (zoom <= 0)
		zoom = 0.1;
	if (zoom == 3)
		zoom = 3;
}
void Camera::setZoom(float newZoom)
{
	zoom = newZoom;
	if (zoom <= 0)
		zoom = 0.1;
	if (zoom == 3)
		zoom = 3;
}

bool Camera::isPointInFrustrum(glm::vec2 point, float margin)
{
	if (point.x < getFrustrumX().x-margin)
		return false;
	if (point.x > getFrustrumX().y+margin)
		return false;
	if (point.y < getFrustrumY().x-margin)
		return false;
	if (point.y > getFrustrumY().y+margin)
		return false;

	return true;
}

glm::vec2 Camera::viewPortPointToWorldCoord(glm::vec2 normalizedP)
{
	glm::vec2 frustumX = getFrustrumX();
	glm::vec2 frustumY = getFrustrumY();

	glm::vec2 p = glm::vec2(frustumX.y - frustumX.x, frustumY.y - frustumY.x) * normalizedP;
	p = p + this->position;

	return p;
}
