#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Viewport.hpp";
#include "../OpenGL/FrameBuffer.hpp"
#include "../Managers/TexturesManager.hpp"
#include "../Managers/Resource/ResourceManager.hpp"

class SpriteSheetEditor
{
public:
	SpriteSheetEditor(int viewportWidth, int viewPortHeight);
	~SpriteSheetEditor();
	void render();
private:
	Resource<Texture>* currTexture=nullptr;
	float imageScale=1.5;
	int viewportWidth;
	float viewPortHeight;
	FrameBuffer* frameBuffer;
	SpriteRenderer* renderer;
	Camera* camera;
};