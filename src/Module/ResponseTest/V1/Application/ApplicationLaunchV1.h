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

#ifndef ApplicationLaunchV1_h
#define ApplicationLaunchV1_h

// C / C++
#include <cstring>

// External

// Project
#include "../../../EventModule.h"


class ApplicationLaunchV1 : public EventModule
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *  
     *  \param b_EventFailed The event processing failed flag.  
     */
    
    ApplicationLaunchV1(bool& b_EventFailed) noexcept : EventModule("ApplicationLaunchV1",
                                                                    b_EventFailed),
                                                        s_PackagePath("/opt/mrh/de.mrh.version.soa"),
                                                        s_LaunchInput("test launch"),
                                                        s32_LaunchCommandID(700)
    {
        MRH_EvD_A_LaunchSOA_U c_Data;
        c_Data.s32_LaunchCommandID = s32_LaunchCommandID;
        strncpy(c_Data.p_PackagePath, s_PackagePath.c_str(), MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX_TERMINATED);
        strncpy(c_Data.p_LaunchInput, s_LaunchInput.c_str(), MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX_TERMINATED);
        
        MRH_Event* p_Event = MRH_EVD_CreateSetEvent(MRH_EVENT_APP_LAUNCH_SOA_U, &c_Data);
        
        try
        {
            MRH::AB::EventStorage::Singleton().Add(p_Event);
        }
        catch (...)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to add user event!", 
                                                   "ApplicationLaunchV1.h", __LINE__);
            MRH_EVD_DestroyEvent(p_Event);
        }
    }
    
    /**
     *  Default destructor.
     */
    
    ~ApplicationLaunchV1() noexcept
    {}
    
private:
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Hand a received event to the module.
     *
     *  \param p_Event The received event.
     */
    
    void _HandleEvent(const MRH_Event* p_Event) override
    {
        MRH_EvD_A_LaunchSOA_S c_Data;
        
        if (MRH_EVD_ReadEvent(&c_Data, p_Event->u32_Type, p_Event) < 0)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to read event!", 
                                                   "ApplicationLaunchV1.h", __LINE__);
            SetEventFailed();
        }
        else if (s_PackagePath.compare(c_Data.p_PackagePath) != 0)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Wrong package path returned!", 
                                                   "ApplicationLaunchV1.h", __LINE__);
            SetEventFailed();
        }
        else if (s_LaunchInput.compare(c_Data.p_LaunchInput) != 0)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Wrong launch input returned!", 
                                                   "ApplicationLaunchV1.h", __LINE__);
            SetEventFailed();
        }
        else if (c_Data.s32_LaunchCommandID != s32_LaunchCommandID)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Wrong launch command ID returned!", 
                                                   "ApplicationLaunchV1.h", __LINE__);
            SetEventFailed();
        }
        else
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Event check succeeded.", 
                                                   "ApplicationLaunchV1.h", __LINE__);
            SetEventHandled();
        }
    }
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Check if the module can handle a event.
     *
     *  \param u32_Type The type of the event to handle.
     *
     *  \return true if the event can be used, false if not.
     */
    
    bool _CanHandleEvent(MRH_Uint32 u32_Type) noexcept override
    {
        return (u32_Type == MRH_EVENT_APP_LAUNCH_SOA_S);
    }
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::string s_PackagePath;
    std::string s_LaunchInput;
    MRH_Sint32 s32_LaunchCommandID;
    
protected:
    
};

#endif /* ApplicationLaunchV1_h */
