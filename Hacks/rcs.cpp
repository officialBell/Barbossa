//
//  recoil.cpp
//  vHook
//
//  Created by James Henson on 11/1/17.
//  Copyright © 2017 ViKiNG. All rights reserved.
//

#include "rcs.h"
 //RCS
auto LegitRecoil(C_BaseEntity* local, CUserCmd* cmd) -> void {

    if(!vars.aimbot.rcs)
        return;
    
    C_BaseEntity* localplayer = (C_BaseEntity*) pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    if(!localplayer)
        return;
    
    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) pEntList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
    if(!activeWeapon->IsBomb())
        return;
    
    if(cmd->buttons & IN_ATTACK) {
        Vector localVec = local->GetPunchAngles() * 2.f;
        
        cmd->viewangles.x -= localVec.x * (2.f / 100.f * vars.aimbot.rcsf);;
        cmd->viewangles.y -= localVec.y * (2.f / 100.f * vars.aimbot.rcsf);;
    }
}

