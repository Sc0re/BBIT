#include "ImguiManipulationState.h"

ImguiManipulationState::ImguiManipulationState()
{
	window_flags |= ImGuiWindowFlags_MenuBar;
	manip_panel_flags |= ImGuiWindowFlags_NoCollapse;
	mainMenuBarVisible = true;
	userInputPanelVisible = true;
	inputDataVisible = true;
	show_app_metrics = false;
	show_app_style_editor = false;
	show_app_about = false;
}
ImguiManipulationState::~ImguiManipulationState()
{
}

void ImguiManipulationState::showUI(bool* p_open)
{
	if (mainMenuBarVisible) showMainMenuBar(&mainMenuBarVisible);
	if (userInputPanelVisible) showUserInputPanel(&userInputPanelVisible);

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
void ImguiManipulationState::showMainMenuBar(bool* p_open)
{
	static bool shown_once_already = false;
	if (!shown_once_already)
	{
		ImGui::OpenPopup("Begin");
	}

	if (ImGui::BeginPopupModal("Begin", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(u8"Bem vindo(a) ao nosso estudo sobre percepção corporal.");
		ImGui::Text(u8"Nesse estudo você será convidado(a) a responder como percebe o tamanho");
		ImGui::Text(u8"de partes do seu corpo. Nos próximos minutos, imagens do seu corpo serão ");
		ImGui::Text(u8"apresentadas e sua tarefa será estimar o tamanho de partes específicas do ");
		ImGui::Text(u8"corpo pression xando os botões \" + \" para aumentar o tamanho a região do corpo ");
		ImGui::Text(u8"selecionada e \" - \" para diminuir a região do corpo selecionada. Ao final, ");
		ImGui::Text(u8"quando sinalizado, você deverá indicar seu corpo desejado pressionando os ");
		ImGui::Text(u8"botões \" + \" e \" - \" até que atinja os tamanhos considerados por você como ideais.");
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			shown_once_already = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::EndPopup();
		
	}
	if (ImGui::BeginMainMenuBar()) // 
	{
		if (ImGui::BeginMenu("File"))
		{
			showMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z", false, false)) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Development Options"))
		{
			if (ImGui::MenuItem("App Metrics", NULL, false))
			{
				show_app_metrics = true;
			}
			if (ImGui::MenuItem("Style Editor", NULL, false))
			{
				show_app_style_editor = true;
			}
			if (ImGui::MenuItem("About..", NULL, false))
			{
				show_app_about = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
void ImguiManipulationState::showMenuFile()
{
	if (ImGui::MenuItem("New"))
	{

	} // TODO: Create fresh instance, clear all assets
	if (ImGui::MenuItem("Load", "Ctrl+O"))
	{

	} // TODO: Load in image file
	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{

	} // TODO: Save modification data to text file
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		// Open or close the user input panel
		ImGui::MenuItem("<<User Input Panel>>", "", &userInputPanelVisible);
		ImGui::EndMenu();
	}

	if (ImGui::MenuItem("Quit", "Esc")) { shutdownState = true; } // WIP
}
void ImguiManipulationState::showUserInputPanel(bool* p_open)
{
	if (!ImGui::Begin("", p_open, manip_panel_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	/*
		State Initialization
		--------------------
		Generates a random floating point number between -10 and 10. The resultant number
		does not need to be precise because it is truncated after two decimal places and
		will almost certainly be modified by the user during operation.
	*/
	static int8_t  shoulder_distance_i8_v = 0;
	static int8_t  chest_i8_v             = 0;
	static int8_t  waist_i8_v             = 0;
	static int8_t  hips_i8_v              = 0;
	static int8_t  upper_arms_i8_v        = 0;
	static int8_t  lower_arms_i8_v        = 0;
	static int8_t  thighs_i8_v            = 0;
	static int8_t  calves_i8_v            = 0;

	/*
		Original State Storage
		----------------------
		The randomly generated numbers are stored here as
		a means to maintain the definition of the original
		slider ranges, as well as provide a backup of the
		initial value in case the user wishes to reset it.
	*/
	static int8_t  O_shoulder_distance_i8_v = shoulder_distance_i8_v;
	static int8_t  O_chest_i8_v             = chest_i8_v;
	static int8_t  O_waist_i8_v             = waist_i8_v;
	static int8_t  O_hips_i8_v              = hips_i8_v;
	static int8_t  O_upper_arms_i8_v        = upper_arms_i8_v;
	static int8_t  O_lower_arms_i8_v        = lower_arms_i8_v;
	static int8_t  O_thighs_i8_v            = thighs_i8_v;
	static int8_t  O_calves_i8_v            = calves_i8_v;

	/*
		Range Modifier
		--------------
		Defines the minimum and maximum distortion percentage.
		e.g. if rangeModifier = 40, distortion can be from -40% to +40%.

		Range Restrainers
		-----------------
		Restrains the variables such that they will not go beyond the range.
	*/
	const int rangeModifier = 40;

	static std::vector<int8_t> valueRecords;
	static bool hasCreatedVector = false;
	if (!hasCreatedVector)
	{
		valueRecords.push_back(shoulder_distance_i8_v);
		valueRecords.push_back(chest_i8_v);
		valueRecords.push_back(waist_i8_v);
		valueRecords.push_back(hips_i8_v);
		valueRecords.push_back(upper_arms_i8_v);
		valueRecords.push_back(lower_arms_i8_v);
		valueRecords.push_back(thighs_i8_v);
		valueRecords.push_back(calves_i8_v);
		hasCreatedVector = true;
	}
	if (hasCreatedVector)
	{
		for (int8_t record : valueRecords)
		{
			
		}
	}
	
	/*
		Step Modifier
		-------------
		Defines the % at which each button press modifies (and thus distorts)
		the image.

		IMPORTANT: DO NOT SET TO DATA TYPES OTHER THAN AN INTEGER (8-bits)
	*/
	const int8_t stepModifier = 5;

	// Shoulder Distance
	if (ImGui::Button(" - ")) { shoulder_distance_i8_v -= stepModifier; }
	ImGui::SameLine();
	if (ImGui::Button(" + ")) { shoulder_distance_i8_v += stepModifier; }
	ImGui::SameLine();
	ImGui::Text("Shoulder Distance: %+d%%", shoulder_distance_i8_v);
	if (shoulder_distance_i8_v < -rangeModifier) { shoulder_distance_i8_v = -rangeModifier; }
	if (shoulder_distance_i8_v >  rangeModifier) { shoulder_distance_i8_v =  rangeModifier; }
	
	// Chest
	if (ImGui::Button(" - ")) { chest_i8_v -= stepModifier; }
	ImGui::SameLine();
	if (ImGui::Button(" + ")) { chest_i8_v += stepModifier; }
	ImGui::SameLine();
	ImGui::Text("Chest");
	if (chest_i8_v < -rangeModifier) { chest_i8_v = -rangeModifier; }
	if (chest_i8_v > rangeModifier)  { chest_i8_v =  rangeModifier; }
	
	// Waist
	if (ImGui::Button(" - ")) { waist_i8_v -= stepModifier; }
	ImGui::SameLine();
	if (ImGui::Button(" + ")) { waist_i8_v += stepModifier; }
	ImGui::SameLine();
	ImGui::Text("Waist");
	if (waist_i8_v < -rangeModifier) { waist_i8_v = -rangeModifier; }
	if (waist_i8_v > rangeModifier)  { waist_i8_v =  rangeModifier; }

	// Hips
	if (ImGui::Button(" - ")) { hips_i8_v -= stepModifier; }
	ImGui::SameLine();
	if (ImGui::Button(" + ")) { hips_i8_v += stepModifier; }
	ImGui::SameLine();
	ImGui::Text("Hips");
	if (hips_i8_v < -rangeModifier) { hips_i8_v = -rangeModifier; }
	if (hips_i8_v > rangeModifier)  { hips_i8_v =  rangeModifier; }
	
	// Upper Arms
	if (ImGui::Button(" - ")) { upper_arms_i8_v -= stepModifier; }
	ImGui::SameLine();
	if (ImGui::Button(" + ")) { upper_arms_i8_v += stepModifier; }
	ImGui::SameLine();
	ImGui::Text("Upper Arms");
	if (upper_arms_i8_v < -rangeModifier) { upper_arms_i8_v = -rangeModifier; }
	if (upper_arms_i8_v > rangeModifier)  { upper_arms_i8_v =  rangeModifier; }

	// Lower Arms
	if (ImGui::Button(" - ")) { lower_arms_i8_v -= stepModifier; }
	ImGui::SameLine();
	if (ImGui::Button(" + ")) { lower_arms_i8_v += stepModifier; }
	ImGui::SameLine();
	ImGui::Text("Lower Arms");
	if (lower_arms_i8_v < -rangeModifier) { lower_arms_i8_v = -rangeModifier; }
	if (lower_arms_i8_v > rangeModifier) {  lower_arms_i8_v =  rangeModifier; }

	// Thighs
	if (ImGui::Button(" - ")) { thighs_i8_v -= stepModifier; }
	ImGui::SameLine();
	if (ImGui::Button(" + ")) { thighs_i8_v += stepModifier; }
	ImGui::SameLine();
	ImGui::Text("Thighs");
	if (thighs_i8_v < -rangeModifier) { thighs_i8_v = -rangeModifier; }
	if (thighs_i8_v > rangeModifier)  { thighs_i8_v =  rangeModifier; }

	// Calves
	if (ImGui::Button(" - ")) { calves_i8_v -= stepModifier; }
	ImGui::SameLine();
	if (ImGui::Button(" + ")) { calves_i8_v += stepModifier; }
	ImGui::SameLine();
	ImGui::Text("Calves");
	if (calves_i8_v < -rangeModifier) { calves_i8_v = -rangeModifier; }
	if (calves_i8_v > rangeModifier)  { calves_i8_v =  rangeModifier; }

	/*
		Reset
		-----
		Resets all values to the original values. The original values are defined as the
		starting value after any randomization, as the randomization procedure sets the
		defaults. If further randomization is done, the newly generated number will be
		the default, and a reset will not restore previously generated random numbers.
	*/
	ImGui::Separator();
	static bool dont_ask_me_next_time = false;
	if (ImGui::Button("Reset All")) // Revert all values
	{
		if (!dont_ask_me_next_time)
		{
			ImGui::OpenPopup("Reset?");
		}
		else {
			shoulder_distance_i8_v = O_shoulder_distance_i8_v;
			chest_i8_v             = O_chest_i8_v;
			waist_i8_v             = O_waist_i8_v;
			hips_i8_v              = O_hips_i8_v;
			upper_arms_i8_v        = O_upper_arms_i8_v;
			lower_arms_i8_v        = O_lower_arms_i8_v;
			thighs_i8_v            = O_thighs_i8_v;
			calves_i8_v            = O_calves_i8_v;
		}
	}
	if (ImGui::BeginPopupModal("Reset?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("All values will be set to zero.\nThis operation cannot be undone!\n\n");
		ImGui::Separator();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
		ImGui::PopStyleVar();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			shoulder_distance_i8_v = O_shoulder_distance_i8_v;
			chest_i8_v             = O_chest_i8_v;
			waist_i8_v             = O_waist_i8_v;
			hips_i8_v              = O_hips_i8_v;
			upper_arms_i8_v        = O_upper_arms_i8_v;
			lower_arms_i8_v        = O_lower_arms_i8_v;
			thighs_i8_v            = O_thighs_i8_v;
			calves_i8_v            = O_calves_i8_v;

			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
		/*
		/*
			Randomize
			---------
			Randomizes all values. This will also resize the sliders so that the newly
			generated numbers are equidistant from their minima & maxima. It will also
			redefine the "original" value, so that resets will revert to the new initial
			randomly generated number.
		
		if (ImGui::Button("(Pseudo-)Randomize All")) // Randomize all values.
		{
			upper_torso_f32_v = (static_cast<float>(rand() % 2000) - 1000) / 100.0;
			lower_torso_f32_v = (static_cast<float>(rand() % 2000) - 1000) / 100.0;
			waist_f32_v = (static_cast<float>(rand() % 2000) - 1000) / 100.0;
			hips_f32_v = (static_cast<float>(rand() % 2000) - 1000) / 100.0;
			upper_arms_f32_v = (static_cast<float>(rand() % 2000) - 1000) / 100.0;
			lower_arms_f32_v = (static_cast<float>(rand() % 2000) - 1000) / 100.0;
			thighs_f32_v = (static_cast<float>(rand() % 2000) - 1000) / 100.0;
			calves_f32_v = (static_cast<float>(rand() % 2000) - 1000) / 100.0;

			O_upper_torso_f32_v = upper_torso_f32_v;
			O_lower_torso_f32_v = lower_torso_f32_v;
			O_waist_f32_v = waist_f32_v;
			O_hips_f32_v = hips_f32_v;
			O_upper_arms_f32_v = upper_arms_f32_v;
			O_lower_arms_f32_v = lower_arms_f32_v;
			O_thighs_f32_v = thighs_f32_v;
			O_calves_f32_v = calves_f32_v; 
			
		} 
		*/

	ImGui::End();
}

/*
	Adjusts the body part defined by its STEP_ID.
	Change is dependent on which button is pressed. (+/-)
*/
void BBIT::ImguiManipulationState::morphBodyPart(BBIT::STEP_ID id, int percentage)
{
}