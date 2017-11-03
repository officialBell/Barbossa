#include "esp.h"
#include "../Hacks/autowall.h"

void DrawSkeleton(C_BaseEntity* pEntity, Color color)
{
    studiohdr_t* pStudioModel = pModelInfo->GetStudioModel( pEntity->GetModel() );
    if ( !pStudioModel )
        return;
    
    static matrix3x4_t pBoneToWorldOut[128];
    if ( !pEntity->SetupBones( pBoneToWorldOut, 128, 256, 0) )
        return;
    
    for ( int i = 0; i < pStudioModel->numbones; i++ )
    {
        mstudiobone_t* pBone = pStudioModel->pBone( i );
        
        if ( !pBone || !( pBone->flags & 256 ) || pBone->parent == -1 )
            continue;
        
        Vector vBone1 = pEntity->GetBonePosition(i);
        Vector vBoneOut1;
        
        Vector vBone2 = pEntity->GetBonePosition(pBone->parent);
        Vector vBoneOut2;
        
        if(WorldToScreen(vBone1, vBoneOut1) && WorldToScreen(vBone2, vBoneOut2))
        {
            draw->drawline(vBoneOut1.x, vBoneOut1.y, vBoneOut2.x, vBoneOut2.y, color);
        }
        
    }
    
    
}


Color GetColorBase(Color& col)
{
    return Color(col.r(), col.g(), col.b(), 255);
}


void DrawHealthbar(int x, int y, int w, int h, int health, Color color)
{
    if(health > 100)
    {
        health = 100;
    }
    int hw = h - ((h) * health) / 100;
    draw->fillrgba(x, y - 1, w, h + 2, Color(0, 0, 0, 200));
    draw->fillrgba(x, y + hw - 1, w, h - hw + 2, color);
    draw->drawbox(x, y - 1, w, h + 2, Color(0, 0, 0, 200));
    
}
/*
// Note: Work on later
void DrawArmourBar(int x, int y, int w, int h, int armour, Color color)
{
    if(armour > 100)
    {
        armour = 100;
    }
    int hw = h - ((h) * armour) / 100;
    draw->fillrgba(x + 1, y + 1, w, h + 2, Color(0, 0, 0, 200));
    draw->fillrgba(x, y + hw - 1, w, h - hw + 2, color);
    draw->drawbox(x + 1, y + 1, w, h + 2, Color(0, 0, 0, 200));
    
}
*/

void DrawScope(C_BaseEntity* local) {
    if(!vars.misc.noscope || !local || (local && !local->IsScoped()))
        return;
    
    auto weapon = GetActiveWeapon(local);
    
    if(!weapon)
        return;
    
    if(*weapon->GetItemDefinitionIndex() == WEAPON_AUG || *weapon->GetItemDefinitionIndex() == WEAPON_SG556)
        return;
    
    int w, h;
    pEngine->GetScreenSize(w, h);
    
    draw->drawline(w / 2, 0, w / 2, h, vars.colors.scope);
    draw->drawline(0, h / 2, w, h / 2, vars.colors.scope);
    
}

void DrawBombPlanted(C_BaseEntity* local, C_BasePlantedC4* bomb)
{
    if(vars.visuals.bomb)
        return;
    
    if(!bomb->IsBombTicking())
        return;
    
    boxstruct Box;
    
    if(!DrawPlayerBox(bomb, Box))
        return;
    
    float flBlow    = bomb->GetBombTime();
    float bombTimer = flBlow - (pGlobals->interval_per_tick * local->GetTickBase());
    
    char buffer[128];
    
    if((local->HasDefuser() && flBlow < 5.25f) || (!local->HasDefuser() && flBlow < 10.25f))
        sprintf(buffer, "No time %.f", bombTimer);
    
    if((local->HasDefuser() && flBlow > 5.25f) || (!local->HasDefuser() && flBlow > 10.25f))
        sprintf(buffer, "Bomb %.f", bombTimer);
    
    if (bomb->IsBombDefused() || !bomb->IsBombTicking() || bombTimer <= 0.f)
        sprintf(buffer, "Bomb");
    
    draw->drawstring(Box.x, Box.y, Color::Red(), espfont, buffer);
    
}

void DrawOtherESP() {
  // WIP
    if(!vars.visuals.enabled)
        return;
    
    if(vars.misc.antiscreenshot && pEngine->IsTakingScreenshot())
        return;
    
    int getscreenw, getscreenh;
    pEngine->GetScreenSize(getscreenw, getscreenh);
    
    C_BaseEntity* local = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    
    
    for(int i = 0; i < pEntList->GetHighestEntityIndex(); i++)
    {
        auto* entity = pEntList->GetClientEntity(i);
        
        if(!entity)
            continue;
        
        
        int classID = entity->GetClientClass()->m_ClassID;
        C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*)entity;
        C_BasePlantedC4* pC4 = (C_BasePlantedC4*)entity;
        
        
        if(!weapon || !pC4)
            continue;
        
        
        // Draw planted bomb
        if(vars.visuals.bomb && classID == CPlantedC4)
            DrawBombPlanted(local, pC4);
    
    }
}

void DrawPlayerESP()
{
    if(!vars.visuals.enabled)
        return;
    
    if(vars.misc.antiscreenshot && pEngine->IsTakingScreenshot())
        return;
    
    int getscreenw, getscreenh;
    pEngine->GetScreenSize(getscreenw, getscreenh);
    
    auto* local = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    
    for(int i = 0; i < pEntList->GetHighestEntityIndex(); i++)
    {
        auto* entity = pEntList->GetClientEntity(i);
        
        if(!entity)
            continue;
        
        if(entity->GetHealth() < 1)
            continue;
        
        if(entity->GetDormant())
            continue;
        
        if(entity->GetTeam() != 2 && entity->GetTeam() != 3)
            continue;
        
        if(vars.visuals.enemyonly && entity->GetTeam() == local->GetTeam())
            continue;
        
        player_info_t info;
        
        pEngine->GetPlayerInfo(i, &info);
        
        auto vis = IsVisible(local, entity);
        
        Color playercolor = [&]() -> Color
        {
            if(entity->GetTeam() == 3)
                return vis ? GetColorBase(vars.colors.ctbox) : GetColorBase(vars.colors.ctbox_ign);
            else if(entity->GetTeam() == 2)
                return vis ? GetColorBase(vars.colors.tbox) : GetColorBase(vars.colors.tbox_ign);
        }();
        
        boxstruct players;
        char getYaw[255], getPitch[255];
        
        sprintf(getYaw, "Y: %1.0f", entity->GetYawRotation());
        sprintf(getPitch, "X: %1.0f", entity->GetHeadRotation());
        
        Vector vFrom = GetHitboxPosition(entity, vars.aimbot.hitbox);
        Vector vW2s;
        Vector vvvv;
        Vector origin = entity->GetVecOrigin();
        if(DrawPlayerBox(entity, players))
        {
            
            
            if(vars.visuals.box)
                draw->drawboxoutline(players.x, players.y, players.w, players.h, playercolor);
                    
            if(vars.visuals.name)
                draw->drawstring(players.x + players.w / 2, players.y - 12, Color::White(), espfont, info.name, true);
                            
            if(vars.visuals.healthtext)
                draw->drawstring(players.x + players.w / 2, players.y + players.h + 8, Color::White(), espfont, std::to_string(entity->GetHealth()).c_str(), true);
            
            if(vars.visuals.health)
                DrawHealthbar(players.x - 5, players.y, 3, players.h, entity->GetHealth(), Color(43, 206, 91, 255));
            
            if(vars.visuals.armour)
                DrawHealthbar(players.x, players.y + players.h + 3, players.w, 2, entity->GetArmor(), Color(72, 136, 189, 255));
            
            if(vars.visuals.snapline)
                draw->drawline(getscreenw / 2, getscreenh, players.x + players.w / 2, players.y + players.h, playercolor);
        
                                                                    
            if(vars.visuals.headhitbox)
            {
                if(WorldToScreen(origin, vvvv) && WorldToScreen(vFrom, vW2s))
                {
                    draw->drawline(getscreenw / 2, getscreenh, vW2s.x, vW2s.y, playercolor);
                }
            }
            
            if(vars.visuals.skeleton)
                DrawSkeleton(entity, Color::White());
            
            if(entity->IsDefusing())
                draw->drawstring(players.x + players.w / 2, players.y - 27, Color::Red(), espfont, "DEFUSING", true);
            
            if(entity->IsGrabbingHostage())
                draw->drawstring(players.x + players.w / 2, players.y - 27, Color::Red(), espfont, "RESCUING", true);
            
            if(entity->IsRescuing())
                draw->drawstring(players.x + players.w / 2, players.y - 27, Color::Red(), espfont, "CARRYING", true);
            
            if((entity->IsScoped()))
                draw->drawstring(players.x + players.w / 2, players.y - 27, Color::Red(), espfont, "SCOPED", true);
            
            if((entity->GetFlashDuration() - pGlobals->curtime > 2.0f))
                draw->drawstring(players.x + players.w / 2, players.y - 27, Color::Yellow(), espfont, "Flashed");

            draw->drawstring(players.x + players.w, players.y + 12, Color::Red(), espfont, getYaw);
            draw->drawstring(players.x + players.w, players.y + 25, Color::Red(), espfont, getPitch);
        }
    
    }
    
}


/* Display menu */
void pwnmymenu()
{
    
    if(pInputSystem->IsButtonDown(KEY_LALT) || pInputSystem->IsButtonDown(KEY_RALT) || pInputSystem->IsButtonDown(KEY_INSERT)) {
        vars.menu = !vars.menu;
    }
    
}
