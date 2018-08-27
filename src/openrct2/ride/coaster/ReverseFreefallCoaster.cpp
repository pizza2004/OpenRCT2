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
    SPR_REVERSE_FREEFALL_RC_STATION_SW_NE = 22162,
    SPR_REVERSE_FREEFALL_RC_STATION_NW_SE = 22163,
    SPR_REVERSE_FREEFALL_RC_FLAT_SW_NE = 22164,
    SPR_REVERSE_FREEFALL_RC_FLAT_NW_SE = 22165,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SW_NE = 22166,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_NW_SE = 22167,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_NE_SW = 22168,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SE_NW = 22169,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SUPPORTS_SW_NE = 22170,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SUPPORTS_NW_SE = 22171,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SUPPORTS_NE_SW = 22172,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SUPPORTS_SE_NW = 22173,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_0 = 22174,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_1 = 22175,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_2 = 22176,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_3 = 22177,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_4 = 22178,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_6 = 22179,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_0 = 22180,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_1 = 22181,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_2 = 22182,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_3 = 22183,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_4 = 22184,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_6 = 22185,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_0 = 22186,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_1 = 22187,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_2 = 22188,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_3 = 22189,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_4 = 22190,
    SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_6 = 22191,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_0 = 22192,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_1 = 22193,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_2 = 22194,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_3 = 22195,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_4 = 22196,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_6 = 22197,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_0 = 22198,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_1 = 22199,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_2 = 22200,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_3 = 22201,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_4 = 22202,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_6 = 22203,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_5 = 22204,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_0 = 22205,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_1 = 22206,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_2 = 22207,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_3 = 22208,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_4 = 22209,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_6 = 22210,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_5 = 22211,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_0 = 22212,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_1 = 22213,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_2 = 22214,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_3 = 22215,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_4 = 22216,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_6 = 22217,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_5 = 22218,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_0 = 22219,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_1 = 22220,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_2 = 22221,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_3 = 22222,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_4 = 22223,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_6 = 22224,
    SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_5 = 22225,
};

static constexpr const uint32_t reverse_freefall_rc_track_pieces_station[4] = {
    SPR_REVERSE_FREEFALL_RC_STATION_SW_NE,
    SPR_REVERSE_FREEFALL_RC_STATION_NW_SE,
    SPR_REVERSE_FREEFALL_RC_STATION_SW_NE,
    SPR_REVERSE_FREEFALL_RC_STATION_NW_SE,
};

static constexpr const uint32_t reverse_freefall_rc_track_pieces_slope[7][4] = {
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_0,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_0,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_0,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_0,
    },
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_1,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_1,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_1,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_1,
    },
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_2,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_2,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_2,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_2,
    },
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_3,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_3,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_3,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_3,
    },
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_4,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_4,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_4,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_4,
    },
    { 0, 0, 0, 0 },
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SW_NE_6,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NW_SE_6,
        SPR_REVERSE_FREEFALL_RC_SLOPE_NE_SW_6,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SE_NW_6,
    },
};

static constexpr const uint32_t reverse_freefall_rc_track_pieces_slope_supports[7][4] = {
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_0,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_0,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_0,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_0,
    },
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_1,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_1,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_1,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_1,
    },
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_2,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_2,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_2,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_2,
    },
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_3,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_3,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_3,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_3,
    },
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_4,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_4,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_4,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_4,
    },
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_5,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_5,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_5,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_5,
    },
    {
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SW_NE_6,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NW_SE_6,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_NE_SW_6,
        SPR_REVERSE_FREEFALL_RC_SLOPE_SUPPORTS_SE_NW_6,
    },
};

static constexpr const uint32_t reverse_freefall_rc_track_pieces_vertical[4] = {
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SW_NE,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_NW_SE,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_NE_SW,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SE_NW,
};

static constexpr const uint32_t reverse_freefall_rc_track_pieces_vertical_supports[4] = {
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SUPPORTS_SW_NE,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SUPPORTS_NW_SE,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SUPPORTS_NE_SW,
    SPR_REVERSE_FREEFALL_RC_VERTICAL_SUPPORTS_SE_NW,
};

static void paint_reverse_freefall_rc_flat(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    if (direction & 1)
    {
        uint32_t imageId = SPR_REVERSE_FREEFALL_RC_FLAT_NW_SE | session->TrackColours[SCHEME_TRACK];
        sub_98197C(session, imageId, 0, 0, 20, 32, 1, height, 6, 0, height);
        paint_util_push_tunnel_right(session, height, TUNNEL_6);
    }
    else
    {
        uint32_t imageId = SPR_REVERSE_FREEFALL_RC_FLAT_SW_NE | session->TrackColours[SCHEME_TRACK];
        sub_98197C(session, imageId, 0, 0, 32, 20, 1, height, 0, 6, height);
        paint_util_push_tunnel_left(session, height, TUNNEL_6);
    }

    wooden_a_supports_paint_setup(session, (direction & 1) ? 1 : 0, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void paint_reverse_freefall_rc_station(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    Ride* ride = get_ride(rideIndex);
    uint32_t imageId;

    if (direction == 0 || direction == 2)
    {
        // height -= 2 (height - 2)
        imageId = SPR_STATION_BASE_B_SW_NE | session->TrackColours[SCHEME_MISC];
        sub_98197C(session, imageId, 0, 0, 32, 28, 1, height - 2, 0, 2, height);
        // height += 2 (height)

        imageId = reverse_freefall_rc_track_pieces_station[direction] | session->TrackColours[SCHEME_TRACK];
        sub_98199C(session, imageId, 0, 0, 32, 20, 1, height, 0, 6, height);

        wooden_a_supports_paint_setup(
            session, (direction & 1) ? 1 : 0, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);
        paint_util_push_tunnel_left(session, height, TUNNEL_6);
    }
    else if (direction == 1 || direction == 3)
    {
        // height -= 2 (height - 2)
        imageId = SPR_STATION_BASE_B_NW_SE | session->TrackColours[SCHEME_MISC];
        sub_98197C(session, imageId, 0, 0, 28, 32, 1, height - 2, 2, 0, height);
        // height += 2 (height)

        imageId = reverse_freefall_rc_track_pieces_station[direction] | session->TrackColours[SCHEME_TRACK];
        sub_98199C(session, imageId, 0, 0, 20, 32, 1, height, 6, 0, height);

        wooden_a_supports_paint_setup(
            session, (direction & 1) ? 1 : 0, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);
        paint_util_push_tunnel_right(session, height, TUNNEL_6);
    }

    track_paint_util_draw_station_platform(session, ride, direction, height, 5, tileElement);

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void paint_reverse_freefall_rc_slope(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    TESTPAINT_IGNORE_ALL();

    static constexpr const int8_t bbHeights03[] = { 1, 6, 14, 37, 64 };
    static constexpr const int8_t bbHeights12[] = { 1, 6, 14, 27, 59 };
    static constexpr const int32_t supportHeights[] = { 48, 64, 128, 176, 208, 240, 240 };
    static constexpr const int32_t tunnelOffsets03[] = { 0, 0, 0, 16, 64 };

    uint32_t supportsImageId = reverse_freefall_rc_track_pieces_slope_supports[trackSequence][direction]
        | session->TrackColours[SCHEME_SUPPORTS];
    uint32_t trackImageId = reverse_freefall_rc_track_pieces_slope[trackSequence][direction]
        | session->TrackColours[SCHEME_TRACK];
    int8_t bbHeight;
    bool isDirection03 = (direction == 0 || direction == 3);
    switch (trackSequence)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            if (isDirection03)
            {
                bbHeight = bbHeights03[trackSequence];
                sub_98197C_rotated(session, direction, supportsImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);
                sub_98199C_rotated(session, direction, trackImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);

                int32_t tunnelOffset = tunnelOffsets03[trackSequence];
                if (direction & 1)
                {
                    paint_util_push_tunnel_right(session, height + tunnelOffset, TUNNEL_6);
                }
                else
                {
                    paint_util_push_tunnel_left(session, height + tunnelOffset, TUNNEL_6);
                }
            }
            else
            {
                bbHeight = bbHeights12[trackSequence];
                sub_98197C_rotated(session, direction, trackImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);
                sub_98199C_rotated(session, direction, supportsImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);
            }

            wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + supportHeights[trackSequence], 0x20);
            break;
        case 5:
            if (wooden_a_supports_paint_setup(
                    session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr))
            {
                uint32_t floorImageId;
                if (direction & 1)
                {
                    floorImageId = SPR_FLOOR_PLANKS_90_DEG | session->TrackColours[SCHEME_SUPPORTS];
                }
                else
                {
                    floorImageId = SPR_FLOOR_PLANKS | session->TrackColours[SCHEME_SUPPORTS];
                }
                sub_98197C(session, floorImageId, 0, 0, 26, 26, 126, height, 3, 3, height);
                sub_98199C_rotated(
                    session, direction, supportsImageId, 0, 0, isDirection03 ? 26 : 18, 26, 126, height, isDirection03 ? 3 : 11,
                    3, height);
            }
            else
            {
                sub_98197C_rotated(
                    session, direction, supportsImageId, 0, 0, isDirection03 ? 26 : 18, 26, 126, height, isDirection03 ? 3 : 11,
                    3, height);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + supportHeights[trackSequence], 0x20);
            break;
        case 6:
            if (isDirection03)
            {
                sub_98197C_rotated(session, direction, supportsImageId, 0, 0, 5, 20, 79, height, 0, 6, height + 128);
                sub_98199C_rotated(session, direction, trackImageId, 0, 0, 5, 20, 79, height, 0, 6, height + 128);
            }
            else
            {
                sub_98197C_rotated(session, direction, trackImageId, 0, 0, 1, 20, 126, height, 27, 6, height);
                sub_98199C_rotated(session, direction, supportsImageId, 0, 0, 1, 20, 126, height, 27, 6, height);
            }
            wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + supportHeights[trackSequence], 0x20);
            break;
    }
}

static void paint_reverse_freefall_rc_vertical(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const rct_tile_element* tileElement)
{
    uint32_t supportsImageId, trackImageId;
    switch (trackSequence)
    {
        case 0:
            supportsImageId = reverse_freefall_rc_track_pieces_vertical_supports[direction]
                | session->TrackColours[SCHEME_SUPPORTS];
            sub_98197C(session, supportsImageId, 0, 0, 26, 26, 79, height, 3, 3, height);
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 80, 0x20);
            break;
        case 1:
            trackImageId = reverse_freefall_rc_track_pieces_vertical[direction] | session->TrackColours[SCHEME_TRACK];
            if (direction == 0 || direction == 3)
            {
                sub_98197C_rotated(session, direction, trackImageId, 0, 0, 2, 20, 79, height, 0, 6, height);
            }
            else
            {
                sub_98197C_rotated(session, direction, trackImageId, 0, 0, 2, 20, 79, height, 30, 6, height);
            }
            paint_util_set_vertical_tunnel(session, height + 80);
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 80, 0x20);
            break;
    }
}

TRACK_PAINT_FUNCTION get_track_paint_function_reverse_freefall_rc(int32_t trackType, int32_t direction)
{
    switch (trackType)
    {
        case TRACK_ELEM_FLAT:
            return paint_reverse_freefall_rc_flat;
        case TRACK_ELEM_END_STATION:
        case TRACK_ELEM_BEGIN_STATION:
        case TRACK_ELEM_MIDDLE_STATION:
            return paint_reverse_freefall_rc_station;
        case TRACK_ELEM_REVERSE_FREEFALL_SLOPE:
            return paint_reverse_freefall_rc_slope;
        case TRACK_ELEM_REVERSE_FREEFALL_VERTICAL:
            return paint_reverse_freefall_rc_vertical;
    }
    return nullptr;
}
