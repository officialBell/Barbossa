#include "main.h"
#include "../Hacks/esp.h"
#include "../Drawings/menu.h"
#include "../Hacks/dlights.h"
#include "../Hacks/spectators.h"
#include "../Hacks/recoilcrosshair.h"

bool qOnce = false;
bool bPanelOnce = false;

void hkPaintTraverse(void* thisptr, VPANEL panel, bool forceRepaint, bool allowForce)
{
    
    if (vars.misc.noscope && !strcmp("HudZoom", pPanel->GetName(panel)))
        return;
    
    painthook->GetOriginalMethod<tPaintTraverse>(42)(thisptr, panel, forceRepaint, allowForce);
    
    auto* local = pEntList->GetClientEntity(pEngine->GetLocalPlayer());

    
    static VPANEL last = 0;
    
    if(!last)
    {
        if(strstr(pPanel->GetName(panel), "FocusOverlayPanel"))
        {
            
            framefont   = draw->createfont(framefont, "Tahoma", 12, FONTFLAG_NONE);          //
            espfont     = draw->createfont(espfont, "Tahoma", 12, FONTFLAG_DROPSHADOW);      // Use for ESP
            specfont    = draw->createfont(specfont, "Tahoma", 12, FONTFLAG_OUTLINE);        // Used for spectatorwindow

            mFont       = draw->createfont(mFont, "Tahoma", 11, FONTFLAG_DROPSHADOW);        // Used for checkboxes
            sFont       = draw->createfont(sFont, "Verdana", 11, FONTFLAG_OUTLINE);          // Used for sliders
            tFont       = draw->createfont(tFont, "Verdana", 11, FONTFLAG_ANTIALIAS);        // Used for tabs
            heading     = draw->createfont(heading, "Tahoma", 13, FONTFLAG_OUTLINE);         // Used for headings
            osFont      = draw->createfont(osFont, "osmiummenufont", 35, FONTFLAG_ANTIALIAS);// Used for icons
            cFont       = draw->createfont(cFont, "Tahoma", 12, FONTFLAG_NONE);              // Used for combos
            
            
            
            last = panel;
        }
        
    }
    
    if(!bPanelOnce)
    {
        vars.colors.ctchams     = Color(40, 204, 67, 255);
        vars.colors.ctchams_ign = Color(0, 124, 255, 255);
        
        vars.colors.tchams      = Color(204, 173, 40, 255);
        vars.colors.tchams_ign  = Color(255, 69, 0, 255);
        
        vars.colors.tbox        = Color(204, 173, 40, 255);
        vars.colors.tbox_ign    = Color(255, 69, 0, 255);
        
        vars.colors.ctbox       = Color(40, 204, 67, 255);
        vars.colors.ctbox_ign   = Color(0, 124, 255, 255);
        
        vars.colors.hands       = Color::Black();
        vars.colors.weapon      = Color::White();
        
        vars.colors.scope       = Color::Red();
        vars.colors.world       = Color::Red(); // :^)
        vars.colors.sky         = Color::Red();
        
        
        
        
        bPanelOnce = true;
    }
    
    if(panel == last)
    {
        
        if(vars.misc.antiscreenshot && pEngine->IsTakingScreenshot())
            return;
        
        pwnmymenu();
        DrawPlayerESP();
        DrawScope(local); // Thnx pwned/bell
        DrawOtherESP();   // Thnx pwned/bell
        rCrosshair(local);
        DLights();        // Thnx Rocc
        Spectatorlist();  // Thnx Bypass
        
        if(vars.menu)
        {
            
            menu->renderMenu();
            draw->drawmouse();
            
            pEngine->ExecuteClientCmd("cl_mouseenable 0");
            
        }
        
    }
    
}
