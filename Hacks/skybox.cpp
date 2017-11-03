#include "skybox.h"
// Pasted from aimtux :P
// WIP

Color color = Color(0, 0, 0, 255);
float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f, a1 = 0.0f;

std::unordered_map<MaterialHandle_t, Color> skyboxMaterials;
std::unordered_map<MaterialHandle_t, Color> skyboxMaterials2;

const char* skyboxNames[] = {
    "cs_baggage_skybox_", // 0
    "cs_tibet",
    "embassy",
    "italy",
    "jungle",
    "office",
    "nukeblank",
    "sky_venice",
    "sky_cs15_daylight01_hdr",
    "sky_cs15_daylight02_hdr",
    "sky_cs15_daylight03_hdr",
    "sky_cs15_daylight04_hdr",
    "sky_csgo_cloudy01",
    "sky_csgo_night_flat",
    "sky_csgo_night02",
    "sky_csgo_night02b",
    "sky_day02_05",
    "sky_day02_05_hdr",
    "sky_dust",
    "vertigo",
    "vertigo_hdr",
    "vertigoblue_hdr",
    "vietnam" // 21
};

void NoSky(ClientFrameStage_t stage)
{
    if (!pEngine->IsInGame() && skyboxMaterials.size() > 0)
    {
        for (const auto& it: skyboxMaterials)
        {
            IMaterial* mat = pMatSystem->GetMaterial(it.first);
            
            if (!mat)
                continue;
            
            mat->GetColorModulate(&r1, &g1, &b1);
            a1 = mat->GetAlphaModulation();
            
            mat->ColorModulate(r1, g1, b1);
            mat->AlphaModulate(a1);
        }
        
        skyboxMaterials.clear();
        skyboxMaterials2.clear();
    }
    
    if(pEngine->IsInGame() && vars.visuals.skybox)
    
    if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END)
        return;
    
    for (MaterialHandle_t i = pMatSystem->FirstMaterial(); i != pMatSystem->InvalidMaterial(); i = pMatSystem->NextMaterial(i))
    {
        IMaterial* mat = pMatSystem->GetMaterial(i);
        
        if (!mat || strcmp(mat->GetTextureGroupName(), TEXTURE_GROUP_SKYBOX) != 0)
            continue;
        
        if (skyboxMaterials.find(i) == skyboxMaterials.end())
        {
            mat->GetColorModulate(&r1, &g1, &b1);
            a1 = mat->GetAlphaModulation();
            skyboxMaterials.emplace(i, Color(r1, g1, b1, a1));
            skyboxMaterials2.emplace(i, Color(r1, g1, b1, a1));
        }
        
        Color color = (vars.misc.nosky) ? skyboxMaterials.find(i)->second : skyboxMaterials2.find(i)->second;
        
        if (skyboxMaterials.at(i) != color)
        {
            mat->ColorModulate(color);
            skyboxMaterials.at(i) = color;
        }
        
    }
    
}
