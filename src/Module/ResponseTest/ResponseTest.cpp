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
#include "./ResponseTest.h"
#include "./V1/V1Response.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

ResponseTest::ResponseTest() noexcept : MRH::AB::Module("ResponseTest"),
                                        e_Version(V1),
                                        b_EventFailed(false)
{}

ResponseTest::~ResponseTest() noexcept
{}

//*************************************************************************************
// Update
//*************************************************************************************

void ResponseTest::HandleEvent(const MRH_Event* p_Event)
{}

MRH::AB::Module::Result ResponseTest::Update()
{
    if (b_EventFailed == true)
    {
        MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Event checking failed!", 
                                               "ResponseTest.cpp", __LINE__);
        return MRH::AB::Module::FINISHED_POP;
    }
    else if (e_Version == VERSION_COUNT)
    {
        MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Successfully checked all events.", 
                                               "ResponseTest.cpp", __LINE__);
        return MRH::AB::Module::FINISHED_POP;
    }
    
    return MRH::AB::Module::FINISHED_APPEND;
}

std::unique_ptr<MRH::AB::Module> ResponseTest::NextModule()
{
    switch (e_Version)
    {
        case V1:
            e_Version = (Version)(e_Version + 1);
            return std::make_unique<V1Response>(b_EventFailed);
            
        default:
            throw MRH::AB::ModuleException(GetName(),
                                           "No module to switch to!");
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool ResponseTest::CanHandleEvent(MRH_Uint32 u32_Type) noexcept
{
    return false;
}
