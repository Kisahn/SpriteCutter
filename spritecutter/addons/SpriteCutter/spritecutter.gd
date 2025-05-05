@tool
extends EditorPlugin

var plugin_cpp := SpriteCutterPlugin.new()

func _enter_tree():
    add_child(plugin_cpp)
    plugin_cpp.initialized_plugin()

func _exit_tree():
    plugin_cpp.uninitialized_plugin()
    remove_child(plugin_cpp)
    plugin_cpp.queue_free()
