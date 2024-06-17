#pragma once

#include "../Graphics/GeneralRenderer.hpp"
#include "../Input/InputManager.hpp"

class Scene
{
public:
	virtual void update(float dt) = 0;
	virtual void draw(RenderingAPI* renderingAPI) = 0;
	virtual void input() = 0;
};

class SceneManager
{
public:
	static SceneManager& getInstance();
	static void initialize(RenderingAPI* renderingAPI);
	void update(float dt=0);
	void draw();
	void input();
	void setScene(Scene* scene);
	Scene* getScene();
protected:
	static SceneManager& instanceImp(RenderingAPI* renderingAPI);
	SceneManager(RenderingAPI* renderingAPI);
	RenderingAPI* renderingAPI;
	Scene* currentScene=nullptr;
};