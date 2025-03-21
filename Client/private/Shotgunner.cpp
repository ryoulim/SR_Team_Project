// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Shotgunner.h"

CShotgunner::CShotgunner(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CShotgunner::CShotgunner(const CShotgunner& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CShotgunner::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CShotgunner::Initialize(void* pArg)
{

	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	m_fDivOffset = 45.f;
	m_isReadyMonster = true;
	Ready_Textures();
	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 3.f;
	m_fAnimationSpeed = 7.f;

	return S_OK;
}

void CShotgunner::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CShotgunner::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CShotgunner::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CShotgunner::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}

HRESULT CShotgunner::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShotgunner::Ready_Textures()
{
	/* ATTACK */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Shotgunner_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Shotgunner_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}

	/* DEATH */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Shotgunner_Death_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Shotgunner_Death_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEATH][i])))))
			return E_FAIL;
	}

	/* MOVE */
	//for (_uint i = 0; i < D_END; i++)
	//{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Shotgunner_Move_";
		_uint num = static_cast<_uint>(0);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Shotgunner_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][0])))))
			return E_FAIL;
	//}


	return S_OK;
}

CShotgunner* CShotgunner::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//프로토타입의 기본정의
	CShotgunner* pInstance = new CShotgunner(pGraphic_Device);

	//기본 정보
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Shotgunner_Walk");
	pInstance->m_szBufferType	= TEXT("Rect");

	//속성
	pInstance->m_iHP			= 80;
	pInstance->m_iAttackPower	= 6;
	pInstance->m_iDefense		= 0;
	pInstance->m_fSpeed			= 4.f;
	pInstance->m_vScale			= { 27.f, 46.f, 1.f };
	pInstance->m_eBehavior		= MODE::MODE_WAIT;

	//부속성
	pInstance->m_strDialogue	= "Gun..Gun...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("ShotgunnerDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CShotgunner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShotgunner::Clone(void* pArg)
{
	CShotgunner* pInstance = new CShotgunner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CShotgunner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShotgunner::Free()
{
	__super::Free();
}