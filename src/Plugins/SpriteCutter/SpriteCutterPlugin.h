#pragma once

#include <godot_cpp/classes/atlas_texture.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>
#include <godot_cpp/classes/sprite3d.hpp>

#include "SpriteCutterDock.h"

/**
 * @class SpriteCutterPlugin
 * @brief Godot editor plugin that adds a Sprite3D to the current scene from an AtlasTexture.
 *
 * This plugin creates a custom bottom dock ("Sprite Cutter") which allows the user to inspect atlas textures.
 * 
 * When a texture is double-clicked, a Sprite3D node is instantiated and added to the edited 3D scene.
 *
 * This plugin uses the Godot undo/redo system to properly register node creation in the editor.
 */
class SpriteCutterPlugin : public godot::EditorPlugin
{
    GDCLASS(SpriteCutterPlugin, godot::EditorPlugin);

    protected:
        /**
         * Binds plugin methods to Godot's ClassDB.
         */
        static void _bind_methods();

    public:
        SpriteCutterPlugin() = default;
        ~SpriteCutterPlugin() override = default;

        /**
         * Initializes the plugin and adds the custom dock to the editor.
         */
        void initialized_plugin();
        
        /**
         * Cleans up and removes the plugin dock from the editor.
         */
        void uninitialized_plugin();

        /**
         * Handles the double-click signal from the SpriteCutterDock.
         * 
         * Creates and adds a Sprite3D node with the selected texture.
         *
         * @param atlas A reference to the AtlasTexture used for the new Sprite3D.
         */
        void _on_sprite_double_clicked(const godot::Ref<godot::AtlasTexture>& atlas);

        // Pointer to the plugin's custom dock UI.
        SpriteCutterDock* dock{ nullptr };
};
