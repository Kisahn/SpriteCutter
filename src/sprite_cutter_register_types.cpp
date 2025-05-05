#include "spritecutter_register_types.h"

void initialize_spritecutter_types(godot::ModuleInitializationLevel p_level) {
    if (p_level == godot::MODULE_INITIALIZATION_LEVEL_EDITOR) {
        godot::ClassDB::register_class<SpriteCutterPlugin>();
        godot::ClassDB::register_class<SpriteCutterDock>();
        godot::ClassDB::register_class<SpriteCutterLeftPanel>();
        godot::ClassDB::register_class<SpriteCutterRightPanel>();
    }
}

void uninitialize_spritecutter_types(godot::ModuleInitializationLevel) {}

extern "C" GDExtensionBool GDE_EXPORT GDExtensionInitializeSpriteCutter(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization* p_initialization
)
{
    godot::GDExtensionBinding::InitObject init_obj(
        p_get_proc_address, p_library, p_initialization
    );
    init_obj.register_initializer(initialize_spritecutter_types);
    init_obj.register_terminator(uninitialize_spritecutter_types);
    init_obj.set_minimum_library_initialization_level(
        godot::MODULE_INITIALIZATION_LEVEL_SCENE
    );
    return init_obj.init();
}
