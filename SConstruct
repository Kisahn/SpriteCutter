#!/usr/bin/env python
import os
from SCons.Script import Default, Glob, Environment, SConscript, Help
from SCons.Variables import Variables, BoolVariable, PathVariable, EnumVariable
from SCons.Errors import UserError

# -------------------------------
# Helpers pour normaliser/valider
# -------------------------------
def normalize_path(val, env):
    return val if os.path.isabs(val) else os.path.join(env.Dir("#").abspath, val)

def validate_parent_dir(key, val, env):
    parent = os.path.dirname(val)
    if not os.path.isdir(normalize_path(parent, env)):
        raise UserError(f"'{key}' référence un dossier invalide : {parent}")

# --------------------
# Configuration projet
# --------------------
core_libname   = "spritecutter"
plugin_libname = "spritecutter"
projectdir     = "spritecutter"

# Environnement de base
base_env = Environment(tools=["default"])

# --------------------------
# Déclaration des Variables
# --------------------------
customs = [os.path.abspath("custom.py")]
opts    = Variables(customs, ARGUMENTS)

opts.Add(BoolVariable("compiledb",
    "Générer compile_commands.json pour l’IDE",
    default=base_env.get("compiledb", False),
))
opts.Add(PathVariable("compiledb_file",
    "Chemin vers compile_commands.json",
    default=base_env.get("compiledb_file", "compile_commands.json"),
    validator=validate_parent_dir,
))
opts.Add(EnumVariable("bits",
    "Architecture cible",
    default="64",
    allowed_values=["32", "64"],
))
opts.Add(BoolVariable("production",
    "Mode production (optimisations + LTO)",
    default=False,
))
opts.Add(EnumVariable("lto",
    "Link Time Optimization",
    default="none",
    allowed_values=["none", "thin", "full"],
))
opts.Add(BoolVariable("use_llvm",
    "Forcer LLVM/Clang sous Windows",
    default=False,
))
opts.Add(BoolVariable("dev_build",
    "Mode dev_build (suffixe .dev, logs verbeux)",
    default=False,
))
opts.Add(BoolVariable("debug_symbols",
    "Inclure symboles de debug",
    default=False,
))
opts.Add(EnumVariable("target",
    "Cible de build",
    default="template_debug",
    allowed_values=[
        "editor",
        "template_debug",
        "template_release",
        "release_debug",
        "release"
    ],
))
opts.Add(EnumVariable("platform",
    "Plateforme de build",
    default="windows",
    allowed_values=[
        "windows",
        "linux",
        "macos",
        "android",
        "web"
    ],
))
opts.Update(base_env)
Help(opts.GenerateHelpText(base_env))

# ------------------------------------------------
# Charger et exécuter le SConscript de godot-cpp
# ------------------------------------------------
env = base_env.Clone()
env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

# --------------------------------
# Désactiver l'édition de liens incrémentielle sous MSVC
# --------------------------------
if env["platform"] == "windows":
    env.Append(LINKFLAGS=['/INCREMENTAL:NO', '/NOLOGO'])

# --------------------------------
# Génération de la compilation DB
# --------------------------------
if env["compiledb"]:
    env.Tool("compilation_db")
    comp_db = env.CompilationDatabase(
        normalize_path(env["compiledb_file"], env)
    )
    env.Alias("compiledb", comp_db)

# -----------------------------
# Collecte récursive des sources
# -----------------------------
def find_dirs(root):
    out = []
    for dp, _, _ in os.walk(root):
        rel = os.path.relpath(dp, root).replace("\\", "/")
        out.append(root + ("" if rel == "." else "/" + rel))
    return out

src_dirs = find_dirs("src")
env.Append(CPPPATH=src_dirs)

all_sources = []
for d in src_dirs:
    all_sources.extend(Glob(os.path.join(d, "*.cpp")))

# -----------------------------------------
# Séparation des sources : core vs plugin
# -----------------------------------------
core_sources   = []
plugin_sources = []

for s in all_sources:
    path_lower = str(s).lower()
    if "spritecutter" in path_lower:
        plugin_sources.append(s)
    else:
        core_sources.append(s)

# ------------------------
# Construction de la lib unique “opallion”
# ------------------------
suffix_clean = env["suffix"].replace(".dev", "").replace(".universal", "")
lib_pref     = env.subst("$SHLIBPREFIX")
lib_suff     = env.subst("$SHLIBSUFFIX")

# Nomme la lib d'après core_libname pour tout regrouper
lib_file    = lib_pref + core_libname + suffix_clean + lib_suff
lib_target  = os.path.join("bin", env["platform"], lib_file)

# Compile core + plugin ensemble
all_sources = core_sources + plugin_sources
opallion_lib = env.SharedLibrary(target=lib_target, source=all_sources)

# ----------------------------------------
# Installation dans <projectdir>/bin/<plat>/…
# ----------------------------------------
dest = [os.path.join(projectdir, "bin", env["platform"], lib_file)]
if env["platform"] == "windows":
    dest.append(dest[0] + ".pdb")
copy_opallion = env.InstallAs(dest, opallion_lib)

# ------------------
# Cibles par défaut
# ------------------
defaults = [opallion_lib, copy_opallion]
if env["compiledb"]:
    defaults.append(comp_db)

Default(*defaults)
