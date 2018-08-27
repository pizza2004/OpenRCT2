/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../Cheats.h"
#include "../Context.h"
#include "../core/MemoryStream.h"
#include "../drawing/Drawing.h"
#include "../interface/Window.h"
#include "../localisation/Localisation.h"
#include "../localisation/StringIds.h"
#include "../peep/Staff.h"
#include "../windows/Intent.h"
#include "../world/Park.h"
#include "../world/Sprite.h"
#include "GameAction.h"

struct StaffSetNameAction : public GameActionBase<GAME_COMMAND_SET_STAFF_NAME, GameActionResult>
{
private:
    uint16_t _spriteIndex;
    std::string _name;

public:
    StaffSetNameAction()
    {
    }
    StaffSetNameAction(uint16_t spriteIndex, const std::string& name)
        : _spriteIndex(spriteIndex)
        , _name(name)
    {
    }

    uint16_t GetActionFlags() const override
    {
        return GameAction::GetActionFlags() | GA_FLAGS::ALLOW_WHILE_PAUSED;
    }

    void Serialise(DataSerialiser& stream) override
    {
        GameAction::Serialise(stream);

        stream << _spriteIndex << _name;
    }

    GameActionResult::Ptr Query() const override
    {
        if (_spriteIndex >= MAX_SPRITES)
        {
            return std::make_unique<GameActionResult>(
                GA_ERROR::INVALID_PARAMETERS, STR_STAFF_ERROR_CANT_NAME_STAFF_MEMBER, STR_NONE);
        }

        if (_name.empty())
        {
            return std::make_unique<GameActionResult>(GA_ERROR::INVALID_PARAMETERS, STR_STAFF_ERROR_CANT_NAME_STAFF_MEMBER);
        }

        rct_peep* peep = GET_PEEP(_spriteIndex);
        if (peep->type != PEEP_TYPE_STAFF)
        {
            log_warning("Invalid game command for sprite %u", _spriteIndex);
            return std::make_unique<GameActionResult>(
                GA_ERROR::INVALID_PARAMETERS, STR_STAFF_ERROR_CANT_NAME_STAFF_MEMBER, STR_NONE);
        }

        rct_string_id newUserStringId = user_string_allocate(
            USER_STRING_HIGH_ID_NUMBER | USER_STRING_DUPLICATION_PERMITTED, _name.c_str());
        if (newUserStringId == 0)
        {
            // TODO: Probably exhausted, introduce new error.
            return std::make_unique<GameActionResult>(
                GA_ERROR::UNKNOWN, STR_STAFF_ERROR_CANT_NAME_STAFF_MEMBER, gGameCommandErrorText);
        }
        user_string_free(newUserStringId);

        return std::make_unique<GameActionResult>();
    }

    GameActionResult::Ptr Execute() const override
    {
        rct_string_id newUserStringId = user_string_allocate(
            USER_STRING_HIGH_ID_NUMBER | USER_STRING_DUPLICATION_PERMITTED, _name.c_str());
        if (newUserStringId == 0)
        {
            // TODO: Probably exhausted, introduce new error.
            return std::make_unique<GameActionResult>(
                GA_ERROR::UNKNOWN, STR_STAFF_ERROR_CANT_NAME_STAFF_MEMBER, gGameCommandErrorText);
        }

        rct_peep* peep = GET_PEEP(_spriteIndex);
        if (peep->type != PEEP_TYPE_STAFF)
        {
            log_warning("Invalid game command for sprite %u", _spriteIndex);
            return std::make_unique<GameActionResult>(
                GA_ERROR::INVALID_PARAMETERS, STR_STAFF_ERROR_CANT_NAME_STAFF_MEMBER, STR_NONE);
        }

        set_format_arg(0, uint32_t, peep->id);
        utf8* curName = gCommonStringFormatBuffer;
        rct_string_id curId = peep->name_string_idx;
        format_string(curName, 256, curId, gCommonFormatArgs);

        if (strcmp(curName, _name.c_str()) == 0)
        {
            return std::make_unique<GameActionResult>(GA_ERROR::OK, STR_NONE);
        }

        user_string_free(peep->name_string_idx);
        peep->name_string_idx = newUserStringId;

        peep_update_name_sort(peep);

        peep_handle_easteregg_name(peep);

        gfx_invalidate_screen();

        auto intent = Intent(INTENT_ACTION_REFRESH_STAFF_LIST);
        context_broadcast_intent(&intent);

        auto res = std::make_unique<GameActionResult>();
        res->Position.x = peep->x;
        res->Position.y = peep->y;
        res->Position.z = peep->z;
        return res;
    }
};
