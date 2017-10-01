#include "main.h"
bool qOnce = false;

void DrawSkeleton(C_BaseEntity* pEntity, Color color) {
    studiohdr_t* pStudioModel = pModelInfo->GetStudioModel( pEntity->GetModel() );
    if ( pStudioModel ) {
        static matrix3x4_t pBoneToWorldOut[128];
        if ( pEntity->SetupBones( pBoneToWorldOut, 128, 256, 0) )
        {
            for ( int i = 0; i < pStudioModel->numbones; i++ )
            {
                mstudiobone_t* pBone = pStudioModel->pBone( i );
                if ( !pBone || !( pBone->flags & 256 ) || pBone->parent == -1 )
                    continue;
                
                
                Vector vBone1 = pEntity->GetBonePosition(i);
                Vector vBoneOut1;
                
                Vector vBone2 = pEntity->GetBonePosition(pBone->parent);
                Vector vBoneOut2;
                
                if(WorldToScreen(vBone1, vBoneOut1) && WorldToScreen(vBone2, vBoneOut2)) {
                    draw->drawline(vBoneOut1.x, vBoneOut1.y, vBoneOut2.x, vBoneOut2.y, color);
                }
            }
        }
    }
}

Color GetColorBase(Color& col) {
    return Color(col.r(), col.g(), col.b(), 255);
}

auto DrawPlayerESP() -> void {
    int getscreenw, getscreenh;
    pEngine->GetScreenSize(getscreenw, getscreenh);
    
    if(vars.misc.antiscreenshot && pEngine->IsTakingScreenshot())
        return;
    
    if(!vars.visuals.enabled)
        return;
    
    auto* local = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    for(int i = 0; i < pEntList->GetHighestEntityIndex(); i++) {
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
        
        Color playercolor = [&]() -> Color {
            if(entity->GetTeam() == 3)
                return vis ? GetColorBase(vars.colors.ctbox) : GetColorBase(vars.colors.ctbox_ign);
            else if(entity->GetTeam() == 2)
                return vis ? GetColorBase(vars.colors.tbox) : GetColorBase(vars.colors.tbox_ign);
        }();
        
        boxstruct players;
        //char getYaw[255], getPitch[255];
        
        //sprintf(getYaw, "Y: %1.0f", entity->GetYawRotation());
        //sprintf(getPitch, "X: %1.0f", entity->GetHeadRotation());
        
        Vector vFrom = GetHitboxPosition(entity, (int)CSGOBones::Head);
        Vector vW2s;
        Vector vvvv;
        Vector origin = entity->GetVecOrigin();
        if(DrawPlayerBox(entity, players)) {
            
            
                if(vars.visuals.box)
                    draw->drawboxoutline(players.x, players.y, players.w, players.h, playercolor);
                
                if(vars.visuals.name)
                    draw->drawstring(players.x + players.w / 2, players.y - 12, Color::White(), espfont, info.name, true);
                 
                if(vars.visuals.healthtext)
                    draw->drawstring(players.x + players.w / 2, players.y + players.h + 8, Color::White(), espfont, std::to_string(entity->GetHealth()).c_str(), true);
                    
                if(vars.visuals.health)
                    draw->drawhealthbar(players.x - 5, players.y, 3, players.h, entity->GetHealth(), Color::Green());
                        
                if(vars.visuals.armour)
                    draw->drawhealthbar(players.x - 10, players.y, 3, players.h, entity->GetArmor(), Color(0, 125, 255, 255));
                    
                if(entity->IsDefusing())
                    if(vars.visuals.defusing)
                    draw->drawstring(players.x + players.w / 2, players.y - 27, Color::Red(), espfont, "DEFUSING", true);
                    
                if(entity->IsGrabbingHostage())
                    if(vars.visuals.grabbing)
                    draw->drawstring(players.x + players.w / 2, players.y - 27, Color::Red(), espfont, "RESCUING", true);
                        
                if(entity->IsRescuing())
                    if(vars.visuals.rescuing)
                    draw->drawstring(players.x + players.w / 2, players.y - 27, Color::Red(), espfont, "CARRYING", true);
                    
                    
                if(vars.visuals.snapline)
                    draw->drawline(getscreenw / 2, getscreenh, players.x + players.w / 2, players.y + players.h, playercolor);
                    
                    if(vars.visuals.headhitbox) {
                        if(WorldToScreen(origin, vvvv) && WorldToScreen(vFrom, vW2s)) {
                            draw->drawline(getscreenw / 2, getscreenh, vW2s.x, vW2s.y, playercolor);
                        }
                    }
                    
                if(vars.visuals.skeleton)
                    DrawSkeleton(entity, Color::White());
            
               // draw->drawstring(players.x + players.w, players.y + 12, Color::White(), espfont, getYaw);
               // draw->drawstring(players.x + players.w, players.y + 25, Color::White(), espfont, getPitch);
        }
    }
}

void getModule(task_t task, mach_vm_address_t * start, off_t * length, const char * module, Byte * buffer = nullptr, bool readBuffer = false)
{
    struct task_dyld_info dyld_info;
    mach_vm_address_t address;
    mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
    if (task_info(task, TASK_DYLD_INFO, (task_info_t)&dyld_info, &count) == KERN_SUCCESS)
    {
        address = dyld_info.all_image_info_addr;
    }
    
    struct dyld_all_image_infos *dyldaii;
    mach_msg_type_number_t size = sizeof(dyld_all_image_infos);
    vm_offset_t readMem;
    kern_return_t kr = vm_read(task, address, size, &readMem, &size);
    if (kr != KERN_SUCCESS)
    {
        return;
    }
    
    dyldaii = (dyld_all_image_infos *) readMem;
    int imageCount = dyldaii->infoArrayCount;
    mach_msg_type_number_t dataCnt = imageCount * 24;
    struct dyld_image_info *g_dii = NULL;
    g_dii = (struct dyld_image_info *) malloc (dataCnt);
    // 32bit bs 64bit
    kern_return_t kr2 = vm_read(task, (mach_vm_address_t)dyldaii->infoArray, dataCnt, &readMem, &dataCnt);
    if (kr2)
    {
        return;
    }
    struct dyld_image_info *dii = (struct dyld_image_info *) readMem;
    for (int i = 0; i < imageCount; i++)
    {
        dataCnt = 1024;
        vm_read(task, (mach_vm_address_t)dii[i].imageFilePath, dataCnt, &readMem, &dataCnt);
        char *imageName = (char *)readMem;
        if (imageName)
        {
            g_dii[i].imageFilePath = strdup(imageName);
        }
        else
        {
            g_dii[i].imageFilePath = NULL;
        }
        g_dii[i].imageLoadAddress = dii[i].imageLoadAddress;
        if (strstr(imageName, module) != NULL)
        {
            struct stat st;
            stat(imageName, &st);
            *start = (mach_vm_address_t)dii[i].imageLoadAddress;
            
            *length = st.st_size;
        }
    }
}

uintptr_t GetLibraryAddress(const char* moduleName)
{
    pid_t pids[1024];
    pid_t p = -1;
    int numberOfProcesses = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    for (int i = 0; i < numberOfProcesses; ++i)
    {
        if (pids[i] == 0) { continue; }
        char name[1024];
        proc_name(pids[i], name, sizeof(name));
        if (!strncmp(name, (const char *)"csgo_osx64", sizeof("csgo_osx64")))
        {
            p = pids[i];
            break;
        }
    }
    task_t task = 0;
    task_for_pid(current_task(), p, &task);
    off_t moduleLength = 0;
    mach_vm_address_t moduleStartAddress;
    getModule(task, &moduleStartAddress, &moduleLength, moduleName);
    return moduleStartAddress;
}


/* Display menu */
auto pwnmymenu() -> void {
    
    if(pInputSystem->IsButtonDown(KEY_LALT) || pInputSystem->IsButtonDown(KEY_RALT) || pInputSystem->IsButtonDown(KEY_INSERT)) {
        vars.menu = !vars.menu;
    }
    
}

bool bPanelOnce = false;

void hkPaintTraverse(void* thisptr, VPANEL panel, bool forceRepaint, bool allowForce) {
    painthook->GetOriginalMethod<tPaintTraverse>(42)(thisptr, panel, forceRepaint, allowForce);
    
    static VPANEL last = 0;
    
    if(!last) {
        if(strstr(pPanel->GetName(panel), "FocusOverlayPanel")) {
            framefont = draw->createfont(framefont, "Tahoma", 14, FONTFLAG_NONE);
            espfont = draw->createfont(espfont, "Tahoma", 12, FONTFLAG_OUTLINE);
            last = panel;
        }
    }
    if(!bPanelOnce) {
        
        vars.colors.ctchams = Color(40, 204, 67, 255);
        vars.colors.ctchams_ign = Color(0, 124, 255, 255);
        
        vars.colors.tchams = Color(204, 173, 40, 255);
        vars.colors.tchams_ign = Color(255, 69, 0, 255);
        
        vars.colors.tbox = Color(204, 173, 40, 255);
        vars.colors.tbox_ign = Color(255, 69, 0, 255);
        
        vars.colors.ctbox = Color(40, 204, 67, 255);
        vars.colors.ctbox_ign = Color(0, 124, 255, 255);
        
        vars.colors.hands = Color::Orange();
        vars.colors.hands_ign = Color::Blue();
        
        vars.colors.world = Color::Green(); // :^)
        
        bPanelOnce = true;
    }
    if(panel == last) {
        
        if(vars.misc.antiscreenshot && pEngine->IsTakingScreenshot())
            return;
        
        pwnmymenu();
        
        DrawPlayerESP();
        
        if(vars.menu) {
            
            
            draw->drawmenu();
            draw->drawmouse();
            pEngine->ExecuteClientCmd("cl_mouseenable 0");
            
        }
        
    }
}
