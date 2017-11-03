//
//  smoothing.cpp
//  vHook
//
//  Created by James Henson on 11/1/17.
//  Copyright © 2017 ViKiNG. All rights reserved.
//

#include "smoothing.h"
//WIP
/*
void Smooth(Vector& angle)
{
    if(!vars.aimbot.enabled)
        return;
    
    if(!vars.aimbot.smooth)
        return;
    
    Vector viewAngles = Vector(0.f, 0.f, 0.f);
    pEngine->GetViewAngles(viewAngles);
    
    Vector delta = angle - viewAngles;
    NormalizeAngles(delta);
    
    float smooth = powf(vars.aimbot.smoothf, 0.4);
    smooth = std::min(0.99f, smooth);
    
    Vector toChange = delta - delta * smooth;
    
    return angle = viewAngles + toChange;
}
*/
