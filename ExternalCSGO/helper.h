#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int screenX = GetSystemMetrics(SM_CXSCREEN);
int screenY = GetSystemMetrics(SM_CYSCREEN);


void ShowMenu(GLFWwindow* Window)
{
	std::cout << "Showing Menu\n";
	glfwSetWindowAttrib(Window, GLFW_MOUSE_PASSTHROUGH, false);

};

void HideMenu(GLFWwindow* Window)
{
	std::cout << "Hiding Menu\n";
	glfwSetWindowAttrib(Window, GLFW_MOUSE_PASSTHROUGH, true);
};

void ConvertToRange(Vector2 Point)
{
    Point.x /= screenX;
    Point.x *= 2.0f;
    Point.x -= 1.0f;

    Point.y /= screenY;
    Point.y *= 2.0f;
    Point.y -= 1.0f;


};

bool WorldToScreen(const Vector3 pos, Vector2 vecScreen, view_matrix_t matrix) {
    float _x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
    float _y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

    float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

    if (w < 0.01f)
        std::cout << "false" << std::endl;
        return false;

    float inv_w = 1.f / w;
    _x *= inv_w;
    _y *= inv_w;

    float x = screenX * .5f;
    float y = screenY * .5f;

    vecScreen.x += 0.5f * _x * screenX + 0.5f;
    vecScreen.y -= 0.5f * _y * screenY + 0.5f;

    ConvertToRange(vecScreen);
    std::cout << "true" << std::endl;
    return true;
}
