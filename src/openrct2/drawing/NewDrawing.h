/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../common.h"

struct rct_drawpixelinfo;
struct rct_palette_entry;

extern rct_string_id DrawingEngineStringIds[3];

int32_t drawing_engine_get_type();
bool drawing_engine_requires_new_window(int32_t srcEngine, int32_t dstEngine);
void drawing_engine_init();
void drawing_engine_resize();
void drawing_engine_set_palette(const rct_palette_entry* colours);
void drawing_engine_copy_rect(int32_t x, int32_t y, int32_t width, int32_t height, int32_t dx, int32_t dy);
void drawing_engine_dispose();

rct_drawpixelinfo* drawing_engine_get_dpi();
bool drawing_engine_has_dirty_optimisations();
void drawing_engine_invalidate_image(uint32_t image);
void drawing_engine_set_vsync(bool vsync);
