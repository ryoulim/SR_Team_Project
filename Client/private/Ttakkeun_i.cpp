// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Ttakkeun_i.h"
#include "FXMgr.h"

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

	m_isReadyMonster = true;
	m_fDivOffset = 22.5f;
	// 보스랑 잡몹 텍스쳐 갯수가 달라서 별도 지정
	Ready_Textures();
	
	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 11.f;
	m_fAnimationSpeed = 13.f;


	return S_OK;
}

void CTtakkeun_i::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CTtakkeun_i::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	return __super::Update(fTimeDelta);
}

void CTtakkeun_i::Late_Update(_float fTimeDelta)
{
	m_pCollider->Update_Collider();
	__super::Late_Update(fTimeDelta);
}

HRESULT CTtakkeun_i::Render()
{
	Animate_Monster();
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}

HRESULT CTtakkeun_i::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTtakkeun_i::Ready_Textures()
{

	/* WALK */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Walk_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
   		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
	 		_wstring(TEXT("Com_Texture")) + L"_Boss_Walk_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_WALK][i])))))
       			return E_FAIL;
	}

	/* FLY */
	//for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	//{
	//	_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Fly_";
	//	_uint num = static_cast<_uint>(i * m_fDivOffset);
	//	_tchar buf[32];
	//	_itow_s((int)num, buf, 10);
	//	sPrototypeTag += buf;
	//	if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
	//		_wstring(TEXT("Com_Texture")) + L"_Boss_Fly_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_FLY][i])))))
	//		return E_FAIL;
	//}

	/* FLY_ATTACK */
	//for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	//{
	//	_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Fly_Attack_";
	//	_uint num = static_cast<_uint>(i * m_fDivOffset);
	//	_tchar buf[32];
	//	_itow_s((int)num, buf, 10);
	//	sPrototypeTag += buf;
	//	if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
	//		_wstring(TEXT("Com_Texture")) + L"_Boss_Fly_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_FLY_ATTACK][i])))))
	//		return E_FAIL;
	//}

	/* JUMP */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Jump_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Boss_Jump_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_JUMP][i])))))
			return E_FAIL;
	}

	/* LAVA_ATTACK */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Lava_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Boss_Lava_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_LAVA_ATTACK][i])))))
			return E_FAIL;
	}


	//divein은 0만 이미지 존재 ****
	if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_Boss_Lava_DiveIn")),
		_wstring(TEXT("Com_Texture")) + L"_Boss_Lava_DiveIn", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_LAVA_DIVEIN][0])))))
		return E_FAIL;



	//if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

 	return S_OK;
}

//HRESULT CTtakkeun_i::Set_TextureType()
//{
//	if (KEY_DOWN(DIK_1)) // 테스트용
//		int a = 0;
//
//	_float degree = m_fPlayersViewAngle / m_fDivOffset;
//	_float div = 0.f;
//	_float mod = modff(degree, &div);
//
//	m_iDegree = (_uint)div + (_uint)(mod > 0.5f ? 1 : 0); // 기준 각도에서 +- 하는 중
//
//	// state는 보스의 상태에 따라 결정
//	// 임시로 walk만 지정
//	m_iState = (_uint)(STATE_WALK);
//
//	return S_OK;
//}

void CTtakkeun_i::On_Collision(CGameObject* pCollisionedObject, const _wstring& strLayerTag)
{
	//위치탐색
	_float3 vImpactPos = CalculateEffectPos();

	//몬스터 사망
	if (0 >= m_iHP)
	{
		CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 120.f, 160.f, 1.f }, TEXT("Effect_Explor"), 32);
		m_bDead = true;

		return;
	}

	// 이펙트 생성
	m_iHP += -10;
	CFXMgr::Get_Instance()->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
}

CTtakkeun_i* CTtakkeun_i::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//프로토타입의 기본정의
	CTtakkeun_i* pInstance = new CTtakkeun_i(pGraphic_Device);

	//기본 정보
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Ttakkeun_i_Walk");
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