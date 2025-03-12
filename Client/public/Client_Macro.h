#pragma once

#pragma region 프로토타입 넣는 매크로들

#define ADD_TEXTURE(Name,Path,Cnt)																					\
if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, _wstring(TEXT("Prototype_Component_Texture_"))+L###Name ,	\
CTexture::Create(m_pGraphic_Device, TEXT(Path), Cnt))))																\
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

#define CRETAE_OBJ_POOL(Name,Num)																					\
if(FAILED(m_pGameInstance->Create_Object_Pool(m_eNextLevelID, _wstring(TEXT("Prototype_GameObject_")) + L###Name,	\
_wstring(TEXT("ObjectPool_")) + L###Name, Num)))																	\
return E_FAIL

#pragma endregion

#pragma region 유틸리티

#define _VUp		_float3(0.f,1.f,0.f)

#define FWINCX static_cast<_float>(g_iWinSizeX)
#define FWINCY static_cast<_float>(g_iWinSizeY)

#define KEY_DOWN		m_pGameInstance->Key_Down
#define	KEY_PRESSING	m_pGameInstance->Key_Pressing
#define KEY_UP			m_pGameInstance->Key_Up

#pragma endregion
