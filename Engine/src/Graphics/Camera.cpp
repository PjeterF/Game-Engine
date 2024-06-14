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

bool Camera::pointIsInFrustrum(glm::vec2 point)
{
	if (point.x < getFrustrumX().x)
		return false;
	if (point.x > getFrustrumX().y)
		return false;
	if (point.y < getFrustrumY().x)
		return false;
	if (point.y > getFrustrumY().y)
		return false;

	return true;
}
