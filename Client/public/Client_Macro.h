#pragma once

#pragma region 프로토타입 넣는 매크로들

#define ADD_TEXTURE(Name,Path,Cnt)																					\
if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, _wstring(TEXT("Prototype_Component_Texture_"))+L###Name ,	\
CTexture::Create(m_pGraphic_Device, TEXT(Path), Cnt))))																\
return E_FAIL

#define ADD_TEXTURE_EX(Name,Path,Cnt,...)																			\
if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, _wstring(TEXT("Prototype_Component_Texture_"))+L###Name ,	\
CTexture::Create(m_pGraphic_Device, TEXT(Path), Cnt, __VA_ARGS__))))												\
return E_FAIL

#define ADD_SOUND(Name,Path)																				\
if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, _wstring(TEXT("Prototype_Component_Sound_"))+L###Name ,	\
CSoundController::Create(Path))))															\
return E_FAIL

#define ADD_SOUND_EX(Name,Path,...)																				\
if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, _wstring(TEXT("Prototype_Component_Sound_"))+L###Name ,	\
CSoundController::Create(Path, __VA_ARGS__))))															\
return E_FAIL

#define ADD_MODEL(Name)																								\
if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, _wstring(TEXT("Prototype_Component_VIBuffer_"))+L###Name,	\
CVIBuffer_##Name::Create(m_pGraphic_Device))))																		\
return E_FAIL

#define ADD_MODEL_EX(Name,...)																						\
if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, _wstring(TEXT("Prototype_Component_VIBuffer_"))+L###Name,	\
CVIBuffer_##Name::Create(m_pGraphic_Device, __VA_ARGS__ ))))														\
return E_FAIL

#define ADD_PRTOBJ(Name)																							\
if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, _wstring(TEXT("Prototype_GameObject_"))+L###Name ,		\
C##Name::Create(m_pGraphic_Device))))																				\
return E_FAIL

#define ADD_PRTOBJ_EX(Name,...)																						\
if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, _wstring(TEXT("Prototype_GameObject_"))+L###Name ,		\
C##Name::Create(m_pGraphic_Device, __VA_ARGS__ ))))																	\
return E_FAIL

#define ADD_PRTCOM(Name)																							\
if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, _wstring(TEXT("Prototype_Component_")) +L###Name,			\
C##Name::Create(m_pGraphic_Device))))																				\
return E_FAIL

#define ADD_PRTCOM_EX(Name,...)																						\
if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, _wstring(TEXT("Prototype_Component_")) +L###Name,			\
C##Name::Create(m_pGraphic_Device, __VA_ARGS__ ))))																	\
return E_FAIL	

#define CREATE_OBJ_POOL(Name,Num,Arg)																					\
if(FAILED(m_pGameInstance->Create_Object_Pool(m_eNextLevelID, _wstring(TEXT("Prototype_GameObject_")) + L###Name,	\
_wstring(TEXT("ObjectPool_")) + L###Name, Num,Arg)))																	\
return E_FAIL

#pragma endregion

#pragma region 유틸리티

#define GET_PLAYER m_pGameInstance->Find_Object(LEVEL_STATIC,TEXT("Layer_Pawn"))
#define GET_PLAYER_TRANSFORM static_cast<CTransform*>(GET_PLAYER->Find_Component(TEXT("Com_Transform")))

#define GET_CAMERA											\
_float4x4 matCamWorld;										\
m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);	\
matCamWorld.MakeInverseMat(matCamWorld);

#define Get_CamaraPos GET_CAMERA											\
_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };	

#define _VUp		_float3(0.f,1.f,0.f)

#define FWINCX static_cast<_float>(g_iWinSizeX)
#define FWINCY static_cast<_float>(g_iWinSizeY)

#define KEY_DOWN		m_pGameInstance->Key_Down
#define	KEY_PRESSING	m_pGameInstance->Key_Pressing
#define KEY_UP			m_pGameInstance->Key_Up

#define MOUSE_DOWN		m_pGameInstance->Mouse_Down
#define MOUSE_PRESSING	m_pGameInstance->Mouse_Pressing
#define MOUSE_UP		m_pGameInstance->Mouse_Up

#pragma endregion

#pragma region 몬스터 생성 매크로

#define SPAWN_CULTIST(x,y,z, level, Standby)				\
{CMonster::DESC Cultist_iDesc{};							\
Cultist_iDesc.fSpeedPerSec = 60.f;							\
Cultist_iDesc.fRotationPerSec = RADIAN(180.f);				\
Cultist_iDesc.vActive = true;								\
Cultist_iDesc.eLevel = level;								\
Cultist_iDesc.fAttackDistance = 400.f;						\
Cultist_iDesc.fDetectiveDistance = 500.f;					\
Cultist_iDesc.bStandby = Standby;						    \
_float3 Pos = { x, y, z };									\
Cultist_iDesc.vPosition = Pos;								\
Cultist_iDesc.vReturnPos = Pos;								\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Cultist"),						\
level, L"Layer_Monster", &Cultist_iDesc)))					\
return E_FAIL;}												\

#define SPAWN_SHOTGUNNER(x,y,z, level, Standby)				\
{CMonster::DESC Shotgunner_iDesc{};							\
Shotgunner_iDesc.fSpeedPerSec = 60.f;						\
Shotgunner_iDesc.fRotationPerSec = RADIAN(180.f);			\
Shotgunner_iDesc.vActive = true;							\
Shotgunner_iDesc.eLevel = level;							\
Shotgunner_iDesc.fAttackDistance = 300.f;					\
Shotgunner_iDesc.fDetectiveDistance = 700.f; 				\
Shotgunner_iDesc.bStandby = Standby;						\
_float3 Pos = { x, y, z };									\
Shotgunner_iDesc.vPosition = Pos;							\
Shotgunner_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Shotgunner"),					\
level, L"Layer_Monster", &Shotgunner_iDesc)))				\
return E_FAIL;}												\

#define SPAWN_GREATER(x,y,z, level, Standby)				\
{CMonster::DESC Greater_iDesc{};							\
Greater_iDesc.fSpeedPerSec = 60.f;							\
Greater_iDesc.fRotationPerSec = RADIAN(180.f);				\
Greater_iDesc.vActive = true;								\
Greater_iDesc.eLevel = level;								\
Greater_iDesc.fAttackDistance = 400.f;						\
Greater_iDesc.fDetectiveDistance = 500.f; 					\
Greater_iDesc.bStandby = Standby;							\
_float3 Pos = { x, y, z };									\
Greater_iDesc.vPosition = Pos;								\
Greater_iDesc.vReturnPos = Pos;								\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Greater"),						\
level, L"Layer_Monster", &Greater_iDesc)))					\
return E_FAIL;}												\

#define SPAWN_WENTEKO(x,y,z, level, Standby)				\
{CMonster::DESC Wenteko_iDesc{};							\
Wenteko_iDesc.fSpeedPerSec = 60.f;							\
Wenteko_iDesc.fRotationPerSec = RADIAN(180.f);				\
Wenteko_iDesc.vActive = true;								\
Wenteko_iDesc.eLevel = level;								\
Wenteko_iDesc.fAttackDistance = 400.f;						\
Wenteko_iDesc.fDetectiveDistance = 500.f;					\
Wenteko_iDesc.bStandby = Standby;							\
_float3 Pos = { x, y, z };									\
Wenteko_iDesc.vPosition = Pos;								\
Wenteko_iDesc.vReturnPos = Pos;								\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Wenteko"),						\
level, L"Layer_Monster", &Wenteko_iDesc)))					\
return E_FAIL;}												\

#define SPAWN_NUKEMUTANT(x,y,z, level, Standby)				\
{CMonster::DESC Nukemutant_iDesc{};							\
Nukemutant_iDesc.fSpeedPerSec = 60.f;						\
Nukemutant_iDesc.fRotationPerSec = RADIAN(180.f);			\
Nukemutant_iDesc.vActive = true;							\
Nukemutant_iDesc.eLevel = level;							\
Nukemutant_iDesc.fAttackDistance = 400.f;					\
Nukemutant_iDesc.fDetectiveDistance = 500.f; 				\
Nukemutant_iDesc.bStandby = Standby;						\
_float3 Pos = { x, y, z };									\
Nukemutant_iDesc.vPosition = Pos;							\
Nukemutant_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Nukemutant"),					\
level, L"Layer_Monster", &Nukemutant_iDesc)))				\
return E_FAIL;}												\

#define SPAWN_MECHSECT(x,y,z, level, Standby)				\
{CMonster::DESC Mechsect_iDesc{};							\
Mechsect_iDesc.fSpeedPerSec = 60.f;							\
Mechsect_iDesc.fRotationPerSec = RADIAN(180.f);				\
Mechsect_iDesc.vActive = true;								\
Mechsect_iDesc.eLevel = level;								\
Mechsect_iDesc.fAttackDistance = 300.f;						\
Mechsect_iDesc.fDetectiveDistance = 600.f; 					\
Mechsect_iDesc.bStandby = Standby;							\
_float3 Pos = { x, y, z };									\
Mechsect_iDesc.vPosition = Pos;								\
Mechsect_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Mechsect"),						\
level, L"Layer_Monster", &Mechsect_iDesc)))					\
return E_FAIL;}												\

#define SPAWN_ARCHANGEL(x,y,z, level, Standby)				\
{CMonster::DESC Archangel_iDesc{};							\
Archangel_iDesc.fSpeedPerSec = 60.f;						\
Archangel_iDesc.fRotationPerSec = RADIAN(180.f);			\
Archangel_iDesc.vActive = true;								\
Archangel_iDesc.eLevel = level;								\
Archangel_iDesc.fAttackDistance = 600.f;					\
Archangel_iDesc.fDetectiveDistance = 800.f; 				\
Archangel_iDesc.bStandby = Standby;							\
_float3 Pos = { x, y, z };									\
Archangel_iDesc.vPosition = Pos;							\
Archangel_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Archangel"),						\
level, L"Layer_Monster", &Archangel_iDesc)))				\
return E_FAIL;}												\

#define SPAWN_SHOTGUNNER_EX(x,y,z, level, Standby)			\
{CMonster::DESC Shotgunner_iDesc{};							\
Shotgunner_iDesc.fSpeedPerSec = 60.f;						\
Shotgunner_iDesc.fRotationPerSec = RADIAN(180.f);			\
Shotgunner_iDesc.vActive = true;							\
Shotgunner_iDesc.eLevel = level;							\
Shotgunner_iDesc.fAttackDistance = 300.f;					\
Shotgunner_iDesc.fDetectiveDistance = 700.f; 				\
Shotgunner_iDesc.bStandby = Standby;						\
_float3 Pos = { x, y, z };									\
Shotgunner_iDesc.vPosition = Pos;							\
Shotgunner_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Shotgunner"),					\
level, L"Layer_Monster", &Shotgunner_iDesc)))				\
return EVN_NONE;}											\

#define SPAWN_NUKEMUTANT_EX(x,y,z, level)					\
{CMonster::DESC Nukemutant_iDesc{};							\
Nukemutant_iDesc.fSpeedPerSec = 60.f;						\
Nukemutant_iDesc.fRotationPerSec = RADIAN(180.f);			\
Nukemutant_iDesc.vActive = true;							\
Nukemutant_iDesc.eLevel = level;							\
Nukemutant_iDesc.fAttackDistance = 400.f;					\
Nukemutant_iDesc.fDetectiveDistance = 500.f; 				\
Nukemutant_iDesc.bStandby = false;							\
_float3 Pos = { x, y, z };									\
Nukemutant_iDesc.vPosition = Pos;							\
Nukemutant_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Nukemutant"),					\
level, L"Layer_Monster", &Nukemutant_iDesc)))				\
return EVN_NONE;}											\

#define SPAWN_SHOTGUNNER_ALT(x,y,z, level, Standby)			\
{CMonster::DESC Shotgunner_iDesc{};							\
Shotgunner_iDesc.fSpeedPerSec = 60.f;						\
Shotgunner_iDesc.fRotationPerSec = RADIAN(180.f);			\
Shotgunner_iDesc.vActive = true;							\
Shotgunner_iDesc.eLevel = level;							\
Shotgunner_iDesc.fAttackDistance = 300.f;					\
Shotgunner_iDesc.fDetectiveDistance = 700.f; 				\
Shotgunner_iDesc.bStandby = Standby;						\
_float3 Pos = { x, y, z };									\
Shotgunner_iDesc.vPosition = Pos;							\
Shotgunner_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Shotgunner"),					\
level, L"Layer_Monster", &Shotgunner_iDesc)))				\
return;}													\

#define SPAWN_NUKEMUTANT_ALT(x,y,z, level)					\
{CMonster::DESC Nukemutant_iDesc{};							\
Nukemutant_iDesc.fSpeedPerSec = 60.f;						\
Nukemutant_iDesc.fRotationPerSec = RADIAN(180.f);			\
Nukemutant_iDesc.vActive = true;							\
Nukemutant_iDesc.eLevel = level;							\
Nukemutant_iDesc.fAttackDistance = 400.f;					\
Nukemutant_iDesc.fDetectiveDistance = 500.f; 				\
Nukemutant_iDesc.bStandby = false;							\
_float3 Pos = { x, y, z };									\
Nukemutant_iDesc.vPosition = Pos;							\
Nukemutant_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Nukemutant"),					\
level, L"Layer_Monster", &Nukemutant_iDesc)))				\
return;}													\

#define SPAWN_ARCHANGEL_ALT(x,y,z, level, Standby)			\
{CMonster::DESC Archangel_iDesc{};							\
Archangel_iDesc.fSpeedPerSec = 60.f;						\
Archangel_iDesc.fRotationPerSec = RADIAN(180.f);			\
Archangel_iDesc.vActive = true;								\
Archangel_iDesc.eLevel = level;								\
Archangel_iDesc.fAttackDistance = 600.f;					\
Archangel_iDesc.fDetectiveDistance = 800.f; 				\
Archangel_iDesc.bStandby = Standby;							\
_float3 Pos = { x, y, z };									\
Archangel_iDesc.vPosition = Pos;							\
Archangel_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Archangel"),						\
level, L"Layer_Monster", &Archangel_iDesc)))				\
return;}													\

#pragma endregion
