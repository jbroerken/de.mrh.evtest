/**
 *  Copyright (C) 2022 The MRH Project Authors.
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

// C / C++
#include <unistd.h>

// External

// Project
#include "./UserV1Response.h"
#include "../ServiceAvailV1.h"
#include "./UserAccessV1.h"
#include "./UserGetLocationV1.h"
#include "../ServiceCustomCommandV1.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

UserV1Response::UserV1Response(bool& b_EventFailed) noexcept : MRH::AB::Module("UserV1Response"),
                                                               b_EventFailed(b_EventFailed),
                                                               u32_Event(MRH_EVENT_USER_AVAIL_U)
{
    MRH::AB::ModuleLogger::Singleton().Log(GetName(), "V1 user event checking started...", 
                                           "UserV1Response.cpp", __LINE__);
}

UserV1Response::~UserV1Response() noexcept
{
    MRH::AB::ModuleLogger::Singleton().Log(GetName(), "V1 user event checking finished.", 
                                           "UserV1Response.cpp", __LINE__);
}

//*************************************************************************************
// Update
//*************************************************************************************

void UserV1Response::HandleEvent(const MRH_Event* p_Event)
{}

MRH::AB::Module::Result UserV1Response::Update()
{
    if (b_EventFailed == true)
    {
        MRH::AB::ModuleLogger::Singleton().Log(GetName(), "V1 user event checking failed!", 
                                               "UserV1Response.cpp", __LINE__);
        return MRH::AB::Module::FINISHED_POP;
    }
    else if (u32_Event == MRH_EVENT_TYPE_COUNT)
    {
        MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Finished checking user V1 events.", 
                                               "UserV1Response.cpp", __LINE__);
        return MRH::AB::Module::FINISHED_POP;
    }
    
    return MRH::AB::Module::FINISHED_APPEND;
}

std::unique_ptr<MRH::AB::Module> UserV1Response::NextModule()
{
    switch (u32_Event)
    {
        case MRH_EVENT_USER_AVAIL_U:
            u32_Event = MRH_EVENT_USER_ACCESS_DOCUMENTS_U;
            return std::make_unique<ServiceAvailV1>(b_EventFailed,
                                                    "User",
                                                    MRH_EVENT_USER_AVAIL_U,
                                                    MRH_EVENT_USER_AVAIL_S);
            
        case MRH_EVENT_USER_ACCESS_DOCUMENTS_U:
            u32_Event = MRH_EVENT_USER_ACCESS_PICTURES_U;
            return std::make_unique<UserAccessV1>(b_EventFailed,
                                                  "Documents",
                                                  MRH_EVENT_USER_ACCESS_DOCUMENTS_U,
                                                  MRH_EVENT_USER_ACCESS_DOCUMENTS_S);
            
        case MRH_EVENT_USER_ACCESS_PICTURES_U:
            u32_Event = MRH_EVENT_USER_ACCESS_MUSIC_U;
            return std::make_unique<UserAccessV1>(b_EventFailed,
                                                  "Pictures",
                                                  MRH_EVENT_USER_ACCESS_PICTURES_U,
                                                  MRH_EVENT_USER_ACCESS_PICTURES_S);
            
        case MRH_EVENT_USER_ACCESS_MUSIC_U:
            u32_Event = MRH_EVENT_USER_ACCESS_VIDEOS_U;
            return std::make_unique<UserAccessV1>(b_EventFailed,
                                                  "Music",
                                                  MRH_EVENT_USER_ACCESS_MUSIC_U,
                                                  MRH_EVENT_USER_ACCESS_MUSIC_S);
            
        case MRH_EVENT_USER_ACCESS_VIDEOS_U:
            u32_Event = MRH_EVENT_USER_ACCESS_DOWNLOADS_U;
            return std::make_unique<UserAccessV1>(b_EventFailed,
                                                  "Videos",
                                                  MRH_EVENT_USER_ACCESS_VIDEOS_U,
                                                  MRH_EVENT_USER_ACCESS_VIDEOS_S);
            
        case MRH_EVENT_USER_ACCESS_DOWNLOADS_U:
            u32_Event = MRH_EVENT_USER_ACCESS_CLIPBOARD_U;
            return std::make_unique<UserAccessV1>(b_EventFailed,
                                                  "Downloads",
                                                  MRH_EVENT_USER_ACCESS_DOWNLOADS_U,
                                                  MRH_EVENT_USER_ACCESS_DOWNLOADS_S);
            
        case MRH_EVENT_USER_ACCESS_CLIPBOARD_U:
            u32_Event = MRH_EVENT_USER_ACCESS_INFO_PERSON_U;
            return std::make_unique<UserAccessV1>(b_EventFailed,
                                                  "Clipboard",
                                                  MRH_EVENT_USER_ACCESS_CLIPBOARD_U,
                                                  MRH_EVENT_USER_ACCESS_CLIPBOARD_S);
            
        case MRH_EVENT_USER_ACCESS_INFO_PERSON_U:
            u32_Event = MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U;
            return std::make_unique<UserAccessV1>(b_EventFailed,
                                                  "Person Info",
                                                  MRH_EVENT_USER_ACCESS_INFO_PERSON_U,
                                                  MRH_EVENT_USER_ACCESS_INFO_PERSON_S);
            
        case MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U:
            u32_Event = MRH_EVENT_USER_ACCESS_CLEAR_U;
            return std::make_unique<UserAccessV1>(b_EventFailed,
                                                  "Residence Info",
                                                  MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U,
                                                  MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_S);
            
        case MRH_EVENT_USER_ACCESS_CLEAR_U:
            u32_Event = MRH_EVENT_USER_GET_LOCATION_U;
            return std::make_unique<UserAccessV1>(b_EventFailed,
                                                  "Access Clear",
                                                  MRH_EVENT_USER_ACCESS_CLEAR_U,
                                                  MRH_EVENT_USER_ACCESS_CLEAR_S);
            
        case MRH_EVENT_USER_GET_LOCATION_U:
            u32_Event = MRH_EVENT_USER_CUSTOM_COMMAND_U;
            return std::make_unique<UserGetLocationV1>(b_EventFailed);
            
        case MRH_EVENT_USER_CUSTOM_COMMAND_U:
            u32_Event = MRH_EVENT_TYPE_COUNT;
            return std::make_unique<ServiceCustomCommandV1>(b_EventFailed,
                                                            "User",
                                                            MRH_EVENT_USER_CUSTOM_COMMAND_U,
                                                            MRH_EVENT_USER_CUSTOM_COMMAND_S);
            
        default:
            throw MRH::AB::ModuleException(GetName(),
                                           "No module to switch to!");
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool UserV1Response::CanHandleEvent(MRH_Uint32 u32_Type) noexcept
{
    return false;
}
