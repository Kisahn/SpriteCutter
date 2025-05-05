#pragma once

#include <godot_cpp/godot.hpp>

// Plugins
#include "Plugins/SpriteCutter/SpriteCutterPlugin.h"
#include "Plugins/SpriteCutter/SpriteCutterDock.h"
#include "Plugins/SpriteCutter/SpriteCutterLeftPanel.h"
#include "Plugins/SpriteCutter/SpriteCutterRightPanel.h"

void initialize_spritecutter_types(godot::ModuleInitializationLevel p_level);
void uninitialize_spritecutter_types(godot::ModuleInitializationLevel p_level);
