/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "../../drawing/Drawing.h"
#include "../../interface/Viewport.h"
#include "../../paint/Paint.h"
#include "../../paint/Supports.h"
#include "../../paint/tile_element/Paint.TileElement.h"
#include "../../sprites.h"
#include "../../world/Map.h"
#include "../../world/Sprite.h"
#include "../RideData.h"
#include "../TrackData.h"
#include "../TrackPaint.h"

enum
{
    SPR_WOODEN_WILD_MOUSE_FLAT_SW_NE = 28535,
    SPR_WOODEN_WILD_MOUSE_FLAT_NW_SE = 28536,
    SPR_WOODEN_WILD_MOUSE_25_DEG_SW_NE = 28537,
    SPR_WOODEN_WILD_MOUSE_60_DEG_SW_NE = 28538,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_SW_NE = 28539,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_SW_NE = 28540,
    SPR_WOODEN_WILD_MOUSE_25_DEG_NE_SW = 28541,
    SPR_WOODEN_WILD_MOUSE_60_DEG_NE_SW = 28542,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_NE_SW = 28543,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_FRONT_NE_SW = 28544,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_NE_SW = 28545,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_FRONT_NE_SW = 28546,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_SW_NE = 28547,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_SW_NE = 28548,
    SPR_WOODEN_WILD_MOUSE_25_DEG_NW_SE = 28549,
    SPR_WOODEN_WILD_MOUSE_60_DEG_NW_SE = 28550,
    SPR_WOODEN_WILD_MOUSE_25_DEG_SE_NW = 28551,
    SPR_WOODEN_WILD_MOUSE_60_DEG_SE_NW = 28552,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_NW_SE = 28553,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_FRONT_NW_SE = 28554,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_SE_NW = 28555,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_SE_NW = 28556,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_SE_NW = 28557,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_SE_NW = 28558,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_NW_SE = 28559,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_FRONT_NW_SE = 28560,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_NE_SW = 28561,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_NE_SW = 28562,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_NW_SE = 28563,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_NW_SE = 28564,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_SW_NE = 28565,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_NW_SE = 28566,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_NE_SW = 28567,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_SE_NW = 28568,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_FRONT_NW_SE = 28569,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_FRONT_NE_SW = 28570,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_SW_NE = 28571,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_NW_SE = 28572,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_NE_SW = 28573,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_SE_NW = 28574,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_FRONT_NW_SE = 28575,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_FRONT_NE_SW = 28576,
    SPR_WOODEN_WILD_MOUSE_25_DEG_CHAIN_SW_NE = 28577,
    SPR_WOODEN_WILD_MOUSE_60_DEG_CHAIN_SW_NE = 28578,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_CHAIN_SW_NE = 28579,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_SW_NE = 28580,
    SPR_WOODEN_WILD_MOUSE_25_DEG_CHAIN_NE_SW = 28581,
    SPR_WOODEN_WILD_MOUSE_60_DEG_CHAIN_NE_SW = 28582,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_CHAIN_NE_SW = 28583,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_FRONT_NE_SW = 28584,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_CHAIN_NE_SW = 28585,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_FRONT_NE_SW = 28586,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_CHAIN_SW_NE = 28587,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_SW_NE = 28588,
    SPR_WOODEN_WILD_MOUSE_25_DEG_CHAIN_NW_SE = 28589,
    SPR_WOODEN_WILD_MOUSE_60_DEG_CHAIN_NW_SE = 28590,
    SPR_WOODEN_WILD_MOUSE_25_DEG_CHAIN_SE_NW = 28591,
    SPR_WOODEN_WILD_MOUSE_60_DEG_CHAIN_SE_NW = 28592,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_CHAIN_NW_SE = 28593,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_FRONT_NW_SE = 28594,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_CHAIN_SE_NW = 28595,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_SE_NW = 28596,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_CHAIN_SE_NW = 28597,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_SE_NW = 28598,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_CHAIN_NW_SE = 28599,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_FRONT_NW_SE = 28600,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_NE_SW = 28601,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_NE_SW = 28602,
    SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_NW_SE = 28603,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_NW_SE = 28604,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_SW_NE = 28605,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_NW_SE = 28606,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_NE_SW = 28607,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_SE_NW = 28608,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_FRONT_NW_SE = 28609,
    SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_FRONT_NE_SW = 28610,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_SW_NE = 28611,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_NW_SE = 28612,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_NE_SW = 28613,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_SE_NW = 28614,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_FRONT_NW_SE = 28615,
    SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_FRONT_NE_SW = 28616,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_1_SW_NE = 28617,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_1_NW_SE = 28618,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_1_NE_SW = 28619,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_1_SE_NW = 28620,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SW_SE_PART_0 = 28621,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SW_SE_PART_1 = 28622,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SW_SE_PART_2 = 28623,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NW_SW_PART_0 = 28624,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NW_SW_PART_1 = 28625,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NW_SW_PART_2 = 28626,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NE_NW_PART_0 = 28627,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NE_NW_PART_1 = 28628,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NE_NW_PART_2 = 28629,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SE_NE_PART_0 = 28630,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SE_NE_PART_1 = 28631,
    SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SE_NE_PART_2 = 28632,
};

/** rct2: 0x008A5464 */
static void wooden_wild_mouse_track_flat(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const uint32_t imageIds[4] = {
        SPR_WOODEN_WILD_MOUSE_FLAT_SW_NE,
        SPR_WOODEN_WILD_MOUSE_FLAT_NW_SE,
        SPR_WOODEN_WILD_MOUSE_FLAT_SW_NE,
        SPR_WOODEN_WILD_MOUSE_FLAT_NW_SE,
    };

    uint32_t imageId = imageIds[direction] | session->TrackColours[SCHEME_TRACK];
    sub_98196C_rotated(session, direction, imageId, 0, 6, 32, 20, 1, height);
    wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);
    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_0);
    paint_util_set_segment_support_height(
        session,
        paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_BC | SEGMENT_C0 | SEGMENT_C8 | SEGMENT_D4, direction),
        height, 0x20);
    paint_util_set_segment_support_height(
        session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void wooden_wild_mouse_track_station(
    paint_session* session, uint8_t rideIndex, [[maybe_unused]] uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const uint32_t imageIds[4][2] = {
        { SPR_WOODEN_WILD_MOUSE_FLAT_SW_NE, SPR_STATION_BASE_B_SW_NE },
        { SPR_WOODEN_WILD_MOUSE_FLAT_NW_SE, SPR_STATION_BASE_B_NW_SE },
        { SPR_WOODEN_WILD_MOUSE_FLAT_SW_NE, SPR_STATION_BASE_B_SW_NE },
        { SPR_WOODEN_WILD_MOUSE_FLAT_NW_SE, SPR_STATION_BASE_B_NW_SE },
    };

    sub_98197C_rotated(
        session, direction, imageIds[direction][1] | session->TrackColours[SCHEME_MISC], 0, 0, 32, 28, 1, height - 2, 0, 2,
        height);
    sub_98199C_rotated(
        session, direction, imageIds[direction][0] | session->TrackColours[SCHEME_TRACK], 0, 6, 32, 20, 1, height, 0, 0,
        height);
    wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);
    track_paint_util_draw_station(session, rideIndex, direction, height, tileElement);
    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

/** rct2: 0x008A5474 */
static void wooden_wild_mouse_track_25_deg_up(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const uint32_t imageIds[2][4] = {
        {
            SPR_WOODEN_WILD_MOUSE_25_DEG_SW_NE,
            SPR_WOODEN_WILD_MOUSE_25_DEG_NW_SE,
            SPR_WOODEN_WILD_MOUSE_25_DEG_NE_SW,
            SPR_WOODEN_WILD_MOUSE_25_DEG_SE_NW,
        },
        {
            SPR_WOODEN_WILD_MOUSE_25_DEG_CHAIN_SW_NE,
            SPR_WOODEN_WILD_MOUSE_25_DEG_CHAIN_NW_SE,
            SPR_WOODEN_WILD_MOUSE_25_DEG_CHAIN_NE_SW,
            SPR_WOODEN_WILD_MOUSE_25_DEG_CHAIN_SE_NW,
        },
    };

    uint8_t isChained = track_element_is_lift_hill(tileElement) ? 1 : 0;
    uint32_t imageId = imageIds[isChained][direction] | session->TrackColours[SCHEME_TRACK];
    sub_98197C_rotated(session, direction, imageId, 0, 2, 32, 25, 1, height, 0, 3, height);

    wooden_a_supports_paint_setup(
        session, direction & 1, 9 + direction, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_1);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_2);
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 56, 0x20);
}

/** rct2: 0x008A5484 */
static void wooden_wild_mouse_track_60_deg_up(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const uint32_t imageIds[2][4] = {
        {
            SPR_WOODEN_WILD_MOUSE_60_DEG_SW_NE,
            SPR_WOODEN_WILD_MOUSE_60_DEG_NW_SE,
            SPR_WOODEN_WILD_MOUSE_60_DEG_NE_SW,
            SPR_WOODEN_WILD_MOUSE_60_DEG_SE_NW,
        },
        {
            SPR_WOODEN_WILD_MOUSE_60_DEG_CHAIN_SW_NE,
            SPR_WOODEN_WILD_MOUSE_60_DEG_CHAIN_NW_SE,
            SPR_WOODEN_WILD_MOUSE_60_DEG_CHAIN_NE_SW,
            SPR_WOODEN_WILD_MOUSE_60_DEG_CHAIN_SE_NW,
        },
    };

    uint8_t isChained = track_element_is_lift_hill(tileElement) ? 1 : 0;
    uint32_t imageId = imageIds[isChained][direction] | session->TrackColours[SCHEME_TRACK];
    if (direction == 0 || direction == 3)
    {
        sub_98197C_rotated(session, direction, imageId, 0, 2, 32, 25, 1, height, 0, 3, height);
    }
    else
    {
        session->WoodenSupportsPrependTo = sub_98197C_rotated(
            session, direction, imageId, 0, 6, 2, 24, 93, height, 28, 4, height - 16);
    }

    wooden_a_supports_paint_setup(
        session, direction & 1, 21 + direction, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_1);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 56, TUNNEL_2);
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 104, 0x20);
}

/** rct2: 0x008A5494 */
static void wooden_wild_mouse_track_flat_to_25_deg_up(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const uint32_t imageIds[2][4] = {
        {
            SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_SW_NE,
            SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_NW_SE,
            SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_NE_SW,
            SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_SE_NW,
        },
        {
            SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_CHAIN_SW_NE,
            SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_CHAIN_NW_SE,
            SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_CHAIN_NE_SW,
            SPR_WOODEN_WILD_MOUSE_FLAT_TO_25_DEG_CHAIN_SE_NW,
        },
    };

    uint8_t isChained = track_element_is_lift_hill(tileElement) ? 1 : 0;
    uint32_t imageId = imageIds[isChained][direction] | session->TrackColours[SCHEME_TRACK];
    sub_98197C_rotated(session, direction, imageId, 0, 2, 32, 25, 1, height, 0, 3, height);

    wooden_a_supports_paint_setup(
        session, direction & 1, 1 + direction, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_0);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_2);
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 48, 0x20);
}

/** rct2: 0x008A54A4 */
static void wooden_wild_mouse_track_25_deg_up_to_60_deg_up(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const uint32_t imageIds[2][4][2] = {
        {
            { SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_SW_NE, 0 },
            { SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_NW_SE, SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_FRONT_NW_SE },
            { SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_NE_SW, SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_FRONT_NE_SW },
            { SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_SE_NW, 0 },
        },
        {
            { SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_SW_NE, 0 },
            { SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_NW_SE, SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_FRONT_NW_SE },
            { SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_NE_SW, SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_FRONT_NE_SW },
            { SPR_WOODEN_WILD_MOUSE_25_DEG_TO_60_DEG_CHAIN_SE_NW, 0 },
        },
    };

    uint8_t isChained = track_element_is_lift_hill(tileElement) ? 1 : 0;
    uint32_t imageId;
    if (direction == 0 || direction == 3)
    {
        imageId = imageIds[isChained][direction][0] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 2, 32, 25, 1, height, 0, 3, height);
    }
    else
    {
        imageId = imageIds[isChained][direction][0] | session->TrackColours[SCHEME_TRACK];
        session->WoodenSupportsPrependTo = sub_98197C_rotated(
            session, direction, imageId, 0, 6, 2, 24, 43, height, 28, 4, height + 2);
        imageId = imageIds[isChained][direction][1] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 6, 32, 2, 43, height, 0, 4, height);
    }

    wooden_a_supports_paint_setup(
        session, direction & 1, 13 + direction, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_1);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 24, TUNNEL_2);
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 72, 0x20);
}

static void wooden_wild_mouse_track_60_deg_to_25_deg_up(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const uint32_t imageIds[2][4][2] = {
        {
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_SW_NE, 0 },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_NW_SE, SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_FRONT_NW_SE },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_NE_SW, SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_FRONT_NE_SW },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_SE_NW, 0 },
        },
        {
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_SW_NE, 0 },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_NW_SE, SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_FRONT_NW_SE },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_NE_SW, SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_FRONT_NE_SW },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_25_DEG_CHAIN_SE_NW, 0 },
        },
    };

    uint8_t isChained = track_element_is_lift_hill(tileElement) ? 1 : 0;
    uint32_t imageId;
    if (direction == 0 || direction == 3)
    {
        imageId = imageIds[isChained][direction][0] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 2, 32, 25, 1, height, 0, 3, height);
    }
    else
    {
        imageId = imageIds[isChained][direction][0] | session->TrackColours[SCHEME_TRACK];
        session->WoodenSupportsPrependTo = sub_98197C_rotated(
            session, direction, imageId, 0, 6, 2, 24, 43, height, 28, 4, height + 2);
        imageId = imageIds[isChained][direction][1] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 6, 32, 2, 43, height, 0, 4, height);
    }

    wooden_a_supports_paint_setup(
        session, direction & 1, 17 + direction, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_1);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 24, TUNNEL_2);
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 72, 0x20);
}

/** rct2: 0x008A54C4 */
static void wooden_wild_mouse_track_25_deg_up_to_flat(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const uint32_t imageIds[2][4] = {
        {
            SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_SW_NE,
            SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_NW_SE,
            SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_NE_SW,
            SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_SE_NW,
        },
        {
            SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_CHAIN_SW_NE,
            SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_CHAIN_NW_SE,
            SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_CHAIN_NE_SW,
            SPR_WOODEN_WILD_MOUSE_25_DEG_TO_FLAT_CHAIN_SE_NW,
        },
    };

    uint8_t isChained = track_element_is_lift_hill(tileElement) ? 1 : 0;
    uint32_t imageId = imageIds[isChained][direction] | session->TrackColours[SCHEME_TRACK];
    sub_98197C_rotated(session, direction, imageId, 0, 2, 32, 25, 1, height, 0, 3, height);

    wooden_a_supports_paint_setup(
        session, direction & 1, 5 + direction, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_0);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_12);
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 40, 0x20);
}

/** rct2: 0x008A54D4 */
static void wooden_wild_mouse_track_25_deg_down(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    wooden_wild_mouse_track_25_deg_up(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

/** rct2: 0x008A54E4 */
static void wooden_wild_mouse_track_60_deg_down(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    wooden_wild_mouse_track_60_deg_up(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

/** rct2: 0x008A54F4 */
static void wooden_wild_mouse_track_flat_to_25_deg_down(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    wooden_wild_mouse_track_25_deg_up_to_flat(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

/** rct2: 0x008A5504 */
static void wooden_wild_mouse_track_25_deg_down_to_60_deg_down(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    wooden_wild_mouse_track_60_deg_to_25_deg_up(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

/** rct2: 0x008A5514 */
static void wooden_wild_mouse_track_60_deg_down_to_25_deg_down(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    wooden_wild_mouse_track_25_deg_up_to_60_deg_up(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

/** rct2: 0x008A5524 */
static void wooden_wild_mouse_track_25_deg_down_to_flat(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    wooden_wild_mouse_track_flat_to_25_deg_up(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

static void wooden_wild_mouse_track_right_quarter_turn_3(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const sprite_bb imageIds[4][3] = {
        {
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SW_SE_PART_0, { 0, 6, 0 }, { 0, 0, 0 }, { 32, 20, 1 } },
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SW_SE_PART_1, { 16, 16, 0 }, { 0, 0, 0 }, { 16, 16, 1 } },
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SW_SE_PART_2, { 6, 0, 0 }, { 0, 0, 0 }, { 20, 32, 1 } },
        },
        {
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NW_SW_PART_0, { 6, 0, 0 }, { 0, 0, 0 }, { 20, 32, 1 } },
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NW_SW_PART_1, { 16, 0, 0 }, { 0, 0, 0 }, { 16, 16, 1 } },
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NW_SW_PART_2, { 0, 6, 0 }, { 0, 0, 0 }, { 32, 20, 1 } },
        },
        {
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NE_NW_PART_0, { 0, 6, 0 }, { 0, 0, 0 }, { 32, 20, 1 } },
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NE_NW_PART_1, { 0, 0, 0 }, { 0, 0, 0 }, { 16, 16, 1 } },
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_NE_NW_PART_2, { 6, 0, 0 }, { 0, 0, 0 }, { 20, 32, 1 } },
        },
        {
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SE_NE_PART_0, { 6, 0, 0 }, { 0, 0, 0 }, { 20, 32, 1 } },
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SE_NE_PART_1, { 0, 16, 0 }, { 0, 0, 0 }, { 16, 16, 1 } },
            { SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_3_SE_NE_PART_2, { 0, 6, 0 }, { 0, 0, 0 }, { 32, 20, 1 } },
        }
    };
    static uint8_t supportType[] = { 4, 5, 2, 3 };

    track_paint_util_right_quarter_turn_3_tiles_paint_4(
        session, height, direction, trackSequence, session->TrackColours[SCHEME_TRACK], imageIds);
    track_paint_util_right_quarter_turn_3_tiles_tunnel(session, height, direction, trackSequence, TUNNEL_0);

    switch (trackSequence)
    {
        case 0:
        case 3:
            wooden_a_supports_paint_setup(
                session, supportType[direction], 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);
            break;
    }

    int32_t blockedSegments = 0;
    switch (trackSequence)
    {
        case 0:
            blockedSegments = SEGMENTS_ALL;
            break;
        case 2:
            blockedSegments = SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4;
            break;
        case 3:
            blockedSegments = SEGMENTS_ALL;
            break;
    }
    paint_util_set_segment_support_height(session, paint_util_rotate_segments(blockedSegments, direction), 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void wooden_wild_mouse_track_left_quarter_turn_3(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    trackSequence = mapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
    wooden_wild_mouse_track_right_quarter_turn_3(session, rideIndex, trackSequence, (direction + 1) % 4, height, tileElement);
}

static void wooden_wild_mouse_track_left_quarter_turn_1(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const uint32_t imageIds[4] = {
        SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_1_SW_NE,
        SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_1_NW_SE,
        SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_1_NE_SW,
        SPR_WOODEN_WILD_MOUSE_QUARTER_TURN_1_SE_NW,
    };
    static uint8_t supportType[] = { 5, 2, 3, 4 };

    uint32_t imageId = imageIds[direction] | session->TrackColours[SCHEME_TRACK];
    switch (direction)
    {
        case 0:
            sub_98197C(session, imageId, 6, 0, 26, 24, 1, height, 6, 2, height);
            break;
        case 1:
            sub_98196C(session, imageId, 0, 0, 26, 26, 1, height);
            break;
        case 2:
            sub_98197C(session, imageId, 0, 6, 24, 26, 1, height, 2, 6, height);
            break;
        case 3:
            sub_98196C(session, imageId, 6, 6, 24, 24, 1, height);
            break;
    }
    wooden_a_supports_paint_setup(session, supportType[direction], 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);
    track_paint_util_left_quarter_turn_1_tile_tunnel(session, direction, height, 0, TUNNEL_0, 0, TUNNEL_0);
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

/** rct2: 0x008A55D4 */
static void wooden_wild_mouse_track_right_quarter_turn_1(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    wooden_wild_mouse_track_left_quarter_turn_1(session, rideIndex, trackSequence, (direction - 1) & 3, height, tileElement);
}

/** rct2: 0x008A55E4 */
static void wooden_wild_mouse_track_flat_to_60_deg_up(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const uint32_t imageIds[2][4][2] = {
        {
            { SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_SW_NE, 0 },
            { SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_NW_SE, SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_FRONT_NW_SE },
            { SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_NE_SW, SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_FRONT_NE_SW },
            { SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_SE_NW, 0 },
        },
        {
            { SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_SW_NE, 0 },
            { SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_NW_SE, SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_FRONT_NW_SE },
            { SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_NE_SW, SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_FRONT_NE_SW },
            { SPR_WOODEN_WILD_MOUSE_FLAT_TO_60_DEG_CHAIN_SE_NW, 0 },
        },
    };

    uint8_t isChained = track_element_is_lift_hill(tileElement) ? 1 : 0;
    uint32_t imageId;
    if (direction == 0 || direction == 3)
    {
        imageId = imageIds[isChained][direction][0] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 2, 32, 25, 1, height, 0, 3, height);
    }
    else
    {
        imageId = imageIds[isChained][direction][0] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 6, 2, 24, 43, height, 28, 4, height + 2);
        imageId = imageIds[isChained][direction][1] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 6, 32, 2, 43, height, 0, 4, height);
    }

    wooden_a_supports_paint_setup(
        session, direction & 1, 29 + direction, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_0);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 24, TUNNEL_2);
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 64, 0x20);
}

/** rct2: 0x008A55F4 */
static void wooden_wild_mouse_track_60_deg_up_to_flat(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    static constexpr const uint32_t imageIds[2][4][2] = {
        {
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_SW_NE, 0 },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_NW_SE, SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_FRONT_NW_SE },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_NE_SW, SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_FRONT_NE_SW },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_SE_NW, 0 },
        },
        {
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_SW_NE, 0 },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_NW_SE, SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_FRONT_NW_SE },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_NE_SW, SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_FRONT_NE_SW },
            { SPR_WOODEN_WILD_MOUSE_60_DEG_TO_FLAT_CHAIN_SE_NW, 0 },
        },
    };

    uint8_t isChained = track_element_is_lift_hill(tileElement) ? 1 : 0;
    uint32_t imageId;
    if (direction == 0 || direction == 3)
    {
        imageId = imageIds[isChained][direction][0] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 2, 32, 25, 1, height, 0, 3, height);
    }
    else
    {
        imageId = imageIds[isChained][direction][0] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 6, 2, 24, 43, height, 28, 4, height + 2);
        imageId = imageIds[isChained][direction][1] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 6, 32, 2, 43, height, 0, 4, height);
    }

    wooden_a_supports_paint_setup(
        session, direction & 1, 33 + direction, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_1);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 24, TUNNEL_0);
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 72, 0x20);
}

/** rct2: 0x008A5604 */
static void wooden_wild_mouse_track_flat_to_60_deg_down(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    wooden_wild_mouse_track_60_deg_up_to_flat(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

/** rct2: 0x008A5614 */
static void wooden_wild_mouse_track_60_deg_down_to_flat(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    wooden_wild_mouse_track_flat_to_60_deg_up(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

TRACK_PAINT_FUNCTION get_track_paint_function_wooden_wild_mouse(int32_t trackType, int32_t direction)
{
    switch (trackType)
    {
        case TRACK_ELEM_FLAT:
            return wooden_wild_mouse_track_flat;
        case TRACK_ELEM_END_STATION:
        case TRACK_ELEM_BEGIN_STATION:
        case TRACK_ELEM_MIDDLE_STATION:
            return wooden_wild_mouse_track_station;
        case TRACK_ELEM_25_DEG_UP:
            return wooden_wild_mouse_track_25_deg_up;
        case TRACK_ELEM_60_DEG_UP:
            return wooden_wild_mouse_track_60_deg_up;
        case TRACK_ELEM_FLAT_TO_25_DEG_UP:
            return wooden_wild_mouse_track_flat_to_25_deg_up;
        case TRACK_ELEM_25_DEG_UP_TO_60_DEG_UP:
            return wooden_wild_mouse_track_25_deg_up_to_60_deg_up;
        case TRACK_ELEM_60_DEG_UP_TO_25_DEG_UP:
            return wooden_wild_mouse_track_60_deg_to_25_deg_up;
        case TRACK_ELEM_25_DEG_UP_TO_FLAT:
            return wooden_wild_mouse_track_25_deg_up_to_flat;
        case TRACK_ELEM_25_DEG_DOWN:
            return wooden_wild_mouse_track_25_deg_down;
        case TRACK_ELEM_60_DEG_DOWN:
            return wooden_wild_mouse_track_60_deg_down;
        case TRACK_ELEM_FLAT_TO_25_DEG_DOWN:
            return wooden_wild_mouse_track_flat_to_25_deg_down;
        case TRACK_ELEM_25_DEG_DOWN_TO_60_DEG_DOWN:
            return wooden_wild_mouse_track_25_deg_down_to_60_deg_down;
        case TRACK_ELEM_60_DEG_DOWN_TO_25_DEG_DOWN:
            return wooden_wild_mouse_track_60_deg_down_to_25_deg_down;
        case TRACK_ELEM_25_DEG_DOWN_TO_FLAT:
            return wooden_wild_mouse_track_25_deg_down_to_flat;
        case TRACK_ELEM_LEFT_QUARTER_TURN_3_TILES:
            return wooden_wild_mouse_track_left_quarter_turn_3;
        case TRACK_ELEM_RIGHT_QUARTER_TURN_3_TILES:
            return wooden_wild_mouse_track_right_quarter_turn_3;
        case TRACK_ELEM_LEFT_QUARTER_TURN_1_TILE:
            return wooden_wild_mouse_track_left_quarter_turn_1;
        case TRACK_ELEM_RIGHT_QUARTER_TURN_1_TILE:
            return wooden_wild_mouse_track_right_quarter_turn_1;
        case TRACK_ELEM_FLAT_TO_60_DEG_UP:
            return wooden_wild_mouse_track_flat_to_60_deg_up;
        case TRACK_ELEM_60_DEG_UP_TO_FLAT:
            return wooden_wild_mouse_track_60_deg_up_to_flat;
        case TRACK_ELEM_FLAT_TO_60_DEG_DOWN:
            return wooden_wild_mouse_track_flat_to_60_deg_down;
        case TRACK_ELEM_60_DEG_DOWN_TO_FLAT:
            return wooden_wild_mouse_track_60_deg_down_to_flat;
    }
    return nullptr;
}
