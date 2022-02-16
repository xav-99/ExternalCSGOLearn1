#pragma once

#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "moreStuff/memory.h"
#include "moreStuff/vector.h"
#include "moreStuff/offsets.h"
#include "helper.h"

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <cmath>


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;


    const char* glsl_version = "#version 130";

    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

    GLFWwindow* window = glfwCreateWindow(screenX, screenY, "v1", NULL, NULL);

    if (window == NULL)
        return 1;

    glfwSetWindowAttrib(window, GLFW_DECORATED, false);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    bool show_demo_window = true;
    bool show_another_window = false;

    bool bMenuVis = true;
    
    
    // Main loop
    
    int refresh_rate = 144;
    int update_time = round(1000 / refresh_rate);


    const auto memory = Memory{ "csgo.exe" };

    const auto client = memory.GetModuleAddress("client.dll");
    std::cout << client << std::endl;
    const auto engine = memory.GetModuleAddress("engine.dll");
    auto oldPunch = Vector2{ };

    constexpr const auto teamColour = Colour{ 12, 17, 114 };
    constexpr const auto enemyColour = Colour{ 113, 1, 147 };
    


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            bMenuVis = !bMenuVis;
            if (bMenuVis)
            {
                ShowMenu(window);
            }
            else
            {
                HideMenu(window);
            };

        };

        //DRAW HERE
        if (bMenuVis)
        {
            if (ImGui::Button("exit"))
            {
                return 0;
            };
        };

        //main shit
        
        view_matrix_t vm = memory.Read<view_matrix_t>(client + offset::dwViewMatrix);
        const auto& localPlayer = memory.Read<std::uintptr_t>(client + offset::dwLocalPlayer);
        const auto& localTeam = memory.Read<std::int32_t>(localPlayer + offset::m_iTeamNum);

        const auto& playerhealth = memory.Read<std::int32_t>(localPlayer + offset::m_iHealth);
        //std::cout << playerhealth << std::endl;

        Vector2 lineOrigin(0.0f, -1.0f);

        for (auto i = 1; i <= 64; ++i)
        {
            const auto& entity = memory.Read<std::uintptr_t>(client + offset::dwEntityList + i * 0x10);

            if (memory.Read<bool>(entity + offset::m_bDormant) || !memory.Read<std::int32_t>(entity + offset::m_iHealth)) {
                continue;
            };

            std::cout << "case 2\n";

            Vector2 screenCoords;

            const auto boneMatrix = memory.Read<std::uintptr_t>(entity + offset::m_dwBoneMatrix);


            const auto& entityHeadPos = Vector3{
                memory.Read<float>(boneMatrix + 0x30 * 8 + 0x0C),
                memory.Read<float>(boneMatrix + 0x30 * 8 + 0x1C),
                memory.Read<float>(boneMatrix + 0x30 * 8 + 0x2C)
            };


            if (!WorldToScreen(entityHeadPos, screenCoords, vm)) continue;
            Vector2 screenposHead = screenCoords;

            const auto& entityFootPos = Vector3{
                memory.Read<float>(boneMatrix + 0x30 * 1 + 0x0C),
                memory.Read<float>(boneMatrix + 0x30 * 1 + 0x1C),
                memory.Read<float>(boneMatrix + 0x30 * 1 + 0x2C)
            };

            if (memory.Read<std::int32_t>(entity + offset::m_iTeamNum) == localTeam)
            {
            }
            else
            {
            };

            WorldToScreen(entityFootPos, screenCoords, vm);

            Vector2 screenposFoot = screenCoords;
            float height = screenposHead.y - screenposFoot.y;
            float width = height / 2.4f;

            glBegin(GL_LINES);
            glVertex2f(lineOrigin.x, lineOrigin.y);
            glVertex2f(screenposFoot.x, screenposFoot.y);
            glEnd();



        };
        
        //end main shit


        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}