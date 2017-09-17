#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"

int main (int argc, char **argv)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_Window *window = SDL_CreateWindow("ImGui SDL2+OpenGL+SDL_Render example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

    int oglIdx = -1;
    int nRD = SDL_GetNumRenderDrivers();
    for(int i=0; i<nRD; i++)
    {
        SDL_RendererInfo info;
        if(!SDL_GetRenderDriverInfo(i, &info))
        {
            if(!strcmp(info.name, "opengl"))
            {
                oglIdx = i;
            }
        }
    }
    SDL_Renderer * renderer = SDL_CreateRenderer(window, oglIdx, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 

    // Setup ImGui binding
    ImGui_ImplSdl_Init(window);

    glewExperimental = true;
    glewInit ();

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    SDL_Surface *bitmapSurface = SDL_LoadBMP ("test.bmp");
    SDL_Texture *bitmapTex = SDL_CreateTextureFromSurface (renderer, bitmapSurface);
    SDL_FreeSurface (bitmapSurface);

    // Main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSdl_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }

        ImGui_ImplSdl_NewFrame(window);

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }
        
        // Rendering
        SDL_Rect vp;
        vp.x = vp.y = 0;
        vp.w = (int) ImGui::GetIO ().DisplaySize.x;
        vp.h = (int) ImGui::GetIO ().DisplaySize.y;
        SDL_RenderSetViewport (renderer, &vp);
        SDL_SetRenderDrawColor (renderer, clear_color.x * 255.0f, clear_color.y * 255.0f, clear_color.z * 255.0f, clear_color.w * 255.0f);
        SDL_RenderClear (renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rct;
        rct.x = rct.y = 50 + (SDL_GetTicks () >> 9);
        rct.h = rct.w = 100;
        SDL_Rect rct2;
        rct2.x = rct2.y = 160;
        rct2.w = rct2.h = 256;
        SDL_RenderCopy (renderer, bitmapTex, 0, &rct2);
        SDL_RenderDrawLine(renderer, 0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        SDL_RenderDrawRect (renderer, &rct);

        glUseProgram (0);
        ImGui::Render();
        
        SDL_RenderPresent(renderer);  
    }

    // Cleanup
    ImGui_ImplSdl_Shutdown();
    SDL_DestroyTexture(bitmapTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
