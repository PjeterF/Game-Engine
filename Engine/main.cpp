#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/OpenGL/ShaderProgram.hpp"
#include "src/OpenGL/FrameBuffer.hpp"
#include "src/Graphics/SpriteRenderer.hpp"
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <math.h>
#include "src/Graphics/Camera.hpp"

#include "Application/Games/Game1/Game1.hpp"
#include "Application/Games/ZumaGame/ZumaGame.hpp"

int main(void)
{
    Game1 game1(1920, 1080, "TestGame");
    game1.initialize();
    game1.run();

    /*ZumaGame zuma(1920, 1080, "ZumaGame");
    zuma.initialize();
    zuma.run();*/
}