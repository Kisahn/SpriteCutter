#include "SpriteCutterRightPanel.h"

// Register method to Godot's scripting system
void SpriteCutterRightPanel::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("_notification", "p_what"), &SpriteCutterRightPanel::_notification);
}

SpriteCutterRightPanel::SpriteCutterRightPanel() {
    // Create the scroll container
    scroll = memnew(godot::ScrollContainer);
    scroll->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->set_clip_contents(true);
    scroll->set_horizontal_scroll_mode(godot::ScrollContainer::ScrollMode::SCROLL_MODE_AUTO);
    scroll->set_vertical_scroll_mode(godot::ScrollContainer::ScrollMode::SCROLL_MODE_DISABLED);
    add_child(scroll);

    // Create the ItemList that will hold the AtlasTextures
    list = memnew(godot::ItemList);
    list->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    list->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    list->set_clip_contents(true);
    list->set_icon_mode(godot::ItemList::IconMode::ICON_MODE_TOP);
    list->set_select_mode(godot::ItemList::SelectMode::SELECT_SINGLE);
    list->set_wraparound_items(false);
    scroll->add_child(list);

    // Compute and apply icon size
    adjust_icon_size();
}

void SpriteCutterRightPanel::_notification(int p_what) {
    if (p_what == NOTIFICATION_RESIZED) {
        adjust_icon_size();
    }
}

void SpriteCutterRightPanel::adjust_icon_size() {
    int avail_h = scroll->get_size().y;
    int icon_h = avail_h - TEXT_HEIGHT - PADDING - BORDER_EXTRA;

    // Enforce a minimum icon size
    if (icon_h < 16)
        icon_h = 16;
    
    list->set_fixed_icon_size(godot::Vector2(icon_h, icon_h));
}

void SpriteCutterRightPanel::clear() {
    list->clear();
    subs_storage.clear();
}

void SpriteCutterRightPanel::populate(const godot::Vector<godot::Ref<godot::AtlasTexture>>& subs) {
    // Always start from an empty state
    clear();

    subs_storage = subs;
    
    if (subs_storage.is_empty())
        return;
    
    // Show all textures in a single row (no wrapping)
    list->set_max_columns(subs_storage.size());

    // Adjust icon size before adding
    adjust_icon_size();

    // Add each AtlasTexture to the list with its index as label
    for (int i = 0; i < subs_storage.size(); ++i) {
        godot::String txt = godot::String::num_int64(i + 1);
        list->add_item(txt, subs_storage[i]);
    }
}
