#pragma once
#include "DearImGui.h"
#include "GlobalVariables.h"
#include "GlobalLists.h"

namespace Controller
{
    // Our state
	static bool grid_on = true;
	static int polygon_mode = 1;
    static bool show_demo_window = false;
    static bool show_another_window = false;
	static bool show_general_criteria_window = false;
	static bool show_add_bridge_window = false;
	static bool show_edit_bridge_parameters = false;
	static bool show_edit_bridge_section = false;
	static bool show_edit_bridge_columns = false;
	static bool show_add_road_window = false;
	static bool show_edit_road_parameters = false;
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
				if (ImGui::MenuItem("General Criteria"))
				{
					if (!show_general_criteria_window)
						show_general_criteria_window = true;
				}
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
				if (ImGui::MenuItem("Alignment", NULL, false, false))
				{
					/*if (show_alignment_editor)
						show_alignment_editor = false;
					else show_alignment_editor = true;*/
				}
				if (ImGui::MenuItem("Add Road"))
				{
					if (!show_add_road_window)
					{
						show_add_road_window = true;
						roadName = "Untitled";
						roadWidth = 8.0f;
						dSpeed = 40.0f;
					}
				}
				if (ImGui::MenuItem("Edit Road"))
				{
					if (show_edit_road_parameters)
						show_edit_road_parameters = false;
					else show_edit_road_parameters = true;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Bridge"))
			{
				if (ImGui::MenuItem("Add Bridge"))
				{
					if (!show_add_bridge_window)
					{
						show_add_bridge_window = true;
						bridgeName = "Untitled";
						cross_station = 250.0f;
						h_clearance = 60.0f;
					}
				}
				if (ImGui::BeginMenu("Edit Bridge"))
				{
					if (ImGui::MenuItem("General parameters"))
					{
						if (!show_edit_bridge_parameters)
						{
							show_edit_bridge_parameters = true;
						}
					}
					if (ImGui::MenuItem("Cross-section"))
					{
						if (!show_edit_bridge_section)
							show_edit_bridge_section = true;
					}
					if (ImGui::MenuItem("Columns"))
					{
						if (!show_edit_bridge_columns)
							show_edit_bridge_columns = true;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Show general criteria
		if (show_general_criteria_window)
		{
			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(275, 220), ImGuiCond_Once);
			ImGui::Begin("General Criteria", &show_general_criteria_window, ImGuiWindowFlags_NoResize);

			ImGui::Columns(2, NULL, false);
			ImGui::SetColumnWidth(0, 160.0f);

			// VERTICAL CLEARANCE

			ImGui::PushID(901);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Vertical clearance");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(80);
			ImGui::DragFloat("m", &v_clearance, 0.01f, 0.00f, 1000.00f, "%.2f");
			ImGui::NextColumn();
			ImGui::PopID();

			// MAIN SPAN

			ImGui::PushID(902);
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
			if (ImGui::Button("   OK   "))
				show_general_criteria_window = false;
			ImGui::SameLine(205);
			if (ImGui::Button(" Cancel "))
				show_general_criteria_window = false;

			// END

			ImGui::End();
		}

		// Show add road window
		if (show_add_road_window)
		{
			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(255, 175), ImGuiCond_Once);
			ImGui::Begin("New Road", &show_add_road_window, ImGuiWindowFlags_NoResize);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

			ImGui::Columns(2, NULL, false);
			ImGui::SetColumnWidth(0, 120.0f);

			// ROAD NAME

			ImGui::PushID(901);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Road name");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(120);
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

			ImGui::SetNextItemWidth(120);
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
			ImGui::DragFloat("m", &roadWidth, 0.01f, 0.0f, 50.0f, "%.2f");
			ImGui::NextColumn();
			ImGui::PopID();

			// DESIGN SPEED

			ImGui::PushID(904);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Design speed");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(60);
			ImGui::DragFloat("km/h", &dSpeed, 1.0f, 0.0f, 200.0f, "%.0f");
			ImGui::NextColumn();
			ImGui::PopID();

			// VEHICLE

			ImGui::PushID(905);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Design vehicle");
			ImGui::NextColumn();

			static bool* selectedVehicle = new bool[vehicles.size()];
			static std::string CurrentVehicleName = " ";
			static int CurrentVehicle;

			ImGui::SetNextItemWidth(120);
			if (ImGui::BeginCombo("", CurrentVehicleName.c_str())) {
				for (int i = 0; i < vehicles.size(); i++) {
					if (ImGui::Selectable(vehicles[i]->name.c_str(), &selectedVehicle[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
						CurrentVehicleName = vehicles[i]->name.c_str();
						CurrentVehicle = i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::NextColumn();
			ImGui::PopID();

			// BUTTONS

			ImGui::Separator();
			ImGui::Columns(1);
			if (ImGui::Button("   OK   ")) {
				roadways.push_back(new Road(roadName, roadWidth, dSpeed, alignments[CurrentAlignment], vehicles[CurrentVehicle]));
				show_add_road_window = false;
			}
			ImGui::SameLine(165);
			if (ImGui::Button(" Cancel "))
				show_add_road_window = false;

			// END

			ImGui::End();
		}

		// Show edit road parameters
		if (show_edit_road_parameters)
		{
			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(255, 180), ImGuiCond_Once);
			ImGui::Begin("Road parameters", &show_edit_road_parameters, ImGuiWindowFlags_NoResize);

			// ROAD

			ImGui::PushID(901);
			ImGui::AlignTextToFramePadding();

			static bool* roadSelected = new bool[roadways.size()];
			static std::string CurrentRoadName = " ";
			static std::string CurrentAlignmentName = " ";
			static std::string CurrentVehicleName = " ";
			static int CurrentRoad = -1;

			ImGui::SetNextItemWidth(210);
			if (ImGui::BeginCombo("Road", CurrentRoadName.c_str())) {
				for (int i = 0; i < roadways.size(); i++) {
					if (ImGui::Selectable(roadways[i]->name.c_str(), &roadSelected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
						CurrentRoadName = roadways[i]->name.c_str();
						CurrentRoad = i;
						CurrentAlignmentName = roadways[i]->alignment->name;
						CurrentVehicleName = roadways[i]->vehicle->name;
					}
				}
				ImGui::EndCombo();
			}
			//ImGui::NextColumn();
			ImGui::PopID();

			if (CurrentRoad >= 0)
			{
				ImGui::Separator();
				ImGui::Columns(2, NULL, false);
				ImGui::SetColumnWidth(0, 120.0f);

				// ALIGNMENT

				ImGui::PushID(902);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Alignment");
				ImGui::NextColumn();

				static bool* selected = new bool[alignments.size()];
				static int CurrentAlignment;

				ImGui::SetNextItemWidth(120);
				if (ImGui::BeginCombo("", CurrentAlignmentName.c_str())) {
					for (int i = 0; i < alignments.size(); i++) {
						if (ImGui::Selectable(alignments[i]->name.c_str(), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
							CurrentAlignmentName = alignments[i]->name.c_str();
							CurrentAlignment = i;
							roadways[CurrentRoad]->alignment = alignments[i];
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
				ImGui::DragFloat("m", &roadways[CurrentRoad]->width, 0.01f, 0.0f, 50.0f, "%.2f");
				ImGui::NextColumn();
				ImGui::PopID();

				// SPEED

				ImGui::PushID(904);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Design speed");
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(60);
				ImGui::DragFloat("km/h", &roadways[CurrentRoad]->speed, 1.0f, 0.0f, 200.00f, "%.0f");
				ImGui::NextColumn();
				ImGui::PopID();

				// VEHICLE

				ImGui::PushID(905);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Design vehicle");
				ImGui::NextColumn();

				static bool* vehicleSelected = new bool[vehicles.size()];
				static int CurrentVehicle;

				ImGui::SetNextItemWidth(120);
				if (ImGui::BeginCombo("", CurrentVehicleName.c_str())) {
					for (int i = 0; i < vehicles.size(); i++) {
						if (ImGui::Selectable(vehicles[i]->name.c_str(), &vehicleSelected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
							CurrentVehicleName = vehicles[i]->name.c_str();
							CurrentVehicle = i;
							roadways[CurrentRoad]->vehicle = vehicles[i];
						}
					}
					ImGui::EndCombo();
				}
				ImGui::NextColumn();
				ImGui::PopID();

				// BUTTONS

				ImGui::Separator();
				ImGui::Columns(1);
				if (ImGui::Button(" UPDATE ")) {

					// ATUALIZA TODOS OS MODELOS

					///*std::vector<Bridge*>::iterator it = bridges.begin() + CurrentBridge;
					//bridges.erase(it);*/
					ourMesh_List.clear(); //TODO: apagar apenas os modelos da rodovia corrente
					////bridges.push_back(new BoxGirder(CurrentBridgeName, roadways[CurrentRoad], cross_station, h_clearance));
					for (int i = 0; i < bridges.size(); i++)
					{
						bridges[i]->SetupBridge();
						bridges[i]->SetupSection();
						int new_nPiers = bridges[i]->alignment->getProfileLength() / bridges[i]->mainSpan;
						bridges[i]->SetupPiers(new_nPiers);
						//bridges[i]->UpdatePiers();
						bridges[i]->Update();
						for (int j = 0; j < bridges[i]->model.size(); j++)
							ourMesh_List.push_back(Mesh(bridges[i]->model[j]));
					}
					//show_edit_road_parameters = false;
				}
				ImGui::SameLine(185);
				if (ImGui::Button(" Cancel "))
					show_edit_road_parameters = false;
			}

			// END

			ImGui::End();
		}

		// Show add bridge window
		if (show_add_bridge_window)
		{
			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(295, 175), ImGuiCond_Once);
			ImGui::Begin("New Bridge", &show_add_bridge_window, ImGuiWindowFlags_NoResize);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			
			ImGui::Columns(2, NULL, false);
			ImGui::SetColumnWidth(0, 160.0f);

			// BRIDGE NAME

			ImGui::PushID(901);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Bridge name");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(120);
			ImGui::InputText("", &bridgeName, 0, NULL, NULL);
			ImGui::NextColumn();
			ImGui::PopID();

			// ROADWAY

			ImGui::PushID(902);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Roadway");
			ImGui::NextColumn();

			static bool* selected = new bool[roadways.size()];
			static std::string CurrentRoadName = " ";
			static int CurrentRoad;

			ImGui::SetNextItemWidth(120);
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

			ImGui::PushID(903);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Cross station");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(80);
			ImGui::DragFloat("m", &cross_station, 0.01f, 0.00f, 1000.00f, "%.2f");
			ImGui::NextColumn();
			ImGui::PopID();

			// VERTICAL CLEARANCE

			ImGui::PushID(904);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Vertical clearance");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(80);
			ImGui::DragFloat("m", &v_clearance, 0.01f, 0.00f, 1000.00f, "%.2f");
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

			// BUTTONS

			ImGui::Separator();
			ImGui::Columns(1);
			if (ImGui::Button("   OK   ")) {
				bridges.push_back(new BoxGirder(bridgeName, roadways[CurrentRoad], cross_station, v_clearance, h_clearance, column_stations));
				for (int i = 0; i < bridges.back()->model.size(); i++)
					ourMesh_List.push_back(Mesh(bridges.back()->model[i]));
				show_add_bridge_window = false;
			}
			ImGui::SameLine(225);
			if (ImGui::Button(" Cancel "))
				show_add_bridge_window = false;
			
			// END
			
			ImGui::End();
		}
    
		// Show edit bridge parameters
		if (show_edit_bridge_parameters)
		{
			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(275, 270), ImGuiCond_Once);
			ImGui::Begin("General parameters", &show_edit_bridge_parameters, ImGuiWindowFlags_NoResize);

			// BRIDGE

			ImGui::PushID(901);
			ImGui::AlignTextToFramePadding();

			static bool* bridgeSelected = new bool[bridges.size()];
			static std::string CurrentBridgeName = " ";
			static std::string CurrentTypeName = " ";
			static std::string CurrentRoadName = " ";
			static int CurrentBridge = -1;
			
			ImGui::SetNextItemWidth(210);
			if (ImGui::BeginCombo("Bridge", CurrentBridgeName.c_str())) {
				for (int i = 0; i < bridges.size(); i++) {
					if (ImGui::Selectable(bridges[i]->name.c_str(), &bridgeSelected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
						CurrentBridgeName = bridges[i]->name.c_str();
						CurrentBridge = i;
						CurrentTypeName = typeid(*bridges[i]).name();
						CurrentRoadName = bridges[i]->road->name;
						nPiers = bridges[i]->piers.size();
					}
				}
				ImGui::EndCombo();
			}
			//ImGui::NextColumn();
			ImGui::PopID();

			if (CurrentBridge >= 0)
			{
				ImGui::Separator();
				ImGui::Columns(2, NULL, false);
				ImGui::SetColumnWidth(0, 160.0f);

				// ROADWAY

				ImGui::PushID(902);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Roadway");
				ImGui::NextColumn();

				static bool* selected = new bool[roadways.size()];
				static int CurrentRoad;

				ImGui::SetNextItemWidth(100);
				if (ImGui::BeginCombo("", CurrentRoadName.c_str())) {
					for (int i = 0; i < roadways.size(); i++) {
						if (ImGui::Selectable(roadways[i]->name.c_str(), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
							CurrentRoadName = roadways[i]->name.c_str();
							CurrentRoad = i;
							bridges[CurrentBridge]->road = roadways[i];
						}
					}
					ImGui::EndCombo();
				}
				ImGui::NextColumn();
				ImGui::PopID();

				// CROSS STATION

				ImGui::PushID(903);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Cross station");
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", &bridges[CurrentBridge]->CS, 0.01f, 0.00f, 1000.00f, "%.2f");
				ImGui::NextColumn();
				ImGui::PopID();

				// HORIZONTAL CLEARANCE

				ImGui::PushID(904);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Horizontal clearance");
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", &bridges[CurrentBridge]->HC, 0.01f, 0.00f, 1000.00f, "%.2f");
				ImGui::NextColumn();
				ImGui::PopID();

				// VERTICAL CLEARANCE

				ImGui::PushID(905);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Vertical clearance");
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", &bridges[CurrentBridge]->VC, 0.01f, 0.00f, 1000.00f, "%.2f");
				ImGui::NextColumn();
				ImGui::PopID();

				// MAIN SPAN

				ImGui::PushID(906);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Main span");
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", &bridges[CurrentBridge]->mainSpan, 0.01f, 0.00f, 1000.00f, "%.2f");
				ImGui::NextColumn();
				ImGui::PopID();

				// NUMBER OF PIERS

				ImGui::PushID(907);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Number of piers");
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("", &nPiers, 1.0f, 1.0f, 100.0f, "%.0f");
				ImGui::NextColumn();
				ImGui::PopID();

				// WIDENING
				
				ImGui::Checkbox("Widening", &bridges[CurrentBridge]->hasWidening);

				// HAUNCHED GIRDER

				ImGui::Checkbox("Haunched Girder", &bridges[CurrentBridge]->haunchedGirder);

				// BUTTONS

				ImGui::Separator();
				ImGui::Columns(1);
				if (ImGui::Button(" UPDATE ")) {
					bridges[CurrentBridge]->SetupBridge();
					bridges[CurrentBridge]->SetupSection();
					bridges[CurrentBridge]->SetupPiers(nPiers);
					bridges[CurrentBridge]->Update();
					/*std::vector<Bridge*>::iterator it = bridges.begin() + CurrentBridge;
					bridges.erase(it);*/
					ourMesh_List.clear(); //TODO: apagar apenas os modelos da ponte corrente
					//bridges.push_back(new BoxGirder(CurrentBridgeName, roadways[CurrentRoad], cross_station, h_clearance));
					for (int i = 0; i < bridges.size(); i++)
						for (int j = 0; j < bridges[i]->model.size(); j++)
							ourMesh_List.push_back(Mesh(bridges[i]->model[j]));
					//show_edit_bridge_parameters = false;
				}
				ImGui::SameLine(205);
				if (ImGui::Button(" Cancel "))
					show_edit_bridge_parameters = false;
			}

			// END

			ImGui::End();
		}

		// Show edit bridge section
		if (show_edit_bridge_section)
		{
			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(275, 295), ImGuiCond_Once);
			ImGui::Begin("Cross-section", &show_edit_bridge_section, ImGuiWindowFlags_NoResize);

			// DATA

			float* float_data;

			// BRIDGE

			ImGui::PushID(901);
			ImGui::AlignTextToFramePadding();

			static bool* bridgeSelected = new bool[bridges.size()];
			static std::string CurrentBridgeName = " ";
			static std::string CurrentTypeName = " ";
			static int CurrentBridge = -1;

			ImGui::SetNextItemWidth(210);
			if (ImGui::BeginCombo("Bridge", CurrentBridgeName.c_str())) {
				for (int i = 0; i < bridges.size(); i++) {
					if (ImGui::Selectable(bridges[i]->name.c_str(), &bridgeSelected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
						CurrentBridgeName = bridges[i]->name.c_str();
						CurrentTypeName = typeid(*bridges[i]).name();
						CurrentBridge = i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopID();

			//TODO: não alterar parâmetros de forma dinâmica (caso queira cancelar)
			if (CurrentBridge >= 0)
			{
				ImGui::Separator();
				ImGui::Columns(2, NULL, false);
				ImGui::SetColumnWidth(0, 160.0f);

				// DECK HEIGHT

				ImGui::PushID(902);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Deck height");
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", &bridges[CurrentBridge]->H, 0.01f, 0.00f, 1000.00f, "%.2f");
				ImGui::NextColumn();
				ImGui::PopID();

				// TOTAL WIDTH

				ImGui::PushID(903);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Total width");
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", &bridges[CurrentBridge]->B, 0.01f, 0.00f, 1000.00f, "%.2f");
				ImGui::NextColumn();
				ImGui::PopID();

				// Calculation Results BUTTON

				ImGui::Columns(1);
				if (ImGui::Button("      Show Calculation Results      "))
					bridges[CurrentBridge]->SetupSection();

				ImGui::Columns(2, NULL, false);
				ImGui::SetColumnWidth(0, 160.0f);

				// CANTILEVER LENGTH

				ImGui::PushID(904);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Cantilever length");
				ImGui::NextColumn();
				float_data = new float(bridges[CurrentBridge]->get_CantileverLength());
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", float_data, 0.01f, 0.00f, 1000.00f, "%.2f");
				bridges[CurrentBridge]->set_CantileverLength(*float_data);
				delete float_data;
				ImGui::NextColumn();
				ImGui::PopID();

				// DECK THICKNESS

				ImGui::PushID(905);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Deck thickness");
				ImGui::NextColumn();
				float_data = new float(bridges[CurrentBridge]->get_DeckThickness());
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", float_data, 0.01f, 0.00f, 1000.00f, "%.2f");
				bridges[CurrentBridge]->set_DeckThickness(*float_data);
				delete float_data;
				ImGui::NextColumn();
				ImGui::PopID();

				// WEB THICKNESS

				ImGui::PushID(906);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Web thickness");
				ImGui::NextColumn();
				float_data = new float(bridges[CurrentBridge]->get_WebThickness());
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", float_data, 0.01f, 0.00f, 1000.00f, "%.2f");
				bridges[CurrentBridge]->set_WebThickness(*float_data);
				delete float_data;
				ImGui::NextColumn();
				ImGui::PopID();

				// HAUNCH HEIGHT

				ImGui::PushID(907);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Haunch height");
				ImGui::NextColumn();
				float_data = new float(bridges[CurrentBridge]->get_HaunchHeight());
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", float_data, 0.01f, 0.00f, 1000.00f, "%.2f");
				bridges[CurrentBridge]->set_HaunchHeight(*float_data);
				delete float_data;
				ImGui::NextColumn();
				ImGui::PopID();

				// BOTTOM WIDTH

				ImGui::PushID(908);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Bottom width");
				ImGui::NextColumn();
				float_data = new float(bridges[CurrentBridge]->get_BottomWidth());
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", float_data, 0.01f, 0.00f, 1000.00f, "%.2f");
				bridges[CurrentBridge]->set_BottomWidth(*float_data);
				delete float_data;
				ImGui::NextColumn();
				ImGui::PopID();

				// HAUNCH WIDTH	

				ImGui::PushID(909);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Haunch width");
				ImGui::NextColumn();
				float_data = new float(bridges[CurrentBridge]->get_HaunchWidth());
				ImGui::SetNextItemWidth(80);
				ImGui::DragFloat("m", float_data, 0.01f, 0.00f, 1000.00f, "%.2f");
				bridges[CurrentBridge]->set_HaunchWidth(*float_data);
				delete float_data;
				ImGui::NextColumn();
				ImGui::PopID();

				// BUTTONS

				ImGui::Separator();
				ImGui::Columns(1);
				if (ImGui::Button(" UPDATE ")) {
					bridges[CurrentBridge]->UpdatePiers();
					bridges[CurrentBridge]->Update();
					ourMesh_List.clear(); //TODO: apagar apenas os modelos da ponte corrente
					for (int i = 0; i < bridges[CurrentBridge]->model.size(); i++)
						ourMesh_List.push_back(Mesh(bridges[CurrentBridge]->model[i]));
					show_edit_bridge_section = false;
				}
				ImGui::SameLine(205);
				if (ImGui::Button(" Cancel "))
					show_edit_bridge_section = false;
			}

			// END

			ImGui::End();
		}
	
		// Show edit bridge columns
		if (show_edit_bridge_columns)
		{
			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(275, 295), ImGuiCond_Once);
			ImGui::Begin("Columns", &show_edit_bridge_columns, ImGuiWindowFlags_NoResize);

			// BRIDGE

			ImGui::PushID(901);
			ImGui::AlignTextToFramePadding();

			static bool* bridgeSelected = new bool[bridges.size()];
			static std::string CurrentBridgeName = " ";
			static int CurrentBridge = -1;

			ImGui::SetNextItemWidth(210);
			if (ImGui::BeginCombo("Bridge", CurrentBridgeName.c_str())) {
				for (int i = 0; i < bridges.size(); i++) {
					if (ImGui::Selectable(bridges[i]->name.c_str(), &bridgeSelected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
						CurrentBridgeName = bridges[i]->name.c_str();
						CurrentBridge = i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopID();

			ImGui::Separator();

			//TODO: não alterar parâmetros de forma dinâmica (caso queira cancelar)
			if (CurrentBridge >= 0)
			{
				/*ImGui::Separator();
				ImGui::Columns(2, NULL, false);
				ImGui::SetColumnWidth(0, 160.0f);*/

				// PIER

				ImGui::PushID(902);
				ImGui::AlignTextToFramePadding();

				static bool* pierSelected = new bool[bridges[CurrentBridge]->piers.size()];
				static std::string CurrentPierName = " ";
				static int CurrentPier = -1;

				ImGui::SetNextItemWidth(125); //210
				if (ImGui::BeginCombo("Pier", CurrentPierName.c_str())) {
					for (int i = 0; i < bridges[CurrentBridge]->piers.size(); i++) {
						std::string pierLabel = "P" + std::to_string(i + 1);
						if (ImGui::Selectable(pierLabel.c_str(), &pierSelected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None)) {
							CurrentPierName = "P" + std::to_string(i + 1);
							CurrentPier = i;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::PopID();

				ImGui::SameLine();
				if (ImGui::Button(" Add ")) {
					// apenas aumentando o tamanho do vetor
					//bridges[CurrentBridge]->piers.resize(bridges[CurrentBridge]->piers.size() + 1);
					// ou adicionando um pilar novo no meio
					bridges[CurrentBridge]->AddPier();
				}
				ImGui::SameLine();
				if (ImGui::Button(" Del ")) {
					std::vector<Pier>::iterator it = bridges[CurrentBridge]->piers.begin() + CurrentPier;
					bridges[CurrentBridge]->piers.erase(it);
				}

				ImGui::Columns(2, NULL, false);
				ImGui::SetColumnWidth(0, 130.0f); //160.0f

				if (CurrentPier >= 0)
				{
					// PIER WIDTH

					ImGui::PushID(903);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Section width");
					ImGui::NextColumn();
					ImGui::SetNextItemWidth(80);
					ImGui::DragFloat("m", &bridges[CurrentBridge]->piers[CurrentPier].b, 0.01f, 0.01f, 100.0f, "%.2f");
					ImGui::NextColumn();
					ImGui::PopID();

					// PIER HEIGHT

					ImGui::PushID(904);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Section height");
					ImGui::NextColumn();
					ImGui::SetNextItemWidth(80);
					ImGui::DragFloat("m", &bridges[CurrentBridge]->piers[CurrentPier].h, 0.01f, 0.01f, 100.0f, "%.2f");
					ImGui::NextColumn();
					ImGui::PopID();

					// PIER DIRECTION

					ImGui::PushID(905);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Direction");
					ImGui::NextColumn();
					ImGui::SetNextItemWidth(80);
					ImGui::DragFloat("degrees", &bridges[CurrentBridge]->piers[CurrentPier].ang, 0.01f, 0.0f, 360.0f, "%.2f");
					ImGui::NextColumn();
					ImGui::PopID();

					// PIER STATION

					ImGui::PushID(906);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Station");
					ImGui::NextColumn();
					ImGui::SetNextItemWidth(80);
					ImGui::DragFloat("m", &bridges[CurrentBridge]->piers[CurrentPier].station, 0.01f, -1000.0f, 1000.0f, "%.2f");
					ImGui::NextColumn();
					ImGui::PopID();

					// PIER DEPTH

					ImGui::PushID(907);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Depth");
					ImGui::NextColumn();
					ImGui::SetNextItemWidth(80);
					ImGui::DragFloat("m", &bridges[CurrentBridge]->piers[CurrentPier].depth, 0.01f, -100.0f, 100.0f, "%.2f");
					ImGui::NextColumn();
					ImGui::PopID();
				}

				// BUTTONS

				ImGui::Separator();
				ImGui::Columns(1);
				if (ImGui::Button(" UPDATE ")) {
					bridges[CurrentBridge]->UpdatePiers();
					bridges[CurrentBridge]->Update();
					ourMesh_List.clear(); //TODO: apagar apenas os modelos da ponte corrente
					for (int i = 0; i < bridges[CurrentBridge]->model.size(); i++)
						ourMesh_List.push_back(Mesh(bridges[CurrentBridge]->model[i]));
					//show_edit_bridge_columns = false;
				}
				ImGui::SameLine(205);
				if (ImGui::Button(" Cancel "))
					show_edit_bridge_columns = false;
			}

			// END

			ImGui::End();
		}
	}
}
