#pragma once

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/editor_resource_picker.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

/**
* @class SpriteCutterLeftPanel
* @brief SpriteCutterLeftPanel is the left-side panel of the Sprite Cutter dock.
*
* It allows the user to select a `Texture2D` using an `EditorResourcePicker`,
* 
* preview it inside a `TextureRect`, and then press a button to trigger
* 
* the slicing process.
*
* It emits two signals:
* 
* - `texture_changed` when a new texture is selected.
* 
* - `cut_requested` when the "Cut Sprite" button is pressed.
*/
class SpriteCutterLeftPanel : public godot::VBoxContainer
{
    GDCLASS(SpriteCutterLeftPanel, godot::VBoxContainer);

    public:
        /**
         * @brief Constructs the left panel and sets up its layout and signals.
         */
        SpriteCutterLeftPanel();
        ~SpriteCutterLeftPanel() override = default;

        /**
         * @brief Returns the currently selected texture.
         *
         * @return The Texture2D currently assigned to the panel.
         */
        godot::Ref<godot::Texture2D> get_texture() const { return texture; }

    protected:
        static void _bind_methods();

    private:
        /**
         * @brief Called when the user picks a new resource.
         *
         * Casts it to Texture2D and updates the preview and stored reference.
         *
         * @param res The resource selected in the picker.
         */
        void _on_texture_picked(const godot::Ref<godot::Resource>& res);

        /**
         * @brief Called when the user presses the "Cut Sprite" button.
         *
         * Emits the `cut_requested` signal.
         */
        void _on_cut_pressed();

        // Picker for selecting a Texture2D resource
        godot::EditorResourcePicker* picker = nullptr;

        // UI element that displays the selected texture
        godot::TextureRect* texture_rect = nullptr;
        
        // Button that triggers the cut action
        godot::Button* cut_button = nullptr;

        // The currently selected texture
        godot::Ref<godot::Texture2D> texture;
};
