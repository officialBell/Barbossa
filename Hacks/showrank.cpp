//
//  showrank.cpp
//  vHook
//
//  Created by Warlauke on 10/25/17.
//  Copyright © 2017 ViKiNG. All rights reserved.
//

#include "showrank.h"

void showranks(CUserCmd* cmd)
{
    if(!vars.misc.showrank)
        return;
    
    if(!(cmd->buttons & IN_SCORE))
        return;
    
    float input[3] = { 0.f };
    MsgFunc_ServerRankRevealAll(input);
}
