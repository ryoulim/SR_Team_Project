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
	if (m_bDead)
		return EVN_DEAD;

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

void CWenteko::On_Collision(CGameObject* pCollisionedObject, const _wstring& strLayerTag)
{
	//위치탐색
	_float3 vImpactPos = CalculateEffectPos();

	//몬스터 사망
	if (0 >= m_iHP)
	{
		CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 70.f, 110.f, 1.f }, TEXT("Effect_Explor"), 32);
		m_bDead = true;

		return;
	}

	// 이펙트 생성
	m_iHP += -10;
	CFXMgr::Get_Instance()->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
}

HRESULT CWenteko::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

CWenteko* CWenteko::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//프로토타입의 기본정의
	CWenteko* pInstance = new CWenteko(pGraphic_Device);

	//기본 정보
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Wenteko_Walk");
	pInstance->m_szBufferType	= TEXT("Rect");

	//속성
	pInstance->m_iHP			= 100;
	pInstance->m_iAttackPower	= 5;
	pInstance->m_iDefense		= 1;
	pInstance->m_fSpeed			= 8.f;
	pInstance->m_vScale			= { 50.f, 70.f, 1.f };
	pInstance->m_eBehavior		= MODE::MODE_WAIT;

	//부속성
	pInstance->m_strDialogue	= "Wente..Wente...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("WentenkoDrop");


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