// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Mechsect.h"

CMechsect::CMechsect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CMechsect::CMechsect(const CMechsect& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CMechsect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMechsect::Initialize(void* pArg)
{

	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 8.f;

	return S_OK;
}

void CMechsect::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMechsect::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CMechsect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMechsect::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}

HRESULT CMechsect::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

CMechsect* CMechsect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//프로토타입의 기본정의
	CMechsect* pInstance = new CMechsect(pGraphic_Device);

	//기본 정보
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Mechsect_Walk");
	pInstance->m_szBufferType	= TEXT("Rect");

	//속성
	pInstance->m_iHP			= 50;
	pInstance->m_iAttackPower	= 2;
	pInstance->m_iDefense		= 1;
	pInstance->m_fSpeed			= 20.f;
	pInstance->m_vScale			= { 45.f, 20.f, 1.f };
	pInstance->m_eBehavior		= MODE::MODE_WAIT;

	//부속성
	pInstance->m_strDialogue	= "Mechs..Mec...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("MechsectDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMechsect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMechsect::Clone(void* pArg)
{
	CMechsect* pInstance = new CMechsect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMechsect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMechsect::Free()
{
	__super::Free();
}