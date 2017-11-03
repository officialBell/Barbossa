//
//  aimbot.cpp
//  vHook
//

#include "../Hacks/aimbot.h"
#include "../Hacks/antiaim.h"
#include "../Hacks/autowall.h"

C_BaseEntity* Aimbot::curTarget = nullptr;

int MakeHitscan(C_BaseEntity* entity)
{
    vector<int> hitboxes;


    hitboxes.push_back(HITBOX_HEAD);
    hitboxes.push_back(HITBOX_NECK);
    hitboxes.push_back(HITBOX_LOWER_NECK);
    hitboxes.push_back(HITBOX_PELVIS);
    hitboxes.push_back(HITBOX_BODY);
    hitboxes.push_back(HITBOX_THORAX);
    hitboxes.push_back(HITBOX_CHEST);
    hitboxes.push_back(HITBOX_UPPER_CHEST);
    hitboxes.push_back(HITBOX_RIGHT_THIGH);
    hitboxes.push_back(HITBOX_LEFT_THIGH);
    hitboxes.push_back(HITBOX_RIGHT_CALF);
    hitboxes.push_back(HITBOX_LEFT_CALF);
    hitboxes.push_back(HITBOX_RIGHT_FOOT);
    hitboxes.push_back(HITBOX_LEFT_FOOT);
    hitboxes.push_back(HITBOX_RIGHT_HAND);
    hitboxes.push_back(HITBOX_LEFT_HAND);
    hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
    hitboxes.push_back(HITBOX_RIGHT_FOREARM);
    hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
    hitboxes.push_back(HITBOX_LEFT_FOREARM);

    if(vars.aimbot.hitscan)
    {
        
        int highestdamage_hitbox = 0;
        int highestdamage = 0;
        
        for(auto hit : hitboxes)
        {
            
            Vector vDest = GetHitboxPosition(entity, hit);
            float thisDamage = 0.f;
            Autowall::FireBulletData data;
            
            thisDamage = Autowall::GetDamage(vDest, true, data);
            
            if(thisDamage > highestdamage)
            {
                highestdamage = thisDamage;
                highestdamage_hitbox = hit;
            }
            
        }
        
        return highestdamage_hitbox;
    }
    else
    {
        return vars.aimbot.hitbox;
    }
    
}



void DoAim(CUserCmd* pCmd, C_BaseEntity* local, C_BaseCombatWeapon* weapon, float& flForward, float& flSide, bool& bSendPacket)
{
    
    if(!vars.aimbot.enabled)
        return;
    
    Vector eyepos = local->GetEyePosition();
    
    for(int i = 0; i < pEntList->GetHighestEntityIndex(); i++)
    {
        auto* entity = pEntList->GetClientEntity(i);
        
        if(!entity)
            continue;
        
        if(entity == local)
            continue;
        
        if(entity->GetTeam() == local->GetTeam())
            continue;
        
        if(entity->GetHealth() < 1)
            continue;
        
        if(entity->GetDormant())
            continue;
        
        if(entity->GetImmune())
            continue;
        
        if(weapon->GetAmmo() < 1)
            continue;
        
        if(weapon->IsKnife() || weapon->IsBomb())
            continue;
        
        if(gCorrections[entity->GetIndex()].whitelist)
            continue;
        
        Vector vFrom, vTo;
        
        Aimbot::curTarget = entity;
        
        vFrom = GetHitboxPosition(entity, MakeHitscan(entity));
        
        if(vars.aimbot.selfpred)
            eyepos += entity->GetVelocity() * pGlobals->interval_per_tick;
        
        vTo = CalcAngle(eyepos, vFrom);
        
        auto isVISIBLE = IsVisible(local, entity);
        float getdmg = 0.f;
        Autowall::FireBulletData data;
        bool canHit = false;
        
        if(vars.aimbot.autowall)
        {
            getdmg = Autowall::GetDamage(vFrom, true, data);
            canHit = getdmg >= vars.aimbot.mindmg;
        }
        
        
        atTargets = vTo;
        
        if(canHit || isVISIBLE)
        {
            if(GetFOV(pCmd->viewangles, local->GetEyePosition(), vFrom) <= vars.aimbot.fov)
            {
                if(vars.aimbot.autoshoot)
                {

                    pCmd->buttons |= IN_ATTACK;
                }
                
                if (vars.aimbot.autoscope && weapon->GetCSWpnData()->m_iZoomLevels > 0 && !local->IsScoped())
                    pCmd->buttons |= IN_ATTACK2;
        
                if(vars.aimbot.autostop)
                {
                    flForward = 0.f;
                    flSide = 0.f;
                }
                
                if(vars.aimbot.autocrouch)
                {
                    pCmd->buttons |= IN_DUCK;
                }
                
                bool bAttack = true;
                
                if (weapon->GetNextPrimaryAttack() - pGlobals->interval_per_tick > local->GetTickBase() * pGlobals->interval_per_tick)
                    bAttack = false;
                
                if(pCmd->buttons & IN_ATTACK)
                {
                    if(!vars.aimbot.silent)
                    {
                        pCmd->viewangles = vTo;
                        pEngine->SetViewAngles(pCmd->viewangles);
                    }
                    else
                    {
                        pCmd->viewangles = vTo;
                    }
                    
                }
                
            }
            
        }
        
    }
    
}
