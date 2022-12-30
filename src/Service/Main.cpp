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

// C / C++
#include <cstring>

// External
#include <libmrhs.h>
#include <libmrhevdata.h>

// Project

// Namespace
namespace
{
    // Context to send with
    const MRH_S_SendContext* p_SendContext;

    // Event to send
    MRH_Uint32 u32_ToSend = MRH_EVENT_SAY_NOTIFICATION_SERVICE_U;

    // Update
    int i_Update = 0;
}


// Prevent name wrangling
#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Init
    //*************************************************************************************

    int MRH_Init(const MRH_S_SendContext* p_SendContext)
    {
        ::p_SendContext = p_SendContext;
    
        return 0;
    }

    //*************************************************************************************
    // Update
    //*************************************************************************************

    int MRH_Update(void)
    {
        /**
         +  Quit
         */

        if (i_Update < 0)
        {
            return -1;
        }

        /**
         *  Create Event
         */

        MRH_Event* p_Event = NULL;

        switch (u32_ToSend)
        {
            // Chain

            // Last
            case MRH_EVENT_SAY_NOTIFICATION_SERVICE_U:
            {
                MRH_EvD_S_NotificationService_U c_Data;

                c_Data.u8_Type = 0;
                strncpy(c_Data.p_String, "Hello World (Service)!", MRH_EVD_S_NOTIFICATION_BUFFER_MAX_TERMINATED);

                if ((p_Event = MRH_EVD_CreateSetEvent(u32_ToSend, &c_Data)) == NULL)
                {
                    return -1;
                }

                // @NOTE: We want one empty update!
                i_Update = -1;
                break;
            }
        }

        /**
         *  Send Event
         */

        while (true)
        {
            switch (MRH_S_SendEvent(p_SendContext, &p_Event))
            {
                case MRH_S_SEND_OK:
                    return 0;
                case MRH_S_SEND_RETRY:
                    break;

                default:
                    return -1;
            }
        }
    }

    //*************************************************************************************
    // Exit
    //*************************************************************************************

    void MRH_Exit(void)
    {}

#ifdef __cplusplus
}
#endif
