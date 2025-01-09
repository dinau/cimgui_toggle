// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

#include "imgui_toggle.h"
#include "imgui_toggle_presets.h"

static void imgui_toggle_example();
static void imgui_toggle_simple();
static void imgui_toggle_custom();
static void imgui_toggle_state(const ImGuiToggleConfig& config, ImGuiToggleStateConfig& state);

static void imgui_toggle_example()
{
	// use some lovely gray backgrounds for "off" toggles
	// the default would otherwise use your theme's frame background colors.
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.45f, 0.45f, 0.45f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.65f, 0.65f, 0.65f, 1.0f));
// a toggle that will allow the user to view the demo for simple toggles or a custom toggle
	static bool show_custom_toggle = false;
	ImGui::Toggle(
		show_custom_toggle ? "Showing Custom Toggle" : "Showing Simple Toggles"
		, &show_custom_toggle);

	ImGui::Separator();

	if (show_custom_toggle)
	{
		imgui_toggle_custom();
	}
	else
	{
		imgui_toggle_simple();
	}

	// pop the color styles
	ImGui::PopStyleColor(2);
}

static void imgui_toggle_simple()
{
	static bool values[] = { true, true, true, true, true, true, true, true };
	size_t value_index = 0;

	const ImVec4 green(0.16f, 0.66f, 0.45f, 1.0f);
	const ImVec4 green_hover(0.0f, 1.0f, 0.57f, 1.0f);
	const ImVec4 salmon(1.0f, 0.43f, 0.35f, 1.0f);
	const ImVec4 green_shadow(0.0f, 1.0f, 0.0f, 0.4f);

	// a default and default animated toggle
	ImGui::Toggle("Default Toggle", &values[value_index++]);
	ImGui::Toggle("Animated Toggle", &values[value_index++], ImGuiToggleFlags_Animated);

	// this toggle draws a simple border around it's frame and knob
	ImGui::Toggle("Bordered Knob", &values[value_index++], ImGuiToggleFlags_Bordered, 1.0f);

	// this toggle draws a simple shadow around it's frame and knob
	ImGui::PushStyleColor(ImGuiCol_BorderShadow, green_shadow);
	ImGui::Toggle("Shadowed Knob", &values[value_index++], ImGuiToggleFlags_Shadowed, 1.0f);

	// this toggle draws the shadow & and the border around it's frame and knob.
	ImGui::Toggle("Bordered + Shadowed Knob", &values[value_index++], ImGuiToggleFlags_Bordered | ImGuiToggleFlags_Shadowed, 1.0f);
	ImGui::PopStyleColor(1);

	// this toggle uses stack-pushed style colors to change the way it displays
	ImGui::PushStyleColor(ImGuiCol_Button, green);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, green_hover);
	ImGui::Toggle("Green Toggle", &values[value_index++]);
	ImGui::PopStyleColor(2);

	ImGui::Toggle("Toggle with A11y Labels", &values[value_index++], ImGuiToggleFlags_A11y);

	// this toggle shows no label
	ImGui::Toggle("##Toggle With Hidden Label", &values[value_index++]);
}

static void imgui_toggle_custom()
{
	static ImGuiToggleConfig config;
	static bool toggle_custom = true;

	ImGui::NewLine();

	ImGui::Toggle("Customized Toggle", &toggle_custom, config);

	ImGui::NewLine();

	// these first settings are used no matter the toggle's state.
	ImGui::Text("Persistent Toggle Settings");

	// animation duration controls how long the toggle animates, in seconds. if set to 0, animation is disabled.
	if (ImGui::SliderFloat("Animation Duration (seconds)", &config.AnimationDuration, ImGuiToggleConstants::AnimationDurationMinimum, 2.0f))
	{
		// if the user adjusted the animation duration slider, go ahead and turn on the animation flags.
		config.Flags |= ImGuiToggleFlags_Animated;
	}

	// frame rounding sets how round the frame is when drawn, where 0 is a rectangle, and 1 is a circle.
	ImGui::SliderFloat("Frame Rounding (scale)", &config.FrameRounding, ImGuiToggleConstants::FrameRoundingMinimum, ImGuiToggleConstants::FrameRoundingMaximum);

	// knob rounding sets how round the knob is when drawn, where 0 is a rectangle, and 1 is a circle.
	ImGui::SliderFloat("Knob Rounding (scale)", &config.KnobRounding, ImGuiToggleConstants::KnobRoundingMinimum, ImGuiToggleConstants::KnobRoundingMaximum);

	// size controls the width and the height of the toggle frame
	ImGui::SliderFloat2("Size (px: w, h)", &config.Size.x, 0.0f, 200.0f, "%.0f");

	// width ratio sets how wide the toggle is with relation to the frame height. if Size is non-zero, this is unused.
	ImGui::SliderFloat("Width Ratio (scale)", &config.WidthRatio, ImGuiToggleConstants::WidthRatioMinimum, ImGuiToggleConstants::WidthRatioMaximum);

	// a11y style sets the type of additional on/off indicator drawing
	if (ImGui::Combo("A11y Style", &config.A11yStyle,
		"Label\0"
		"Glyph\0"
		"Dot\0"
		"\0"))
	{
		// if the user adjusted the a11y style combo, go ahead and turn on the a11y flag.
		config.Flags |= ImGuiToggleFlags_A11y;
	}

	// some tabs to adjust the "state" settings of the toggle (configuration dependent on if the toggle is on or off.)
	if (ImGui::BeginTabBar("State"))
	{
		if (ImGui::BeginTabItem("\"Off State\" Settings"))
		{
			imgui_toggle_state(config, config.Off);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("\"On State\"Settings"))
		{
			imgui_toggle_state(config, config.On);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::Separator();

	// flags for various toggle features
	ImGui::Text("Flags");
	ImGui::Columns(2);
	ImGui::Text("Meta Flags");
	ImGui::NextColumn();
	ImGui::Text("Individual Flags");
	ImGui::Separator();
	ImGui::NextColumn();

	// should the toggle have borders (sets all border flags)
	ImGui::CheckboxFlags("Bordered", &config.Flags, ImGuiToggleFlags_Bordered);

	// should the toggle have shadows (sets all shadow flags)
	ImGui::CheckboxFlags("Shadowed", &config.Flags, ImGuiToggleFlags_Shadowed);

	ImGui::NextColumn();

	// should the toggle animate
	ImGui::CheckboxFlags("Animated", &config.Flags, ImGuiToggleFlags_Animated);

	// should the toggle have a bordered frame
	ImGui::CheckboxFlags("BorderedFrame", &config.Flags, ImGuiToggleFlags_BorderedFrame);

	// should the toggle have a bordered knob
	ImGui::CheckboxFlags("BorderedKnob", &config.Flags, ImGuiToggleFlags_BorderedKnob);

	// should the toggle have a shadowed frame
	ImGui::CheckboxFlags("ShadowedFrame", &config.Flags, ImGuiToggleFlags_ShadowedFrame);

	// should the toggle have a shadowed knob
	ImGui::CheckboxFlags("ShadowedKnob", &config.Flags, ImGuiToggleFlags_ShadowedKnob);

	// should the toggle draw a11y glyphs
	ImGui::CheckboxFlags("A11y", &config.Flags, ImGuiToggleFlags_A11y);
	ImGui::Columns();

	ImGui::Separator();

	// what follows are some configuration presets. check the source of those functions to see how they work.
	ImGui::Text("Configuration Style Presets");

	if (ImGui::Button("Reset to Default"))
	{
		config = ImGuiTogglePresets::DefaultStyle();
	}
	ImGui::SameLine();

	if (ImGui::Button("Rectangle"))
	{
		config = ImGuiTogglePresets::RectangleStyle();
	}
	ImGui::SameLine();

	if (ImGui::Button("Glowing"))
	{
		config = ImGuiTogglePresets::GlowingStyle();
	}
	ImGui::SameLine();

	if (ImGui::Button("iOS"))
	{
		config = ImGuiTogglePresets::iOSStyle();
	}
	ImGui::SameLine();

	if (ImGui::Button("Material"))
	{
		config = ImGuiTogglePresets::MaterialStyle();
	}
	ImGui::SameLine();

	if (ImGui::Button("Minecraft"))
	{
		config = ImGuiTogglePresets::MinecraftStyle();
	}
}

static void imgui_toggle_state(const ImGuiToggleConfig& config, ImGuiToggleStateConfig& state)
{
	// some values to use for slider limits
	const float border_thickness_max_pixels = 50.0f;
	const float max_height = config.Size.y > 0 ? config.Size.y : ImGui::GetFrameHeight();
	const float half_max_height = max_height * 0.5f;

	// knob offset controls how far into or out of the frame the knob should draw.
	ImGui::SliderFloat2("Knob Offset (px: x, y)", &state.KnobOffset.x, -half_max_height, half_max_height);

	// knob inset controls how many pixels the knob is set into the frame. negative values will cause it to grow outside the frame.
	// for circular knobs, we will just use a single value, while for we will use top/left/bottom/right offsets.
	const bool is_rounded = config.KnobRounding >= 1.0f;
	if (is_rounded)
	{
		float inset_average = state.KnobInset.GetAverage();
		ImGui::SliderFloat("Knob Inset (px)", &inset_average, -half_max_height, half_max_height);
		state.KnobInset = inset_average;
	}
	else
	{
		ImGui::SliderFloat4("Knob Inset (px: t, l, b, r)", state.KnobInset.Offsets, -half_max_height, half_max_height);
	}

	// how thick should the frame border be (if enabled)
	ImGui::SliderFloat("Frame Border Thickness (px)", &state.FrameBorderThickness, 0.0f, border_thickness_max_pixels);

	// how thick should the knob border be (if enabled)
	ImGui::SliderFloat("Knob Border Thickness (px)", &state.KnobBorderThickness, 0.0f, border_thickness_max_pixels);
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void imgui_toggle_demo(void){
  ImGui::Begin("ImGui_Toggle demo");                          // Create a window called "Hello, world!" and append into it.
  imgui_toggle_example();
  ImGui::End();
}


// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        imgui_toggle_demo();

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

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
