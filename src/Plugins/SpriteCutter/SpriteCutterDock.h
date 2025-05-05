#pragma once

#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/split_container.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>

#include "SpriteCutterLeftPanel.h"
#include "SpriteCutterRightPanel.h"
#include "SpriteCutterAutoSlicer.h"

/**
 * @class SpriteCutterDock
 * @brief Custom dock UI displayed in the Godot editor for slicing and previewing textures.
 * 
 * This dock provides a split layout with:
 * 
 * - a left panel to load a texture and trigger the slicing process;
 * 
 * - a right panel to preview the generated AtlasTexture regions.
 *
 * Emits a signal when the user double-clicks on one of the generated regions.
 */
class SpriteCutterDock : public godot::PanelContainer {
    GDCLASS(SpriteCutterDock, godot::PanelContainer);

    public:
        /**
         * @brief Constructor.
         * 
         * Initializes the UI by calling setup_ui().
         */
        SpriteCutterDock();
        ~SpriteCutterDock() override = default;

        /**
         * @brief Sets the split container offset to the default ratio.
         */
        void adjust_split_offset();

        /**
         * @brief Returns the right panel (used to populate or access selected items).
         * @return Pointer to the SpriteCutterRightPanel instance.
         */
        SpriteCutterRightPanel* get_right_panel() const { return right_panel; }

    protected:
        static void _bind_methods();

        /**
         * @brief Notification handler for lifecycle events.
         * @param what Notification type.
         */
        void _notification(int what);

    private:
        /**
         * @brief Creates and adds all the UI elements to the dock.
         */
        void setup_ui();

        /**
         * @brief Connects signals from child nodes to local methods.
         */
        void connect_signals();


        /**
         * @brief Called when the user loads a new texture.
         * 
         * Clears the right panel.
         * 
         * @param tex The newly loaded texture.
         */
        void _on_texture_changed(const godot::Ref<godot::Texture2D>& tex);

        /**
         * @brief Called when the user requests to cut the texture.
         * 
         * Slices it using SpriteCutterAutoSlicer and populates the right panel.
         */
        void _on_cut_requested();

        /**
         * @brief Called when a region is activated (double-clicked).
         * 
         * Emits the sprite_double_clicked signal.
         * @param index Index of the clicked item.
         */
        void _on_item_activated(int index);

        // Container that holds left and right panels
        godot::SplitContainer* split{ nullptr };

        // Panel containing the texture loader and cut button
        SpriteCutterLeftPanel* left_panel{ nullptr };

        // Panel displaying the generated AtlasTexture regions
        SpriteCutterRightPanel* right_panel{ nullptr };

        // Default ratio between left and right panels (used on initial split offset)
        static constexpr float SPLIT_RATIO = 0.18f;
};
