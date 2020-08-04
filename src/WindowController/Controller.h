#pragma once
#include "DearImGui.h"
#include "GlobalVariables.h"

namespace Controller
{
    // Our state
	static bool grid_on = true;
	static int polygon_mode = 1;
    static bool show_demo_window = false;
    static bool show_another_window = false;
	static bool show_add_bridge_window = false;
	static bool show_edit_bridge_parameters = false;
	static bool show_add_road_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    inline void ShowDemo()
    {
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
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

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

    inline void MainMenu(GLFWwindow* window)
    {
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "N"))
				{
					solids.clear();
					ourMesh_List.clear();
					bridges.clear();
				}
				/*if (ImGui::MenuItem("Open", NULL, false, false)) {}
				if (ImGui::MenuItem("Save", NULL, false, false)) {}
				if (ImGui::MenuItem("Import", NULL, false, false)) {}
				if (ImGui::BeginMenu("Export"))
				{
					if (ImGui::MenuItem("Wavefront (.obj)", NULL, false, false)) {}
					if (ImGui::MenuItem("Half-edge (.hed)", NULL, false, false)) {}
					ImGui::EndMenu();
				}*/
				ImGui::Separator();
				//if (ImGui::MenuItem("Properties", "P"))
				//{
				//	/*if (show_app_property_editor)
				//		show_app_property_editor = false;
				//	else show_app_property_editor = true;*/
				//}
				if (ImGui::MenuItem("Quit", "ESC")) { glfwSetWindowShouldClose(window, true); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				ImGui::Checkbox("Grid", &grid_on);
				if (ImGui::MenuItem("Zoom Extents", NULL, false, false)) {}
				ImGui::Separator();
				if (ImGui::BeginMenu("3D Views"))
				{
					if (ImGui::MenuItem("Top", NULL, false, false))
					{
						/*float d = glm::length(camera.View - camera.Position);
						camera.Position = camera.WorldUp * d;
						camera.Up = glm::vec3(0.0f, 0.0f, -1.0f);
						camera.update3DView();*/
					}
					if (ImGui::MenuItem("Bottom", NULL, false, false)) {}
					if (ImGui::MenuItem("Left", NULL, false, false)) {}
					if (ImGui::MenuItem("Right", NULL, false, false)) {}
					if (ImGui::MenuItem("Front", NULL, false, false)) {}
					if (ImGui::MenuItem("Back", NULL, false, false)) {}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Visual Styles"))
				{
					ImGui::RadioButton("Wireframe", &polygon_mode, 0);
					ImGui::RadioButton("Solid", &polygon_mode, 1);
					//ImGui::RadioButton("Textured", &polygon_mode, 2);
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Road"))
			{
				if (ImGui::MenuItem("Alignment"))
				{
					/*if (show_alignment_editor)
						show_alignment_editor = false;
					else show_alignment_editor = true;*/
				}
				if (ImGui::MenuItem("Add Road"))
				{
					if (!show_add_road_window)
						show_add_road_window = true;
				}
				if (ImGui::MenuItem("Edit Road"))
				{
					/*if (show_section_data_window)
						show_section_data_window = false;
					else show_section_data_window = true;*/
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Bridge"))
			{
				if (ImGui::MenuItem("Add Bridge"))
				{
					if (!show_add_bridge_window)
						show_add_bridge_window = true;
				}
				if (ImGui::BeginMenu("Edit Bridge"))
				{
					if (ImGui::MenuItem("General parameters"))
					{
						if (!show_edit_bridge_parameters)
							show_edit_bridge_parameters = true;
					}
					if (ImGui::MenuItem("Cross-section", NULL, false, false)) {}
					if (ImGui::MenuItem("Columns", NULL, false, false)) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Show add bridge window
		if (show_add_bridge_window)
		{
			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(275, 220), ImGuiCond_Once);
			ImGui::Begin("New Bridge", &show_add_bridge_window, ImGuiWindowFlags_NoResize);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 160.0f);

			// BRIDGE NAME

			ImGui::PushID(901);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Bridge name");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(100);
			ImGui::InputText("", &bridgeName, 0, NULL, NULL);
			ImGui::NextColumn();
			ImGui::PopID();

			// BRIDGE TYPE

			ImGui::PushID(902);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Bridge type");
			ImGui::NextColumn();

			static bool* typeSelected = new bool[bridgeTypes.size()];
			static std::string CurrentBridgeName = "Box Girder";
			static int CurrentBridgeType;

			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("", CurrentBridgeName.c_str())) {
				for (int i = 0; i < bridgeTypes.size(); i++) {
					if (ImGui::Selectable(bridgeTypes[i].c_str(), &typeSelected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
						CurrentBridgeName = bridgeTypes[i].c_str();
						CurrentBridgeType = i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::NextColumn();
			ImGui::PopID();

			// ROADWAY

			ImGui::PushID(903);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Roadway");
			ImGui::NextColumn();

			static bool* selected = new bool[roadways.size()];
			static std::string CurrentRoadName = " ";
			static int CurrentRoad;

			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("", CurrentRoadName.c_str())) {
				for (int i = 0; i < roadways.size(); i++) {
					if (ImGui::Selectable(roadways[i]->name.c_str(), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
						CurrentRoadName = roadways[i]->name.c_str();
						CurrentRoad = i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::NextColumn();
			ImGui::PopID();

			// CROSS STATION

			ImGui::PushID(904);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Cross station");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(80);
			ImGui::DragFloat("m", &cross_station, 0.01f, 0.00f, 1000.00f, "%.2f");
			ImGui::NextColumn();
			ImGui::PopID();

			// HORIZONTAL CLEARANCE

			ImGui::PushID(905);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Horizontal clearance");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(80);
			ImGui::DragFloat("m", &h_clearance, 0.01f, 0.00f, 1000.00f, "%.2f");
			ImGui::NextColumn();
			ImGui::PopID();

			// VERTICAL CLEARANCE

			ImGui::PushID(906);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Vertical clearance");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(80);
			ImGui::DragFloat("m", &v_clearance, 0.01f, 0.00f, 1000.00f, "%.2f");
			ImGui::NextColumn();
			ImGui::PopID();

			// MAIN SPAN

			ImGui::PushID(907);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Main span");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(80);
			ImGui::DragFloat("m", &main_span, 0.01f, 0.00f, 1000.00f, "%.2f");
			ImGui::NextColumn();
			ImGui::PopID();

			// BUTTONS

			ImGui::Separator();
			ImGui::Columns(1);
			if (ImGui::Button("   OK   ")) {
				if (CurrentBridgeType == 0) {
					bridges.push_back(new BoxGirder(bridgeName, roadways[CurrentRoad], cross_station, v_clearance, h_clearance, main_span));
					for (int i = 0; i < bridges.back()->model.size(); i++)
						ourMesh_List.push_back(Mesh(bridges.back()->model[i]));
					show_add_bridge_window = false;
				}
			}
			ImGui::SameLine(205);
			if (ImGui::Button(" Cancel "))
				show_add_bridge_window = false;
			
			// END
			
			ImGui::End();
		}

		// Show add road window
		if (show_add_road_window)
		{
			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(235, 150), ImGuiCond_Once);
			ImGui::Begin("New Road", &show_add_road_window, ImGuiWindowFlags_NoResize);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 120.0f);

			// ROAD NAME

			ImGui::PushID(901);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Road name");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(100);
			ImGui::InputText("", &roadName, 0, NULL, NULL);
			ImGui::NextColumn();
			ImGui::PopID();

			// ALIGNMENT

			ImGui::PushID(902);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Alignment");
			ImGui::NextColumn();

			static bool* selected = new bool[alignments.size()];
			static std::string CurrentAlignmentName = " ";
			static int CurrentAlignment;

			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("", CurrentAlignmentName.c_str())) {
				for (int i = 0; i < alignments.size(); i++) {
					if (ImGui::Selectable(alignments[i]->name.c_str(), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
						CurrentAlignmentName = alignments[i]->name.c_str();
						CurrentAlignment = i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::NextColumn();
			ImGui::PopID();

			// ROAD WIDTH

			ImGui::PushID(903);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Road width");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(60);
			ImGui::DragFloat("m", &roadWidth, 0.01f, 0.00f, 1000.00f, "%.2f");
			ImGui::NextColumn();
			ImGui::PopID();

			// DESIGN SPEED

			ImGui::PushID(904);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Design speed");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(60);
			ImGui::DragFloat("km/h", &dSpeed, 0.01f, 0.00f, 1000.00f, "%.2f");
			ImGui::NextColumn();
			ImGui::PopID();

			// BUTTONS

			ImGui::Separator();
			ImGui::Columns(1);
			if (ImGui::Button("   OK   ")) {
				roadways.push_back(new Road(roadName, roadWidth, dSpeed, alignments[CurrentAlignment]));
				show_add_road_window = false;
			}
			ImGui::SameLine(165);
			if (ImGui::Button(" Cancel "))
				show_add_road_window = false;

			// END

			ImGui::End();
		}
    
		// Show edit bridge parameters
		if (show_edit_bridge_parameters)
		{
			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(275, 220), ImGuiCond_Once);
			ImGui::Begin("General parameters", &show_edit_bridge_parameters, ImGuiWindowFlags_NoResize);

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 160.0f);

			// BRIDGE

			ImGui::PushID(901);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Bridge");
			ImGui::NextColumn();

			static bool* selected = new bool[bridges.size()];
			static std::string CurrentBridgeName = " ";
			static int CurrentBridge;
			
			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("", CurrentBridgeName.c_str())) {
				for (int i = 0; i < bridges.size(); i++) {
					if (ImGui::Selectable(bridges[i]->name.c_str(), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
						CurrentBridgeName = bridges[i]->name.c_str();
						CurrentBridge = i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::NextColumn();
			ImGui::PopID();

			// BRIDGE TYPE

			/*ImGui::PushID(902);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Bridge type");
			ImGui::NextColumn();

			static bool* typeSelected = new bool[bridgeTypes.size()];
			static std::string CurrentTypeName = bridges[CurrentBridge].
			static int CurrentType;

			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("", CurrentTypeName.c_str())) {
				for (int i = 0; i < bridgeTypes.size(); i++) {
					if (ImGui::Selectable(bridgeTypes[i].c_str(), &typeSelected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
						CurrentTypeName = bridgeTypes[i].c_str();
						CurrentType = i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::NextColumn();
			ImGui::PopID();*/

			// BUTTONS

			ImGui::Separator();
			ImGui::Columns(1);
			if (ImGui::Button(" Update ")) {
				/*bridges.push_back(new BoxGirder(bridgeName, roadways[CurrentRoad], cross_station, v_clearance, h_clearance, main_span));
				for (int i = 0; i < bridges.back()->model.size(); i++)
					ourMesh_List.push_back(Mesh(bridges.back()->model[i]));*/
				show_edit_bridge_parameters = false;
			}
			ImGui::SameLine(205);
			if (ImGui::Button(" Cancel "))
				show_edit_bridge_parameters = false;

			// END

			ImGui::End();
		}
	}
}
