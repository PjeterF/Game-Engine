#include "Camera.hpp"

void Camera::pan(float x, float y)
{
	offset = offset - zoom*panningSpeedScale*glm::vec2(x, y);
}
void Camera::changeZoom(float increment)
{
	zoom = zoom + increment;
	if (zoom <= 0)
	{
		zoom = 0.05;
	}
}
void Camera::setZoom(float newZoom)
{
	zoom = newZoom;
	if (zoom <= 0)
	{
		zoom = 0.05;
	}
}
