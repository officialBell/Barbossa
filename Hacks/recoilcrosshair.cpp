#include "recoilcrosshair.h"
// Doesn't move sadly will find fix when I can
void rCrosshair(C_BaseEntity* local) {
    
    if (!vars.visuals.rccrosshair)
        return;
    
    if (!pEngine->IsInGame())
        return;
    
    C_BaseEntity* localplayer = (C_BaseEntity*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    
    if (!localplayer || !localplayer->GetAlive())
        return;
    
    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
    if (!activeWeapon)
        return;
    
    int ScreenWidth, ScreenHeight;
    pEngine->GetScreenSize(ScreenWidth, ScreenHeight);
    
    int x = (int) (ScreenWidth * 0.5f);
    int y = (int) (ScreenHeight * 0.5f);
    
    int crosshairX = (int) (x);
    int crosshairY = (int) (y);
    
    // line horizontal
    draw->Line(Vector2D(crosshairX - 3, crosshairY), Vector2D(crosshairX + 4, crosshairY), vars.colors.crosshair);
    // line vertical
    draw->Line(Vector2D(crosshairX, crosshairY + 3), Vector2D(crosshairX, crosshairY - 4), vars.colors.crosshair);
}
