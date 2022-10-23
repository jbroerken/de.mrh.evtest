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

#ifndef EventModule_h
#define EventModule_h

// C / C++

// External
#include <libmrhab/Module/MRH_Module.h>
#include <libmrhevdata.h>

// Project

// Pre-defined
#define TIMED_MODULE_TIMEOUT_MS 30000


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
     *  Hand a received event to the module.
     *
     *  \param p_Event The received event.
     */
    
    void HandleEvent(const MRH_Event* p_Event) override
    {
        switch (p_Event->u32_Type)
        {
            case MRH_EVENT_PERMISSION_DENIED:
                MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Event is permission protected.", 
                                                       "EventModule.h", __LINE__);
                b_EventHandled = true;
                break;
            case MRH_EVENT_PASSWORD_REQUIRED:
                MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Event is password protected.", 
                                                       "EventModule.h", __LINE__);
                b_EventHandled = true;
                break;
            case MRH_EVENT_NOT_IMPLEMENTED_S:
                MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Event is not implemented.", 
                                                       "EventModule.h", __LINE__);
                b_EventHandled = true;
                break;
                
            default:
                _HandleEvent(p_Event);
                break;
        }
    }
    
    /**
     *  Perform a module update.
     *
     *  \return The module update result.
     */
    
    MRH::AB::Module::Result Update() override
    {
        if (b_EventHandled == true)
        {
            return MRH::AB::Module::FINISHED_POP;
        }
        else if (b_EventFailed == true)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Module popped due to event failure.", 
                                                   "EventModule.h", __LINE__);
            return MRH::AB::Module::FINISHED_POP;
        }
        else if (c_Timer.GetTimerFinished() == true)
        {
            MRH::AB::ModuleLogger::Singleton().Log(GetName(), "Module popped due to timeout.", 
                                                   "EventModule.h", __LINE__);
            
            b_EventFailed = true;
            return MRH::AB::Module::FINISHED_POP;
        }
        
        return MRH::AB::Module::IN_PROGRESS;
    }
    
    /**
     *  Get the module to switch to.
     *
     *  \return The module switch information.
     */
    
    std::unique_ptr<MRH::AB::Module> NextModule() override
    {
        throw MRH::AB::ModuleException(GetName(),
                                       "No module to switch to!");
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
    
    bool CanHandleEvent(MRH_Uint32 u32_Type) noexcept override
    {
        switch (u32_Type)
        {
            case MRH_EVENT_UNK:
                return false;
                
            case MRH_EVENT_PERMISSION_DENIED:
            case MRH_EVENT_PASSWORD_REQUIRED:
            case MRH_EVENT_NOT_IMPLEMENTED_S:
                return true;
                
            default:
                return _CanHandleEvent(u32_Type);
        }
    }
    
private:
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Hand a received event to the module.
     *
     *  \param p_Event The received event.
     */
    
    virtual void _HandleEvent(const MRH_Event* p_Event)
    {
        throw MRH::AB::ModuleException(GetName(), 
                                       "Base timed handle event function called!");
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
    
    virtual bool _CanHandleEvent(MRH_Uint32 u32_Type) noexcept
    {
        return false;
    }
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    bool& b_EventFailed;
    
    MRH::AB::ModuleTimer c_Timer;
    bool b_EventHandled;
    
protected:
    
    //*************************************************************************************
    // Constructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *  
     *  \param s_Name The module name.
     *  \param b_EventFailed The event processing failed flag.
     */
    
    EventModule(std::string const& s_Name,
                bool& b_EventFailed) noexcept : MRH::AB::Module(s_Name,
                                                                true),
                                                c_Timer(TIMED_MODULE_TIMEOUT_MS),
                                                b_EventFailed(b_EventFailed),
                                                b_EventHandled(false)
    {
        b_EventFailed = false;
    }
    
    //*************************************************************************************
    // Setters
    //*************************************************************************************
    
    /**
     *  Set the event handled flag.
     */
    
    void SetEventHandled() noexcept
    {
        b_EventHandled = true;
    }
    
    /**
     *  Set the event failed flag.
     */
    
    void SetEventFailed() noexcept
    {
        b_EventFailed = true;
    }
};

#endif /* EventModule_h */
