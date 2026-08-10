// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "selection_menu.hpp"
#include "systems/cpp_scripting_system/cpp_entity_context.hpp"


void OmnificDemo::SelectionMenu::on_entity_start()
{
    Omnific::Window& window = Omnific::Platform::get_window();
    window.set_to_windowed(853, 480);
    window.change_title("Omnific Demo");
    std::string project_print_string = "";
    std::vector<std::string> project_selection_letters;
    std::vector<std::string> project_names_list;

    for (const auto& pair : this->project_paths)
    {
        project_selection_letters.push_back(pair.first);
    }

    for (const auto& pair : this->project_name_type_pairs)
    {
        project_names_list.push_back(pair.first);
    }

    for (int i = 0; i < project_names_list.size(); ++i)
    {
        std::string project_name = project_names_list[i];
        std::shared_ptr<Omnific::Entity> project_button_entity = std::make_shared<Omnific::Entity>(project_name + " Button");
        std::shared_ptr<Omnific::Button> project_button = std::make_shared<Omnific::Button>();
        project_button->label->set_text(project_selection_letters[i] + ": " + project_name);
        project_button->update_image();
        project_button->scene_hyperlink = this->project_paths[i].second;
        std::shared_ptr<Omnific::Scene> scene = Omnific::CPPEntityContext::get_scene();
        project_button->anchor_pivot = Omnific::GUIElement::PivotPoint::TOP_LEFT;
        project_button->pivot = Omnific::GUIElement::PivotPoint::TOP_LEFT;
        project_button->margin = glm::vec2(0.0f, -60.0f + -(i * (project_button->get_dimensions().y + 1.0f)));
        scene->add_entity(project_button_entity);
        scene->add_component_to_last_entity(project_button);
    }

    std::shared_ptr<Omnific::Label> label = Omnific::CPPEntityContext::get_component_by_type<Omnific::Label>();
    label->set_text(
        "Select one of these demos by typing the letter in front of it.\n\n");

}

void OmnificDemo::SelectionMenu::on_input()
{
    Omnific::Inputs& inputs = Omnific::Platform::get_inputs();

    for (const auto& pair : this->project_paths)
    {
        if (inputs.is_on_release(pair.first))
        {
            Omnific::SceneManager::load_scene(pair.second);
            Omnific::Platform::get_window().change_title("");
            break;
        }
    }

}

void OmnificDemo::SelectionMenu::list_project(std::string project_name, std::string project_type)
{
    std::string menu_item_path = "";

    if (project_type == "test")
    {
        menu_item_path = "scenes/tests/" + project_name + ".yml";
    }
    else
    {
        menu_item_path = "scenes/" + project_type + "s/" + project_name + "/"+ project_name + ".yml";
    }
    this->project_paths.push_back({this->last_letter, menu_item_path});
    this->last_letter = this->last_letter[0] + 1;
}