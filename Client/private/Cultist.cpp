// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Cultist.h"

CCultist::CCultist(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CCultist::CCultist(const CCultist& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CCultist::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCultist::Initialize(void* pArg)
{

	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 3.f;
	m_fAnimationSpeed = 6.f;

	return S_OK;
}

void CCultist::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CCultist::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CCultist::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCultist::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}

HRESULT CCultist::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

CCultist* CCultist::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//프로토타입의 기본정의
	CCultist* pInstance = new CCultist(pGraphic_Device);

	//기본 정보
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Cultist_Run");
	pInstance->m_szBufferType	= TEXT("Rect");

	//속성
	pInstance->m_iHP			= 150;
	pInstance->m_iAttackPower	= 8;
	pInstance->m_iDefense		= 1;
	pInstance->m_fSpeed			= 10.f;
	pInstance->m_vScale			= { 27.f, 56.f, 1.f };
	pInstance->m_eBehavior		= MODE::MODE_WAIT;

	//부속성
	pInstance->m_strDialogue	= "Cultist..Cultist...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("CultistDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCultist");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCultist::Clone(void* pArg)
{
	CCultist* pInstance = new CCultist(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCultist");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCultist::Free()
{
	__super::Free();
}