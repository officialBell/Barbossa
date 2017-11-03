//
//  menu.cpp
//  vHook
//

#include "menu.h"

cMenu* menu = new cMenu();
menuT tab;
colors col;

Color ControlBackground = Color(44, 44, 44, 255);
Color ControlColor      = Color(30, 30, 30, 255);

Color FontColor         = Color(250, 250, 250, 250);
Color inactive          = Color(82, 82, 82, 255);
Color Menu              = Color(255, 0, 0, 255);
Color Cursor            = Color(255, 0, 0, 255);
Color MainColor         = Color(255, 0, 0, 255);

bool WasPressed, WasReleased;

//=====================WasPressed=========================//

auto Pressed (ButtonCode_t code) -> void {
    if (pInputSystem->IsButtonDown(code))
    {
        WasPressed = true;
    }
    else if (!(pInputSystem->IsButtonDown(code)))
    {
        if (WasPressed)
            WasReleased = true;
            else
            {
                WasReleased = false;
            }
        WasPressed = false;
    }
}

//==================Buttons and shit=====================//

void cMenu::renderCheckbox(int x, int y, const char* str, bool *var) {
    
    int size = 12;
    
    draw->fillrgba(x, y, size, size, ControlBackground);
    
    if (*var)
        draw->fillrgba( x, y, size, size, MainColor );
    
    if (draw->inArea(x, y, size, size)) {
        
        if (*var)
            draw->fillrgba( x, y, size, size, MainColor );
        
        if (WasReleased)
            *var = !*var;
    }
    
    draw->drawbox(x, y, size, size, Color::Black()); // Outline
    
    draw->drawstring(x - 100, y, FontColor, mFont, str);
    
}

void cMenu::renderCheckbox(int x, int y, bool *var) {
    
    int size = 12;
    
    draw->fillrgba(x, y, size, size, ControlBackground);
    
    if (*var)
        draw->fillrgba( x, y, size, size, MainColor );
    
    if (draw->inArea(x, y, size, size)) {
        
        if (*var)
            draw->fillrgba( x, y, size, size, MainColor );
        
        if (WasReleased)
            *var = !*var;
    }
    
    draw->drawbox(x, y, size, size, Color::Black()); // Outline
    
}

void cMenu::renderSlider(int x, int y, int w, const char* szString, int &value, int max, int min) {
    
    int h = 8;
    
    int curValue = value * ((float)w / (float)(max));
    
    if (draw->GetMouse().x > x - w / 2 && draw->GetMouse().y > y - 2 && draw->GetMouse().x < x + w + w / 2 && draw->GetMouse().y < y + h + 2) {
        if (pInputSystem->IsButtonDown(MOUSE_LEFT)) {
            value = (draw->CalcPos(x) / ((float)w / (float)(max)));
            if (value > max)
                value = max;
            if (value < min)
                value = min;
            
        }
    }
    
    
    draw->RectOutlined(x, y + 5, w + 2, h, 1, ControlBackground, Color::Black());
    draw->fillrgba( x, y + 5, curValue, h, MainColor );                       // Color till cursor
    
    draw->drawstring(x + curValue - 5, y + 3, Color::White(), sFont, to_string(value).c_str());
    draw->drawstring(x, y - 9, FontColor, mFont, szString);
    
}

void cMenu::renderSlider(int x, int y, int w, const char* szString, float &value, int max, int min) {
    
    int h = 8;
    
    int curValue = value * ((float)w / (float)(max));
    
    if (draw->GetMouse().x > x - w / 2 && draw->GetMouse().y > y - 2 && draw->GetMouse().x < x + w + w / 2 && draw->GetMouse().y < y + h + 2) {
        if (pInputSystem->IsButtonDown(MOUSE_LEFT)) {
            value = (draw->CalcPos(x) / ((float)w / (float)(max)));
            if (value > max)
                value = max;
            if (value < min)
                value = min;
            
        }
    }
    
    
    draw->RectOutlined(x, y + 5, w + 2, h, 1, ControlBackground, Color::Black());
    draw->fillrgba( x, y + 5, curValue, h, MainColor );                       // Color till cursor
    
    draw->drawstring(x + curValue - 5, y + 3, Color::White(), sFont, to_string(value).c_str());
    draw->drawstring(x, y - 9, FontColor, mFont, szString);
    
}

void cMenu::renderCombo(int x, int y, int w, int h, const char* szString, vector<string> szValue, int& currValue, bool* bOpend) {
    
    int Y = y + h;
    
    if(draw->inArea(x, y, w, h)) {
        if(WasReleased) {
            *bOpend = !*bOpend;
        }
    }
    
    if (*bOpend) {
        
        for ( int i = 0; i < szValue.size(); i++ ) {
            
            auto bHover = draw->inArea(x, Y + 2 + ( i * 20 ), w, 18);
            
            draw->fillrgba( x, Y + 2 + ( i * 20 ), w, 18, bHover ? (col.menu == 0 ? Color( 255, 0, 0, 150 ) : Color(199, 148, 208, 150)) : ControlColor ); // Combo box fill colour
            draw->drawboxoutline( x, Y + 2 + ( i * 20 ), w, 18, Color( 90, 90, 90, 255 ) );
            draw->drawstring(x + 5, Y + 3 + (i*20), Color::White(), cFont, szValue[i].c_str());
            
            if (draw->inArea(x, Y + 2 + ( i * 20 ), w, 18)) {
                if(WasPressed){
                    currValue = i;
                    *bOpend = false;
                }
            }
        }
    }
    
    if(currValue == 0)
        szString = szString;
    else
        szString = szValue[currValue].c_str();
    
    // Draws the box
    draw->fillrgba( x, y, w, h, ControlColor );
    draw->fillrgba( x + w - h, y, h, h, ControlBackground );
    draw->drawboxoutline( x, y, w, h, MainColor );
    draw->drawstring(x + 5, y + (h/2) - 7, FontColor, cFont, szString);
    
    
    if (!*bOpend)
    {
        // draw arrow pointing down
        draw->fillrgba( ( x + w - h + 113 - 107 ), ( y + 8 ), 1, 1, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 114 - 107 ), ( y + 8 ), 1, 2, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 115 - 107 ), ( y + 8 ), 1, 3, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 116 - 107 ), ( y + 8 ), 1, 4, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 117 - 107 ), ( y + 8 ), 1, 3, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 118 - 107 ), ( y + 8 ), 1, 2, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 119 - 107 ), ( y + 8 ), 1, 1, Color( 255, 255, 255, 255 ) );
    }
    else
    {
        // draw arrow pointing up
        draw->fillrgba( ( x + w - h + 113 - 107 ), ( y + 11 ), 1, 1, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 114 - 107 ), ( y + 10 ), 1, 2, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 115 - 107 ), ( y + 9 ), 1, 3, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 116 - 107 ), ( y + 8 ), 1, 4, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 117 - 107 ), ( y + 9 ), 1, 3, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 118 - 107 ), ( y + 10 ), 1, 2, Color( 255, 255, 255, 255 ) );
        draw->fillrgba( ( x + w - h + 119 - 107 ), ( y + 11 ), 1, 1, Color( 255, 255, 255, 255 ) );
    }
}

void cMenu::renderButton(int x, int y, const char* szString, bool* var) {
    
    int w = 60;
    int h = 20;
    
    if(draw->inArea(x, y, w, h)) {
        if(WasPressed) {
            *var = true;
        }
    } else {
        *var = false;
    }
    
    bool bHovering = draw->inArea(x, y, w, h);
    draw->drawgradient(x, y, w, h, bHovering ? Color(255, 136, 57, 150) : Color(44, 44, 44, 255), Color(24, 24, 24, 255));
    draw->drawbox(x, y, w, h, MainColor);
    
    draw->drawstring(x + w / 2, y + h / 2, FontColor, framefont, szString, true);
    
}


void cMenu::playerbutton(int x, int y, int w, int h, int index, int& playercount, const char* szString) {
    y += index * vars.buttonPadding;
    
    if(draw->inArea(x, y, w, h)) {
        if(WasReleased) {
            playercount = index;
        }
    }
    
    bool bHovering = draw->inArea(x, y, w, h);
    draw->drawgradient(x, y, w, h, bHovering ? Color(60, 60, 60, 255), ControlBackground : Color(44, 44, 44, 255), Color(24, 24, 24, 255));
    draw->drawbox(x, y, w, h, MainColor);
    
    draw->drawstring(x + w / 2, y + h / 2, FontColor, framefont, szString, true);
}




//=====================Menu Tabs=========================//

enum mTab
{
    Null    = 0,
    AimTab  = 1,
    VisTab  = 2,
    MiscTab = 3,
    PlayerTab = 4,
    ColorsTab = 5,
};

void cMenu::renderAim(int x, int y) {
    
    // Legit
    draw->drawstring(x + 65, y + 15, FontColor, heading, "Legit");
    vector<string> Hitbox;
    vector<int> hitboxes;

    // Hitbox Combo WIP
    Hitbox.push_back("Head");
    Hitbox.push_back("Neck");
    Hitbox.push_back("LowerNeck");
    Hitbox.push_back("Pelvis");
    Hitbox.push_back("Body");
    Hitbox.push_back("UpperChest");

    
    this->renderCheckbox(x + 115, y + 35, "Enabled", &vars.aimbot.enabled);        // 35
    this->renderCheckbox(x + 115, y + 50, "Trigger", &vars.aimbot.trigger);       // +20
    this->renderSlider(x + 15, y + 70, 110, "FOV", vars.aimbot.fov, 180, 0);      // +10
    this->renderSlider(x + 15, y + 90, 110, "Hitbox", vars.aimbot.hitbox, 20, 0);
    this->renderCheckbox(x + 115, y + 105, "RCS", &vars.aimbot.rcs);
    this->renderSlider(x + 15, y + 125, 110, "", vars.aimbot.rcsf, 50, 0);
    
    
    
    
    // Rage
    draw->drawstring(x + 215, y + 15, FontColor, heading, "Rage");
    
    this->renderCheckbox(x + 265, y + 35, "Silent Aim", &vars.aimbot.silent); //35
    this->renderCheckbox(x + 265, y + 50, "Prediction", &vars.aimbot.prediction); // 20
    this->renderCheckbox(x + 265, y + 65, "Autowall", &vars.aimbot.autowall); // 20
    this->renderSlider(x + 165, y + 80, 110, "", vars.aimbot.mindmg, 100, 0); // 10
    this->renderCheckbox(x + 265, y + 95, "Hitscan", &vars.aimbot.hitscan); // 20
    this->renderCheckbox(x + 265, y + 110, "Autoshoot", &vars.aimbot.autoshoot); // 20
    this->renderCheckbox(x + 265, y + 125, "AutoPistol", &vars.aimbot.autopistol); // 15
    this->renderCheckbox(x + 265, y + 140, "AutoScope", &vars.aimbot.autoscope); // 15
    this->renderCheckbox(x + 265, y + 155, "Autostop", &vars.aimbot.autostop); // 20
    this->renderCheckbox(x + 265, y + 170, "Autocrouch", &vars.aimbot.autocrouch); // 20
    this->renderCheckbox(x + 265, y + 185, "FakeWalk", &vars.aimbot.fakewalk); // 15
    
}

void cMenu::renderVis(int x, int y) {
    draw->drawstring(x + 65, y + 15, FontColor, heading, "Visuals");

    
    this->renderCheckbox(x + 115, y + 35, "Enabled", &vars.visuals.enabled);
    this->renderCheckbox(x + 115, y + 50, "Enemy Only", &vars.visuals.enemyonly);
    this->renderCheckbox(x + 115, y + 65, "Visual Only", &vars.visuals.visonly);
    this->renderCheckbox(x + 115, y + 80, "Box", &vars.visuals.box);
    this->renderCheckbox(x + 115, y + 95, "Name", &vars.visuals.name);
    this->renderCheckbox(x + 115, y + 110, "Health", &vars.visuals.healthtext);
    this->renderCheckbox(x + 115, y + 125, "Cash", &vars.visuals.cash);
    this->renderCheckbox(x + 115, y + 140, "Rescuing", &vars.visuals.rescuing);
    this->renderCheckbox(x + 115, y + 155, "Scoped", &vars.visuals.Scoped);
    this->renderCheckbox(x + 115, y + 170, "Snapline", &vars.visuals.snapline);
    this->renderCheckbox(x + 115, y + 185, "Skeleton", &vars.visuals.skeleton);
    this->renderCheckbox(x + 115, y + 200, "Hitbox", &vars.visuals.headhitbox);
    this->renderCheckbox(x + 115, y + 215, "Player Chams", &vars.visuals.chams);


    
    // Right Side
    draw->drawstring(x + 215, y + 15, FontColor, heading, "More Visuals");
    
    this->renderCheckbox(x + 265, y + 35, "Bomb Timer", &vars.visuals.bomb);
    this->renderCheckbox(x + 265, y + 50, "Health Bar", &vars.visuals.health);
    this->renderCheckbox(x + 265, y + 65, "Armour Bar", &vars.visuals.armour);
    this->renderCheckbox(x + 265, y + 80, "No Recoil", &vars.misc.norecoil);
    this->renderCheckbox(x + 265, y + 95, "No Visual", &vars.misc.novisual);
    this->renderCheckbox(x + 265, y + 110, "Recoil Crosshair", &vars.visuals.rccrosshair);
    this->renderCheckbox(x + 265, y + 125, "DLights", &vars.visuals.dlights);
    this->renderCheckbox(x + 265, y + 140, "World Paint", &vars.misc.worldpaint);
    

    
}

void cMenu::renderMisc(int x, int y) {
    
    draw->drawstring(x + 65, y + 15, FontColor, heading, "Misc");
    
    C_BaseEntity* local = (C_BaseEntity*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    
    this->renderCheckbox(x + 115, y + 35, "Bhop", &vars.misc.bhop);
    this->renderSlider(x + 15, y + 55, 110, "FOV", vars.misc.fov, 70, 0);
    this->renderCheckbox(x + 115, y + 70, "Auto Strafe", &vars.misc.autostrafe);
    this->renderCheckbox(x + 115, y + 85, "Circle Strafe", &vars.misc.cstrafe);
    this->renderCheckbox(x + 115, y + 100, "Show Ranks", &vars.misc.showrank);
    this->renderCheckbox(x + 115, y + 115, "Clan Tag", &vars.misc.clantag);
    this->renderCheckbox(x + 115, y + 130, "No Flash", &vars.misc.noflash);
    this->renderSlider(x + 15, y + 145, 110, "", vars.misc.flashalpha, 255, 0);
    this->renderCheckbox(x + 115, y + 160, "Spammer", &vars.misc.spammer);
    this->renderCheckbox(x + 115, y + 175, "NoScope", &vars.misc.noscope);
    this->renderCheckbox(x + 115, y + 190, "Spectator List", &vars.misc.showspectators);
    this->renderCheckbox(x + 115, y + 205, "Airstuck", &vars.misc.airstuck);
    this->renderCheckbox(x + 115, y + 220, "Moonwalk", &vars.misc.moonwalk);
    
   
    /*
    // Work in progress
    vector<string> fakelag;
    
    fakelag.push_back("Simple");
    fakelag.push_back("Adaptive");
    
    this->renderCheckbox(x + 115, y + 245, "FakeLag", &vars.misc.fakelag);
    this->renderCombo(x + 15, y + 260, 100, 20, "Simple", fakelag, vars.misc.fakelagmode, &vars.fakelag_opend); // 15
    if(!vars.fakelag_opend){
        this->renderSlider(x + 15, y + 275, 110, "", vars.misc.fakelagfactor, 14, 0); // 20
    }
     */
    
    
    // Left Side
    vector<string> antiaim_pitch;
    vector<string> antiaim_yaw;
    vector<string> antiaim_fyaw;
    
    antiaim_pitch.push_back("Off");
    antiaim_pitch.push_back("Emotion");
    antiaim_pitch.push_back("Up");
    antiaim_pitch.push_back("Down");
    antiaim_pitch.push_back("FakeUp");
    antiaim_pitch.push_back("FakeDown");
    antiaim_pitch.push_back("Tankp");
    antiaim_pitch.push_back("Custom");

    antiaim_yaw.push_back("Off");
    antiaim_yaw.push_back("Backwards");
    antiaim_yaw.push_back("Jitter");
    antiaim_yaw.push_back("FakeTroll");
    antiaim_yaw.push_back("FakeStatic");
    antiaim_yaw.push_back("FJitter");
    antiaim_yaw.push_back("SlowSpin");
    antiaim_yaw.push_back("FastSpin");
    antiaim_yaw.push_back("RandomBackJitter");
    antiaim_yaw.push_back("BackJitter");
    antiaim_yaw.push_back("LowerYaw");
    antiaim_yaw.push_back("SidewaysLeft");
    antiaim_yaw.push_back("Sideways Right");
    antiaim_yaw.push_back("LBYBreaker");
    antiaim_yaw.push_back("Skeet");
    antiaim_yaw.push_back("Icecream");
    antiaim_yaw.push_back("CustomAA");

    antiaim_fyaw.push_back("Off");
    antiaim_fyaw.push_back("FakeAngel");
    antiaim_fyaw.push_back("FakeSpin");
    antiaim_fyaw.push_back("FakeLBYHook");
    antiaim_fyaw.push_back("FakeTwoStep");
    antiaim_fyaw.push_back("FakeLowerBody135");
    antiaim_fyaw.push_back("FakeInverseRotation");
    antiaim_fyaw.push_back("FakeJitter");
    antiaim_fyaw.push_back("FakeLBY");
    antiaim_fyaw.push_back("FakeSideLBY");
    antiaim_fyaw.push_back("FakeHead");

    
    draw->drawstring(x + 215, y + 15, FontColor, heading, "More Misc");
    
    this->renderCheckbox(x + 265, y + 35, "Anti Untrust", &vars.misc.antiuntrust);
    this->renderCheckbox(x + 265, y + 50, "AntiAim",&vars.misc.antiaim);
    this->renderCheckbox(x + 265, y + 65, "Thirdperson", &vars.misc.thirdpersonmode);
    this->renderCheckbox(x + 265, y + 80, "At Targers", &vars.misc.attargets);
    this->renderCheckbox(x + 265, y + 95, "Fake", &vars.misc.fakeaa);
    this->renderCheckbox(x + 265, y + 110, "DisableAA with Knife", &vars.misc.knifeaa);
    
    this->renderCombo(x + 165, y + 125, 112, 20, "Off", antiaim_pitch, vars.misc.aaX, &vars.aaX_opend);
    if(!vars.aaX_opend)
        this->renderCombo(x + 165, y + 150, 112, 20, "Off", antiaim_yaw, vars.misc.aaY, &vars.aaY_opend);
    if(((!vars.aaY_opend) && !vars.aaY_opend) && !vars.aaX_opend)
        this->renderCombo(x + 165, y + 175, 112, 20, "Off", antiaim_fyaw, vars.misc.FaaY, &vars.FaaY_opend);
    
    if(vars.misc.aaX == 7) {
    if(((!vars.aaX_opend) && !vars.aaY_opend) && !vars.FaaY_opend)
        this->renderSlider(x + 165, y + 205, 110, "Pitch", vars.misc.customaa, 180, 0);
    }
    
    if(vars.misc.aaY == 16) {
    if(((!vars.aaX_opend) && !vars.aaY_opend) && !vars.FaaY_opend)
        this->renderSlider(x + 165, y + 230, 110, "Fake Yaw", vars.misc.fakeyaw, 180, 0);
        this->renderSlider(x + 165, y + 250, 110, "Real Yaw", vars.misc.realyaw, 180, 0);
    }
    
    
    
    
}

void cMenu::renderPlayer(int x, int y) {
    
    draw->drawstring(x + 15, y + 15, FontColor, heading, "Players List");
    
    if(!pEngine->IsInGame())
        draw->drawstring(x + 15, y + 40, MainColor, heading, "Not in game");
    
    
    std::vector<std::string> PitchResolver, YawResolver;
    
    PitchResolver.push_back("Auto");
    PitchResolver.push_back("Down");
    PitchResolver.push_back("Up");
    
    YawResolver.push_back("Auto");
    YawResolver.push_back("Half Left");
    YawResolver.push_back("Half Right");
    YawResolver.push_back("Jitter");
    YawResolver.push_back("Spin");
    YawResolver.push_back("Lower Body");
    YawResolver.push_back("45 Step");
    
    int bPadding = 0;
    char plinfo[255];
    auto* local = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    
    for(int i = 1; i < 64; i++) {
        auto* entity = pEntList->GetClientEntity(i);
        
        if(!entity)
            continue;
        
        if(entity == local)
            continue;
        
        if(entity->GetTeam() == local->GetTeam())
            continue;
        
        if(entity->GetTeam() != 2 && entity->GetTeam() != 3)
            continue;
        
        player_info_t info;
        pEngine->GetPlayerInfo(i, &info);
        
        this->playerbutton(x + 15, y + 40, 120, 20, bPadding, vars.playerTab, info.name);
        
        
        if(bPadding == vars.playerTab) {
            
            sprintf(plinfo, "Player info for : %s", info.name);
            
            draw->drawstringINDEX(x + 165, y + 25, FontColor, plinfo, 0, false); // 25
            this->renderCheckbox(x + 265, y + 45, "Resolve", &gCorrections[entity->GetIndex()].resolved); // +25
            this->renderCheckbox(x + 265, y + 65, "Whitelist", &gCorrections[entity->GetIndex()].whitelist); // +25
            this->renderCheckbox(x + 265, y + 85, "bAim", &gCorrections[entity->GetIndex()].baim); // +25

            draw->drawstring(x + 165, y + 95, FontColor, mFont, "Pitch"); //
            this->renderCombo(x + 165, y + 105, 125, 20, "Auto", PitchResolver, gCorrections[entity->GetIndex()].dropdown_x, &vars.playerlist_opend_x[entity->GetIndex()]);
            
            if(!vars.playerlist_opend_x[entity->GetIndex()]) {
                draw->drawstring(x + 165, y + 115, FontColor, mFont, "Yaw");
                this->renderCombo(x + 165, y + 125, 125, 20, "Auto", YawResolver, gCorrections[entity->GetIndex()].dropdown_y, &vars.playerlist_opend_y[entity->GetIndex()]);
            }
            
        }
        bPadding++;
    }
}

void cMenu::renderColors(int x, int y) {
    
    vector<string> Colors;
    
    Colors.push_back("CT Colours");
    Colors.push_back("T Colours");
    Colors.push_back("Hand/Weapon Colours");
    Colors.push_back("World Colours");
    
    this->renderCombo(x + 15, y + 10, 125, 20, "CT Colours", Colors, vars.colors.combo, &vars.colors_opend);
    
    
    if(vars.colors.combo == 0) {
        // Box
        draw->drawcolorpicker(x + 145, y + 25, "CT Box", vars.colors.ctbox);
        draw->drawcolorpicker(x + 145, y + 160, "CT Box ignore", vars.colors.ctbox_ign);
        // Chams
        draw->drawcolorpicker(x + 290, y + 25, "CT Chams", vars.colors.ctchams);
        draw->drawcolorpicker(x + 290, y + 160, "CT Chams ignore", vars.colors.ctchams_ign);
        
    }
    
    if(vars.colors.combo == 1) {
        // Box
        draw->drawcolorpicker(x + 145, y + 25, "T Box", vars.colors.tbox);
        draw->drawcolorpicker(x + 145, y + 160, "T Box ignore", vars.colors.tbox_ign);
        // Chams
        draw->drawcolorpicker(x + 290, y + 25, "T Chams", vars.colors.tchams);
        draw->drawcolorpicker(x + 290, y + 160, "T Chams ignore", vars.colors.tchams_ign);
    
    }
    
    if(vars.colors.combo == 2) {
        
        vector<string> handsvector;
        vector<string> weaponsvector;
        
        handsvector.push_back("Lit");
        handsvector.push_back("Texture");
        handsvector.push_back("Wireframe");
        weaponsvector.push_back("Lit");
        weaponsvector.push_back("Texture");
        weaponsvector.push_back("Wireframe");
        
        draw->drawcolorpicker(x + 145, y + 25, "Hands", vars.colors.hands);
        draw->drawcolorpicker(x + 290, y + 25, "Weapon", vars.colors.weapon);
        
        this->renderCheckbox(x + 245, y + 145, "Hand Chams", &vars.visuals.handchams);
        this->renderCombo(x + 145, y + 160, 120, 20, "Lit", handsvector, vars.visuals.handsType, &vars.hands_opend);
        
        this->renderCheckbox(x + 390, y + 145, "Weapon Chams", &vars.visuals.weaponchams);
        this->renderCombo(x + 290, y + 160, 120, 20, "Lit", weaponsvector, vars.visuals.weaponType, &vars.weapons_opend);
    }
    
    if(vars.colors.combo == 3) {
        
        draw->drawcolorpicker(x + 145, y + 25, "World", vars.colors.world);
        draw->drawcolorpicker(x + 290, y + 25, "Sky", vars.colors.sky);
    }
    
}

//=======================Menu==========================//

void cMenu::renderMenu() {
    
    int wScreen, hScreen;
    pEngine->GetScreenSize(wScreen, hScreen);
    
    
    // Tab menu measurements
    static int x = 100;
    static int y = 100;
    int w = 400;
    int h = 260;
    int hh = 10;
    tab.w = 60;
    tab.h = 52;
    
    if((vars.colors.combo != 4) && tab.tab == 5)
        w = 500;
    
    
    draw->RectOutlined(x, y, w, h, 1, ControlColor, MainColor);
    draw->drawline(x + 60, y , x + 60, y + h, MainColor);
    
    if(tab.tab != 1) {
        draw->drawstring(x + 15, y + 10, inactive, osFont, "A");
        draw->drawstring(x + 12, y + 38, inactive, tFont, "Aimbot");
    }
    
    if(tab.tab != 2) {
        draw->drawstring(x + 13, y + 62, inactive, osFont, "B");
        draw->drawstring(x + 11, y + 89, inactive, tFont, "Visuals");
    }
    
    if(tab.tab != 3) {
        draw->drawstring(x + 15, y + 114, inactive, osFont, "C");
        draw->drawstring(x + 17, y + 142, inactive, tFont, "Misc");
    }
    
    if(tab.tab != 4) {
        draw->drawstring(x + 17, y + 166, inactive, osFont, "D");
        draw->drawstring(x + 12, y + 195, inactive, tFont, "Players");
    }
    
    if(tab.tab != 5) {
        draw->drawstring(x + 13, y + 218, inactive, osFont, "G");
        draw->drawstring(x + 10, y + 245, inactive, tFont, "Colours");
    }
    
    
    if(draw->inArea(x, y, tab.w, tab.h) && WasReleased)
        tab.tab = mTab::AimTab;
    
    if(draw->inArea(x, y + 52, tab.w, tab.h) && WasReleased)
        tab.tab = mTab::VisTab;
    
    if(draw->inArea(x, y + 104, tab.w, tab.h) && WasReleased)
        tab.tab = mTab::MiscTab;
    
    if(draw->inArea(x, y + 156, tab.w, tab.h) && WasReleased)
        tab.tab = mTab::PlayerTab;
    
    if(draw->inArea(x, y + 208, tab.w, tab.h) && WasReleased)
        tab.tab = mTab::ColorsTab;
    
    
    
    if(tab.tab == 1) {
        
        draw->fillrgba(x, y, tab.w, tab.h, MainColor);                      // Background when selected
        draw->drawstring(x + 15, y + 10, ControlColor, osFont, "A");
        draw->drawstring(x + 12, y + 38, ControlColor, tFont, "Aimbot");
        
        this->renderAim(x + 60, y);
        
    }
    
    if(tab.tab == 2) {
        
        draw->fillrgba(x, y + 52, tab.w, tab.h, MainColor);                      // Background when selected
        draw->drawstring(x + 13, y + 62, ControlColor, osFont, "B");
        draw->drawstring(x + 11, y + 89, ControlColor, tFont, "Visuals");
        
        this->renderVis(x + 60, y);
        
    }
    
    if(tab.tab == 3) {
        
        draw->fillrgba(x, y + 104, tab.w, tab.h, MainColor);                      // Background when selected
        draw->drawstring(x + 15, y + 114, ControlColor, osFont, "C");
        draw->drawstring(x + 17, y + 142, ControlColor, tFont, "Misc");
        
        this->renderMisc(x + 60, y);
        
    }
    
    if(tab.tab == 4) {
        
        draw->fillrgba(x, y + 156, tab.w, tab.h, MainColor);                      // Background when selected
        draw->drawstring(x + 17, y + 166, ControlColor, osFont, "D");
        draw->drawstring(x + 12, y + 195, ControlColor, tFont, "Players");
        
        
        this->renderPlayer(x + 60, y);
        
    }
    
    if(tab.tab == 5) {
        
        draw->fillrgba(x, y + 208, tab.w, tab.h, MainColor);                      // Background when selected
        draw->drawstring(x + 13, y + 218, ControlColor, osFont, "G");
        draw->drawstring(x + 10, y + 245, ControlColor, tFont, "Colours");
        
        this->renderColors(x + 60, y);
        
    }
    
    
    
    if(col.menu == 0)
        MainColor = Color(255, 0, 0, 255);

    draw->MoveWindow(x, y, w, hh, 0);    // Drag tabs menu
    
    Pressed(MOUSE_LEFT);
    
}

