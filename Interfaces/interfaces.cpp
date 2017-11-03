#include "main.h"

ISurface* pSurface = nullptr;
CEffects* pCEffects = nullptr; // Thnx Rocco
IPanel* pPanel = nullptr;
ICvar* pCvar = nullptr;
IBaseClientDLL* pClient = nullptr;
IClientEntityList* pEntList = nullptr;
IVDebugOverlay* pOverlay = nullptr;
IEngineClient* pEngine = nullptr;
IVModelInfo* pModelInfo = nullptr;
IEngineTrace* pEngineTrace = nullptr;
IClientMode* pClientMode = nullptr;
CGlobalVarsBase* pGlobals = nullptr;
IInputSystem* pInputSystem = nullptr;
CInput* pInput = nullptr;
IMaterialSystem* pMatSystem = nullptr;
IVModelRender* pModelRender = nullptr;
IPrediction* pPrediction = nullptr;
IGameMovement* pGameMovement = nullptr;
IMoveHelper* pMoveHelper = nullptr;
IPhysicsSurfaceProps* pPhysics = nullptr;
//
SendClanTagFn SetTag;
MsgFunc_ServerRankRevealAllFn MsgFunc_ServerRankRevealAll;
RandomIntFn RandomInt;


VMT* painthook;
VMT* createmovehook;
VMT* clientvmt;
VMT* modelhook;
VMT* predhook;
VMT* materialVMT;

