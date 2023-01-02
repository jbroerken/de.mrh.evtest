/**
 *  Copyright (C) 2021 - 2023 The de.mrh.evtest Authors.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef U_AccessContent_V1_h
#define U_AccessContent_V1_h

// C / C++

// External

// Project
#include "./U_AccessClear_V1.h"


class U_AccessContent_V1 : public EventModule
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param u32_RequestEvent The event to request with.
     *  \param u32_ResponseEvent The response event to check for.
     *  \param p_EventSender The event sender to use.
     */

    U_AccessContent_V1(MRH_Uint32 u32_RequestEvent,
                       MRH_Uint32 u32_ResponseEvent,
                       std::shared_ptr<EventSender>& p_EventSender) : EventModule("MRH_EVENT_USER_ACCESS_CONTENT (" + std::to_string(u32_RequestEvent) + ")",
                                                                                  u32_ResponseEvent,
                                                                                  false,
                                                                                  p_EventSender),
                                                                      u32_RequestEvent(u32_RequestEvent)
    {
        MRH_Event* p_Event = MRH_EVD_CreateEvent(u32_RequestEvent, NULL, 0);

        if (p_Event == NULL)
        {
            throw MRH::AB::ModuleException(GetIdentifier(),
                                           "Failed to create MRH_EVENT_USER_ACCESS_CONTENT (" + std::to_string(u32_RequestEvent) + ") event!");
        }

        try
        {
            p_EventSender->Send(p_Event);
        }
        catch (std::exception& e)
        {
            MRH_EVD_DestroyEvent(p_Event);

            throw MRH::AB::ModuleException(GetIdentifier(),
                                           e.what());
        }
    }

    /**
     *  Default destructor.
     */

    ~U_AccessContent_V1() noexcept
    {}

    //*************************************************************************************
    // Update
    //*************************************************************************************

    /**
     *  Get the module to switch to.
     *
     *  \return The module switch information.
     */

    std::unique_ptr<MRH::AB::Module> NextModule() override
    {
        switch (u32_RequestEvent)
        {
            case MRH_EVENT_USER_ACCESS_DOCUMENTS_U:
                return std::make_unique<U_AccessContent_V1>(MRH_EVENT_USER_ACCESS_PICTURES_U,
                                                            MRH_EVENT_USER_ACCESS_PICTURES_S,
                                                            p_EventSender);
            case MRH_EVENT_USER_ACCESS_PICTURES_U:
                return std::make_unique<U_AccessContent_V1>(MRH_EVENT_USER_ACCESS_MUSIC_U,
                                                            MRH_EVENT_USER_ACCESS_MUSIC_S,
                                                            p_EventSender);
            case MRH_EVENT_USER_ACCESS_MUSIC_U:
                return std::make_unique<U_AccessContent_V1>(MRH_EVENT_USER_ACCESS_VIDEOS_U,
                                                            MRH_EVENT_USER_ACCESS_VIDEOS_S,
                                                            p_EventSender);
            case MRH_EVENT_USER_ACCESS_VIDEOS_U:
                return std::make_unique<U_AccessContent_V1>(MRH_EVENT_USER_ACCESS_DOWNLOADS_U,
                                                            MRH_EVENT_USER_ACCESS_DOWNLOADS_S,
                                                            p_EventSender);
            case MRH_EVENT_USER_ACCESS_DOWNLOADS_U:
                return std::make_unique<U_AccessContent_V1>(MRH_EVENT_USER_ACCESS_CLIPBOARD_U,
                                                            MRH_EVENT_USER_ACCESS_CLIPBOARD_S,
                                                            p_EventSender);
            case MRH_EVENT_USER_ACCESS_CLIPBOARD_U:
                return std::make_unique<U_AccessContent_V1>(MRH_EVENT_USER_ACCESS_INFO_PERSON_U,
                                                            MRH_EVENT_USER_ACCESS_INFO_PERSON_S,
                                                            p_EventSender);
            case MRH_EVENT_USER_ACCESS_INFO_PERSON_U:
                return std::make_unique<U_AccessContent_V1>(MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U,
                                                            MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_S,
                                                            p_EventSender);

            case MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U:
            default:
                return std::make_unique<U_AccessClear_V1>(p_EventSender);
        }
    }

private:

    //*************************************************************************************
    // Update
    //*************************************************************************************

    /**
     *  Check if a event check succeeded.
     *
     *  \param p_Event The event to check.
     *
     *  \return true if succeeded, false if not.
     */

    bool EventValid(const MRH_Event* p_Event) noexcept override
    {
        MRH_EvD_Base_Result_t c_Data;

        if (MRH_EVD_ReadEvent(&c_Data, p_Event->u32_Type, p_Event) < 0)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, GetIdentifier() +
                                                                     ": Failed to read event!",
                                             "U_AccessContent_V1.h", __LINE__);
            return false;
        }
        else if (c_Data.u8_Result != MRH_EvD_Base_Result::MRH_EVD_BASE_RESULT_SUCCESS)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, GetIdentifier() +
                                                                     ": Failed to get content access!",
                                             "U_AccessContent_V1.h", __LINE__);
            return false;
        }

        return true;
    }

    //*************************************************************************************
    // Data
    //*************************************************************************************

    MRH_Uint32 u32_RequestEvent;
    
protected:

};

#endif /* U_AccessContent_V1_h */
