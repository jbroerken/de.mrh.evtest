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

#ifndef ListenGetMethodV1_h
#define ListenGetMethodV1_h

// C / C++

// External

// Project
#include "../../../EventModule.h"


class ListenGetMethodV1 : public EventModule
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
    
    ListenGetMethodV1(bool& b_EventFailed) noexcept : EventModule("ListenGetMethodV1",
                                                                  b_EventFailed)
    {
        MRH_Event* p_Event = MRH_EVD_CreateEvent(MRH_EVENT_LISTEN_GET_METHOD_U, NULL, 0);
        
        try
        {
            MRH::AB::EventStorage::Singleton().Add(p_Event);
        }
        catch (...)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to add user event!", 
                                                   "ListenGetMethodV1.h", __LINE__);
            MRH_EVD_DestroyEvent(p_Event);
        }
    }
    
    /**
     *  Default destructor.
     */
    
    ~ListenGetMethodV1() noexcept
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
        MRH_EvD_L_GetMethod_S c_Data;
        
        if (MRH_EVD_ReadEvent(&c_Data, p_Event->u32_Type, p_Event) < 0)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to read event!", 
                                                   "ListenGetMethodV1.h", __LINE__);
            SetEventFailed();
        }
        else if (c_Data.u8_Result == MRH_EvD_Base_Result::MRH_EVD_BASE_RESULT_FAILED)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Get method failed!", 
                                                   "ListenGetMethodV1.h", __LINE__);
            SetEventFailed();
        }
        else
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Event check succeeded.", 
                                                   "ListenGetMethodV1.h", __LINE__);
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
        return (u32_Type == MRH_EVENT_LISTEN_GET_METHOD_S);
    }
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
protected:
    
};

#endif /* ListenGetMethodV1_h */
