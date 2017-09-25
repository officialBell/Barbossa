class ccombobox {
public:
    /* this class only handles all the open checks for the combo boxes, nothing more. */
    bool esp_opend;
    bool hitbox_opend;
    bool aaY_opend;
    bool aaX_opend;
    bool hands_opend;
    bool weapons_opend;
    bool players_opend;
    bool playerlist_opend_x[64];
    bool playerlist_opend_y[64];
    
    bool misc_clantag, misc_name, misc_changename;
    int inputIndex = 0;
};

// Set up your fucking settings here.

class cVariables : public ccombobox {
public:
    bool menu = false;
    int buttonPadding = 24;
    int tabIndex;
    int menueffect = 0;
    
    class cAimbot {
    public:
        bool enabled = false; // Enable
        bool silent = false; // Silent Aim obv
        bool psilent = false; // Perfect Silent
        int fov = 20; // Aimbot Fov
        int hitbox = 2; // Hitbox/Bone
        int pointscale = 73; // Autowall Pointscale
        int bone = 0; // Bone [I dont know why this is here]
        bool hitscan = false; // Hitscan (Does not work)
        bool selfpred = false; // // Self spread obv
        bool autowall = false; // Shoots through walls
        bool autoshoot = false; // Shoots for you
        bool autopistol = false; // Pistol is like AK
        bool autocrouch = false; // Crouch when something is in fov
        bool autostop = false; // Stops when something is in fov
    }aimbot;
    
    class cVisuals {
    public:
        bool enabled = false; // Enable
        bool box = false; // Box ESP
        bool name = false; // Name ESP
        bool health = false; // Health Bar (Does not work)
        bool healthtext = false; // Health Text
        bool weapon = false; // Current weapon ESP (Does not work)
        bool snapline = false; // Snaplines obv
        bool handchams = true; // Handchams obv
        bool weaponchams = true; // Weapon chams obv
        bool skeleton = false; // Skeleton ESP
        bool chams = false; // Player Chams
        bool headhitbox = false; // Snaplines but goes to head
        bool enemyonly = false; // Enemy Only ofc
        
        int weaponType = 1; // Weapon Type 0 - Lit 1 - Texture
        int handsType = 1; // Hand Type 0 - Lit 1 - Texture
        int playersType = 0; // Player Type 0 - Lit 1 - Texture
        
        int weaponchams_alpha = 255; // Weapon Chams Alpha
        int handchams_alpha = 255; // Hand Chams Alpha
        int playerchams_alpha = 255; // Player Chams Alpha
        
        int coltab;
        
        int tchams[4] = { 255, 255, 0, 255 }; // T vis colour
        int ctchams[4] = { 0, 255, 0, 255 }; // CT vis colour
        
        int tchams_ignore[4] = { 236, 81, 63, 255 }; // T nonvis colour
        int ctchams_ignore[4] = { 51, 124, 197, 255 }; // CT nonvis colour (this color is soo fucking p100!)
        
        int tbox[4] = { 255, 255, 0, 255 }; // T vis box colour
        int ctbox[4] = { 0, 255, 0, 255 }; // CT vis box colour
        
        int tbox_ignore[4] = { 236, 81, 63, 255 }; // T nonvis box colour
        int ctbox_ignore[4] = { 51, 124, 197, 255 }; // cT nonvis box colour (this color is soo fucking p100!)
        
        int Sky [4] = { 0, 0 , 0 };
        
    }visuals;
    
    class cMisc {
    public:
        bool enabled = true; // Enabled
        bool antiuntrust = true; // Anti Untrust obv
        bool norecoil = false; // Basically RCS without slider
        bool novisualrecoil = false; // No visual recoil ofc
        bool nosky = false; // No Sky
        bool worldpaint = false; // World Paint
        bool bhop = false; // BunnyHop
        bool autostrafe = false; // Strafes for you ofc
        bool circlestrafe = false; // Circle strafes
        bool thirdpersonmode = false; // Thirdperson (Needs sig)
        bool antiscreenshot = false; // Antiscreenshot
        int fov = 10; // FOV changer
        int aaY = 0; // Y AntiAim
        int aaX = 0; // X AntiAim
        bool attargets = false; // AtTargets ofc
        int customaa = 0; // idk
        int fakeyaw = 180; // Fake Y
        int realyaw = 20; // Real Y
        int fakelagfactor = 0; // Fake Lag (Does not work)
        bool fakelag = false; // Fake lag enable ^
        bool fakeaa = false; // Enable Fake ofc
        bool clantag = true; // Enables Clantag
    }misc;
    
    class cColors {
    public:
        int tab;
        
        Color ctchams;
        Color tchams;
        
        Color ctchams_ign;
        Color tchams_ign;
        
        Color tbox;
        Color tbox_ign;
        
        Color ctbox;
        Color ctbox_ign;
        
        Color hands;
        Color hands_ign;
        
        Color Sky;
        Color world;
        
    }colors;
    
    int playerTab = 0;
};

struct Corrections {
    float x, y;
    bool whitelist;
    bool resolved;
    
    int dropdown_x = 0;
    int dropdown_y = 0;
};

extern Corrections gCorrections[64];


extern cVariables vars;
