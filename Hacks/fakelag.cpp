#include "fakelag.h"
// WIP

void FakeLag(C_BaseEntity* local, CUserCmd* cmd, bool sendpacket)
{
    if (!vars.misc.fakelag)
        return;
    
    if(!local)
        return;
    
    if(!local->GetAlive())
        return;
    
    if (local->GetFlags() & FL_ONGROUND && vars.misc.adaptive)
        return;
    
    if (cmd->buttons & IN_ATTACK)
    {
        sendpacket = true;
        return;
    }
    
    static int FakelagTick;
    int ticksMax = 16;
    
    if (FakelagTick >= ticksMax)
    {
        sendpacket = true;
        FakelagTick = 0;
    }
    
    
    if(vars.misc.fakelagmode == LAG::Simple)
    {
        static int FakelagTick = 1;
        FakelagTick++;
        sendpacket = FakelagTick < 16 - vars.misc.fakelagfactor;
    }
    
    if(vars.misc.fakelagmode == LAG::Adaptive)
    {
        static int FakelagTick = 1;
        FakelagTick++;
        int packetsToChoke = Fakelag_AdaptiveFactor();
        sendpacket = FakelagTick < 16 - packetsToChoke;
    }
}
