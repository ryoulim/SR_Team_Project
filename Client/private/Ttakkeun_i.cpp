// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Ttakkeun_i.h"

CTtakkeun_i::CTtakkeun_i(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CTtakkeun_i::CTtakkeun_i(const CTtakkeun_i& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CTtakkeun_i::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTtakkeun_i::Initialize(void* pArg)
{

	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTtakkeun_i::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CTtakkeun_i::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CTtakkeun_i::Late_Update(_float fTimeDelta)
{
	m_pCollider->Update_Collider(m_pTransformCom);
	__super::Late_Update(fTimeDelta);
}

HRESULT CTtakkeun_i::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}

HRESULT CTtakkeun_i::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTtakkeun_i::Set_TextureType()
{

	return S_OK;
}

CTtakkeun_i* CTtakkeun_i::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//프로토타입의 기본정의
	CTtakkeun_i* pInstance = new CTtakkeun_i(pGraphic_Device);

	//기본 정보
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Ttakkeun_i");
	pInstance->m_szBufferType	= TEXT("Rect");

	//속성
	pInstance->m_iHP			= 500;
	pInstance->m_iAttackPower	= 10;
	pInstance->m_iDefense		= 3;
	pInstance->m_fSpeed			= 10.f;
	pInstance->m_vScale			= { 150.f, 147.f, 1.f };
	pInstance->m_eBehavior		= MODE::MODE_WAIT;

	//부속성
	pInstance->m_strDialogue	= "Boss..Boss...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("BossDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTtakkeun_i");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTtakkeun_i::Clone(void* pArg)
{
	CTtakkeun_i* pInstance = new CTtakkeun_i(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTtakkeun_i");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTtakkeun_i::Free()
{
	__super::Free();
}