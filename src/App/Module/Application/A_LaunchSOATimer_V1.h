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

#ifndef A_LaunchSOATimer_V1_h
#define A_LaunchSOATimer_V1_h

// C / C++

// External

// Project
#include "./A_LaunchSOAClearTimer_V1.h"

// Pre-defined
#ifndef MRH_EVENT_TEST_LAUNCH_PACKAGE_PATH
    #define MRH_EVENT_TEST_LAUNCH_PACKAGE_PATH "/opt/mrh/de.mrh.version.soa"
#endif


class A_LaunchSOATimer_V1 : public EventModule
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

    A_LaunchSOATimer_V1(std::shared_ptr<EventSender>& p_EventSender) : EventModule("MRH_EVENT_APP_LAUNCH_SOA_TIMER",
                                                                                   MRH_EVENT_APP_LAUNCH_SOA_TIMER_S,
                                                                                   false,
                                                                                   p_EventSender)
    {
        memset(c_Launch.p_PackagePath, '\0', MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX_TERMINATED);
        strncpy(c_Launch.p_PackagePath, MRH_EVENT_TEST_LAUNCH_PACKAGE_PATH, MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX);

        memset(c_Launch.p_LaunchInput, '\0', MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX_TERMINATED);
        strncpy(c_Launch.p_LaunchInput, "ÄÖÜ", MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX);

        c_Launch.s32_LaunchCommandID = 1;

        c_Launch.u64_LaunchTimepointS = time(NULL) + 100;


        MRH_Event* p_Event = MRH_EVD_CreateSetEvent(MRH_EVENT_APP_LAUNCH_SOA_TIMER_U, &c_Launch);

        if (p_Event == NULL)
        {
            throw MRH::AB::ModuleException(GetIdentifier(),
                                           "Failed to create MRH_EVENT_APP_LAUNCH_SOA_TIMER_U event!");
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

    ~A_LaunchSOATimer_V1() noexcept
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
        return std::make_unique<A_LaunchSOAClearTimer_V1>(p_EventSender);
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
        MRH_EvD_A_LaunchSOATimer_S c_Data;

        if (MRH_EVD_ReadEvent(&c_Data, p_Event->u32_Type, p_Event) < 0)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, GetIdentifier() +
                                                                     ": Failed to read event!",
                                             "A_LaunchSOATimer_V1.h", __LINE__);
            return false;
        }
        else if (strncmp(c_Data.p_PackagePath, c_Launch.p_PackagePath, MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX) != 0)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, GetIdentifier() +
                                                                     ": Launch path mismatch!",
                                             "A_LaunchSOATimer_V1.h", __LINE__);
            return false;

        }
        else if (strncmp(c_Data.p_LaunchInput, c_Launch.p_LaunchInput, MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX) != 0)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, GetIdentifier() +
                                                                     ": Launch input mismatch!",
                                             "A_LaunchSOATimer_V1.h", __LINE__);
            return false;

        }
        else if (c_Data.s32_LaunchCommandID != c_Launch.s32_LaunchCommandID)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, GetIdentifier() +
                                                                     ": Launch command ID mismatch!",
                                             "A_LaunchSOATimer_V1.h", __LINE__);
            return false;
        }
        else if (c_Data.u64_LaunchTimepointS != c_Launch.u64_LaunchTimepointS)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, GetIdentifier() +
                                                                     ": Launch time point mismatch!",
                                             "A_LaunchSOATimer_V1.h", __LINE__);
            return false;
        }

        return true;
    }

    //*************************************************************************************
    // Data
    //*************************************************************************************

    MRH_EvD_A_LaunchSOATimer_U c_Launch;

protected:

};

#endif /* A_LaunchSOATimer_V1_h */
