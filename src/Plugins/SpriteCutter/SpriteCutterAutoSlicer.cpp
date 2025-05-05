#include "SpriteCutterAutoSlicer.h"

godot::Array SpriteCutterAutoSlicer::slice(const godot::Ref<godot::Texture2D>& texture) {
    godot::Array subs;
    if (!texture.is_valid()) return subs;

    godot::Ref<godot::Image> img = texture->get_image();
    if (!img.is_valid()) return subs;

    // If the image is compressed, decompress it first
    if (img->is_compressed() && img->decompress() != godot::OK) {
        godot::UtilityFunctions::printerr("SpriteCutterAutoSlicer: can't decompress image");
        return subs;
    }

    godot::LocalVector<Region> regions;
    detect_regions(img.ptr(), regions);

    if (regions.is_empty()) return subs;

    merge_regions(regions);

    return build_atlas_textures(texture, regions);
}

void SpriteCutterAutoSlicer::detect_regions(godot::Image* img, godot::LocalVector<Region>& regions) {
    int w = img->get_width(), h = img->get_height();
    godot::LocalVector<bool> visited;
    visited.resize(w * h);
    godot::LocalVector<Pt> stack;

    // 8 directions for neighborhood (orthogonal + diagonals)
    static const int dirs[8][2] = {
        {1,0},{-1,0},{0,1},{0,-1},
        {1,1},{1,-1},{-1,1},{-1,-1}
    };

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int idx = y * w + x;

            // Skip already visited or fully transparent pixels
            if (visited[idx] || img->get_pixel(x, y).a == 0.0f) continue;

            // New region detected — initialize bounds
            int minx = x, maxx = x, miny = y, maxy = y, count = 0;
            stack.clear();
            stack.push_back({ x, y });
            visited[idx] = true;

            // Flood fill
            while (!stack.is_empty()) {
                Pt p = stack[stack.size() - 1];
                stack.remove_at(stack.size() - 1);

                img->set_pixel(p.x, p.y, godot::Color(0, 0, 0, 0));
                ++count;

                // Explore 8 neighbors
                for (auto& d : dirs) {
                    int nx = p.x + d[0], ny = p.y + d[1];
                    if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;

                    int nidx = ny * w + nx;
                    if (visited[nidx]) continue;

                    if (img->get_pixel(nx, ny).a > 0.0f) {
                        visited[nidx] = true;
                        stack.push_back({ nx, ny });

                        // Update bounds
                        minx = godot::MIN(minx, nx);
                        maxx = godot::MAX(maxx, nx);
                        miny = godot::MIN(miny, ny);
                        maxy = godot::MAX(maxy, ny);
                    }
                }
            }

            // Add region if it contains enough pixels
            if (count >= MIN_PIXELS) {
                regions.push_back({
                    godot::Rect2((float)minx, (float)miny, float(maxx - minx + 1), float(maxy - miny + 1)),
                    count
                    });
            }
        }
    }
}

void SpriteCutterAutoSlicer::merge_regions(godot::LocalVector<Region>& regions) {
    // Insertion sort by X to make spatial merging faster
    for (int i = 1; i < (int)regions.size(); ++i) {
        Region key = regions[i];
        int j = i - 1;
        while (j >= 0 && regions[j].rect.position.x > key.rect.position.x) {
            regions[j + 1] = regions[j];
            --j;
        }
        regions[j + 1] = key;
    }

    // Merge overlapping/nearby regions using a margin
    bool merged = true;
    while (merged) {
        merged = false;
        for (int i = 0; i < (int)regions.size(); ++i) {
            for (int j = i + 1; j < (int)regions.size(); ++j) {
                if (regions[i].rect.grow(MERGE_MARGIN).intersects(regions[j].rect)) {
                    godot::Rect2 A = regions[i].rect, B = regions[j].rect;

                    // Create new bounding rect that covers both
                    float x1 = godot::MIN(A.position.x, B.position.x);
                    float y1 = godot::MIN(A.position.y, B.position.y);
                    float x2 = godot::MAX(A.position.x + A.size.x, B.position.x + B.size.x);
                    float y2 = godot::MAX(A.position.y + A.size.y, B.position.y + B.size.y);

                    regions[i].rect = godot::Rect2(x1, y1, x2 - x1, y2 - y1);
                    regions[i].count += regions[j].count;

                    regions.remove_at(j);
                    merged = true;
                    break;
                }
            }
            if (merged) break;
        }
    }
}

godot::Array SpriteCutterAutoSlicer::build_atlas_textures(const godot::Ref<godot::Texture2D>& texture, const godot::LocalVector<Region>& regions) {
    godot::Array subs;

    for (int i = 0; i < (int)regions.size(); ++i) {
        godot::Ref<godot::AtlasTexture> at;
        at.instantiate();

        at->set_atlas(texture);
        at->set_region(regions[i].rect);
        at->set_filter_clip(true);

        subs.append(godot::Variant(at));
    }

    return subs;
}
