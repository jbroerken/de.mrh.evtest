/**
 *  Copyright (C) 2021 - 2022 The de.mrh.evtest Authors.
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

#ifndef P_Set_V1_h
#define P_Set_V1_h

// C / C++

// External

// Project
#include "./P_Custom_V1.h"

// Pre-defined
#ifndef MRH_EVENT_TEST_SET_PASSWORD
    #define MRH_EVENT_TEST_SET_PASSWORD "new_password"
#endif


class P_Set_V1 : public EventModule
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param p_EventSender The event sender to use.
     */

    P_Set_V1(std::shared_ptr<EventSender>& p_EventSender) : EventModule("MRH_EVENT_PASSWORD_SET",
                                                                        MRH_EVENT_PASSWORD_SET_S,
                                                                        false,
                                                                        p_EventSender)
    {
        MRH_EvD_P_Set_U c_Data;

        memset(c_Data.p_String, '\0', MRH_EVD_P_STRING_BUFFER_MAX_TERMINATED);
        strncpy(c_Data.p_String, MRH_EVENT_TEST_SET_PASSWORD, MRH_EVD_P_STRING_BUFFER_MAX);

        MRH_Event* p_Event = MRH_EVD_CreateSetEvent(MRH_EVENT_PASSWORD_SET_U, &c_Data);

        if (p_Event == NULL)
        {
            throw MRH::AB::ModuleException(GetIdentifier(),
                                           "Failed to create MRH_EVENT_PASSWORD_SET_U event!");
        }

        try
        {
            p_EventSender->Send(p_Event);
        }
        catch (std::exception& e)
        {
            MRH_EVD_DestroyEvent(p_Event);

            throw MRH::AB::ModuleException(GetIdentifier(),
                                           e.what());
        }
    }

    /**
     *  Default destructor.
     */

    ~P_Set_V1() noexcept
    {}

    //*************************************************************************************
    // Update
    //*************************************************************************************

    /**
     *  Get the module to switch to.
     *
     *  \return The module switch information.
     */

    std::unique_ptr<MRH::AB::Module> NextModule() override
    {
        return std::make_unique<P_Custom_V1>(p_EventSender);
    }

private:

    //*************************************************************************************
    // Update
    //*************************************************************************************

    /**
     *  Check if a event check succeeded.
     *
     *  \param p_Event The event to check.
     *
     *  \return true if succeeded, false if not.
     */

    bool EventValid(const MRH_Event* p_Event) noexcept override
    {
        MRH_EvD_P_Set_S c_Data;

        if (MRH_EVD_ReadEvent(&c_Data, p_Event->u32_Type, p_Event) < 0)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, GetIdentifier() +
                                                                     ": Failed to read event!",
                                             "P_Set_V1.h", __LINE__);
            return false;
        }
        else if (c_Data.u8_Result != MRH_EvD_Base_Result::MRH_EVD_BASE_RESULT_SUCCESS)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, GetIdentifier() +
                                                                     ": Password set failed!",
                                             "P_Set_V1.h", __LINE__);
            return false;
        }

        return true;
    }

    //*************************************************************************************
    // Data
    //*************************************************************************************

protected:

};

#endif /* P_Set_V1_h */
