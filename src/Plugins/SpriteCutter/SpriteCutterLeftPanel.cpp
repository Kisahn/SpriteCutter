#include "SpriteCutterLeftPanel.h"

// Register methods and signals to Godot's scripting system
void SpriteCutterLeftPanel::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("_on_texture_picked", "res"), &SpriteCutterLeftPanel::_on_texture_picked);
    godot::ClassDB::bind_method(godot::D_METHOD("_on_cut_pressed"), &SpriteCutterLeftPanel::_on_cut_pressed);

    ADD_SIGNAL(godot::MethodInfo("texture_changed", godot::PropertyInfo(godot::Variant::OBJECT, "tex", godot::PROPERTY_HINT_RESOURCE_TYPE, "Texture2D")));
    ADD_SIGNAL(godot::MethodInfo("cut_requested"));
}

SpriteCutterLeftPanel::SpriteCutterLeftPanel() {
    // Resource picker setup
    picker = memnew(godot::EditorResourcePicker);
    picker->set_base_type("Texture2D");
    picker->set_editable(true);
    picker->connect("resource_changed", godot::Callable(this, "_on_texture_picked"));
    add_child(picker);

    // Texture preview
    texture_rect = memnew(godot::TextureRect);
    texture_rect->set_expand_mode(godot::TextureRect::ExpandMode::EXPAND_IGNORE_SIZE);
    texture_rect->set_stretch_mode(godot::TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
    texture_rect->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    texture_rect->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    add_child(texture_rect);

    // "Cut" button
    cut_button = memnew(godot::Button);
    cut_button->set_text("Cut Sprite");
    cut_button->connect("pressed", godot::Callable(this, "_on_cut_pressed"));
    add_child(cut_button);
}

void SpriteCutterLeftPanel::_on_texture_picked(const godot::Ref<godot::Resource>& res) {
    godot::Texture2D* tex_obj = godot::Object::cast_to<godot::Texture2D>(res.ptr());
    texture = godot::Ref<godot::Texture2D>(tex_obj);
    texture_rect->set_texture(texture);
    emit_signal("texture_changed", texture);
}

void SpriteCutterLeftPanel::_on_cut_pressed() {
    emit_signal("cut_requested");
}
