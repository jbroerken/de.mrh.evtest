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
#include "./V1Response.h"
#include "./Custom/CustomV1Response.h"
#include "./Listen/ListenV1Response.h"
#include "./Say/SayV1Response.h"
#include "./Password/PasswordV1Response.h"
#include "./User/UserV1Response.h"
#include "./Application/ApplicationV1Response.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

V1Response::V1Response(bool& b_EventFailed) noexcept : MRH::AB::Module("V1Response"),
                                                       b_EventFailed(b_EventFailed),
                                                       e_Category(LISTEN) // Skip system and custom, nothing in default
{
    MRH::AB::ModuleLogger::Singleton().Log(GetName(), "V1 event checking started...", 
                                           "V1Response.cpp", __LINE__);
}

V1Response::~V1Response() noexcept
{    
    MRH::AB::ModuleLogger::Singleton().Log(GetName(), "V1 event checking finished.", 
                                           "V1Response.cpp", __LINE__);
    
}

//*************************************************************************************
// Update
//*************************************************************************************

void V1Response::HandleEvent(const MRH_Event* p_Event)
{}

MRH::AB::Module::Result V1Response::Update()
{
    if (b_EventFailed == true)
    {
        MRH::AB::ModuleLogger::Singleton().Log(GetName(), "V1 event checking failed!", 
                                               "V1Response.cpp", __LINE__);
        return MRH::AB::Module::FINISHED_POP;
    }
    else if (e_Category == CATEGORY_COUNT)
    {
        MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Finished checking V1 events.", 
                                               "V1Response.cpp", __LINE__);
        return MRH::AB::Module::FINISHED_POP;
    }
    
    return MRH::AB::Module::FINISHED_APPEND;
}

std::unique_ptr<MRH::AB::Module> V1Response::NextModule()
{
    switch (e_Category)
    {
        case CUSTOM:
            e_Category = (Category)(e_Category + 1);
            return std::make_unique<CustomV1Response>(b_EventFailed);
        case LISTEN:
            e_Category = (Category)(e_Category + 1);
            return std::make_unique<ListenV1Response>(b_EventFailed);
        case SAY:
            e_Category = (Category)(e_Category + 1);
            return std::make_unique<SayV1Response>(b_EventFailed);
        case PASSWORD:
            e_Category = (Category)(e_Category + 1);
            return std::make_unique<PasswordV1Response>(b_EventFailed);
        case USER:
            e_Category = (Category)(e_Category + 1);
            return std::make_unique<UserV1Response>(b_EventFailed);
        case APPLICATION:
            e_Category = (Category)(e_Category + 1);
            return std::make_unique<ApplicationV1Response>(b_EventFailed);
            
        default:
            throw MRH::AB::ModuleException(GetName(),
                                           "No module to switch to!");
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool V1Response::CanHandleEvent(MRH_Uint32 u32_Type) noexcept
{
    return false;
}
