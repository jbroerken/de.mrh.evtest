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

#ifndef A_Custom_V1_h
#define A_Custom_V1_h

// C / C++

// External

// Project
#include "../EventModule.h"


class A_Custom_V1 : public EventModule
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

    A_Custom_V1(std::shared_ptr<EventSender>& p_EventSender) : EventModule("MRH_EVENT_APP_CUSTOM_COMMAND",
                                                                           MRH_EVENT_APP_CUSTOM_COMMAND_S,
                                                                           true,
                                                                           p_EventSender)
    {
        MRH_Event* p_Event = MRH_EVD_CreateEvent(MRH_EVENT_APP_CUSTOM_COMMAND_U, NULL, 0);

        if (p_Event == NULL)
        {
            throw MRH::AB::ModuleException(GetIdentifier(),
                                           "Failed to create MRH_EVENT_APP_CUSTOM_COMMAND_U event!");
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

    ~A_Custom_V1() noexcept
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
        throw MRH::AB::ModuleException(GetIdentifier(),
                                       "No module to switch to!");
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
        if (p_Event->u32_Type != MRH_EVENT_APP_CUSTOM_COMMAND_S)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, GetIdentifier() +
                                                                     ": Wrong event type!",
                                             "A_Custom_V1.h", __LINE__);
            return false;
        }

        return true;
    }

    //*************************************************************************************
    // Data
    //*************************************************************************************

protected:

};

#endif /* A_Custom_V1_h */
