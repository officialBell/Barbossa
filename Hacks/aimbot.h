//
//  aimbot.hpp
//  vHook
//
#pragma once

#include "main.h"

void DoAim(CUserCmd* pCmd, C_BaseEntity* local, C_BaseCombatWeapon* weapon, float& flForward, float& flSide, bool& bSendPacket);

namespace Aimbot
{
    extern C_BaseEntity* curTarget;
}
