// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Archangel.h"
#include "FXMgr.h"

CArchangel::CArchangel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CArchangel::CArchangel(const CArchangel& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CArchangel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CArchangel::Initialize(void* pArg)
{

	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 3;
	m_fAnimationSpeed = 6.f;

	return S_OK;
}

void CArchangel::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CArchangel::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;
	if (m_bActive)
		MonsterTick(fTimeDelta);
	return __super::Update(fTimeDelta);
}

void CArchangel::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CArchangel::Render()
{
	Render_DebugFOV();
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}

HRESULT CArchangel::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CArchangel::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	//위치탐색
	_float3 vImpactPos = CalculateEffectPos();

	//몬스터 사망
	if (0 >= m_iHP)
	{
		CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 230.f, 300.f, 1.f }, TEXT("Effect_Explor"), 32);
		m_bDead = true;

		return;
	}

	// 이펙트 생성
	m_iHP += -50;
	CFXMgr::Get_Instance()->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
}

CArchangel* CArchangel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//프로토타입의 기본정의
	CArchangel* pInstance = new CArchangel(pGraphic_Device);

	//기본 정보
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Archangel_Walk");
	pInstance->m_szBufferType	= TEXT("Rect");

	//속성
	pInstance->m_iHP			= 350;
	pInstance->m_iAttackPower	= 20;
	pInstance->m_iDefense		= 3;
	pInstance->m_fSpeed			= 13.f;
	pInstance->m_vScale			= { 61.f, 162.f, 1.f };
	pInstance->m_eState		= MODE::MODE_IDLE;
	pInstance->m_fDetectiveDistance = 300.f;

	//부속성
	pInstance->m_strDialogue	= "Archangel..Archangel...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("ArchangelDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArchangel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CArchangel::Clone(void* pArg)
{
	CArchangel* pInstance = new CArchangel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CArchangel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArchangel::Free()
{
	__super::Free();
}