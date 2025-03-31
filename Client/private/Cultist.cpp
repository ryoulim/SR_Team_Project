// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Cultist.h"
#include "FXMgr.h"

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
	//프로토타입의 기본정의
	m_szTextureID = TEXT("Cultist_Run");
	m_szBufferType = TEXT("Rect");

	//속성
	m_iHP = 150;
	m_iMaxHP = 150;
	m_iAttackPower = 8;
	m_iDefense = 1;
	m_fSpeed = 10.f;
	m_vScale = { 27.f, 56.f, 1.f };
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 300.f;

	//부속성
	m_strDialogue = "Cultist..Cultist...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("CultistDropItem");

	return S_OK;
}

HRESULT CCultist::Initialize(void* pArg)
{

	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDivOffset = 45.f;
	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;

	return S_OK;
}

void CCultist::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CCultist::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_RBRACKET))
	{
		int i = m_eCurMonsterState;
		i++;
		m_eCurMonsterState = MONSTER_STATE(i);
		if (m_eCurMonsterState == STATE_END)
			m_eCurMonsterState = MONSTER_STATE(0);
	}
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

	Ready_Textures();

	return S_OK;
}

HRESULT CCultist::Ready_Textures()
{
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Cultist_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Cultist_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Cultist_Run_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Cultist_Run_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_RUN][i])))))
			return E_FAIL;
	}
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Cultist_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Cultist_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Cultist_Dead",
		_wstring(TEXT("Com_Texture")) + L"_Cultist_Dead", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCultist::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CCultist::STATE_MOVE:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CCultist::STATE_RUN:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CCultist::STATE_ATTACK:
			m_fAnimationMaxFrame = _float(MAX_ATTACK);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CCultist::STATE_STAY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationSpeed = 0.f;
			m_iState = (_uint)(STATE_MOVE);
			break;
		case Client::CCultist::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 8.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CCultist::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CCultist::STATE_STAY:
		m_bRotateAnimation = true;
		return S_OK;
	case Client::CCultist::STATE_MOVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CCultist::STATE_RUN:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CCultist::STATE_ATTACK:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CCultist::STATE_DEAD:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = m_fAnimationMaxFrame - 1.f;
		m_bRotateAnimation = false;
		break;
	}
	return S_OK;
}

void CCultist::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	//그 즉시 배틀모드 진입
	m_eState = MODE::MODE_BATTLE;

	//위치탐색
	_float3 vImpactPos = CalculateEffectPos();

	//몬스터 사망
	if (0 >= m_iHP)
	{
		FX_MGR->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 60.f, 80.f, 1.f }, TEXT("PC_Explosion"), 14);
		m_bDead = true;

		return;
	}

	// 이펙트 생성
	m_iHP += -50;
	FX_MGR->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
}

CCultist* CCultist::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCultist* pInstance = new CCultist(pGraphic_Device);

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