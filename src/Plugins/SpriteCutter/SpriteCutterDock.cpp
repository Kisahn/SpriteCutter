#include "SpriteCutterDock.h"

using namespace godot;

// Register exposed methods and the custom signal
void SpriteCutterDock::_bind_methods() {
    ClassDB::bind_method(D_METHOD("adjust_split_offset"), &SpriteCutterDock::adjust_split_offset);
    ClassDB::bind_method(D_METHOD("_on_texture_changed", "tex"), &SpriteCutterDock::_on_texture_changed);
    ClassDB::bind_method(D_METHOD("_on_cut_requested"), &SpriteCutterDock::_on_cut_requested);
    ClassDB::bind_method(D_METHOD("_on_item_activated", "index"), &SpriteCutterDock::_on_item_activated);

    ADD_SIGNAL(MethodInfo("sprite_double_clicked", PropertyInfo(Variant::OBJECT, "atlas", PROPERTY_HINT_RESOURCE_TYPE, "AtlasTexture")));
}

// Constructor: sets up the dock UI
SpriteCutterDock::SpriteCutterDock() {
    setup_ui();
}

// Reacts to Godot notifications (e.g., when the node enters the scene tree)
void SpriteCutterDock::_notification(int what) {
    PanelContainer::_notification(what);

    if (what == NOTIFICATION_READY) {
        connect_signals();
        adjust_split_offset();
    }
}

void SpriteCutterDock::setup_ui() {
    // SplitContainer setup
    split = memnew(SplitContainer);
    split->set_vertical(false);
    split->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    split->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    split->set_dragger_visibility(SplitContainer::DraggerVisibility::DRAGGER_VISIBLE);
    split->add_theme_constant_override("autohide", 0);
    split->add_theme_constant_override("separation", 2);

    // Custom styling of the split bar
    Ref<StyleBoxFlat> sb; sb.instantiate();
    sb->set_bg_color(Color(0.15, 0.15, 0.15, 0.5));
    sb->set_border_width_all(0);
    split->add_theme_stylebox_override("split_bar_background", sb);
    add_child(split);

    // Create left panel wrapped in a MarginContainer
    left_panel = memnew(SpriteCutterLeftPanel);
    {
        auto m = memnew(MarginContainer);
        m->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        m->set_v_size_flags(Control::SIZE_EXPAND_FILL);
        m->add_theme_constant_override("margin_right", 4);
        m->add_child(left_panel);
        split->add_child(m);
    }

    // Create right panel wrapped in a MarginContainer
    right_panel = memnew(SpriteCutterRightPanel);
    {
        auto m = memnew(MarginContainer);
        m->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        m->set_v_size_flags(Control::SIZE_EXPAND_FILL);
        m->add_theme_constant_override("margin_left", 4);
        m->add_child(right_panel);
        split->add_child(m);
    }
}

void SpriteCutterDock::connect_signals() {
    left_panel->connect("texture_changed", Callable(this, "_on_texture_changed"));
    left_panel->connect("cut_requested", Callable(this, "_on_cut_requested"));
    right_panel->get_list()->connect("item_activated", Callable(this, "_on_item_activated"));
}

void SpriteCutterDock::adjust_split_offset() {
    if (!split) return;
    int offset = int(get_size().x * SPLIT_RATIO);
    split->set_split_offset(offset);
}

void SpriteCutterDock::_on_texture_changed(const Ref<Texture2D>&) {
    UtilityFunctions::print("SpriteCutter: texture chargée → on clear");
    right_panel->clear();
}

void SpriteCutterDock::_on_cut_requested() {
    UtilityFunctions::print("SpriteCutter: Cut requested");

    Ref<Texture2D> tex = left_panel->get_texture();
    if (!tex.is_valid()) {
        UtilityFunctions::printerr("SpriteCutter: PAS de texture chargée !");
        return;
    }

    UtilityFunctions::print("SpriteCutter: texture valide → ", tex->get_width(), "×", tex->get_height());

    // Slice the texture into subregions
    Array arr = SpriteCutterAutoSlicer::slice(tex);
    
    // Convert to typed array of AtlasTexture references
    Vector<Ref<AtlasTexture>> subs;
    subs.resize(arr.size());
    for (uint32_t i = 0; i < arr.size(); ++i) {
        subs.set(i, arr[i]);
    }

    UtilityFunctions::print("SpriteCutter: nombre de régions détectées = ", subs.size());

    // Populate the right panel with the results
    right_panel->populate(subs);
    UtilityFunctions::print("SpriteCutter: populate() terminée");
}

void SpriteCutterDock::_on_item_activated(int index) {
    const Vector<Ref<AtlasTexture>>& subs = right_panel->get_subs();
    if (index < 0 || index >= subs.size()) return;
    emit_signal("sprite_double_clicked", subs[index]);
}
