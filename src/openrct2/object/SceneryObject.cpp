/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "SceneryObject.h"

#include "ObjectJsonHelpers.h"

void SceneryObject::SetPrimarySceneryGroup(const std::string& s)
{
    if (!s.empty())
    {
        auto sgEntry = ObjectJsonHelpers::ParseObjectEntry(s);
        SetPrimarySceneryGroup(&sgEntry);
    }
}
