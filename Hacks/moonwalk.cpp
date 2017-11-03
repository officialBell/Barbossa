//
//  moonwalk.cpp
//  vHook
//
//  Created by Warlauke on 10/25/17.
//  Copyright © 2017 ViKiNG. All rights reserved.
//

#include "moonwalk.h" // Pasted from JingWare

void Moonwalk(CUserCmd* cmd)
{
    if(vars.misc.moonwalk)
        return;
    
    if(!pEngine->IsInGame())
        return;
    
    C_BaseEntity* localplayer = (C_BaseEntity*) pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    if(!localplayer)
        return;
    
    if(cmd->forwardmove > 0)
    {
        cmd->buttons |= IN_BACK;
        cmd->buttons &= ~IN_FORWARD;
    }
    
    if(cmd->forwardmove < 0)
    {
        cmd->buttons |= IN_FORWARD;
        cmd->buttons &= ~IN_BACK;
    }
    
    if(cmd->sidemove < 0)
    {
        cmd->buttons |= IN_MOVERIGHT;
        cmd->buttons &= ~IN_MOVELEFT;
    }
    
    if(cmd->sidemove > 0)
    {
        cmd->buttons |= IN_MOVELEFT;
        cmd->buttons &= ~IN_MOVERIGHT;
    }
}
