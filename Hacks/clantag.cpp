#include "clantag.h"

void clantag() { 
    
    if(!vars.misc.clantag)
        return;

    static int Tick = 0;
    Tick++;
    if (Tick > 0 && Tick < 15) {
        SetClanTag("Barbossa", "Barbossa");
    }
    if (Tick > 30 && Tick < 45) {
        SetClanTag("B arbossa", "Barbossa");
    }
    else if (Tick > 60 && Tick < 75) {
        SetClanTag("B a rbossa", "Barbossa");
    }
    else if (Tick > 90 && Tick < 105) {
        SetClanTag("B a r bossa", "Barbossa");
    }
    else if (Tick > 120 && Tick < 135) {
        SetClanTag("B a r b ossa", "Barbossa");
    }
    else if (Tick > 150 && Tick < 165) {
        SetClanTag("B a r b o ssa", "Barbossa");
    }
    else if (Tick > 180 && Tick < 195) {
        SetClanTag("B a r b o s sa", "Barbossa");
    }
    else if  (Tick > 210 && Tick < 225) {
        SetClanTag("B a r b o s s a", "Barbossa");
    }
    else if  (Tick > 240 && Tick < 255) {
        SetClanTag("B a r b o s sa", "Barbossa");
    }
    else if  (Tick > 270 && Tick < 285) {
        SetClanTag("B a r b o ssa", "Barbossa");
    }
    else if  (Tick > 300 && Tick < 315) {
        SetClanTag("B a r b ossa", "Barbossa");
    }
    else if  (Tick > 330 && Tick < 345) {
        SetClanTag("B a r bossa", "Barbossa");
    }
    else if  (Tick > 360 && Tick < 375) {
        SetClanTag("B a rbossa", "Barbossa");
    }
    else if  (Tick > 390 && Tick < 405) {
        SetClanTag("B arbossa", "Barbossa");
    }
    else if  (Tick > 420 && Tick < 435) {
        SetClanTag("Barbossa", "Barbossa");
    }
    if(Tick > 435) {
        Tick = 0;
    }
}



