/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../common.h"

#include <algorithm>
#include <array>
#include <iterator>
#include <optional>

struct CoordsXYZ;

namespace NewsItem
{
    enum class Type : uint8_t
    {
        Null,
        Ride,
        PeepOnRide,
        Peep,
        Money,
        Blank,
        Research,
        Peeps,
        Award,
        Graph,
        Count
    };

    enum TypeProperty : uint8_t
    {
        HasLocation = 1,
        HasSubject = 2,
    };

    enum Flags : uint8_t
    {
        HasButton = 1 << 0,
    };

    constexpr size_t GetTypeCount()
    {
        return static_cast<size_t>(NewsItem::Type::Count);
    }

    /**
     * A single news item / message.
     */
    struct Object
    {
        NewsItem::Type Type;
        uint8_t Flags;
        uint32_t Assoc;
        uint16_t Ticks;
        uint16_t MonthYear;
        uint8_t Day;
        utf8 Text[256];

        constexpr bool IsEmpty() const noexcept
        {
            return Type == NewsItem::Type::Null;
        }

        constexpr uint8_t GetTypeProperties() const
        {
            switch (Type)
            {
                case NewsItem::Type::Blank:
                    return NewsItem::TypeProperty::HasLocation;
                case NewsItem::Type::Money:
                case NewsItem::Type::Research:
                case NewsItem::Type::Peeps:
                case NewsItem::Type::Award:
                case NewsItem::Type::Graph:
                    return NewsItem::TypeProperty::HasSubject;
                case NewsItem::Type::Ride:
                case NewsItem::Type::PeepOnRide:
                case NewsItem::Type::Peep:
                    return NewsItem::TypeProperty::HasLocation | NewsItem::TypeProperty::HasSubject;
                case NewsItem::Type::Null:
                case NewsItem::Type::Count:
                default:
                    return 0;
            }
        }

        void SetFlags(uint8_t flag)
        {
            Flags |= flag;
        }

        constexpr bool TypeHasSubject() const
        {
            return this->GetTypeProperties() & NewsItem::TypeProperty::HasSubject;
        }

        constexpr bool TypeHasLocation() const
        {
            return this->GetTypeProperties() & NewsItem::TypeProperty::HasLocation;
        }

        constexpr bool HasButton() const noexcept
        {
            return Flags & NewsItem::Flags::HasButton;
        }
    };
} // namespace News

constexpr int32_t NEWS_ITEM_HISTORY_START = 11;
constexpr int32_t MAX_NEWS_ITEMS_ARCHIVE = 50;
constexpr int32_t MAX_NEWS_ITEMS = NEWS_ITEM_HISTORY_START + MAX_NEWS_ITEMS_ARCHIVE;

template<std::size_t N> class NewsItemQueue
{
public:
    static_assert(N > 0, "Cannot instantiate NewsItemQueue with size=0");

    using value_type = typename std::array<NewsItem::Object, N>::value_type;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = typename std::array<NewsItem::Object, N>::iterator;
    using const_iterator = typename std::array<NewsItem::Object, N>::const_iterator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    NewsItemQueue()
    {
        Queue[0].Type = NewsItem::Type::Null;
    }

    constexpr iterator begin() noexcept
    {
        return std::begin(Queue);
    }
    constexpr const_iterator begin() const noexcept
    {
        return cbegin();
    }
    constexpr const_iterator cbegin() const noexcept
    {
        return std::cbegin(Queue);
    }
    iterator end() noexcept
    {
        return std::find_if(std::begin(Queue), std::end(Queue), [](const_reference item) { return item.IsEmpty(); });
    }
    const_iterator end() const noexcept
    {
        return cend();
    }
    const_iterator cend() const noexcept
    {
        return std::find_if(std::cbegin(Queue), std::cend(Queue), [](const_reference item) { return item.IsEmpty(); });
    }

    constexpr bool empty() const noexcept
    {
        return Queue[0].IsEmpty();
    }

    size_type size() const noexcept
    {
        return std::distance(cbegin(), cend());
    }

    reference front() noexcept
    {
        return *begin();
    }
    const_reference front() const noexcept
    {
        return *cbegin();
    }
    reference back() noexcept
    {
        return *end();
    }
    const_reference back() const noexcept
    {
        return *cend();
    }

    void pop_front()
    {
        std::move(std::begin(Queue) + 1, std::end(Queue), std::begin(Queue));
        Queue[N - 1].Type = NewsItem::Type::Null;
    }

    void push_back(const_reference item)
    {
        auto it = end();
        if (!std::distance(it, std::end(Queue)))
        {
            // Reached queue max size, need to free some space
            pop_front();
            Queue[N - 1] = item;
        }
        else
        {
            *it = item;
            ++it;
            if (std::distance(it, std::end(Queue)))
                it->Type = NewsItem::Type::Null;
        }
    }

    reference operator[](size_type n) noexcept
    {
        return Queue[n];
    }
    const_reference operator[](size_type n) const noexcept
    {
        return Queue[n];
    }

    constexpr size_type capacity() const noexcept
    {
        return N;
    }

    void clear() noexcept
    {
        front().Type = NewsItem::Type::Null;
    }

private:
    std::array<NewsItem::Object, N> Queue;
};

struct NewsItemQueues
{
    NewsItem::Object& operator[](size_t index);
    const NewsItem::Object& operator[](size_t index) const;
    NewsItem::Object* At(int32_t index);
    const NewsItem::Object* At(int32_t index) const;
    bool IsEmpty() const;
    void Clear();
    uint16_t IncrementTicks();
    NewsItem::Object& Current();
    const NewsItem::Object& Current() const;
    NewsItem::Object& Oldest();
    const NewsItem::Object& Oldest() const;
    bool CurrentShouldBeArchived() const;
    void ArchiveCurrent();
    NewsItem::Object* FirstOpenOrNewSlot();
    const auto& GetRecent() const
    {
        return Recent;
    }
    const auto& GetArchived() const
    {
        return Archived;
    }

    template<typename Predicate> void ForeachRecentNews(Predicate&& p)
    {
        for (auto& newsItem : Recent)
        {
            p(newsItem);
        }
    }

    template<typename Predicate> void ForeachArchivedNews(Predicate&& p)
    {
        for (auto& newsItem : Archived)
        {
            p(newsItem);
        }
    }

private:
    int32_t RemoveTime() const;
    void AppendToArchive(NewsItem::Object& item);

    NewsItemQueue<NEWS_ITEM_HISTORY_START> Recent;
    NewsItemQueue<MAX_NEWS_ITEMS_ARCHIVE> Archived;
};

extern NewsItemQueues gNewsItems;

void news_item_init_queue();

void news_item_update_current();
void news_item_close_current();

std::optional<CoordsXYZ> news_item_get_subject_location(NewsItem::Type type, int32_t subject);

NewsItem::Object* news_item_add_to_queue(NewsItem::Type type, rct_string_id string_id, uint32_t assoc);
NewsItem::Object* news_item_add_to_queue_raw(NewsItem::Type type, const utf8* text, uint32_t assoc);

void news_item_open_subject(NewsItem::Type type, int32_t subject);

void news_item_disable_news(NewsItem::Type type, uint32_t assoc);

NewsItem::Object* news_item_get(int32_t index);

bool news_item_is_queue_empty();

bool news_item_is_valid_idx(int32_t index);

void news_item_add_to_queue_custom(NewsItem::Object* newNewsItem);
void news_item_remove(int32_t index);
