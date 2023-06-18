#define SDL_MAIN_HANDLED
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include "SDL.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "RayTracingRenderer.h"

#include "Sphere.h"
#include "Cube.h"
#include "Plane.h"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

// ImGUI functions
void initImGUI(SDL_Window* window, SDL_Renderer* renderer);
void drawImGUI(RayTracingRenderer* rtRenderer);
void renderImGUI(SDL_Renderer* renderer);
void cleanImGUI();
void DockSpace();

// SDL functions
int SDLInit();
void SDLClean(SDL_Window* window, SDL_Renderer* renderer);

// input
void eventHandler(SDL_Window* window, SDL_Renderer* renderer);