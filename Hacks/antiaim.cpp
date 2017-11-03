//
//  antiaim.cpp
//  vHook
//
//  Created by Finn Le var on 14/10/17.
//  Copyright © 2017 ViKiNG. All rights reserved.
//

#include "../Hacks/antiaim.h"

Vector atTargets;

void DoAntiaim(CUserCmd* cmd, C_BaseEntity* local, C_BaseCombatWeapon* weapon, bool& bPacket)
{
    
    if(!vars.misc.antiaim)
        return;
    
    if(vars.misc.knifeaa && (weapon->IsKnife() || weapon->IsBomb()))
        return;
    
    if(weapon->IsGrenade())
        return;
    
    static bool fakeswitch = false;
    static bool bFlip = false;
    bool bAttack = true;
    bFlip = !bFlip;
    int maxJitter;
    int random;
    float temp;
    
    if (weapon->GetNextPrimaryAttack() - pGlobals->interval_per_tick > local->GetTickBase() * pGlobals->interval_per_tick)
        bAttack = false;
    
    if(cmd->buttons & IN_ATTACK && (bAttack))
    {
        bPacket = false;
    }
    else
    {
        bPacket = true;
        
        if (vars.misc.fakeaa)
        {
            fakeswitch = !fakeswitch;
            
            if (fakeswitch)
                bPacket = false;
            else
                bPacket = true;
        }
        
        if(vars.misc.aaX > 0)
        {
            if(vars.misc.aaX == VIEW_ANTIAIM_PITCH::Emotion)
            {
                cmd->viewangles.x = 89;
            }
            if(vars.misc.aaX == VIEW_ANTIAIM_PITCH::Up)
            {
                cmd->viewangles.x = -89;
            }
            if(vars.misc.aaX == VIEW_ANTIAIM_PITCH::Down)
            {
                cmd->viewangles.x = 179;
            }
            if(vars.misc.aaX == VIEW_ANTIAIM_PITCH::FakeUp)
            {
                cmd->viewangles.x = -180;
            }
            if(vars.misc.aaX == VIEW_ANTIAIM_PITCH::Fakedown)
            {
                cmd->viewangles.x = 180;
            }
            
            if(vars.misc.aaX == VIEW_ANTIAIM_PITCH::Custom)
            {
                if(bPacket)
                    cmd->viewangles.x = 89;
                else
                    cmd->viewangles.x = vars.misc.customaa;
            }
            
    }
        if(vars.misc.aaY > 0)
        {
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::Backwards)
            {
                cmd->viewangles.y = 180;
            }
            
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::Jitter)
            {
                static bool bjitter;
                cmd->viewangles.y = bjitter ? 90 : -90;
                bjitter = !bjitter;
            }
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::FakeTroll)
            {
                cmd->viewangles.y = 90.0f;
            }
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::FakeStatic)
            {
                static bool f_flip = true;
                f_flip = !f_flip;
                
                if (f_flip)
                {
                    *bSendPacket = false;
                    cmd->viewangles.y += 50.000000f;
                }
                else
                {
                    *bSendPacket = false;
                    cmd->viewangles.y -= 70.000000f;
                }
                
                if (!f_flip)
                {
                    *bSendPacket = true;
                    cmd->viewangles.y += 212.000000f;;
                }
                else if (!f_flip)
                {
                    *bSendPacket = true;
                    cmd->viewangles.y -= 180.000000f;
                }
            }
            
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::FJitter)
            {
                if(fakeswitch) cmd->viewangles.y = 90; else cmd->viewangles.y = local->GetLowerBodyYawTarget();
            }
            
            float factor;
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::SlowSpin)
            {
                factor =  360.0 / M_PHI;
                cmd->viewangles.y = fmodf(pGlobals->curtime * factor, 360.0);
            }
            
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::FastSpin)
            {
                factor =  360.0 / M_PHI;
                factor *= 25;
                cmd->viewangles.y = fmodf(pGlobals->curtime * factor, 360.0);
            }
            
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::RandomBackJitter)
            {
                cmd->viewangles.y -= 180;
                random = rand() % 100;
                maxJitter = rand() % (85-70+1) + 70;
                temp = maxJitter - (rand() % maxJitter);
                if(random < 35 + (rand() % 15))
                    cmd->viewangles.y -= temp;
                else if(random < 85 + (rand()% 15))
                    cmd->viewangles.y += temp;
            }
            
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::BackJitter)
            {
                int random = rand() % 100;
                
                // Small Chance of starting forward
                if (random < 98)
                    // LOoks Backwards
                    cmd->viewangles.y -= 180;
                
                // Some Jitter
                if (random < 15)
                {
                    float change = -70 + (rand() % (int) (140 + 1));
                    cmd->viewangles.y += change;
                }
            if (random == 69)
            {
                float change = -90 + (rand() % (int) ( 180 + 1));
                cmd->viewangles.y += change;
                }
            }
            
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::LowerYaw) // Twist
            {
                
                static int fakeTick = 0;
                fakeTick++;
                
                if (vars.misc.fakeaa)
                {
                    if (!bSendPacket)
                    {
                        
                        if (fakeTick < 300)
                            cmd->viewangles.y += 67.57f;
                        else if (fakeTick > 300)
                            cmd->viewangles.y -= 67.15f;
                        
                        if (fakeTick < 300)
                            cmd->viewangles.y += 67.57f;
                        else if (fakeTick > 300)
                            cmd->viewangles.y -= 67.15f;
                        
                        if (fakeTick > 600)
                            fakeTick = 0;
                        
                    }
                    else
                    {
                        cmd->viewangles.y += 180.f;
                    }
                }
                else
                {
                    cmd->viewangles.y += 180.f;
                }
                
            }
            
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::SidewaysLeft)
            {
                fakeswitch ? cmd->viewangles.y = 90 : cmd->viewangles.y = atTargets.y;
            }
            
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::SidewaysRight)
            {
                fakeswitch ? cmd->viewangles.y = -90 : cmd->viewangles.y = atTargets.y;
            }
            
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::LBYBreaker) {
                QAngle angle_for_yaw;
                static int counter = 0;
                static int motion = 0;
                int ServerTime = (float)pGlobals->interval_per_tick * local->GetTickBase() * 2;
                bool flip = true;
                
                if(flip)
                {
                    if(counter & (48 == 0))
                        motion++;
                        int value = ServerTime % 2;
                    switch (value) {
                        case 0:cmd->viewangles.y = local->GetLowerBodyYawTarget() - 90.00f > 35 ? local->GetLowerBodyYawTarget() - 180.f : local->GetLowerBodyYawTarget() - 90.f; break;
                            *bSendPacket = false;
                            
                        case 1:cmd->viewangles.y = local->GetLowerBodyYawTarget() + 90.00f > 35 ? local->GetLowerBodyYawTarget() - 90.f : local->GetLowerBodyYawTarget() + 90.f; break;
                            *bSendPacket = true;
                    }
                    counter++;
                }
                else
                {
                    if(counter % 48 == 0)
                        motion++;
                        int value = ServerTime % 2;
                    switch (value) {
                    case 0:cmd->viewangles.y = angle_for_yaw.y + (rand() % 100 > 33 ? (rand() % 50 > 13 ? (rand() % 20 + 40) : -(rand() % 20 + 40)) : (rand() % 100 > 71 ? (rand() % 20 + 150) : -(rand() % 20 + 150))); break;
                        *bSendPacket = false;
                    case 1:cmd->viewangles.y = angle_for_yaw.y + (rand() % 100 > 33 ? (rand() % 50 > 13 ? (rand() % 20 + 40) : -(rand() % 20 + 40)) : (rand() % 100 > 71 ? (rand() % 20 + 150) : -(rand() % 20 + 150))); break;
                        *bSendPacket = true;
                    }
                    counter++;
                }
            }
            
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::Skeet) {
                if(cmd->command_number % 2)
                {
                    cmd->viewangles.x = 69.0f;
                    cmd->viewangles.y = 180.0f;
                }
            }
            
            
            
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::ICECREAM)
            {
                static int ChokedPackets = 1;
                ChokedPackets++;
                if (ChokedPackets < 1)
                {
                    *bSendPacket = false;
                    cmd->viewangles.y = -45.0f;
                }
                if(fakeswitch)
                {
                    *bSendPacket = true;
                    cmd->viewangles.y = -180.0f;
                    ChokedPackets = 1;
                }
                
            }
            if(vars.misc.aaY == VIEW_ANTIAIM_YAW::CustomAA)
            {
                bPacket ? cmd->viewangles.y = vars.misc.fakeyaw : cmd->viewangles.y = vars.misc.realyaw;
            }
            
        }
    
    if( vars.misc.FaaY > 0 && ( vars.misc.fakeaa && bPacket ) )
    {
        if(vars.misc.FaaY == VIEW_ANTIIAIM_FYAW::FakeAngel) {
            *bSendPacket = true; //fake angle
            cmd->viewangles.y = 0.f;
        }
        if(vars.misc.FaaY == VIEW_ANTIIAIM_FYAW::FakeSpin   ) {
            int random = rand() % 100;
            int random2 = rand() % 1000;
            static bool dir;
            static float current_y = cmd->viewangles.y;
            IClientEntity* pLocal = local;
            float server_time = local->GetTickBase() * pGlobals->interval_per_tick;
            
            if (bSendPacket) {
                cmd->viewangles.y = (float)(fmod(server_time / 0.39f * 360.0f, 360.0f));
            }
            else
            {
                if (random == 1) dir = !dir;
                if (dir)
                    current_y -= 100.9;
                else
                    current_y += 100.9;
                cmd->viewangles.y = current_y;
                if (random == random2)
                    cmd->viewangles.y -= random;
            }
        }
        if(vars.misc.FaaY == VIEW_ANTIIAIM_FYAW::FakeLBYHook) {
            static bool ySwitch;
            static bool jbool;
            static bool jboolt;
            ySwitch = !ySwitch;
            jbool = !jbool;
            jboolt = !jbool;
            if (jboolt)
            {
                *bSendPacket = true;
                cmd->viewangles.y = local->GetLowerBodyYawTarget() - 90.f;
            }
            else
            {
                *bSendPacket = true;
                cmd->viewangles.y = local->GetLowerBodyYawTarget() + 90.f;
            }
        }
        if(vars.misc.FaaY == VIEW_ANTIIAIM_FYAW::FakeTwoStep) {
            static bool bFlipYaw;
            float flInterval = pGlobals->interval_per_tick;
            float flTickcount = cmd->tick_count;
            float flTime = flInterval * flTickcount;
            if (std::fmod(flTime, 1) == 0.f)
                bFlipYaw = !bFlipYaw;
            
            if (bSendPacket)
                cmd->viewangles.y += bFlipYaw ? 135.f : -135.f;
            else
                cmd->viewangles.y -= local->GetLowerBodyYawTarget() + bFlipYaw ? -135.f : 135.f;
        }
        if(vars.misc.FaaY == VIEW_ANTIIAIM_FYAW::FakeLowerBody135) {
            int flip = (int)floorf(pGlobals->curtime / 1.1) % 2;
            
            static bool bFlipYaw;
            float flInterval = pGlobals->interval_per_tick;
            float flTickcount = cmd->tick_count;
            float flTime = flInterval * flTickcount;
            if (std::fmod(flTime, 1) == 0.f)
                bFlipYaw = !bFlipYaw;
            
            if (bSendPacket)
            {
                if (flip)
                {
                    cmd->viewangles.y += bFlipYaw ? 135.f : -135.f;
                    
                }
                else
                {
                    cmd->viewangles.y -= local->GetLowerBodyYawTarget() + bFlipYaw ? 135.f : -135.f;
                }
            }
            else
            {
                cmd->viewangles.y += 180.f;
            }
        }
        if(vars.misc.FaaY == VIEW_ANTIIAIM_FYAW::FakeInverseRotation) {
            IClientEntity* pLocal = local;
            float server_time = local->GetTickBase() * pGlobals->interval_per_tick;
            
            if (bSendPacket)
            {
                cmd->viewangles.y += (float)(fmod(server_time / 0.80f * 360.0f, 360.0f));
            }
            else
            {
                cmd->viewangles.y -= (float)(fmod(server_time / 0.80f * 360.0f, 360.0f));
            }
        }
        if(vars.misc.FaaY == VIEW_ANTIIAIM_FYAW::FakeJitter) {
            static int jitterangle = 0;
            
            if (jitterangle <= 1)
            {
                cmd->viewangles.y += 135;
            }
            else if (jitterangle > 1 && jitterangle <= 3)
            {
                cmd->viewangles.y += 225;
            }
            
            static int iChoked = -1;
            iChoked++;
            if (iChoked < 1)
            {
                *bSendPacket = true;
                if (jitterangle <= 1)
                {
                    cmd->viewangles.y += 45;
                    jitterangle += 1;
                }
                else if (jitterangle > 1 && jitterangle <= 3)
                {
                    cmd->viewangles.y -= 45;
                    jitterangle += 1;
                }
                else
                {
                    jitterangle = 0;
                }
            }
            else
            {
                *bSendPacket = false;
                iChoked = -1;
            }
        }
        if(vars.misc.FaaY == VIEW_ANTIIAIM_FYAW::FakeLBY) {
            static bool flip_lby = false;
            flip_lby = !flip_lby;
            IClientEntity* pLocal = local;
            float server_time = local->GetTickBase() * pGlobals->interval_per_tick;
            
            if (flip_lby) {
                *bSendPacket = false;
                cmd->viewangles.y += (float)(fmod(server_time / 0.50f * 360.0f, 360.0f));
            }
            else
            {
                *bSendPacket = true;
                cmd->viewangles.y -= local->GetLowerBodyYawTarget() + 180.00f;
            }
        }
        if(vars.misc.FaaY == VIEW_ANTIIAIM_FYAW::FakeSideLBY) {
            int i = 0; i < pEntList->GetHighestEntityIndex(); ++i;
            IClientEntity pEntity = *pEntList->GetClientEntity(i);
            IClientEntity pLocal = *pEntList->GetClientEntity(pEngine->GetLocalPlayer());
            
            static bool isMoving;
            float PlayerIsMoving = abs(local->GetVelocity().Length());
            if (PlayerIsMoving > 0.1) isMoving = true;
            else if (PlayerIsMoving <= 0.1) isMoving = false;
            
            int flip = (int)floorf(pGlobals->curtime / 1.1) % 2;
            static bool bFlipYaw;
            float flInterval = pGlobals->interval_per_tick;
            float flTickcount = cmd->tick_count;
            float flTime = flInterval * flTickcount;
            if (std::fmod(flTime, 1) == 0.f)
                bFlipYaw = !bFlipYaw;
            
            if (PlayerIsMoving <= 0.1)
            {
                if (bSendPacket)
                {
                    cmd->viewangles.y += 180.f;
                }
                else
                {
                    if (flip)
                    {
                        cmd->viewangles.y += bFlipYaw ? 90.f : -90.f;
                        
                    }
                    else
                    {
                        cmd->viewangles.y -= local->GetLowerBodyYawTarget() + bFlipYaw ? 90.f : -90.f;
                    }
                }
            }
            else if (PlayerIsMoving > 0.1)
            {
                if (bSendPacket)
                {
                    cmd->viewangles.y -= 90.f;
                }
                else
                {
                    cmd->viewangles.y += 90.f;
                }
            }
        }
        if(vars.misc.FaaY == VIEW_ANTIIAIM_FYAW::Fakehead) {
            float last_lby = 0.f;
            float last_lby_time = 0.f;
            static bool jitter_switch = false;
            float outgoing_latency = 0.f; //inetchannel
            float current_lby = local->GetLowerBodyYawTarget();
            
            if (current_lby != last_lby || fabs(local->GetVelocity().Length2D()) > 0.1f)
            {
                last_lby_time = pGlobals->curtime;
                last_lby = current_lby;
            }
            
            if (!bSendPacket)
            {
                if (fabs(last_lby_time - pGlobals->curtime) > 1.1f - outgoing_latency)
                {
                    cmd->viewangles.y += 90.f;
                }
                else
                {
                    cmd->viewangles.y += 180.f;
                }
            }
            else
            {
                cmd->viewangles.y -= 90.f;
            }
            
            *bSendPacket = jitter_switch;
            jitter_switch = !jitter_switch;
            }
        
        } // End Of FakeAA Yaw
    }
}
