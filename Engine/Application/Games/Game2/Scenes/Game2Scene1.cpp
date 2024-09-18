#include "Game2Scene1.hpp"

#include <ECS2/SystemsManager.hpp>
#include <ECS2/Systems/CollisionS.hpp>
#include <Input/Windows/GLFWInputManager.hpp>

Game2Scene1::Game2Scene1(Camera& camera) : Scene(camera)
{
	SystemsManager::getInstance().addSystem<CollisionS>(new CollisionS(100));
}

void Game2Scene1::update(float dt)
{
}

void Game2Scene1::draw(RenderingAPI* renderingAPI)
{
	static float f = 0;
	f += 0.1;

	glm::vec2 p0(1000, 100), p1(200, 200);

	renderingAPI->addQuadInstance(p0, { 15, 10 }, f, { 0.5, 0.5, 1, 1 });
	renderingAPI->addQuadInstance(p1, { 10, 15 }, f, { 0.5, 0.5, 1, 1 });
	renderingAPI->addQuadLineInstance(p0, p1, 1, { 1, 1, 1, 1 });
	renderingAPI->drawQuadInstances();

	SystemsManager::getInstance().getSystem<CollisionS>()->drawGrid(*renderingAPI, camera, 3, { 1, 1, 1, 1 }, true);

	ImGui::Begin("Debug");

	ImGui::End();
}

void Game2Scene1::input()
{
	auto& input = GLFWInputManager::getInstance();

	auto cursorPos = (input.getCursorPos() / camera.getZoom() + camera.getPosition());

	float scrollRegionWidth = 0.01f;
	float rate = 10.0f / camera.getZoom();
	auto normalizedCursorPos = input.getNormalizedCursorPos();
	auto cameraPosition = camera.getPosition();
	float wheel = input.mouseWheel();
	float zoomRate = 0.02;

	if (normalizedCursorPos.x < scrollRegionWidth)
		camera.setPosition(cameraPosition.x - rate, cameraPosition.y);
	if (normalizedCursorPos.x > 1.0f - scrollRegionWidth)
		camera.setPosition(cameraPosition.x + rate, cameraPosition.y);
	if (normalizedCursorPos.y < scrollRegionWidth)
		camera.setPosition(cameraPosition.x, cameraPosition.y - rate);
	if (normalizedCursorPos.y > 1.0f - scrollRegionWidth)
		camera.setPosition(cameraPosition.x, cameraPosition.y + rate);
	if (wheel != 0)
		camera.changeZoom(wheel * zoomRate);
}
