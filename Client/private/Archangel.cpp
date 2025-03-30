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

	m_fDivOffset = 45.f;
	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;
	return S_OK;
}

void CArchangel::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
	Resize_Texture(0.35f);
}

EVENT CArchangel::Update(_float fTimeDelta)
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

void CArchangel::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CArchangel::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}

HRESULT CArchangel::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	Ready_Textures();

	return S_OK;
}

HRESULT CArchangel::Ready_Textures()
{
	/* MOVE */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}
	/* ATTACK */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}
	/* FLY */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Fly_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Fly_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_FLY][i])))))
			return E_FAIL;
	}
	/* STAY */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Stay_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Stay_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_STAY][i])))))
			return E_FAIL;
	}

	/* DEATH */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Archangel_Dead",
		_wstring(TEXT("Com_Texture")) + L"_Archangel_Dead", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;

	return S_OK;
}


HRESULT CArchangel::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CArchangel::STATE_MOVE:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CArchangel::STATE_FLY:
			m_fAnimationMaxFrame = _float(MAX_FLY);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CArchangel::STATE_ATTACK:
			m_fAnimationMaxFrame = _float(MAX_ATTACK);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CArchangel::STATE_STAY:
			m_fAnimationMaxFrame = _float(MAX_STAY);
			m_fAnimationSpeed = 0.f;
			break;
		case Client::CArchangel::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 8.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CArchangel::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CArchangel::STATE_STAY:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		return S_OK;
	case Client::CArchangel::STATE_MOVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CArchangel::STATE_FLY:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CArchangel::STATE_ATTACK:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CArchangel::STATE_DEAD:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = m_fAnimationMaxFrame - 1.f;
		m_bRotateAnimation = false;
		break;
	}
	return S_OK;
}


void CArchangel::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	//그 즉시 배틀모드 진입
	m_eState = MODE::MODE_BATTLE;

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
	pInstance->m_iMaxHP			= 350;
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