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

// C / C++

// External
#include <libmrh/MRH_AppLoop.h>
#include <libmrhab.h>

// Project
#include "./Module/Listen/L_Custom_V1.h"
#include "./EventSender.h"
#include "./Revision.h"

// Pre-defined
namespace
{
    MRH::AB::ModuleStack* p_ModuleStack = NULL;
}


// Prevent name wrangling
#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Init
    //*************************************************************************************

    int MRH_Init(const MRH_A_SendContext* p_SendContext, const char* p_LaunchInput, int i_LaunchCommandID)
    {
        try
        {
            std::shared_ptr<EventSender> p_EventSender = std::make_shared<EventSender>(p_SendContext);

            std::unique_ptr<MRH::AB::Module> p_TopModule = std::make_unique<L_Custom_V1>(p_EventSender);

            p_ModuleStack = new MRH::AB::ModuleStack(p_TopModule);

            return 0;
        }
        catch (std::exception& e)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, e.what(),
                                             "Main.cpp", __LINE__);
            return -1;
        }
    }

    //*************************************************************************************
    // Update
    //*************************************************************************************

    int MRH_Update(const MRH_Event* p_Event)
    {
        try
        {
            if (p_ModuleStack->Update(p_Event) == true)
            {
                return 0;
            }
        }
        catch (MRH::AB::ABException& e)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, e.what(),
                                             "Main.cpp", __LINE__);
        }

        return -1;
    }

    //*************************************************************************************
    // Exit
    //*************************************************************************************

    void MRH_Exit(void)
    {
        if (p_ModuleStack != NULL)
        {
            delete p_ModuleStack;
        }
    }
    
#ifdef __cplusplus
}
#endif
