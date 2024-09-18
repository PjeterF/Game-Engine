#pragma once

#include <Managers/Scene/SceneManager.hpp>

class Game2Scene1 : public Scene {
public:
	Game2Scene1(Camera& camera);
	virtual void update(float dt) override;
	virtual void draw(RenderingAPI* renderingAPI) override;
	virtual void input() override;
private:

};