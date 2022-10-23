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

#ifndef SayStringV1_h
#define SayStringV1_h

// C / C++
#include <cstring>

// External

// Project
#include "../../../EventModule.h"


class SayStringV1 : public EventModule
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
    
    SayStringV1(bool& b_EventFailed) noexcept : EventModule("SayStringV1",
                                                            b_EventFailed),
                                                u32_StringID(700)
    {
        MRH_EvD_S_String_U c_Data;
        c_Data.u32_ID = u32_StringID;
        strncpy(c_Data.p_String, "Hello ÄÖÜ!", MRH_EVD_S_STRING_BUFFER_MAX_TERMINATED);
        
        MRH_Event* p_Event = MRH_EVD_CreateSetEvent(MRH_EVENT_SAY_STRING_U, &c_Data);
        
        try
        {
            MRH::AB::EventStorage::Singleton().Add(p_Event);
        }
        catch (...)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to add user event!", 
                                                   "SayStringV1.h", __LINE__);
            MRH_EVD_DestroyEvent(p_Event);
        }
    }
    
    /**
     *  Default destructor.
     */
    
    ~SayStringV1() noexcept
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
        MRH_EvD_S_String_S c_Data;
        
        if (MRH_EVD_ReadEvent(&c_Data, p_Event->u32_Type, p_Event) < 0)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to read event!", 
                                                   "SayStringV1.h", __LINE__);
            SetEventFailed();
        }
        else if (c_Data.u32_ID != u32_StringID)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Wrong string ID returned!", 
                                                   "SayStringV1.h", __LINE__);
            SetEventFailed();
        }
        else
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Event check succeeded.", 
                                                   "SayStringV1.h", __LINE__);
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
        return (u32_Type == MRH_EVENT_SAY_STRING_S);
    }
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    MRH_Uint32 u32_StringID;
    
protected:
    
};

#endif /* SayStringV1_h */
