#include "main.h"

#include "../Hacks/aimbot.h"
#include "../Hacks/antiaim.h"
#include "../Hacks/bhop.h"
#include "../Hacks/clantag.h"
#include "../Hacks/spammer.h"
#include "../Hacks/triggerbot.h"
#include "../Hacks/fakewalk.h"
#include "../Hacks/airstuck.h"
#include "../Hacks/recoilcrosshair.h"
#include "../Hacks/moonwalk.h"
#include "../Hacks/showrank.h"
#include "../Hacks/rcs.h"
#include "../Hacks/smoothing.h"

Vector tpangles;
bool* bSendPacket = nullptr;

void MakeNoRecoil(C_BaseEntity* local, CUserCmd* cmd)
{
    if(!vars.misc.enabled)
        return;
    
    if(!vars.misc.norecoil)
        return;
    
    if(cmd->buttons & IN_ATTACK)
    {
        Vector localVec = local->GetPunchAngles() * 2.f;
        cmd->viewangles.x -= localVec.x;
        cmd->viewangles.y -= localVec.y;
    }
    
}

string GetLocalName()
{
    player_info_t localInfo;
    pEngine->GetPlayerInfo(pEngine->GetLocalPlayer(), &localInfo);
    return localInfo.name;
}

void ChangeName(const char* szName)
{
    ConVar* cvar_name = pCvar->FindVar("name");
    *(int*)((uintptr_t)&cvar_name->fnChangeCallback + 0x15) = 0;
    cvar_name->SetValue(szName);
}

void MakeAutoPistolas(CUserCmd* cmd, C_BaseCombatWeapon* weapon)
{
    if(!vars.aimbot.autopistol)
        return;
    
    if(!weapon->IsPistol())
        return;
        
    if (cmd->buttons & IN_ATTACK)
    {
        static bool bAttack = false;
        
        if (bAttack)
            cmd->buttons |= IN_ATTACK;
        else
            cmd->buttons &= ~IN_ATTACK;
        
        bAttack = !bAttack;
    }
}


void Predictiom(CUserCmd* cmd, C_BaseCombatWeapon* weapon, C_BaseEntity* local)
{
    float backup = pGlobals->frametime;
    
    pGlobals->frametime = pGlobals->interval_per_tick;
    
    weapon->UpdateAccuracyPenalty();
    
    CMoveData data;
    memset(&data, 0, sizeof(data));
    
    pPrediction->SetupMove(local, cmd, pMoveHelper, &data);
    pGameMovement->ProcessMovement(local, &data);
    pPrediction->FinishMove(local, cmd, &data);
    
    pGlobals->frametime = backup;
}

void hacks(CUserCmd* cmd, C_BaseEntity* local, C_BaseCombatWeapon* weapon, Vector& vOldAngles, float& flForwardmove, float& flSidemove, bool& sendpacket) {
    
    DoAutoStrafe(cmd, local);
    
    DoBhop(cmd, local);
    
    Predictiom(cmd, weapon, local);
    
    CirlceStrafe(local, cmd, vOldAngles);
    
    DoAim(cmd, local, weapon, flForwardmove, flSidemove, sendpacket); // Add some black magic shit.
                
    DoAntiaim(cmd, local, weapon, sendpacket); // Add antiaims.
    
    MakeAutoPistolas(cmd, weapon);
    
    MakeNoRecoil(local, cmd); // Add norecoil
    
    showranks(cmd); // Show all ranks
    
    DoTrigger(cmd); // Add triggerbot
    
    clantag(); // Clantag
    
    DoSpammer(); // Spammer
    
    Fakewalk(cmd, local); // Fakewalk
    
    Moonwalk(cmd); // Moonwalk
    
    Airstuck(cmd); // Airstuck
    
    LegitRecoil(local, cmd); // RCS
    
    if(draw->m_szChangedValue[2].length() > 0) // Name Changer.
        ChangeName(draw->m_szChangedValue[2].c_str());
        
    if(draw->m_szChangedValue[3].length() > 0 && vars.misc.clantag) // Clantag Changer
    {
        SetTag(draw->m_szChangedValue[3].c_str(), "Barbossa");
    }
    
}

bool bOnce = false;
bool SendPacket = true;
bool hkCreateMove(void* thisptr, float flSampleInput, CUserCmd* cmd)
{
    createmovehook->GetOriginalMethod<tCreateMove>(25)(thisptr, flSampleInput, cmd);
    
    if(!cmd->command_number)
        return true;
    
    auto* local = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    
    if(!local)
        return false;
    
    if(local->GetLifeState() != 0)
        return false;
    
    if(local->GetHealth() < 1)
        return false;
    
    if(local->GetDormant())
        return false;
    
    if(bSendPacket == nullptr)
        return false;
    
    if(!bOnce)
    {
        if(draw->m_szCurrentString[2].empty())
        {
            draw->m_szCurrentString[2] = GetLocalName();
        }
        bOnce = true;
    }
    
    auto* weapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(local->GetActiveWeapon());
    
    if(!weapon)
        return false;
    
    SendPacket = true;
    *bSendPacket = SendPacket;
    
    
    Vector vOldAngles = cmd->viewangles;
    Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
    Vector qAimAngles;
    
    float forward = cmd->forwardmove;
    float sidemove = cmd->sidemove;
    float upmove = cmd->upmove;
    
    if(pEngine->IsInGame() && pEngine->IsConnected())
    {
        hacks(cmd, local, weapon, vOldAngles, forward, sidemove, *bSendPacket);
        
        // if(!vars.misc.antiuntrust)
        // ClampYaw(cmd->viewangles.y);
        
        if(pInputSystem->IsButtonDown(KEY_N))
            *bSendPacket = false;
        else
            *bSendPacket = true;
        
        
        FixMovement(vOldAngles, cmd);
        ClampMoves(forward, sidemove, upmove);
        
        
        
        if(vars.misc.enabled && vars.misc.antiuntrust)
        {
            cmd->viewangles.ClampAngles();
        }
        
        if(*bSendPacket)
            tpangles = cmd->viewangles;
    }
    return false;
}
