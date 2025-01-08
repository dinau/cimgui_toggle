#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "cimgui_impl.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>

#include "cimgui_toggle.h"
#include "cimgui_toggle_presets.h"
#include "setupFonts.h"
#include "themeGold.h"

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

#define igToggle     Toggle
#define igToggleAnim ToggleAnim

GLFWwindow *window;

static void imgui_toggle_custom(void);
static void imgui_toggle_state(const ImGuiToggleConfig config, ImGuiToggleStateConfig *state);

static void imgui_toggle_simple()
{
	static bool values[] = { true, true, true, true, true, true, true, true };
	size_t value_index = 0;

	const ImVec4 green = {0.16f, 0.66f, 0.45f, 1.0f};
	const ImVec4 green_hover = {0.0f, 1.0f, 0.57f, 1.0f};
	//const ImVec4 salmon = {1.0f, 0.43f, 0.35f, 1.0f};
	const ImVec4 green_shadow = {0.0f, 1.0f, 0.0f, 0.4f};

  ImVec2 sz = {40.0, 20.0};
	// a default and default animated toggle
	Toggle("Default Toggle", &values[value_index++], sz);
	ToggleAnim("Animated Toggle", &values[value_index++], ImGuiToggleFlags_Animated, 1.0f,  sz);

	// this toggle draws a simple border around it's frame and knob
	ToggleAnim("Bordered Knob", &values[value_index++], ImGuiToggleFlags_Bordered, 1.0f, sz);

	// this toggle draws a simple shadow around it's frame and knob
	igPushStyleColor_Vec4(ImGuiCol_BorderShadow, green_shadow);
	ToggleAnim("Shadowed Knob", &values[value_index++], ImGuiToggleFlags_Shadowed, 1.0f, sz);

	// this toggle draws the shadow & and the border around it's frame and knob.
	ToggleAnim("Bordered + Shadowed Knob", &values[value_index++], ImGuiToggleFlags_Bordered | ImGuiToggleFlags_Shadowed, 1.0f, sz);
	igPopStyleColor(1);

	// this toggle uses stack-pushed style colors to change the way it displays
	igPushStyleColor_Vec4(ImGuiCol_Button, green);
	igPushStyleColor_Vec4(ImGuiCol_ButtonHovered, green_hover);
	Toggle("Green Toggle", &values[value_index++], sz);
	igPopStyleColor(2);

	ToggleFlag("Toggle with A11y Labels", &values[value_index++], ImGuiToggleFlags_A11y, sz);

	// this toggle shows no label
	Toggle("##Toggle With Hidden Label", &values[value_index++], sz);
}

static void imgui_toggle_example() {
	// use some lovely gray backgrounds for "off" toggles
	// the default would otherwise use your theme's frame background colors.
  ImVec4 colbg = {0.45f, 0.45f, 0.45f, 1.0f};
	igPushStyleColor_Vec4(ImGuiCol_FrameBg, colbg);
  ImVec4 colHovered = {0.65f, 0.65f, 0.65f, 1.0f};
	igPushStyleColor_Vec4(ImGuiCol_FrameBgHovered, colHovered);

	// a toggle that will allow the user to view the demo for simple toggles or a custom toggle
	static bool show_custom_toggle = false;
  ImVec2 sz = {40.0f, 20.0f};
	Toggle( show_custom_toggle ? "Showing Custom Toggle" : "Showing Simple Toggles" , &show_custom_toggle, sz);
	igSeparator();

	if (show_custom_toggle) {
		imgui_toggle_custom();
	} else {
		imgui_toggle_simple();
	}
	// pop the color styles
	igPopStyleColor(2);
}

static void imgui_toggle_custom(void) {
	static bool toggle_custom = true;
	static ImGuiToggleConfig config;
  static bool fInitReq = true;
  if( fInitReq) {
    fInitReq = false;
    ImGuiToggleConfig_init(&config);
  }

	igNewLine();
	ToggleCfg("Customized Toggle", &toggle_custom, config);

	igNewLine();

	// these first settings are used no matter the toggle's state.
	igText("Persistent Toggle Settings");

	// animation duration controls how long the toggle animates, in seconds. if set to 0, animation is disabled.
	if (igSliderFloat("Animation Duration (seconds)", &config.AnimationDuration, ImGuiToggleConstants_AnimationDurationMinimum, 2.0f, "%.3f", 0))
	{
		// if the user adjusted the animation duration slider, go ahead and turn on the animation flags.
		config.Flags |= ImGuiToggleFlags_Animated;
	}

	// frame rounding sets how round the frame is when drawn, where 0 is a rectangle, and 1 is a circle.
	igSliderFloat("Frame Rounding (scale)", &config.FrameRounding, ImGuiToggleConstants_FrameRoundingMinimum, ImGuiToggleConstants_FrameRoundingMaximum, "%.3f", 0);

	// knob rounding sets how round the knob is when drawn, where 0 is a rectangle, and 1 is a circle.
	igSliderFloat("Knob Rounding (scale)", &config.KnobRounding, ImGuiToggleConstants_KnobRoundingMinimum, ImGuiToggleConstants_KnobRoundingMaximum, "%.3f", 0);

	// size controls the width and the height of the toggle frame
//igSliderFloat2(const char* label, float v[2]    ,float v_min, float v_max,const char* format,ImGuiSliderFlags flags);
	igSliderFloat2("Size (px: w, h)", (float *)&config.Size, 0.0f, 200.0f, "%.0f",0);

	// width ratio sets how wide the toggle is with relation to the frame height. if Size is non-zero, this is unused.
	igSliderFloat("Width Ratio (scale)", &config.WidthRatio, ImGuiToggleConstants_WidthRatioMinimum, ImGuiToggleConstants_WidthRatioMaximum, "%.3f", 0);

	// a11y style sets the type of additional on/off indicator drawing
	if (igCombo_Str("A11y Style", &config.A11yStyle,
		"Label\0"
		"Glyph\0"
		"Dot\0"
		"\0", -1))
	{
		// if the user adjusted the a11y style combo, go ahead and turn on the a11y flag.
		config.Flags |= ImGuiToggleFlags_A11y;
	}

	// some tabs to adjust the "state" settings of the toggle (configuration dependent on if the toggle is on or off.)
	if (igBeginTabBar("State", 0))
	{
		if (igBeginTabItem("\"Off State\" Settings", NULL, 0))
		{
			imgui_toggle_state(config, &config.Off);
			igEndTabItem();
		}

		if (igBeginTabItem("\"On State\"Settings", NULL, 0))
		{
			imgui_toggle_state(config, &config.On);
			igEndTabItem();
		}

		igEndTabBar();
	}

	igSeparator();

	// flags for various toggle features
	igText("Flags");
	igColumns(2, NULL, true);
	igText("Meta Flags");
	igNextColumn();
	igText("Individual Flags");
	igSeparator();
	igNextColumn();

	// should the toggle have borders (sets all border flags)
	igCheckboxFlags_IntPtr("Bordered", &config.Flags, ImGuiToggleFlags_Bordered);

	// should the toggle have shadows (sets all shadow flags)
	igCheckboxFlags_IntPtr("Shadowed", &config.Flags, ImGuiToggleFlags_Shadowed);

	igNextColumn();

	// should the toggle animate
	igCheckboxFlags_IntPtr("Animated", &config.Flags, ImGuiToggleFlags_Animated);

	// should the toggle have a bordered frame
	igCheckboxFlags_IntPtr("BorderedFrame", &config.Flags, ImGuiToggleFlags_BorderedFrame);

	// should the toggle have a bordered knob
	igCheckboxFlags_IntPtr("BorderedKnob", &config.Flags, ImGuiToggleFlags_BorderedKnob);

	// should the toggle have a shadowed frame
	igCheckboxFlags_IntPtr("ShadowedFrame", &config.Flags, ImGuiToggleFlags_ShadowedFrame);

	// should the toggle have a shadowed knob
	igCheckboxFlags_IntPtr("ShadowedKnob", &config.Flags, ImGuiToggleFlags_ShadowedKnob);

	// should the toggle draw a11y glyphs
	igCheckboxFlags_IntPtr("A11y", &config.Flags, ImGuiToggleFlags_A11y);
	igColumns(2, NULL, true);

	igSeparator();

	// what follows are some configuration presets. check the source of those functions to see how they work.
	igText("Configuration Style Presets");
  ImVec2 bsz = {0,0};
	if (igButton("Reset to Default", bsz))
	{
		config = ImGuiTogglePresets_DefaultStyle();
	}
	igSameLine(0.0f, -1.0f);

	if (igButton("Rectangle", bsz))
	{
		config = ImGuiTogglePresets_RectangleStyle();
	}
	igSameLine(0.0f, -1.0f);

	if (igButton("Glowing", bsz))
	{
		config = ImGuiTogglePresets_GlowingStyle();
	}
	igSameLine(0.0f, -1.0f);

	if (igButton("iOS", bsz))
	{
		config = ImGuiTogglePresets_iOSStyle(1.0f, false);
	}
	igSameLine(0.0f, -1.0f);

	if (igButton("Material", bsz))
	{
		config = ImGuiTogglePresets_MaterialStyle(1.0f);
	}
	igSameLine(0.0f, -1.0f);

	if (igButton("Minecraft", bsz))
	{
		config = ImGuiTogglePresets_MinecraftStyle(1.0f);
	}
}

// IMGUI_API  bool   SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
// CIMGUI_API bool igSliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format, ImGuiSliderFlags flags);
//
static void imgui_toggle_state(const ImGuiToggleConfig config, ImGuiToggleStateConfig *state) {
	// some values to use for slider limits
	const float border_thickness_max_pixels = 50.0f;
	const float max_height = config.Size.y > 0 ? config.Size.y : igGetFrameHeight();
	const float half_max_height = max_height * 0.5f;

	// knob offset controls how far into or out of the frame the knob should draw.
	igSliderFloat2("Knob Offset (px: x, y)", (float *)&state->KnobOffset, -half_max_height, half_max_height, "%.3f", 0);

	// knob inset controls how many pixels the knob is set into the frame. negative values will cause it to grow outside the frame.
	// for circular knobs, we will just use a single value, while for we will use top/left/bottom/right offsets.
	const bool is_rounded = config.KnobRounding >= 1.0f;
	if (is_rounded)
	{
		float inset_average = ImOffsetRect_GetAverage(&state->KnobInset);
		igSliderFloat("Knob Inset (px)", &inset_average, -half_max_height, half_max_height, "%.3f", 0);
		state->KnobInset.Top = inset_average;
		state->KnobInset.Left = inset_average;
		state->KnobInset.Right = inset_average;
		state->KnobInset.Bottom = inset_average;
	}
	else
	{
		igSliderFloat4("Knob Inset (px: t, l, b, r)", state->KnobInset.Offsets, -half_max_height, half_max_height, "%.3f", 0);
	}

	// how thick should the frame border be (if enabled)
	igSliderFloat("Frame Border Thickness (px)", &state->FrameBorderThickness, 0.0f, border_thickness_max_pixels, "%.3f", 0);

	// how thick should the knob border be (if enabled)
	igSliderFloat("Knob Border Thickness (px)", &state->KnobBorderThickness, 0.0f, border_thickness_max_pixels, "%.3f", 0);
}

int main(int argc, char *argv[])
{

  if (!glfwInit())
    return -1;

  // Decide GL+GLSL versions
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

#if __APPLE__
  // GL 3.2 Core + GLSL 150
  const char *glsl_version = "#version 150";
#else
  // GL 3.2 + GLSL 130
  const char *glsl_version = "#version 130";
#endif

  // just an extra window hint for resize
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window = glfwCreateWindow(1024, 900, "ImGui_Toggle with C language", NULL, NULL);
  if (!window)
  {
    printf("Failed to create window! Terminating!\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // enable vsync
  glfwSwapInterval(1);

  // check opengl version sdl uses
  printf("opengl version: %s\n", (char *)glGetString(GL_VERSION));

  // setup imgui
  igCreateContext(NULL);

  // set docking
  ImGuiIO *ioptr = igGetIO();
  ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
  //ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
#undef IMGUI_HAS_DOCK
#ifdef IMGUI_HAS_DOCK
  ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
  ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
#endif

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  themeGold();
  //igStyleColorsClassic(NULL);
  //igStyleColorsLight(NULL);
  // ImFontAtlas_AddFontDefault(io.Fonts, NULL);
  //
  setupFonts();

  bool showDemoWindow = true;
  ImVec4 clearColor;
  clearColor.x = 0.45f;
  clearColor.y = 0.55f;
  clearColor.z = 0.60f;
  clearColor.w = 1.00f;

  // main event loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // start imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    if (showDemoWindow)
      igShowDemoWindow(&showDemoWindow);

    // show a simple window that we created ourselves.
    {
      igBegin("Toggle sw demo", NULL, 0);

      imgui_toggle_example();
      igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);

      igEnd();
    }

    // render
    igRender();
    glfwMakeContextCurrent(window);
    glViewport(0, 0, (int)ioptr->DisplaySize.x, (int)ioptr->DisplaySize.y);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
#ifdef IMGUI_HAS_DOCK
    if (ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow *backup_current_window = glfwGetCurrentContext();
      igUpdatePlatformWindows();
      igRenderPlatformWindowsDefault(NULL, NULL);
      glfwMakeContextCurrent(backup_current_window);
    }
#endif
    glfwSwapBuffers(window);
  }

  // clean up
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(NULL);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
