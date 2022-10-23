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

#ifndef ServiceCustomCommandV1_h
#define ServiceCustomCommandV1_h

// C / C++

// External

// Project
#include "../../EventModule.h"


class ServiceCustomCommandV1 : public EventModule
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *  
     *  \param b_EventFailed The event processing failed flag.  
     *  \param s_CategoryName The service category name.
     *  \param u32_SendEvent The event to send.
     *  \param u32_ResponseEvent The expected response event.  
     */
    
    ServiceCustomCommandV1(bool& b_EventFailed,
                           std::string const& s_CategoryName,
                           MRH_Uint32 u32_SendEvent,
                           MRH_Uint32 u32_ResponseEvent) noexcept : EventModule("ServiceCustomCommandV1",
                                                                                b_EventFailed),
                                                                    s_CategoryName(s_CategoryName),
                                                                    u32_ResponseEvent(u32_ResponseEvent)
    {
        MRH_Event* p_Event = MRH_EVD_CreateEvent(u32_SendEvent, NULL, 0);
        
        try
        {
            MRH::AB::EventStorage::Singleton().Add(p_Event);
        }
        catch (...)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to add " + s_CategoryName + " user event!", 
                                                   "ServiceCustomCommandV1.h", __LINE__);
            MRH_EVD_DestroyEvent(p_Event);
        }
    }
    
    /**
     *  Default destructor.
     */
    
    ~ServiceCustomCommandV1() noexcept
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
        MRH_EvD_Base_CustomCommand_t c_Data;
        
        if (MRH_EVD_ReadEvent(&c_Data, p_Event->u32_Type, p_Event) < 0)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to read " + s_CategoryName + " event!", 
                                                   "ServiceCustomCommandV1.h", __LINE__);
            SetEventFailed();
        }
        else
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), s_CategoryName + " event check succeeded.", 
                                                   "ServiceCustomCommandV1.h", __LINE__);
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
        return (u32_Type == u32_ResponseEvent);
    }
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::string s_CategoryName;
    MRH_Uint32 u32_ResponseEvent;
    
protected:
    
};

#endif /* ServiceCustomCommandV1_h */
