/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include <openrct2-ui/interface/Dropdown.h>
#include <openrct2-ui/interface/Widget.h>
#include <openrct2-ui/windows/Window.h>
#include <openrct2/Game.h>
#include <openrct2/actions/ParkSetResearchFundingAction.hpp>
#include <openrct2/localisation/Localisation.h>
#include <openrct2/management/Finance.h>
#include <openrct2/management/NewsItem.h>
#include <openrct2/management/Research.h>
#include <openrct2/ride/RideData.h>
#include <openrct2/sprites.h>
#include <openrct2/world/Park.h>
#include <openrct2/world/Scenery.h>

static constexpr const int32_t WH_DEVELOPMENT = 196;
static constexpr const int32_t WW_DEVELOPMENT = 300;
static constexpr const int32_t WH_FUNDING = 207;
static constexpr const int32_t WW_FUNDING = 320;

// clang-format off
enum {
    WINDOW_RESEARCH_PAGE_DEVELOPMENT,
    WINDOW_RESEARCH_PAGE_FUNDING,
    WINDOW_RESEARCH_PAGE_COUNT
};

enum {
    WIDX_BACKGROUND,
    WIDX_TITLE,
    WIDX_CLOSE,
    WIDX_PAGE_BACKGROUND,
    WIDX_TAB_1,
    WIDX_TAB_2,

    WIDX_CURRENTLY_IN_DEVELOPMENT_GROUP,
    WIDX_LAST_DEVELOPMENT_GROUP,
    WIDX_LAST_DEVELOPMENT_BUTTON,

    WIDX_FUNDING_GROUP = 6,
    WIDX_RESEARCH_FUNDING,
    WIDX_RESEARCH_FUNDING_DROPDOWN_BUTTON,
    WIDX_PRIORITIES_GROUP,
    WIDX_TRANSPORT_RIDES,
    WIDX_GENTLE_RIDES,
    WIDX_ROLLER_COASTERS,
    WIDX_THRILL_RIDES,
    WIDX_WATER_RIDES,
    WIDX_SHOPS_AND_STALLS,
    WIDX_SCENERY_AND_THEMING,
};

#pragma region Widgets

static rct_widget window_research_development_widgets[] = {
    WINDOW_SHIM(STR_RESEARCH_AND_DEVELOPMENT, WW_DEVELOPMENT, WH_DEVELOPMENT),
    MakeWidget     ({  0,  43}, {     WW_DEVELOPMENT, 153}, WWT_RESIZE,   1                                                                ),
    MakeRemapWidget({  3,  17}, {                 31,  27}, WWT_TAB,      1, SPR_TAB,                      STR_RESEARCH_AND_DEVELOPMENT_TIP),
    MakeRemapWidget({ 34,  17}, {                 31,  27}, WWT_TAB,      1, SPR_TAB,                      STR_FINANCES_RESEARCH_TIP       ),
    MakeWidget     ({  3,  47}, {WW_DEVELOPMENT - 10,  70}, WWT_GROUPBOX, 2, STR_CURRENTLY_IN_DEVELOPMENT                                  ),
    MakeWidget     ({  3, 124}, {WW_DEVELOPMENT - 10,  65}, WWT_GROUPBOX, 2, STR_LAST_DEVELOPMENT                                          ),
    MakeWidget     ({265, 161}, {                 24,  24}, WWT_FLATBTN,  2, 0xFFFFFFFF,                   STR_RESEARCH_SHOW_DETAILS_TIP   ),
    { WIDGETS_END },
};

static rct_widget window_research_funding_widgets[] = {
    WINDOW_SHIM(STR_RESEARCH_FUNDING, WW_FUNDING, WH_FUNDING),
    MakeWidget     ({  0,  43}, {     WW_FUNDING, 164}, WWT_RESIZE,   1                                                                                    ),
    MakeRemapWidget({  3,  17}, {             31,  27}, WWT_TAB,      1, SPR_TAB,                              STR_RESEARCH_AND_DEVELOPMENT_TIP            ),
    MakeRemapWidget({ 34,  17}, {             31,  27}, WWT_TAB,      1, SPR_TAB,                              STR_FINANCES_RESEARCH_TIP                   ),
    MakeWidget     ({  3,  47}, { WW_FUNDING - 6,  45}, WWT_GROUPBOX, 2, STR_RESEARCH_FUNDING_                                                             ),
    MakeWidget     ({  8,  59}, {            160,  14}, WWT_DROPDOWN, 2, 0xFFFFFFFF,                           STR_SELECT_LEVEL_OF_RESEARCH_AND_DEVELOPMENT),
    MakeWidget     ({156,  60}, {             11,  12}, WWT_BUTTON,   2, STR_DROPDOWN_GLYPH,                   STR_SELECT_LEVEL_OF_RESEARCH_AND_DEVELOPMENT),
    MakeWidget     ({  3,  96}, { WW_FUNDING - 6, 107}, WWT_GROUPBOX, 2, STR_RESEARCH_PRIORITIES                                                           ),
    MakeWidget     ({  8, 108}, {WW_FUNDING - 16,  12}, WWT_CHECKBOX, 2, STR_RESEARCH_NEW_TRANSPORT_RIDES,     STR_RESEARCH_NEW_TRANSPORT_RIDES_TIP        ),
    MakeWidget     ({  8, 121}, {WW_FUNDING - 16,  12}, WWT_CHECKBOX, 2, STR_RESEARCH_NEW_GENTLE_RIDES,        STR_RESEARCH_NEW_GENTLE_RIDES_TIP           ),
    MakeWidget     ({  8, 134}, {WW_FUNDING - 16,  12}, WWT_CHECKBOX, 2, STR_RESEARCH_NEW_ROLLER_COASTERS,     STR_RESEARCH_NEW_ROLLER_COASTERS_TIP        ),
    MakeWidget     ({  8, 147}, {WW_FUNDING - 16,  12}, WWT_CHECKBOX, 2, STR_RESEARCH_NEW_THRILL_RIDES,        STR_RESEARCH_NEW_THRILL_RIDES_TIP           ),
    MakeWidget     ({  8, 160}, {WW_FUNDING - 16,  12}, WWT_CHECKBOX, 2, STR_RESEARCH_NEW_WATER_RIDES,         STR_RESEARCH_NEW_WATER_RIDES_TIP            ),
    MakeWidget     ({  8, 173}, {WW_FUNDING - 16,  12}, WWT_CHECKBOX, 2, STR_RESEARCH_NEW_SHOPS_AND_STALLS,    STR_RESEARCH_NEW_SHOPS_AND_STALLS_TIP       ),
    MakeWidget     ({  8, 186}, {WW_FUNDING - 16,  12}, WWT_CHECKBOX, 2, STR_RESEARCH_NEW_SCENERY_AND_THEMING, STR_RESEARCH_NEW_SCENERY_AND_THEMING_TIP    ),
    { WIDGETS_END },
};

static rct_widget *window_research_page_widgets[] = {
    window_research_development_widgets,
    window_research_funding_widgets
};

#pragma endregion

#pragma region Events

static void window_research_development_mouseup(rct_window *w, rct_widgetindex widgetIndex);
static void window_research_development_update(rct_window *w);
static void window_research_development_invalidate(rct_window *w);
static void window_research_development_paint(rct_window *w, rct_drawpixelinfo *dpi);

static void window_research_funding_mouseup(rct_window *w, rct_widgetindex widgetIndex);
static void window_research_funding_mousedown(rct_window *w, rct_widgetindex widgetIndex, rct_widget* widget);
static void window_research_funding_dropdown(rct_window *w, rct_widgetindex widgetIndex, int32_t dropdownIndex);
static void window_research_funding_update(rct_window *w);
static void window_research_funding_invalidate(rct_window *w);
static void window_research_funding_paint(rct_window *w, rct_drawpixelinfo *dpi);

//
static rct_window_event_list window_research_development_events = {
    nullptr,
    window_research_development_mouseup,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    window_research_development_update,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    window_research_development_invalidate,
    window_research_development_paint,
    nullptr
};

// 0x009890E8
static rct_window_event_list window_research_funding_events = {
    nullptr,
    window_research_funding_mouseup,
    nullptr,
    window_research_funding_mousedown,
    window_research_funding_dropdown,
    nullptr,
    window_research_funding_update,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    window_research_funding_invalidate,
    window_research_funding_paint,
    nullptr
};

static rct_window_event_list *window_research_page_events[] = {
    &window_research_development_events,
    &window_research_funding_events
};

#pragma endregion

#pragma region Enabled widgets

static uint32_t window_research_page_enabled_widgets[] = {
    (1 << WIDX_CLOSE) |
    (1 << WIDX_TAB_1) |
    (1 << WIDX_TAB_2) |
    (1 << WIDX_LAST_DEVELOPMENT_BUTTON),

    (1 << WIDX_CLOSE) |
    (1 << WIDX_TAB_1) |
    (1 << WIDX_TAB_2) |
    (1 << WIDX_RESEARCH_FUNDING) |
    (1 << WIDX_RESEARCH_FUNDING_DROPDOWN_BUTTON) |
    (1 << WIDX_TRANSPORT_RIDES) |
    (1 << WIDX_GENTLE_RIDES) |
    (1 << WIDX_ROLLER_COASTERS) |
    (1 << WIDX_THRILL_RIDES) |
    (1 << WIDX_WATER_RIDES) |
    (1 << WIDX_SHOPS_AND_STALLS) |
    (1 << WIDX_SCENERY_AND_THEMING)
};

#pragma endregion

const int32_t window_research_tab_animation_loops[] = { 16, 16 };

static constexpr const rct_string_id ResearchCategoryNames[] = {
    STR_RESEARCH_CATEGORY_TRANSPORT,
    STR_RESEARCH_CATEGORY_GENTLE,
    STR_RESEARCH_CATEGORY_ROLLERCOASTER,
    STR_RESEARCH_CATEGORY_THRILL,
    STR_RESEARCH_CATEGORY_WATER,
    STR_RESEARCH_CATEGORY_SHOP,
    STR_RESEARCH_CATEGORY_SCENERY_GROUP,
};

static constexpr const rct_string_id ResearchStageNames[] = {
    STR_RESEARCH_STAGE_INITIAL_RESEARCH,
    STR_RESEARCH_STAGE_DESIGNING,
    STR_RESEARCH_STAGE_COMPLETING_DESIGN,
    STR_RESEARCH_STAGE_UNKNOWN,
};
// clang-format on

static void window_research_set_page(rct_window* w, int32_t page);
static void window_research_set_pressed_tab(rct_window* w);
static void window_research_draw_tab_images(rct_drawpixelinfo* dpi, rct_window* w);

rct_window* window_research_open()
{
    rct_window* w;

    w = window_bring_to_front_by_class(WC_RESEARCH);
    if (w == nullptr)
    {
        w = window_create_auto_pos(WW_FUNDING, WH_FUNDING, window_research_page_events[0], WC_RESEARCH, WF_10);
        w->widgets = window_research_page_widgets[0];
        w->enabled_widgets = window_research_page_enabled_widgets[0];
        w->number = 0;
        w->page = 0;
        w->frame_no = 0;
        w->disabled_widgets = 0;
        research_update_uncompleted_types();
    }

    w->page = 0;
    w->Invalidate();
    w->width = WW_DEVELOPMENT;
    w->height = WH_DEVELOPMENT;
    w->Invalidate();

    w->widgets = window_research_page_widgets[0];
    w->enabled_widgets = window_research_page_enabled_widgets[0];
    w->hold_down_widgets = 0;
    w->event_handlers = window_research_page_events[0];
    w->pressed_widgets = 0;
    w->disabled_widgets = 0;
    window_init_scroll_widgets(w);

    return w;
}

#pragma region Development page

/**
 *
 *  rct2: 0x006B6B38
 */
static void window_research_development_mouseup(rct_window* w, rct_widgetindex widgetIndex)
{
    switch (widgetIndex)
    {
        case WIDX_CLOSE:
            window_close(w);
            break;
        case WIDX_TAB_1:
        case WIDX_TAB_2:
            window_research_set_page(w, widgetIndex - WIDX_TAB_1);
            break;
        case WIDX_LAST_DEVELOPMENT_BUTTON:
            news_item_open_subject(News::ItemType::Research, gResearchLastItem->rawValue);
            break;
    }
}

/**
 *
 *  rct2: 0x0069CBA6
 */
static void window_research_development_update(rct_window* w)
{
    // Tab animation
    if (++w->frame_no >= window_research_tab_animation_loops[w->page])
        w->frame_no = 0;
    widget_invalidate(w, WIDX_TAB_1);
}

/**
 *
 *  rct2: 0x006B6819
 */
static void window_research_development_invalidate(rct_window* w)
{
    if (w->widgets != window_research_page_widgets[WINDOW_RESEARCH_PAGE_DEVELOPMENT])
    {
        w->widgets = window_research_page_widgets[WINDOW_RESEARCH_PAGE_DEVELOPMENT];
        window_init_scroll_widgets(w);
    }

    window_research_set_pressed_tab(w);

    window_research_development_widgets[WIDX_LAST_DEVELOPMENT_BUTTON].type = WWT_EMPTY;
    if (gResearchLastItem.has_value())
    {
        auto type = gResearchLastItem->type;
        window_research_development_widgets[WIDX_LAST_DEVELOPMENT_BUTTON].type = WWT_FLATBTN;
        window_research_development_widgets[WIDX_LAST_DEVELOPMENT_BUTTON].image = type == Research::EntryType::Ride
            ? SPR_NEW_RIDE
            : SPR_NEW_SCENERY;
    }
}

/**
 *
 *  rct2: 0x006B689B
 */
static void window_research_development_paint(rct_window* w, rct_drawpixelinfo* dpi)
{
    window_draw_widgets(w, dpi);
    window_research_draw_tab_images(dpi, w);

    window_research_development_page_paint(w, dpi, WIDX_CURRENTLY_IN_DEVELOPMENT_GROUP);
}

void window_research_development_page_paint(rct_window* w, rct_drawpixelinfo* dpi, rct_widgetindex baseWidgetIndex)
{
    baseWidgetIndex = baseWidgetIndex - WIDX_CURRENTLY_IN_DEVELOPMENT_GROUP;

    auto screenCoords = w->windowPos
        + ScreenCoordsXY{ 10, w->widgets[WIDX_CURRENTLY_IN_DEVELOPMENT_GROUP + baseWidgetIndex].top + 12 };
    rct_string_id stringId;

    if (gResearchProgressStage == RESEARCH_STAGE_FINISHED_ALL)
    {
        stringId = STR_RESEARCH_UNKNOWN;
        gfx_draw_string_left_wrapped(dpi, &stringId, screenCoords, 296, STR_RESEARCH_TYPE_LABEL, COLOUR_BLACK);
        screenCoords.y += 25;

        // Progress
        stringId = STR_RESEARCH_COMPLETED_AL;
        gfx_draw_string_left_wrapped(dpi, &stringId, screenCoords, 296, STR_RESEARCH_PROGRESS_LABEL, COLOUR_BLACK);
        screenCoords.y += 15;

        auto ft = Formatter::Common();
        ft.Add<rct_string_id>(STR_RESEARCH_STAGE_UNKNOWN);
        gfx_draw_string_left(dpi, STR_RESEARCH_EXPECTED_LABEL, gCommonFormatArgs, COLOUR_BLACK, screenCoords);
    }
    else
    {
        // Research type
        std::array<rct_string_id, 2> strings = { STR_RESEARCH_UNKNOWN, 0 };
        rct_string_id label = STR_RESEARCH_TYPE_LABEL;
        if (gResearchProgressStage != RESEARCH_STAGE_INITIAL_RESEARCH)
        {
            strings[0] = ResearchCategoryNames[gResearchNextItem->category];
            if (gResearchProgressStage != RESEARCH_STAGE_DESIGNING)
            {
                strings[0] = gResearchNextItem->GetName();
                if (gResearchNextItem->type == Research::EntryType::Ride)
                {
                    auto rtd = RideTypeDescriptors[gResearchNextItem->baseRideType];
                    if (!rtd.HasFlag(RIDE_TYPE_FLAG_LIST_VEHICLES_SEPARATELY))
                    {
                        if (gResearchNextItem->flags & RESEARCH_ENTRY_FLAG_FIRST_OF_TYPE)
                        {
                            strings[0] = rtd.Naming.Name;
                        }
                        else
                        {
                            strings[1] = rtd.Naming.Name;
                            label = STR_RESEARCH_TYPE_LABEL_VEHICLE;
                        }
                    }
                }
            }
        }
        gfx_draw_string_left_wrapped(dpi, &strings, screenCoords, 296, label, COLOUR_BLACK);
        screenCoords.y += 25;

        // Progress
        stringId = ResearchStageNames[gResearchProgressStage];
        gfx_draw_string_left_wrapped(dpi, &stringId, screenCoords, 296, STR_RESEARCH_PROGRESS_LABEL, COLOUR_BLACK);
        screenCoords.y += 15;

        // Expected
        auto ft = Formatter::Common();
        if (gResearchProgressStage != RESEARCH_STAGE_INITIAL_RESEARCH && gResearchExpectedDay != 255)
        {
            // TODO: Should probably use game date format setting
            ft.Add<rct_string_id>(STR_RESEARCH_EXPECTED_FORMAT);
            ft.Add<rct_string_id>(DateDayNames[gResearchExpectedDay]);
            ft.Add<rct_string_id>(DateGameMonthNames[gResearchExpectedMonth]);
        }
        else
        {
            ft.Add<rct_string_id>(STR_RESEARCH_STAGE_UNKNOWN);
        }
        gfx_draw_string_left(dpi, STR_RESEARCH_EXPECTED_LABEL, gCommonFormatArgs, COLOUR_BLACK, screenCoords);
    }

    // Last development
    screenCoords = w->windowPos + ScreenCoordsXY{ 10, w->widgets[WIDX_LAST_DEVELOPMENT_GROUP + baseWidgetIndex].top + 12 };

    if (gResearchLastItem.has_value())
    {
        rct_string_id lastDevelopmentFormat = STR_EMPTY;
        std::array<rct_string_id, 2> strings = { gResearchLastItem->GetName(), 0 };
        auto type = gResearchLastItem->type;
        if (type == Research::EntryType::Scenery)
        {
            lastDevelopmentFormat = STR_RESEARCH_SCENERY_LABEL;
        }
        else
        {
            lastDevelopmentFormat = STR_RESEARCH_RIDE_LABEL;
            auto rtd = RideTypeDescriptors[gResearchLastItem->baseRideType];
            if (!rtd.HasFlag(RIDE_TYPE_FLAG_LIST_VEHICLES_SEPARATELY))
            {
                if (gResearchLastItem->flags & RESEARCH_ENTRY_FLAG_FIRST_OF_TYPE)
                {
                    strings[0] = rtd.Naming.Name;
                }
                else
                {
                    strings[1] = rtd.Naming.Name;
                    lastDevelopmentFormat = STR_RESEARCH_VEHICLE_LABEL;
                }
            }
        }

        gfx_draw_string_left_wrapped(dpi, &strings, screenCoords, 266, lastDevelopmentFormat, COLOUR_BLACK);
    }
}

#pragma endregion

#pragma region Funding page

/**
 *
 *  rct2: 0x0069DB3F
 */
static void window_research_funding_mouseup(rct_window* w, rct_widgetindex widgetIndex)
{
    switch (widgetIndex)
    {
        case WIDX_CLOSE:
            window_close(w);
            break;
        case WIDX_TAB_1:
        case WIDX_TAB_2:
            window_research_set_page(w, widgetIndex - WIDX_TAB_1);
            break;
        case WIDX_TRANSPORT_RIDES:
        case WIDX_GENTLE_RIDES:
        case WIDX_ROLLER_COASTERS:
        case WIDX_THRILL_RIDES:
        case WIDX_WATER_RIDES:
        case WIDX_SHOPS_AND_STALLS:
        case WIDX_SCENERY_AND_THEMING:
        {
            auto activeResearchTypes = gResearchPriorities;
            activeResearchTypes ^= 1 << (widgetIndex - WIDX_TRANSPORT_RIDES);
            auto gameAction = ParkSetResearchFundingAction(activeResearchTypes, gResearchFundingLevel);
            GameActions::Execute(&gameAction);
            break;
        }
    }
}

/**
 *
 *  rct2: 0x0069DB66
 */
static void window_research_funding_mousedown(rct_window* w, rct_widgetindex widgetIndex, rct_widget* widget)
{
    rct_widget* dropdownWidget;
    int32_t i;

    if (widgetIndex != WIDX_RESEARCH_FUNDING_DROPDOWN_BUTTON)
        return;

    dropdownWidget = widget - 1;

    for (i = 0; i < 4; i++)
    {
        gDropdownItemsFormat[i] = STR_DROPDOWN_MENU_LABEL;
        gDropdownItemsArgs[i] = ResearchFundingLevelNames[i];
    }
    window_dropdown_show_text_custom_width(
        { w->windowPos.x + dropdownWidget->left, w->windowPos.y + dropdownWidget->top }, dropdownWidget->height() + 1,
        w->colours[1], 0, DROPDOWN_FLAG_STAY_OPEN, 4, dropdownWidget->width() - 3);

    int32_t currentResearchLevel = gResearchFundingLevel;
    dropdown_set_checked(currentResearchLevel, true);
}

/**
 *
 *  rct2: 0x0069DB6D
 */
static void window_research_funding_dropdown(rct_window* w, rct_widgetindex widgetIndex, int32_t dropdownIndex)
{
    if (widgetIndex != WIDX_RESEARCH_FUNDING_DROPDOWN_BUTTON || dropdownIndex == -1)
        return;

    auto gameAction = ParkSetResearchFundingAction(gResearchPriorities, dropdownIndex);
    GameActions::Execute(&gameAction);
}

/**
 *
 *  rct2: 0x0069DC23
 */
static void window_research_funding_update(rct_window* w)
{
    // Tab animation
    if (++w->frame_no >= window_research_tab_animation_loops[w->page])
        w->frame_no = 0;
    widget_invalidate(w, WIDX_TAB_2);
}

/**
 *
 *  rct2: 0x0069DA64
 */
static void window_research_funding_invalidate(rct_window* w)
{
    if (w->widgets != window_research_page_widgets[WINDOW_RESEARCH_PAGE_FUNDING])
    {
        w->widgets = window_research_page_widgets[WINDOW_RESEARCH_PAGE_FUNDING];
        window_init_scroll_widgets(w);
    }

    window_research_set_pressed_tab(w);

    if ((gParkFlags & PARK_FLAGS_NO_MONEY) || (gResearchProgressStage == RESEARCH_STAGE_FINISHED_ALL))
    {
        window_research_funding_widgets[WIDX_RESEARCH_FUNDING].type = WWT_EMPTY;
        window_research_funding_widgets[WIDX_RESEARCH_FUNDING_DROPDOWN_BUTTON].type = WWT_EMPTY;
    }
    else
    {
        window_research_funding_widgets[WIDX_FUNDING_GROUP].type = WWT_GROUPBOX;
        window_research_funding_widgets[WIDX_RESEARCH_FUNDING].type = WWT_DROPDOWN;
        window_research_funding_widgets[WIDX_RESEARCH_FUNDING_DROPDOWN_BUTTON].type = WWT_BUTTON;

        // Current funding
        int32_t currentResearchLevel = gResearchFundingLevel;
        window_research_funding_widgets[WIDX_RESEARCH_FUNDING].text = ResearchFundingLevelNames[currentResearchLevel];
    }

    // Checkboxes
    uint8_t activeResearchTypes = gResearchPriorities;
    for (int32_t i = 0; i < 7; i++)
    {
        int32_t mask = 1 << i;
        int32_t widgetMask = 1 << (i + WIDX_TRANSPORT_RIDES);

        // Set checkbox disabled if research type is complete
        if (gResearchUncompletedCategories & mask)
        {
            w->disabled_widgets &= ~widgetMask;

            // Set checkbox ticked if research type is active
            if (activeResearchTypes & mask)
                w->pressed_widgets |= widgetMask;
            else
                w->pressed_widgets &= ~widgetMask;
        }
        else
        {
            w->disabled_widgets |= widgetMask;
            w->pressed_widgets &= ~widgetMask;
        }
    }
}

/**
 *
 *  rct2: 0x0069DAF0
 */
static void window_research_funding_paint(rct_window* w, rct_drawpixelinfo* dpi)
{
    window_draw_widgets(w, dpi);
    window_research_draw_tab_images(dpi, w);

    window_research_funding_page_paint(w, dpi, WIDX_RESEARCH_FUNDING);
}

void window_research_funding_page_paint(rct_window* w, rct_drawpixelinfo* dpi, rct_widgetindex baseWidgetIndex)
{
    if (gParkFlags & PARK_FLAGS_NO_MONEY)
        return;

    int32_t currentResearchLevel = gResearchFundingLevel;
    money32 currentResearchCostPerWeek = research_cost_table[currentResearchLevel];
    gfx_draw_string_left(
        dpi, STR_RESEARCH_COST_PER_MONTH, &currentResearchCostPerWeek, COLOUR_BLACK, w->windowPos + ScreenCoordsXY{ 10, 77 });
}

#pragma endregion

#pragma region Common

/**
 *
 *  rct2: 0x0069CAC5
 */
static void window_research_set_page(rct_window* w, int32_t page)
{
    w->page = page;
    w->frame_no = 0;
    if (w->viewport != nullptr)
    {
        w->viewport->width = 0;
        w->viewport = nullptr;
    }

    w->enabled_widgets = window_research_page_enabled_widgets[page];
    w->hold_down_widgets = 0;
    w->event_handlers = window_research_page_events[page];
    w->widgets = window_research_page_widgets[page];
    w->disabled_widgets = 0;
    w->pressed_widgets = 0;

    w->Invalidate();
    if (w->page == WINDOW_RESEARCH_PAGE_DEVELOPMENT)
    {
        w->width = WW_DEVELOPMENT;
        w->height = WH_DEVELOPMENT;
    }
    else
    {
        w->width = WW_FUNDING;
        w->height = WH_FUNDING;
    }
    window_event_resize_call(w);
    window_event_invalidate_call(w);

    window_init_scroll_widgets(w);
    w->Invalidate();
}

static void window_research_set_pressed_tab(rct_window* w)
{
    int32_t i;
    for (i = 0; i < WINDOW_RESEARCH_PAGE_COUNT; i++)
        w->pressed_widgets &= ~(1 << (WIDX_TAB_1 + i));
    w->pressed_widgets |= 1LL << (WIDX_TAB_1 + w->page);
}

static void window_research_draw_tab_image(rct_drawpixelinfo* dpi, rct_window* w, int32_t page, int32_t spriteIndex)
{
    rct_widgetindex widgetIndex = WIDX_TAB_1 + page;

    if (!(w->disabled_widgets & (1LL << widgetIndex)))
    {
        if (w->page == page)
        {
            int32_t frame = w->frame_no / 2;
            if (page == WINDOW_RESEARCH_PAGE_DEVELOPMENT)
                frame %= 8;
            spriteIndex += frame;
        }

        gfx_draw_sprite(
            dpi, spriteIndex, w->windowPos + ScreenCoordsXY{ w->widgets[widgetIndex].left, w->widgets[widgetIndex].top }, 0);
    }
}

static void window_research_draw_tab_images(rct_drawpixelinfo* dpi, rct_window* w)
{
    window_research_draw_tab_image(dpi, w, WINDOW_RESEARCH_PAGE_DEVELOPMENT, SPR_TAB_FINANCES_RESEARCH_0);
    window_research_draw_tab_image(dpi, w, WINDOW_RESEARCH_PAGE_FUNDING, SPR_TAB_FINANCES_SUMMARY_0);
}

#pragma endregion
