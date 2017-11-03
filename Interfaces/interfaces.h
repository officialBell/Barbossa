#pragma once

extern ISurface* pSurface;
extern CEffects* pCEffects; // Thnx Rocco
extern IPanel* pPanel;
extern ICvar* pCvar;
extern IBaseClientDLL* pClient;
extern IClientEntityList* pEntList;
extern IVDebugOverlay* pOverlay;
extern IEngineClient* pEngine;
extern IVModelInfo* pModelInfo;
extern IEngineTrace* pEngineTrace;
extern IClientMode* pClientMode;
extern CGlobalVarsBase* pGlobals;
extern IInputSystem* pInputSystem;
extern CInput* pInput;
extern IMaterialSystem* pMatSystem;
extern IVModelRender* pModelRender;
extern IPrediction* pPrediction;
extern IGameMovement* pGameMovement;
extern IMoveHelper* pMoveHelper;
extern IPhysicsSurfaceProps* pPhysics;
//
extern SendClanTagFn SetTag;
typedef void (*SendClanTagFn) (const char*, const char*);
extern MsgFunc_ServerRankRevealAllFn MsgFunc_ServerRankRevealAll;
extern RandomIntFn RandomInt;



extern VMT* painthook;
extern VMT* createmovehook;
extern VMT* clientvmt;
extern VMT* modelhook;
extern VMT* predhook;
extern VMT* materialVMT;
