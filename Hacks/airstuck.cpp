//
//  Airstuck.cpp
//  vHook
//
//  Created by Warlauke on 10/21/17.
//  Copyright © 2017 ViKiNG. All rights reserved.
//

#include "airstuck.h"

void Airstuck(CUserCmd* cmd) {
    
    if(!vars.misc.airstuck)
        return;
    
    if(cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2)
        return;
    
    if(pInputSystem->IsButtonDown(KEY_Z))
        cmd->tick_count = 16777216;
}
