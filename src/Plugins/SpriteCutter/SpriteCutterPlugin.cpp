#include "SpriteCutterPlugin.h"

// Register methods to Godot's scripting system (so they can be called or connected via the editor)
void SpriteCutterPlugin::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("initialized_plugin"), &SpriteCutterPlugin::initialized_plugin);
    godot::ClassDB::bind_method(godot::D_METHOD("uninitialized_plugin"), &SpriteCutterPlugin::uninitialized_plugin);
    godot::ClassDB::bind_method(godot::D_METHOD("_on_sprite_double_clicked", "atlas"), &SpriteCutterPlugin::_on_sprite_double_clicked);
}

void SpriteCutterPlugin::initialized_plugin() {
    if (dock) return;

    // Create the custom dock UI
    dock = memnew(SpriteCutterDock);
    dock->set_name("SpriteCutter");
    add_control_to_bottom_panel(dock, "Sprite Cutter");

    // Connect the dock signal to the handler method in this plugin
    dock->connect("sprite_double_clicked", godot::Callable(this, "_on_sprite_double_clicked"));

    godot::UtilityFunctions::print("SpriteCutterPlugin initialized");
}

void SpriteCutterPlugin::uninitialized_plugin() {
    if (!dock) return;

    // Disconnect the signal (safety before removal)
    dock->disconnect("sprite_double_clicked", godot::Callable(this, "_on_sprite_double_clicked"));
    
    // Remove the dock from the UI if it's still active
    if (dock->is_inside_tree())
        remove_control_from_bottom_panel(dock);

    dock->queue_free();
    dock = nullptr;

    godot::UtilityFunctions::print("SpriteCutterPlugin uninitialized");
}

void SpriteCutterPlugin::_on_sprite_double_clicked(const godot::Ref<godot::AtlasTexture>& atlas) {
    // Ignore invalid input
    if (!atlas.is_valid()) {
        return;
    }

    // Get the root node of the currently edited scene
    godot::Node* edited = get_editor_interface()->get_edited_scene_root();
    godot::Node3D* root3d = godot::Object::cast_to<godot::Node3D>(edited);
    if (!root3d) {
        return;
    }

    // Create a new Sprite3D node and assign the texture
    godot::Sprite3D* sprite = memnew(godot::Sprite3D);
    sprite->set_texture(atlas);

    // Set up the undo/redo action for adding the sprite
    godot::EditorUndoRedoManager* ur = get_undo_redo();
    ur->create_action("Add Sprite3D");

    // Define what happens when the action is done
    ur->add_do_method(root3d, "add_child", sprite);
    ur->add_do_method(sprite, "set_owner", root3d);

    // Define what happens when the action is undone
    ur->add_undo_method(root3d, "remove_child", sprite);
    ur->add_undo_method(sprite, "set_owner", nullptr);

    // Apply the action to the editor
    ur->commit_action();
}
