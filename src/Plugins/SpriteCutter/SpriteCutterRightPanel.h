#pragma once

#include <godot_cpp/classes/atlas_texture.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/scroll_container.hpp>

/**
 * @class SpriteCutterRightPanel
 * @brief Displays a scrollable list of AtlasTexture previews.
 *
 * This panel is used to show the result of slicing a Texture2D.
 * 
 * Each AtlasTexture is represented as an icon in a grid.
 * 
 * The panel handles resizing and icon scaling automatically.
 */
class SpriteCutterRightPanel : public godot::PanelContainer
{
    GDCLASS(SpriteCutterRightPanel, godot::PanelContainer);

    public:
        /**
         * @brief Constructs the right panel and initializes its layout.
         */
        SpriteCutterRightPanel();
        ~SpriteCutterRightPanel() override = default;

        /**
         * @brief Clears the UI list and internal storage of sliced regions.
         */
        void clear();

        /**
         * @brief Populates the list with the given sliced textures.
         * @param subs A vector of AtlasTexture references to display.
         */
        void populate(const godot::Vector<godot::Ref<godot::AtlasTexture>>& subs);

        /**
         * @brief Returns the stored list of sliced AtlasTextures.
         * @return A constant reference to the vector of textures.
         */
        const godot::Vector<godot::Ref<godot::AtlasTexture>>& get_subs() const { return subs_storage; }

        /**
         * @brief Returns the ItemList UI node.
         * @return A pointer to the ItemList instance.
         */
        godot::ItemList* get_list() const { return list; }

    protected:
        static void _bind_methods();

    private:
        /**
         * @brief Recalculates and sets the icon size in the list based on panel height.
         */
        void adjust_icon_size();

        /**
         * @brief Handles notification events such as resizing.
         * @param p_what Type of notification (e.g., NOTIFICATION_RESIZED).
         */
        void _notification(int p_what);

        // Scrollable container that holds the ItemList
        godot::ScrollContainer* scroll = nullptr;

        // Displays the sliced textures as icons
        godot::ItemList* list = nullptr;

        // Internal storage of current textures
        godot::Vector<godot::Ref<godot::AtlasTexture>> subs_storage;

        // Height reserved for the text below each icon
        static constexpr int TEXT_HEIGHT = 20;

        // Padding between icon and panel edge
        static constexpr int PADDING = 4;

        // Extra space reserved for scroll bars and borders
        static constexpr int BORDER_EXTRA = 30;
};
