//
//  noflash.cpp
//  vHook
//
//  Created by Warlauke on 10/25/17.
//  Copyright © 2017 ViKiNG. All rights reserved.
//

#include "noflash.h"

void MakeNoflash(ClientFrameStage_t stage) {
    
    if (!pEngine->IsInGame())
        return;
    
    if(!vars.misc.noflash)
        return;
    
    if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
        return;
    
    C_BaseEntity* localplayer = (C_BaseEntity*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    
    if (!localplayer)
        return;
    
    if (vars.misc.noflash) {
        *localplayer->GetFlashMaxAlpha() = 255.0f - vars.misc.flashalpha;
    } else {
        *localplayer->GetFlashMaxAlpha() = 255.0f;
    }
}
