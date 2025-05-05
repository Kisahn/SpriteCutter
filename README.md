# ✂️ Godot Sprite Cutter – C++ GDExtension Plugin

Godot Sprite Cutter is a plugin for **Godot 4**, developed in **modern C++17** using **GDExtension** and `godot-cpp`.  
It adds a native tool to the editor that automatically slices a spritesheet into individual sprite regions.  
The goal is to simplify the workflow for artists and designers by automating tedious slicing operations inside the editor.

This plugin was designed to reduce manual sprite region setup when importing spritesheets into Godot.  
By integrating directly into the editor via `.gdextension`, the plugin allows instant slicing based on pixel content, without relying on grid alignment or manual region definitions.

---

## ⚙️ Main Features

- Automatic slicing of spritesheets based on image content
- Native Godot 4 plugin written in C++ using GDExtension
- Integration into the editor for immediate usability
- Precompiled binaries for quick setup
- Lightweight and dependency-free (aside from `godot-cpp`)

---

## 🧱 Technologies Used

| Component        | Description                                |
|------------------|--------------------------------------------|
| Godot 4.4        | Game engine and editor integration target  |
| C++17            | Core plugin implementation                 |
| godot-cpp        | C++ bindings for Godot (submodule)         |
| GDExtension      | Interface used to extend Godot in native code |
| SCons            | Build system used for compilation          |

---

## 📁 Project Structure

godot-sprite-cutter/  
├── src/                  ← C++ source files  
├── bin/                  ← Compiled .gdextension, .dll, .lib  
├── spritecutter/         ← Godot 4 demo project  
│   ├── bin/              ← Plugin binaries used in the demo project  
│   ├── assets/           ← Sample spritesheets for testing  
│   ├── scenes/           ← Demo scene (plugin_test.tscn)  
│   ├── addons/           ← Folder containing the plugin definition  
│   └── project.godot  
├── godot-cpp/            ← Submodule (GDExtension C++ bindings)  
├── LICENSE  
├── README.md  
└── SConstruct            ← Build script using SCons  

---

## 🚀 Getting Started

To use the plugin:

**Note**: This plugin requires **Godot 4.4**.

1. Clone this repository using `git clone --recurse-submodules`  
2. Open the `spritecutter/project.godot` file in **Godot 4.4**  
3. In the Godot editor, open the demo scene located at `spritecutter/scenes/plugin_test.tscn`  
4. Go to **Project > Project Settings**, then open the **"Plugins"** tab and **enable the plugin named "SpriteCutter"**  
5. Once enabled, a **new tab named "Sprite Cutter"** will appear at the bottom of the Godot editor  
6. Drag and drop a spritesheet from `spritecutter/assets/` into the tool's drop zone  
7. Click the **"Cut Sprite"** button to generate regions automatically  
8. Double-click any generated region to add it to the currently open scene.  
   A new `Sprite3D` node containing the sliced texture will be added, and you’ll see it appear in the **Scene Tree (left panel)**.  
9. You can use **Ctrl+Z** and **Ctrl+Y** to undo/redo sprite placement or creation

The plugin is immediately usable in-editor. All assets and the demo scene are included for quick testing.

---

## 📦 Included Binaries

Precompiled `.gdextension`, `.dll`, and `.lib` files are included in the following folders:

- `bin/windows/`
- `spritecutter/bin/`

Only these files are versioned. All other intermediates (`.pdb`, `.exp`, `.obj`, etc.) are ignored via `.gitignore`.

---

## 🧪 Build Instructions

1. Install SCons and a C++ compiler
2. Compile `godot-cpp` inside its folder with `generate_bindings=yes`
3. Run `scons platform=windows` from the root directory

The compiled plugin will be placed in `bin/`, and copied into `spritecutter/bin/` for immediate use.

---

## 📜 License

This project is licensed under the MIT License.  
See the [`LICENSE`](./LICENSE) file for details.

---

## 👤 Author

[LinkedIn – Jean Deck](https://www.linkedin.com/in/jean-deck-2b915aa9/)  
[GitHub – Kisahn](https://github.com/Kisahn)