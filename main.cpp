#include "main.h"

// Our state
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
bool done = false;
bool saveFrame = false;

// SDL things
SDL_WindowFlags window_flags;
SDL_Window* window;
SDL_Renderer* renderer;

#define RENDER_WIDTH 800
#define RENDER_HEIGHT 600

#define WIDTH 1280
#define HEIGHT 720

// Main code
int main(int, char**){
    
    if(SDLInit() != 0)
        return 1;
    initImGUI(window, renderer);

    RayTracingRenderer rtRenderer(renderer);

    // SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, RENDER_WIDTH, RENDER_HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);

    // Main loop
    while (!done){

        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);

        eventHandler(window, renderer);

        drawImGUI();

        // Lock the surface to access its pixel data
        if (SDL_LockSurface(surface) != 0) {
            // Handle error
            break;
        }

        // Access pixel data and update framebuffer
        Uint32* pixels = (Uint32*)surface->pixels;
        int pitch = surface->pitch / sizeof(Uint32);
        for (int y = 0; y < surface->h; ++y) {
            for (int x = 0; x < surface->w; ++x) {
                // Write pixel data to framebuffer
                Uint32 color = 0xff00ffff /* calculate or retrieve pixel color */;
                pixels[y * pitch + x] = color;
            }
        }

        // Unlock the surface to make it visible
        SDL_UnlockSurface(surface);

        // Convert the SDL surface to an ImGUI texture
        ImTextureID textureID = (ImTextureID)(intptr_t)SDL_CreateTextureFromSurface(renderer, surface);

        // Display the image using ImGUI
        ImGui::Begin("Image Window");
        ImGui::Image(textureID, ImVec2(RENDER_WIDTH, RENDER_HEIGHT));
        ImGui::End();


        // Update the display with the modified framebuffer
        // SDL_UpdateWindowSurface(window);
        renderImGUI(renderer);

        SDL_RenderPresent(renderer);

        // Destroy the SDL texture
        SDL_DestroyTexture((SDL_Texture*)textureID);

        // rtRenderer.render(WIDTH, HEIGHT);

        // for (size_t i = 0; i < 50; i++)
        //     for (size_t j = 0; j < 50; j++)
        //         rtRenderer.putPixel(glm::vec2(i,j), glm::vec3(255, 0, 0));
        
        // if(saveFrame){
        //     frameToBMP();
        //     saveFrame = false;
        // }
        
        // drawImGUI();
        // renderImGUI(renderer);

        // SDL_RenderPresent(renderer);
        
    }

    cleanImGUI();
    SDL_FreeSurface(surface);
    SDLClean(window, renderer);

    return 0;
}

void frameToBMP(){
    
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, WIDTH, HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
    SDL_SaveBMP(surface, "screenshot.bmp");
    SDL_FreeSurface(surface);

}

void SDLClean(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int SDLInit(){
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0){
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
    #ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    // Create window with SDL_Renderer graphics context
    window_flags = (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, window_flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 1;
    }
    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);

    return 0;
}

void cleanImGUI(){
    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void initImGUI(SDL_Window* window, SDL_Renderer* renderer){
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
}

void drawImGUI(){
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    // for (size_t i = 0; i < 50; i++)
    //     for (size_t j = 0; j < 50; j++)
    //         drawList->AddLine(ImVec2(i, j), ImVec2(i, j), IM_COL32(255, 255, 0, 255), 1.0f);

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        if(ImGui::Button("Screenshot"))
            saveFrame = true;

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
}

void renderImGUI(SDL_Renderer* renderer){
    // Rendering
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void eventHandler(SDL_Window* window, SDL_Renderer* renderer){

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            done = true;
    }
}
