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

#define RENDER_WIDTH    640
#define RENDER_HEIGHT   640

#define WIDTH 1280
#define HEIGHT 720

// Main code
int main(int, char**){
    
    if(SDLInit() != 0)
        return 1;
    initImGUI(window, renderer);

    RayTracingRenderer rtRenderer(RENDER_WIDTH, RENDER_HEIGHT);

    // rtRenderer.addShape( new Sphere(glm::vec3(-0.5f, 0.f, 0.f), 0.25f) );
    // rtRenderer.addShape( new Sphere(glm::vec3(0.5f, 0.f, 0.f), 0.25f) );
    // rtRenderer.addShape( new Cube(glm::vec3(0.f, 0.f, -1.5f), 1.f) );
    // rtRenderer.addShape(
    //     new Plane(
    //         glm::vec3(  -10.f,   -10.f,  -10.5f),
    //         glm::vec3(  -10.f,    10.f,  -10.5f),
    //         glm::vec3(   10.f,    10.f,  -10.5f),
    //         glm::vec3(   10.f,   -10.f,  -10.5f)
    //     )
    // );

    rtRenderer.addLight( new DirectionalLight(-1.f, -1.f, -1.f) );
    rtRenderer.addLight( new DirectionalLight(1.f, 1.f, 1.f) );

    // Main loop
    while (!done){

        SDL_RenderClear(renderer);

        eventHandler(window, renderer);

        drawImGUI(&rtRenderer);

        // Convert the SDL surface to an ImGUI texture
        ImTextureID textureID = rtRenderer.render(renderer);

        // Display the image using ImGUI
        ImGui::Begin("Image Window");
        ImGui::Image(textureID, ImVec2(RENDER_WIDTH, RENDER_HEIGHT));
        ImGui::End();

        // Update the display with the modified framebuffer
        // SDL_UpdateWindowSurface(window);
        renderImGUI(renderer);

        SDL_RenderPresent(renderer);

        // if(saveFrame){
        //     frameToBMP(textureID);
        //     saveFrame = false;
        // }
        
    }

    cleanImGUI();
    SDLClean(window, renderer);

    return 0;
}

void frameToBMP(ImTextureID textureID){
    
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

void drawImGUI(RayTracingRenderer* rtRenderer){
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
    DockSpace();

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Software RayTracer");                        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        ImGui::Separator();

        ImGui::InputInt("Bounces", (int *)rtRenderer->getBouncesRef());

        ImGui::Separator();

        Camera* cam = rtRenderer->getCamRef();
        ImGui::DragFloat3("Camera Position", glm::value_ptr(cam->position), 0.1f);

        ImGui::Separator();

        glm::vec3 clearColor = rtRenderer->getClearColor();
        if(ImGui::ColorEdit3("clear color", glm::value_ptr(clearColor))) // Edit 3 floats representing a color
            rtRenderer->setClearColor(clearColor);
        
        if(ImGui::Button("Screenshot"))
            rtRenderer->takeScreenshot();

        ImGui::Checkbox("Enable Shadow", &rtRenderer->activateShadow );

        ImGui::End();
        ImGui::Begin("Objects"); 

        if(ImGui::Button("Add Sphere"))
            rtRenderer->addShape(new Sphere(glm::vec3(0.f, 0.f, 0.f), 0.25f));
        
        ImGui::SameLine();

        if(ImGui::Button("Add Cube"))
            rtRenderer->addShape(new Cube(glm::vec3(0.f, 0.f, 0.f), 0.25f));

        ImGui::Separator();

        for (size_t i = 0; i < rtRenderer->getSceneSize(); i++){

            ImGui::PushID(i);

            Shape* selectedShape = rtRenderer->getShape(i);

            float roughness = selectedShape->getRoughness();
            if( ImGui::DragFloat("Roughness", &roughness, 0.025f, 0.f, 1.f) )
                selectedShape->setRoughness(roughness);

            glm::vec3 shapeColor = selectedShape->getAlbedo();
            if(ImGui::ColorEdit3("Albedo", glm::value_ptr(shapeColor) )) // Edit 3 floats representing a color
                selectedShape->setAlbedo(shapeColor);

            glm::vec3 shapePosition = selectedShape->getPosition();
            if(ImGui::DragFloat3("Position", glm::value_ptr(shapePosition), 0.1f))
                selectedShape->setPosition(shapePosition);

            if(Sphere *sphere = dynamic_cast<Sphere *>(selectedShape)){
                float radius = sphere->getRadius();
                if( ImGui::DragFloat("Radius", &radius, 0.1f) )
                    sphere->setRadius(radius);
            }

            if(Cube *cube = dynamic_cast<Cube *>(selectedShape)){
                float s = cube->getScale();
                if( ImGui::DragFloat("Scale", &s, 0.1f) )
                    cube->setScale(s);
            }

            if( ImGui::Button("Remove") )
                rtRenderer->removeShape(i);

            ImGui::Separator();

            ImGui::PopID();

        }

        ImGui::End();
        ImGui::Begin("Lights"); 

        const char* light_model_names[LIGHT_MODEL_COUNT] = { "Phong", "Oren-Nayar", "Cook-Torrance" };
        
        for (size_t i = 0; i < rtRenderer->getLightsSize(); i++){

            ImGui::PushID(i);

            DirectionalLight* light = rtRenderer->getLight(i);
            if(ImGui::DragFloat3("Direction Light", glm::value_ptr(light->direction), 0.1f))
                light->direction = glm::normalize( light->direction );

            const char* elem_name = (light->model >= 0 && light->model < LIGHT_MODEL_COUNT) ? light_model_names[light->model] : "Unknown";
            ImGui::SliderInt("Lighting Model", (int*)&light->model, 0, LIGHT_MODEL_COUNT - 1, elem_name);
            

            if( ImGui::Button("Remove") )
                rtRenderer->removeLight(i);

            ImGui::Separator();

            ImGui::PopID();

        }

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

void DockSpace(){
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each other.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen){
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", (bool*)0, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();
}