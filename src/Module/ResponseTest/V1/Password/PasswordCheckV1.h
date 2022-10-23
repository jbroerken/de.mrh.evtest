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

#ifndef PasswordCheckV1_h
#define PasswordCheckV1_h

// C / C++
#include <cstring>

// External

// Project
#include "../../../EventModule.h"

// Pre-defined
#ifndef MRH_PASSWORD_VALID
    #define MRH_PASSWORD_VALID "password"
#endif
#ifndef MRH_PASSWORD_INVALID
    #define MRH_PASSWORD_INVALID "invalidpw"
#endif


class PasswordCheckV1 : public EventModule
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
    
    PasswordCheckV1(bool& b_EventFailed) noexcept : EventModule("PasswordCheckV1",
                                                                b_EventFailed),
                                                    b_ValidChecked(false)
    {
        CreatePasswordEvent(MRH_PASSWORD_VALID);
    }
    
    /**
     *  Default destructor.
     */
    
    ~PasswordCheckV1() noexcept
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
        MRH_EvD_P_Check_S c_Data;
        
        if (MRH_EVD_ReadEvent(&c_Data, p_Event->u32_Type, p_Event) < 0)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to read event!", 
                                                   "PasswordCheckV1.h", __LINE__);
            SetEventFailed();
            return;
        }
        
        if (b_ValidChecked == false)
        {
            if (c_Data.u8_Result == MRH_EvD_Base_Result::MRH_EVD_BASE_RESULT_FAILED)
            {
                MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Valid password is invalid!", 
                                                       "PasswordCheckV1.h", __LINE__);
                SetEventFailed();
            }
            else
            {
                CreatePasswordEvent(MRH_PASSWORD_INVALID);
                b_ValidChecked = true;
            }
        }
        else
        {
            if (c_Data.u8_Result == MRH_EvD_Base_Result::MRH_EVD_BASE_RESULT_SUCCESS)
            {
                MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Invalid password is valid!", 
                                                       "PasswordCheckV1.h", __LINE__);
                SetEventFailed();
            }
            else
            {
                MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Event check succeeded.", 
                                                       "PasswordCheckV1.h", __LINE__);
                SetEventHandled();
            }
        }
    }
    
    //*************************************************************************************
    // Create
    //*************************************************************************************
    
    /**
     *  Create a password event.
     *  
     *  \param s_Password The password to use.
     */
    
    void CreatePasswordEvent(std::string const& s_Password) noexcept
    {
        MRH_EvD_P_Check_U c_Data;
        strncpy(c_Data.p_String, s_Password.c_str(), MRH_EVD_P_STRING_BUFFER_MAX_TERMINATED);
        
        MRH_Event* p_Event = MRH_EVD_CreateSetEvent(MRH_EVENT_PASSWORD_CHECK_U, &c_Data);
        
        try
        {
            MRH::AB::EventStorage::Singleton().Add(p_Event);
        }
        catch (...)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Failed to add user event!", 
                                                   "PasswordCheckV1.h", __LINE__);
            MRH_EVD_DestroyEvent(p_Event);
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
        return (u32_Type == MRH_EVENT_PASSWORD_CHECK_S);
    }
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    bool b_ValidChecked;
    
protected:
    
};

#endif /* PasswordCheckV1_h */
