#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/OpenGL/ShaderProgram.hpp"
#include "src/OpenGL/FrameBuffer.hpp"
#include "src/zumalogic/CubicBezierSpline.hpp"
#include "src/Graphics/SpriteRenderer.hpp"
#include "src/zumalogic/Sprite.hpp"
#include <vector>
#include <iostream>
#include "src/zumalogic/Snake.hpp"
#include <chrono>
#include <thread>
#include <ctime>
#include <math.h>
#include "src/Managers/TexturesManager.hpp"
#include "Application.hpp"
#include "src/Graphics/Camera.hpp"

#include "src/GUI/Viewport.hpp"
#include "src/GUI/EditMenu.hpp"
#include "src/GUI/Subcomponents/RouteEdit.hpp"

#include "ZumaApplication/ZumaApp.hpp"

int main(void)
{
    //Application app(1800, 1000, "App1");
    //app.run();

    ZumaApp zumaApp(1800, 1000, "ZumaApp");
    zumaApp.run();
    return 0;
}