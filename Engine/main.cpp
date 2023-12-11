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

#include "ZumaApplication/ZumaApp.hpp"

int main(void)
{
    ZumaApp zumaApp(1800, 1000, "ZumaApp");
    zumaApp.run();
    return 0;
}