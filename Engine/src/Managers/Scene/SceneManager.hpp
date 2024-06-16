#pragma once

class Scene
{
public:
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual void input() = 0;
};

class SceneManager
{
public:
	static SceneManager& getInstance();
	void update(float dt=0);
	void draw();
	void input();
	void setScene(Scene* scene);
	Scene* getScene();
private:
	Scene* currentScene=nullptr;
};