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

// External

// Project
#include "./PasswordV1Response.h"
#include "../ServiceAvailV1.h"
#include "./PasswordCheckV1.h"
#include "./PasswordSetV1.h"
#include "../ServiceCustomCommandV1.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PasswordV1Response::PasswordV1Response(bool& b_EventFailed) noexcept : MRH::AB::Module("PasswordV1Response"),
                                                                       b_EventFailed(b_EventFailed),
                                                                       u32_Event(MRH_EVENT_PASSWORD_AVAIL_U)
{
    MRH::AB::ModuleLogger::Singleton().Log(GetName(), "V1 password event checking started...", 
                                           "PasswordV1Response.cpp", __LINE__);
}

PasswordV1Response::~PasswordV1Response() noexcept
{
    MRH::AB::ModuleLogger::Singleton().Log(GetName(), "V1 password event checking finished.", 
                                           "PasswordV1Response.cpp", __LINE__);
}

//*************************************************************************************
// Update
//*************************************************************************************

void PasswordV1Response::HandleEvent(const MRH_Event* p_Event)
{}

MRH::AB::Module::Result PasswordV1Response::Update()
{
    if (b_EventFailed == true)
    {
        MRH::AB::ModuleLogger::Singleton().Log(GetName(), "V1 password event checking failed!", 
                                               "PasswordV1Response.cpp", __LINE__);
        return MRH::AB::Module::FINISHED_POP;
    }
    else if (u32_Event == MRH_EVENT_TYPE_COUNT)
    {
        MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Finished checking password V1 events.", 
                                               "PasswordV1Response.cpp", __LINE__);
        return MRH::AB::Module::FINISHED_POP;
    }
    
    return MRH::AB::Module::FINISHED_APPEND;
}

std::unique_ptr<MRH::AB::Module> PasswordV1Response::NextModule()
{
    switch (u32_Event)
    {
        case MRH_EVENT_PASSWORD_AVAIL_U:
            u32_Event = MRH_EVENT_PASSWORD_CHECK_U;
            return std::make_unique<ServiceAvailV1>(b_EventFailed,
                                                    "Password",
                                                    MRH_EVENT_PASSWORD_AVAIL_U,
                                                    MRH_EVENT_PASSWORD_AVAIL_S);
            
        case MRH_EVENT_PASSWORD_CHECK_U:
            u32_Event = MRH_EVENT_PASSWORD_SET_U;
            return std::make_unique<PasswordCheckV1>(b_EventFailed);
            
        case MRH_EVENT_PASSWORD_SET_U:
            u32_Event = MRH_EVENT_PASSWORD_CUSTOM_COMMAND_U;
            return std::make_unique<PasswordSetV1>(b_EventFailed);
            
        case MRH_EVENT_PASSWORD_CUSTOM_COMMAND_U:
            u32_Event = MRH_EVENT_TYPE_COUNT;
            return std::make_unique<ServiceCustomCommandV1>(b_EventFailed,
                                                            "Password",
                                                            MRH_EVENT_PASSWORD_CUSTOM_COMMAND_U,
                                                            MRH_EVENT_PASSWORD_CUSTOM_COMMAND_S);
            
        default:
            throw MRH::AB::ModuleException(GetName(),
                                           "No module to switch to!");
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool PasswordV1Response::CanHandleEvent(MRH_Uint32 u32_Type) noexcept
{
    return false;
}
