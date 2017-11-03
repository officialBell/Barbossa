//
//  antiaim.hpp
//  vHook
//
#pragma once

#include "main.h"

extern Vector atTargets;

enum VIEW_ANTIAIM_PITCH
{
    Emotion     = 1,
    Up          = 2,
    Down        = 3,
    FakeUp      = 4,
    Fakedown    = 5,
    Tankp       = 6,
    Custom      = 7,
};

enum VIEW_ANTIAIM_YAW
{
    Backwards       = 1,
    Jitter          = 2,
    FakeTroll       = 3,
    FakeStatic      = 4,
    FJitter         = 5,
    SlowSpin        = 6,
    FastSpin        = 7,
    RandomBackJitter= 8,
    BackJitter      = 9,
    LowerYaw        = 10,
    SidewaysLeft    = 11,
    SidewaysRight   = 12,
    LBYBreaker      = 13,
    Skeet           = 14,
    ICECREAM        = 15,
    CustomAA        = 16,
};

enum VIEW_ANTIIAIM_FYAW
{
    FakeAngel       = 1,
    FakeSpin        = 2,
    FakeLBYHook     = 3,
    FakeTwoStep     = 4,
    FakeLowerBody135= 5,
    FakeInverseRotation = 6,
    FakeJitter      = 7,
    FakeLBY         = 8,
    FakeSideLBY     = 9,
    Fakehead        = 10,
};

void DoAntiaim(CUserCmd* cmd, C_BaseEntity* local, C_BaseCombatWeapon* weapon, bool& bPacket);
