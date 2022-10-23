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

#ifndef SayNotificationV1_h
#define SayNotificationV1_h

// C / C++
#include <cstring>

// External

// Project
#include "../../../EventModule.h"


class SayNotificationV1 : public EventModule
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
    
    SayNotificationV1(bool& b_EventFailed) noexcept : EventModule("SayNotificationV1",
                                                                  b_EventFailed)
    {
        MRH_EvD_S_NotificationApp_U c_Data;
        c_Data.u8_Type = 0;
        strncpy(c_Data.p_String, "Hellö 123!", MRH_EVD_S_NOTIFICATION_BUFFER_MAX_TERMINATED);
        
        MRH_Event* p_Event = MRH_EVD_CreateSetEvent(MRH_EVENT_SAY_NOTIFICATION_APP_U, &c_Data);
        
        try
        {
            MRH::AB::EventStorage::Singleton().Add(p_Event);
        }
        catch (...)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to add user event!", 
                                                   "SayNotificationV1.h", __LINE__);
            MRH_EVD_DestroyEvent(p_Event);
        }
    }
    
    /**
     *  Default destructor.
     */
    
    ~SayNotificationV1() noexcept
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
        MRH_EvD_S_NotificationApp_S c_Data;
        
        if (MRH_EVD_ReadEvent(&c_Data, p_Event->u32_Type, p_Event) < 0)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to read event!", 
                                                   "SayNotificationV1.h", __LINE__);
            SetEventFailed();
        }
        else if (c_Data.u8_Result == MRH_EvD_Base_Result::MRH_EVD_BASE_RESULT_FAILED)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Notification result failure!", 
                                                   "SayNotificationV1.h", __LINE__);
            SetEventFailed();
        }
        else
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Event check succeeded.", 
                                                   "SayNotificationV1.h", __LINE__);
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
        return (u32_Type == MRH_EVENT_SAY_NOTIFICATION_APP_S);
    }
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
protected:
    
};

#endif /* SayNotificationV1_h */
