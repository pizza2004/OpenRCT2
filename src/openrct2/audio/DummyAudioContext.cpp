/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "AudioContext.h"

namespace OpenRCT2::Audio
{
    class DummyAudioContext final : public IAudioContext
    {
        IAudioMixer* GetMixer() override
        {
            return nullptr;
        }

        std::vector<std::string> GetOutputDevices() override
        {
            return std::vector<std::string>();
        }
        void SetOutputDevice(const std::string& /*deviceName*/) override
        {
        }

        IAudioSource* CreateStreamFromWAV(const std::string& /*path*/) override
        {
            return nullptr;
        }

        void StartTitleMusic() override
        {
        }

        IAudioChannel* PlaySound(int32_t /*soundId*/, int32_t /*volume*/, int32_t /*pan*/) override
        {
            return nullptr;
        }
        IAudioChannel* PlaySoundAtLocation(int32_t /*soundId*/, int16_t /*x*/, int16_t /*y*/, int16_t /*z*/) override
        {
            return nullptr;
        }
        IAudioChannel* PlaySoundPanned(
            int32_t /*soundId*/, int32_t /*pan*/, int16_t /*x*/, int16_t /*y*/, int16_t /*z*/) override
        {
            return nullptr;
        }

        void ToggleAllSounds() override
        {
        }
        void PauseSounds() override
        {
        }
        void UnpauseSounds() override
        {
        }

        void StopAll() override
        {
        }
        void StopCrowdSound() override
        {
        }
        void StopRainSound() override
        {
        }
        void StopRideMusic() override
        {
        }
        void StopTitleMusic() override
        {
        }
        void StopVehicleSounds() override
        {
        }
    };

    std::unique_ptr<IAudioContext> CreateDummyAudioContext()
    {
        return std::make_unique<DummyAudioContext>();
    }
} // namespace OpenRCT2::Audio
