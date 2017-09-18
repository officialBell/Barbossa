#include "main.h"

ISurface* pSurface = nullptr;
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

VMT* painthook;
VMT* createmovehook;
VMT* clientvmt;
VMT* modelhook;
VMT* predhook;
