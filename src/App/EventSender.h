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

#ifndef EventSender_h
#define EventSender_h

// C / C++

// External
#include <libmrh/Send/MRH_SendEvent.h>
#include <libmrhab/Event/MRH_EventStorage.h>
#include <libmrhab/MRH_ABLogger.h>

// Project


class EventSender : private MRH::AB::EventStorage
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param p_SendContext The send context used for event sending.
     */

    EventSender(const MRH_A_SendContext* p_SendContext) noexcept : EventStorage("Event Sender"),
                                                                   p_SendContext(p_SendContext)
    {}

    /**
     *  Default destructor.
     */

    ~EventSender() noexcept
    {}

    //*************************************************************************************
    // Send
    //*************************************************************************************

    /**
     *  Send a event to services. Previous unsent events will also be attempted to be sent.
     *
     *  \param p_Event The event to send. The event is consumed.
     */

    void Send(MRH_Event*& p_Event)
    {
        MRH::AB::Logger& c_Logger = MRH::AB::Logger::Singleton();

        try
        {
            Add(p_Event);

            MRH_Event* p_ToSend;

            while ((p_ToSend = GetEvent(0)) != NULL)
            {
                c_Logger.Log(MRH::AB::Logger::INFO, "Sending event " +
                                                    std::to_string(p_ToSend->u32_Type) +
                                                    "...",
                             "EventSender.h", __LINE__);

                if (MRH_A_SendEvent(p_SendContext, &p_ToSend) == MRH_A_Send_Result::MRH_A_SEND_FAILURE)
                {
                    throw MRH::AB::ABException("Failed to send event!");
                }
            }
        }
        catch (std::exception& e)
        {
            throw MRH::AB::ABException(e.what());
        }
    }

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

    const MRH_A_SendContext* p_SendContext;

protected:

};

#endif /* EventSender_h */
