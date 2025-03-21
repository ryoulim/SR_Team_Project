// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Nukemutant.h"

CNukemutant::CNukemutant(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CNukemutant::CNukemutant(const CNukemutant& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CNukemutant::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNukemutant::Initialize(void* pArg)
{

	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 3.f;
	m_fAnimationSpeed = 6.f;

	return S_OK;
}

void CNukemutant::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CNukemutant::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CNukemutant::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CNukemutant::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}

HRESULT CNukemutant::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

CNukemutant* CNukemutant::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//프로토타입의 기본정의
	CNukemutant* pInstance = new CNukemutant(pGraphic_Device);

	//기본 정보
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Nukemutant_Walk");
	pInstance->m_szBufferType	= TEXT("Rect");

	//속성
	pInstance->m_iHP			= 50;
	pInstance->m_iAttackPower	= 15;
	pInstance->m_iDefense		= 0;
	pInstance->m_fSpeed			= 15.f;
	pInstance->m_vScale			= { 27.f, 56.f, 1.f };
	pInstance->m_eBehavior		= MODE::MODE_WAIT;

	//부속성
	pInstance->m_strDialogue	= "Nuke..Nu...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("NukemutantDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNukemutant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNukemutant::Clone(void* pArg)
{
	CNukemutant* pInstance = new CNukemutant(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CNukemutant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNukemutant::Free()
{
	__super::Free();
}