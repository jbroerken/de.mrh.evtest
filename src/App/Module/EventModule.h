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

#ifndef EventModule_h
#define EventModule_h

// C / C++
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <string>

// External
#include <libmrhab/Module/MRH_Module.h>
#include <libmrhab/MRH_ABLogger.h>

// Project
#include "../EventSender.h"


class EventModule : public MRH::AB::Module
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    /**
     *  Default destructor.
     */

    virtual ~EventModule() noexcept
    {}

    //*************************************************************************************
    // Update
    //*************************************************************************************

    /**
     *  Perform a module update.
     *
     *  \param p_Event The received event to update with.
     *
     *  \return The module update result.
     */

    MRH::AB::Module::Result Update(const MRH_Event* p_Event) override
    {
        /**
         *  System Events?
         */

        if (p_Event->u32_Type == MRH_EVENT_NOT_IMPLEMENTED_S)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::WARNING, GetIdentifier() +
                                                                       ": MRH_EVENT_NOT_IMPLEMENTED_S",
                                             "EventModule.h", __LINE__);
            if (b_IsLast == true)
            {
                return MRH::AB::Module::FINISHED_POP;
            }
            else
            {
                return MRH::AB::Module::FINISHED_REPLACE;
            }
        }
        else if (p_Event->u32_Type == MRH_EVENT_PERMISSION_DENIED)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::WARNING, GetIdentifier() +
                                                                       ": MRH_EVENT_PERMISSION_DENIED",
                                             "EventModule.h", __LINE__);
            if (b_IsLast == true)
            {
                return MRH::AB::Module::FINISHED_POP;
            }
            else
            {
                return MRH::AB::Module::FINISHED_REPLACE;
            }
        }

        /**
         *  Event Check
         */

        if (EventValid(p_Event) == true)
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, "Event test succeeded: " +
                                                                     GetIdentifier(),
                                             "EventModule.h", __LINE__);
            if (b_IsLast == true)
            {
                return MRH::AB::Module::FINISHED_POP;
            }
            else
            {
                return MRH::AB::Module::FINISHED_REPLACE;
            }
        }
        else
        {
            MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::ERROR, "Event test failed: " +
                                                                     GetIdentifier(),
                                             "EventModule.h", __LINE__);

            return MRH::AB::Module::FINISHED_POP;
        }
    }

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Check if the module can handle a received event.
     *
     *  \param u32_Type The type of the received event to handle.
     *
     *  \return true if the event can be handled, false if not.
     */

    bool CanHandleEvent(MRH_Uint32 u32_Type) const noexcept override
    {
        switch (u32_Type)
        {
            case MRH_EVENT_NOT_IMPLEMENTED_S:
                return true;
            case MRH_EVENT_PERMISSION_DENIED:
                return true;

            default:
                if (u32_Type == u32_ResponseEvent)
                {
                    return true;
                }
                return false;
        }
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

    virtual bool EventValid(const MRH_Event* p_Event) noexcept
    {
        return false;
    }

    //*************************************************************************************
    // Data
    //*************************************************************************************

    MRH_Uint32 u32_ResponseEvent;
    bool b_IsLast;

protected:

    //*************************************************************************************
    // Constructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param s_Identifier The module identifier.
     *  \param u32_ResponseEvent The response event to check for.
     *  \param b_IsLast If this is the last event to check.
     *  \param p_EventSender The event sender to use.
     */

    EventModule(std::string const& s_Identifier,
                MRH_Uint32 u32_ResponseEvent,
                bool b_IsLast,
                std::shared_ptr<EventSender>& p_EventSender) noexcept : MRH::AB::Module(s_Identifier),
                                                                        u32_ResponseEvent(u32_ResponseEvent),
                                                                        b_IsLast(b_IsLast),
                                                                        p_EventSender(p_EventSender)
    {
        MRH::AB::Logger::Singleton().Log(MRH::AB::Logger::INFO, "Performing event test: " +
                                                                GetIdentifier(),
                                         "EventModule.h", __LINE__);
    }

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::shared_ptr<EventSender> p_EventSender;
};

#endif /* EventModule_h */
