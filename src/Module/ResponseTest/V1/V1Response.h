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

#ifndef V1Response_h
#define V1Response_h

// C / C++

// External
#include <libmrhab/Module/MRH_Module.h>

// Project


class V1Response : public MRH::AB::Module
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *  
     *  \param b_EventFailed The event process failed flag.
     */
    
    V1Response(bool& b_EventFailed) noexcept;
    
    /**
     *  Default destructor.
     */
    
    ~V1Response() noexcept;
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Hand a received event to the module.
     *
     *  \param p_Event The received event.
     */
    
    void HandleEvent(const MRH_Event* p_Event) override;
    
    /**
     *  Perform a module update.
     *
     *  \return The module update result.
     */
    
    MRH::AB::Module::Result Update() override;
    
    /**
     *  Get the module to switch to.
     *
     *  \return The module switch information.
     */
    
    std::unique_ptr<MRH::AB::Module> NextModule() override;
    
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
    
    bool CanHandleEvent(MRH_Uint32 u32_Type) noexcept override;
    
private:
    
    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    enum Category
    {
        //SYSTEM = 0,
        CUSTOM = 0,
        LISTEN = 1,
        SAY = 2,
        PASSWORD = 3,
        USER = 4,
        APPLICATION = 5,
        
        CATEGORY_MAX = APPLICATION,
        
        CATEGORY_COUNT = CATEGORY_MAX + 1
    };
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    bool& b_EventFailed;
    Category e_Category;
    
protected:
    
};

#endif /* V1Response_h */
