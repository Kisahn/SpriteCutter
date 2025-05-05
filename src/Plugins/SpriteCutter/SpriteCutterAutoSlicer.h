#pragma once

#include <godot_cpp/classes/atlas_texture.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/templates/local_vector.hpp>

/**
 * @class SpriteCutterAutoSlicer
 * @brief Utility class to automatically slice a texture into regions based on transparency.
 *
 * This class provides a static interface to extract non-transparent regions
 * 
 * from a texture and convert them into `AtlasTexture` objects that can be
 * 
 * used by Godot's rendering system.
 *
 * The algorithm scans pixels, groups connected opaque areas into regions,
 * 
 * merges nearby ones, and returns a list of sliced textures.
 */
class SpriteCutterAutoSlicer {
    public:
        /**
         * @brief Automatically slices the given texture and returns a list of AtlasTextures.
         *
         * The algorithm extracts visible (non-transparent) areas from the image.
         *
         * @param texture The input texture to slice.
         * @return An array of AtlasTexture objects representing the detected regions.
         */
        static godot::Array slice(const godot::Ref<godot::Texture2D>& texture);

    private:
        /**
         * @brief Represents a detected region with a rectangle and pixel count.
         */
        struct Region
        { 
            godot::Rect2 rect;
            int count;
        };

        /**
         * @brief A helper point struct used during region exploration.
         */
        struct Pt
        {
            int x, y;
        };

        /**
         * @brief Detects opaque pixel regions in the given image.
         *
         * Uses a non-recursive flood-fill to group connected opaque pixels.
         * @param img The image to scan.
         * @param regions Output list of detected regions.
         */
        static void detect_regions(godot::Image* img, godot::LocalVector<Region>& regions);
        
        /**
         * @brief Merges regions that are close to each other spatially.
         *
         * Prevents fragmented small regions and overlaps by combining overlapping or nearby areas.
         *
         * @param regions List of regions to process. Modified in-place.
         */
        static void merge_regions(godot::LocalVector<Region>& regions);
        
        /**
         * @brief Converts the regions into AtlasTextures using the original texture as atlas source.
         *
         * @param texture The original texture to use as the atlas base.
         * @param regions The detected regions to convert.
         * @return An array of AtlasTextures.
         */
        static godot::Array build_atlas_textures(const godot::Ref<godot::Texture2D>& texture, const godot::LocalVector<Region>& regions);

        // Minimum pixels to consider a region valid
        static constexpr int MIN_PIXELS = 100;

        // Margin used when merging nearby regions
        static constexpr float MERGE_MARGIN = 5.0f;
};
