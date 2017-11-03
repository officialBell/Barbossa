//
//  fakewalk.cpp
//  vHook
//
//  Created by Warlauke on 10/20/17.
//  Copyright © 2017 ViKiNG. All rights reserved.
//

#include "fakewalk.h" // Pasted from JingWare

void Fakewalk(CUserCmd* cmd, C_BaseEntity* local)
{

    if(vars.aimbot.fakewalk)
        return;
        
    if(!pEngine->IsInGame())
        return;
    
    C_BaseEntity* localplayer = (C_BaseEntity*) pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    if(!localplayer)
        return;
    
    if(!pInputSystem->IsButtonDown(KEY_LALT))
        return;
    
    static int iChoked = -1;
    iChoked++;
    
    if (iChoked < 3)
    {
        *bSendPacket = false;
        cmd->tick_count += 10;
        cmd += 7 + cmd->tick_count % 2 ? 0 : 1;
        cmd->buttons |= local->GetMoveType() == IN_BACK;
        cmd->forwardmove = cmd->sidemove = 0.f;
    }
    else
    {
        *bSendPacket = true;
        iChoked = -1;
        pGlobals->frametime *= (local->GetVelocity().Length2D()) / 1.f;
        cmd->buttons |= local->GetMoveType() == IN_FORWARD;
    }
    
    
}
