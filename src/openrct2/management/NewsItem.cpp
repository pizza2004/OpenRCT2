/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "NewsItem.h"

#include "../Context.h"
#include "../Input.h"
#include "../OpenRCT2.h"
#include "../audio/audio.h"
#include "../interface/Window.h"
#include "../interface/Window_internal.h"
#include "../localisation/Date.h"
#include "../localisation/Localisation.h"
#include "../management/Research.h"
#include "../ride/Ride.h"
#include "../util/Util.h"
#include "../windows/Intent.h"
#include "../world/Location.hpp"
#include "../world/Sprite.h"

NewsItemQueues gNewsItems;

NewsItem::Object& NewsItemQueues::Current()
{
    return Recent.front();
}

const NewsItem::Object& NewsItemQueues::Current() const
{
    return Recent.front();
}

bool news_item_is_valid_idx(int32_t index)
{
    if (index >= NewsItem::MaxItems)
    {
        log_error("Tried to get news item past MAX_NEWS.");
        return false;
    }
    return true;
}

NewsItem::Object* news_item_get(int32_t index)
{
    return gNewsItems.At(index);
}

NewsItem::Object& NewsItemQueues::operator[](size_t index)
{
    return const_cast<NewsItem::Object&>(const_cast<const NewsItemQueues&>(*this)[index]);
}

const NewsItem::Object& NewsItemQueues::operator[](size_t index) const
{
    if (index < Recent.capacity())
        return Recent[index];
    else
        return Archived[index - Recent.capacity()];
}

NewsItem::Object* NewsItemQueues::At(int32_t index)
{
    return const_cast<NewsItem::Object*>(const_cast<const NewsItemQueues&>(*this).At(index));
}

const NewsItem::Object* NewsItemQueues::At(int32_t index) const
{
    if (news_item_is_valid_idx(index))
    {
        return &(*this)[index];
    }
    else
    {
        return nullptr;
    }
}

bool news_item_is_queue_empty()
{
    return gNewsItems.IsEmpty();
}

bool NewsItemQueues::IsEmpty() const
{
    return Recent.empty();
}

/**
 *
 *  rct2: 0x0066DF32
 */
void NewsItemQueues::Clear()
{
    Recent.clear();
    Archived.clear();
}

void news_item_init_queue()
{
    gNewsItems.Clear();
    assert(gNewsItems.IsEmpty());

    // Throttles for warning types (PEEP_*_WARNING)
    for (auto& warningThrottle : gPeepWarningThrottle)
    {
        warningThrottle = 0;
    }

    auto intent = Intent(INTENT_ACTION_INVALIDATE_TICKER_NEWS);
    context_broadcast_intent(&intent);
}

uint16_t NewsItemQueues::IncrementTicks()
{
    return ++Current().Ticks;
}

static void news_item_tick_current()
{
    int32_t ticks = gNewsItems.IncrementTicks();
    // Only play news item sound when in normal playing mode
    if (ticks == 1 && (gScreenFlags == SCREEN_FLAGS_PLAYING))
    {
        // Play sound
        audio_play_sound(SoundId::NewsItem, 0, context_get_width() / 2);
    }
}

int32_t NewsItemQueues::RemoveTime() const
{
    if (!Recent[5].IsEmpty() && !Recent[4].IsEmpty() && !Recent[3].IsEmpty() && !Recent[2].IsEmpty())
    {
        return 256;
    }
    return 320;
}

bool NewsItemQueues::CurrentShouldBeArchived() const
{
    return Current().Ticks >= RemoveTime();
}

/**
 *
 *  rct2: 0x0066E252
 */
void news_item_update_current()
{
    // Check if there is a current news item
    if (gNewsItems.IsEmpty())
        return;

    auto intent = Intent(INTENT_ACTION_INVALIDATE_TICKER_NEWS);
    context_broadcast_intent(&intent);

    // Update the current news item
    news_item_tick_current();

    // Removal of current news item
    if (gNewsItems.CurrentShouldBeArchived())
        gNewsItems.ArchiveCurrent();
}

/**
 *
 *  rct2: 0x0066E377
 */
void news_item_close_current()
{
    gNewsItems.ArchiveCurrent();
}

void NewsItemQueues::ArchiveCurrent()
{
    // Check if there is a current message
    if (IsEmpty())
        return;

    Archived.push_back(Current());

    // Invalidate the news window
    window_invalidate_by_class(WC_RECENT_NEWS);

    // Dequeue the current news item, shift news up
    Recent.pop_front();

    // Invalidate current news item bar
    auto intent = Intent(INTENT_ACTION_INVALIDATE_TICKER_NEWS);
    context_broadcast_intent(&intent);
}

/**
 * Get the (x,y,z) coordinates of the subject of a news item.
 * If the new item is no longer valid, return LOCATION_NULL in the x-coordinate
 *
 *  rct2: 0x0066BA74
 */
std::optional<CoordsXYZ> news_item_get_subject_location(NewsItem::Type type, int32_t subject)
{
    std::optional<CoordsXYZ> subjectLoc{ std::nullopt };

    switch (type)
    {
        case NewsItem::Type::Ride:
        {
            Ride* ride = get_ride(subject);
            if (ride == nullptr || ride->overall_view.isNull())
            {
                break;
            }
            auto rideViewCentre = ride->overall_view.ToTileCentre();
            subjectLoc = CoordsXYZ{ rideViewCentre, tile_element_height(rideViewCentre) };
            break;
        }
        case NewsItem::Type::PeepOnRide:
        {
            auto peep = TryGetEntity<Peep>(subject);
            if (peep == nullptr)
                break;

            subjectLoc = CoordsXYZ{ peep->x, peep->y, peep->z };
            if (subjectLoc->x != LOCATION_NULL)
                break;

            if (peep->State != PEEP_STATE_ON_RIDE && peep->State != PEEP_STATE_ENTERING_RIDE)
            {
                subjectLoc = std::nullopt;
                break;
            }

            // Find which ride peep is on
            Ride* ride = get_ride(peep->CurrentRide);
            if (ride == nullptr || !(ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK))
            {
                subjectLoc = std::nullopt;
                break;
            }

            // Find the first car of the train peep is on
            auto sprite = TryGetEntity<Vehicle>(ride->vehicles[peep->CurrentTrain]);
            // Find the actual car peep is on
            for (int32_t i = 0; i < peep->CurrentCar && sprite != nullptr; i++)
            {
                sprite = TryGetEntity<Vehicle>(sprite->next_vehicle_on_train);
            }
            if (sprite != nullptr)
            {
                subjectLoc = CoordsXYZ{ sprite->x, sprite->y, sprite->z };
            }
            break;
        }
        case NewsItem::Type::Peep:
        {
            auto peep = TryGetEntity<Peep>(subject);
            if (peep != nullptr)
            {
                subjectLoc = CoordsXYZ{ peep->x, peep->y, peep->z };
            }
            break;
        }
        case NewsItem::Type::Blank:
        {
            auto subjectUnsigned = static_cast<uint32_t>(subject);
            auto subjectXY = CoordsXY{ static_cast<int16_t>(subjectUnsigned & 0xFFFF),
                                       static_cast<int16_t>(subjectUnsigned >> 16) };
            if (!subjectXY.isNull())
            {
                subjectLoc = CoordsXYZ{ subjectXY, tile_element_height(subjectXY) };
            }
            break;
        }
        default:
            break;
    }
    return subjectLoc;
}

NewsItem::Object* NewsItemQueues::FirstOpenOrNewSlot()
{
    for (auto emptySlots = Recent.capacity() - Recent.size(); emptySlots < 2; ++emptySlots)
    {
        ArchiveCurrent();
    }

    auto res = Recent.end();
    // The for loop above guarantees there is always an extra element to use
    assert(Recent.capacity() - Recent.size() >= 2);
    auto newsItem = res + 1;
    newsItem->Type = NewsItem::Type::Null;

    return &*res;
}

/**
 *
 *  rct2: 0x0066DF55
 */
NewsItem::Object* news_item_add_to_queue(NewsItem::Type type, rct_string_id string_id, uint32_t assoc)
{
    utf8 buffer[256];
    void* args = gCommonFormatArgs;

    // overflows possible?
    format_string(buffer, 256, string_id, args);
    return news_item_add_to_queue_raw(type, buffer, assoc);
}

NewsItem::Object* news_item_add_to_queue_raw(NewsItem::Type type, const utf8* text, uint32_t assoc)
{
    NewsItem::Object* newsItem = gNewsItems.FirstOpenOrNewSlot();
    newsItem->Type = type;
    newsItem->Flags = 0;
    newsItem->Assoc = assoc;
    newsItem->Ticks = 0;
    newsItem->MonthYear = gDateMonthsElapsed;
    newsItem->Day = ((days_in_month[date_get_month(newsItem->MonthYear)] * gDateMonthTicks) >> 16) + 1;
    safe_strcpy(newsItem->Text, text, sizeof(newsItem->Text));

    return newsItem;
}

/**
 * Opens the window/tab for the subject of the news item
 *
 *  rct2: 0x0066EBE6
 *
 */
void news_item_open_subject(NewsItem::Type type, int32_t subject)
{
    switch (type)
    {
        case NewsItem::Type::Null:
            break;
        case NewsItem::Type::Ride:
        {
            auto intent = Intent(WC_RIDE);
            intent.putExtra(INTENT_EXTRA_RIDE_ID, subject);
            context_open_intent(&intent);
            break;
        }
        case NewsItem::Type::PeepOnRide:
        case NewsItem::Type::Peep:
        {
            auto peep = TryGetEntity<Peep>(subject);
            if (peep != nullptr)
            {
                auto intent = Intent(WC_PEEP);
                intent.putExtra(INTENT_EXTRA_PEEP, peep);
                context_open_intent(&intent);
            }
            break;
        }
        case NewsItem::Type::Money:
            context_open_window(WC_FINANCES);
            break;
        case NewsItem::Type::Blank:
            break;
        case NewsItem::Type::Research:
        {
            auto item = ResearchItem(subject, 0, 0);
            if (item.type == Research::EntryType::Ride)
            {
                auto intent = Intent(INTENT_ACTION_NEW_RIDE_OF_TYPE);
                intent.putExtra(INTENT_EXTRA_RIDE_TYPE, item.baseRideType);
                intent.putExtra(INTENT_EXTRA_RIDE_ENTRY_INDEX, item.entryIndex);
                context_open_intent(&intent);
                break;
            }

            // Check if window is already open
            auto window = window_bring_to_front_by_class(WC_SCENERY);
            if (window == nullptr)
            {
                window = window_find_by_class(WC_TOP_TOOLBAR);
                if (window != nullptr)
                {
                    window->Invalidate();
                    if (!tool_set(window, WC_TOP_TOOLBAR__WIDX_SCENERY, TOOL_ARROW))
                    {
                        input_set_flag(INPUT_FLAG_6, true);
                        context_open_window(WC_SCENERY);
                    }
                }
            }

            // Switch to new scenery tab
            window = window_find_by_class(WC_SCENERY);
            if (window != nullptr)
                window_event_mouse_down_call(window, WC_SCENERY__WIDX_SCENERY_TAB_1 + subject);
            break;
        }
        case NewsItem::Type::Peeps:
        {
            auto intent = Intent(WC_GUEST_LIST);
            intent.putExtra(INTENT_EXTRA_GUEST_LIST_FILTER, GLFT_GUESTS_THINKING_X);
            intent.putExtra(INTENT_EXTRA_RIDE_ID, subject);
            context_open_intent(&intent);
            break;
        }
        case NewsItem::Type::Award:
            context_open_window_view(WV_PARK_AWARDS);
            break;
        case NewsItem::Type::Graph:
            context_open_window_view(WV_PARK_RATING);
            break;
        case NewsItem::Type::Count:
            break;
    }
}

/**
 *
 *  rct2: 0x0066E407
 */
void news_item_disable_news(NewsItem::Type type, uint32_t assoc)
{
    // TODO: write test invalidating windows
    gNewsItems.ForeachRecentNews([type, assoc](auto& newsItem) {
        if (type == newsItem.Type && assoc == newsItem.Assoc)
        {
            newsItem.SetFlags(NewsItem::Flags::HasButton);
            if (&newsItem == &gNewsItems.Current())
            {
                auto intent = Intent(INTENT_ACTION_INVALIDATE_TICKER_NEWS);
                context_broadcast_intent(&intent);
            }
        }
    });

    gNewsItems.ForeachArchivedNews([type, assoc](auto& newsItem) {
        if (type == newsItem.Type && assoc == newsItem.Assoc)
        {
            newsItem.SetFlags(NewsItem::Flags::HasButton);
            window_invalidate_by_class(WC_RECENT_NEWS);
        }
    });
}

void news_item_add_to_queue_custom(NewsItem::Object* newNewsItem)
{
    NewsItem::Object* newsItem = gNewsItems.FirstOpenOrNewSlot();
    *newsItem = *newNewsItem;
}

void news_item_remove(int32_t index)
{
    if (index < 0 || index >= NewsItem::MaxItems)
        return;

    // News item is already null, no need to remove it
    if (gNewsItems[index].Type == NewsItem::Type::Null)
        return;

    size_t newsBoundary = index < NewsItem::HistoryStart ? NewsItem::HistoryStart : NewsItem::MaxItems;
    for (size_t i = index; i < newsBoundary - 1; i++)
    {
        gNewsItems[i] = gNewsItems[i + 1];
    }
    gNewsItems[newsBoundary - 1].Type = NewsItem::Type::Null;
}
