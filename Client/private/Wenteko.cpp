// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Wenteko.h"
#include "FXMgr.h"

CWenteko::CWenteko(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CWenteko::CWenteko(const CWenteko& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CWenteko::Initialize_Prototype()
{
	//프로토타입의 기본정의
	m_szTextureID = TEXT("Wenteko_Walk");
	m_szBufferType = TEXT("Rect");

	//속성
	m_iHP = 100;
	m_iMaxHP = 100;
	m_iAttackPower = 5;
	m_iDefense = 1;
	m_fSpeed = 8.f;
	m_vScale = { 50.f, 70.f, 1.f };
	m_eState = MODE::MODE_IDLE;

	//부속성
	m_strDialogue = "Wente..Wente...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("WentenkoDrop");

	return S_OK;
}

HRESULT CWenteko::Initialize(void* pArg)
{

	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 5.f;
	m_fAnimationSpeed = 8.f;

	return S_OK;
}

void CWenteko::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CWenteko::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CWenteko::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWenteko::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}

void CWenteko::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	//그 즉시 배틀모드 진입
	m_eState = MODE::MODE_BATTLE;

	//위치탐색
	_float3 vImpactPos = CalculateEffectPos();

	//몬스터 사망
	if (0 >= m_iHP)
	{
		FX_MGR->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 70.f, 110.f, 1.f }, TEXT("Effect_Explor"), 32);
		m_bDead = true;

		return;
	}

	// 이펙트 생성
	FX_MGR->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);

	__super::On_Collision(MyColliderID, OtherColliderID);
}

HRESULT CWenteko::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

CWenteko* CWenteko::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWenteko* pInstance = new CWenteko(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWenteko");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWenteko::Clone(void* pArg)
{
	CWenteko* pInstance = new CWenteko(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWenteko");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWenteko::Free()
{
	__super::Free();
}