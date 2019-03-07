#include "ImguiSetupState.h"

BBIT::ImguiSetupState::ImguiSetupState()
{
	window_flags |= ImGuiWindowFlags_MenuBar;
	mainMenuBarVisible = true;
	show_app_metrics = false;
	show_app_style_editor = false;
	show_app_about = false;
	userInputPanelVisible = true;
	fileWindowVisible = false;
}
BBIT::ImguiSetupState::~ImguiSetupState()
{
}

/*
	Displays, each frame, the elements of the UI that are enabled.
	By default, the menu bar and input panels should be visible, while
	all of the core ImGui functions should be disabled by default in
	release (no use to end user).
*/
void BBIT::ImguiSetupState::showUI(bool* p_open)
{
	if (mainMenuBarVisible) showMainMenuBar(&mainMenuBarVisible);
	if (userInputPanelVisible) showUserInputPanel(&userInputPanelVisible);
	if (fileWindowVisible) showFileWindow(&fileWindowVisible);

	// Dear Imgui fncts
	if (show_app_metrics) { ImGui::ShowMetricsWindow(&show_app_metrics); }
	if (show_app_style_editor) { ImGui::Begin("Style Editor", &show_app_style_editor); ImGui::ShowStyleEditor(); ImGui::End(); }
	if (show_app_about)
	{
		ImGui::Begin("About Dear ImGui", &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Dear ImGui, %s", ImGui::GetVersion());
		ImGui::Separator();
		ImGui::Text("By Omar Cornut and all dear imgui contributors.");
		ImGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");
		ImGui::End();
	}
}
/*
	This is the content information for the main menu bar (the
	one that runs across the top of the screen. This may end up
	being invisible to the end user upon release. The user-purposed
	functions would then be placed in the user input panel (the floating
	panel to the right of the subject's image).
*/
void BBIT::ImguiSetupState::showMainMenuBar(bool* p_open)
{
	if (ImGui::BeginMainMenuBar()) // 
	{
		if (ImGui::BeginMenu("File"))
		{
			// Separated into its own function for readability
			showMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Reset", "CTRL+Z", false, false)) {} // TODO
			ImGui::EndMenu();
		}
		// TODO: Disable for release, replace with something for
		//       the researcher (mostly style options).
		if (ImGui::BeginMenu("Development Options"))
		{
			/*
				Shows version, frame time, FPS, geometry data,
				and window information (primarily counts). Useful
				for debugging rogue windows.
			*/
			if (ImGui::MenuItem("App Metrics", NULL, false))
			{
				show_app_metrics = true;
			}
			/*
				Change the style of the interface (fonts, colors,
				spacing, etc.). By default, this should be the light
				theme.

				TODO: Implement a more user-friendly scheme. The
				default ImGui fonts/styles are a little too much
				geared towards developers.
			*/
			if (ImGui::MenuItem("Style Editor", NULL, false))
			{
				show_app_style_editor = true;
			}
			/*
				In-program credits for the interface. Good work
				should always be credited :)
			*/
			if (ImGui::MenuItem("About..", NULL, false))
			{
				show_app_about = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
/*
	This is the "File" part of the main menu bar (the one that
	runs across the top of the screen). It is separated here for
	readability purposes only.
*/
void BBIT::ImguiSetupState::showMenuFile()
{
	ImGui::MenuItem("File Window", NULL, &fileWindowVisible);
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		// Open or close the user input panel
		ImGui::MenuItem("<<User Input Panel>>", "", &userInputPanelVisible);
		ImGui::EndMenu();
	}

	if (ImGui::MenuItem("Quit", "Esc")) 
	{ 
		shutdownState = true; 
	}
}

void BBIT::ImguiSetupState::showUserInputPanel(bool * p_open)
{
	if (!ImGui::Begin("", p_open, setup_panel_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
		
	ImGui::Text("Instructions");
	ImGui::Text("------------");
	ImGui::Text("1: Press 'Z' to toggle triangle visibility");
	ImGui::Text("2: Place markers as indicated on the image below");
	ImGui::Text("   to the nearest possible vertex.");
	ImGui::Text("3: Click \"Finish Setup\" when complete.");

	ImGui::Separator();
	// TODO: Image Display here
	ImGui::Separator();

	if (ImGui::Button("Finish Setup")) { ImGui::OpenPopup("Proceed?"); }
	if (ImGui::BeginPopupModal("Proceed?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you wish to proceed to the manipulation stage?");
		if (ImGui::Button("Yes", ImVec2(200, 0)))
		{
			this->setupComplete = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(200, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();

	}
	ImGui::End();
}
void BBIT::ImguiSetupState::showFileWindow(bool* p_open)
{
	if (!ImGui::Begin("File", p_open, setup_panel_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	/*
	if (ImGui::Button("Load New Image"))
	{
		/*
			Not functional here using typical methods.

			See: https://github.com/ocornut/imgui/issues/331
			TODO: Implement a fix
		*/
	/*
		ImGui::OpenPopup("Load");
		if (ImGui::BeginPopupModal("Load", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Warning: This operation will clear all changes.");
			
		}
		
	}
	*/

	static char path[128] = "../imagens/<replace with image (e.g. arnold.png)>";
	static int i0 = 123;
	ImGui::InputText("", path, IM_ARRAYSIZE(path));
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{

	}
}


