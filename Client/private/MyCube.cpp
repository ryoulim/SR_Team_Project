// 내 클래스 이름 : MyCube
// 부모 클래스 이름 : Statue

#include "MyCube.h"
#include "GameInstance.h"

CMyCube::CMyCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue{ pGraphic_Device }
{
}

CMyCube::CMyCube(const CMyCube& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CMyCube::Reset(void* pArg)
{
	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vInitPos);
		m_pTransformCom->LookAt(pDesc->vInitPos+pDesc->vLook);
		m_pTransformCom->Scaling(pDesc->vScale);
	}

	return m_pTransformCom->Initialize(pArg);
}

HRESULT CMyCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMyCube::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("MyCube");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	//if (pArg != nullptr)
	//{
	//	DESC* pDesc = static_cast<DESC*>(pArg);
	//	m_pTransformCom->LookAt(pDesc->vLook);
	//}

	return S_OK;
}

void CMyCube::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMyCube::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);

	//m_fTimeAcc += fTimeDelta;
	//if (m_fTimeAcc > 3.f)
	//{
	//	m_fTimeAcc = 0.f;
	//	m_pGameInstance->Deactive_Object(TEXT("ObjectPool_MyCube"), this);
	//	return EVN_OFF;
	//}

	return __super::Update(fTimeDelta);
}

void CMyCube::Late_Update(_float fTimeDelta)
{
	m_pCollider->Update_Collider(m_pTransformCom);
	__super::Late_Update(fTimeDelta);
}

HRESULT CMyCube::Render()
{
	LightOn();

	return __super::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CMyCube::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_AABB_Cube::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = pDesc->vScale;

	/* For.Com_Collider */			
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CMyCube::LightOn()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

#pragma region 조명 설치

#pragma region 조명의 속성 정의
	D3DLIGHT9 LightDesc{};

	// 광원의 타입 : 점 광원
	LightDesc.Type = D3DLIGHT_POINT;

	// 광원의 위치
	LightDesc.Position = _float3(250.f, 70.f, 150.f);

	// 조명이 미치는 범위
	LightDesc.Range = 100.f;

	// 광원으로부터 멀어질 때의 감쇄값
	LightDesc.Attenuation1 = 0.00000001f;

	// 기본 설정
	LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
#pragma endregion

	// 0번 광원을 설치한다.
	m_pGraphic_Device->SetLight(0, &LightDesc);

#pragma region 재질 설정
	D3DMATERIAL9		MtrlDesc{};
	MtrlDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlDesc.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
#pragma endregion

#pragma endregion

	// 장치에 재질을 설정한다
	m_pGraphic_Device->SetMaterial(&MtrlDesc);

	// 0번 광원을 켠다
	m_pGraphic_Device->LightEnable(0, true);
}

CMyCube* CMyCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMyCube* pInstance = new CMyCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMyCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMyCube::Clone(void* pArg)
{
	CMyCube* pInstance = new CMyCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMyCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMyCube::Free()
{
	__super::Free();
	Safe_Release(m_pCollider);
}