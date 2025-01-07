
struct SDL_Renderer;
 bool ImGui_ImplSDLRenderer3_Init(SDL_Renderer* renderer);
 void ImGui_ImplSDLRenderer3_Shutdown();
 void ImGui_ImplSDLRenderer3_NewFrame();
 void ImGui_ImplSDLRenderer3_RenderDrawData(ImDrawData* draw_data, SDL_Renderer* renderer);
 bool ImGui_ImplSDLRenderer3_CreateFontsTexture();
 void ImGui_ImplSDLRenderer3_DestroyFontsTexture();
 bool ImGui_ImplSDLRenderer3_CreateDeviceObjects();
 void ImGui_ImplSDLRenderer3_DestroyDeviceObjects();
struct ImGui_ImplSDLRenderer3_RenderState
{
    SDL_Renderer* Renderer;
};