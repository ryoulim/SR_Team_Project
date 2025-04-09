// 내 클래스 이름 : Skull
// 부모 클래스 이름 : UI

#define SkullScale1 50
#define SkullScale2 35
#define SkullOFFSET	40	

#include "Skull.h"

_float4x4	CSkull::m_Orthomat{};

CSkull::CSkull(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CSkull::CSkull(const CSkull& Prototype)
	: CUI(Prototype)
{
}

HRESULT CSkull::Initialize_Prototype()
{
	D3DXMatrixOrthoLH(&m_Orthomat, FWINCX, FWINCY, 0.f, 1.f);
	return S_OK;
}

HRESULT CSkull::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Skull");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaling(SkullScale1);
	m_fDepth = _float(UI_OBJECT_HUD);

	return S_OK;
}

void CSkull::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CSkull::Update(const _float3& vPosition, _float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	_float4x4 view, proj, viewproj;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &view);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &proj);
	viewproj = view * proj;

	_float3 clip{};

	// 월드 → 클립 공간
	D3DXVec3TransformCoord(&clip, &vPosition, &viewproj);

	// 화면 중심 기준 직교 공간 좌표로 변환
	_float3 vOutScreenPos;
	vOutScreenPos.x = clip.x * (FWINCX * 0.5f);
	vOutScreenPos.y = clip.y * (FWINCY * 0.5f) + SkullOFFSET; 
	vOutScreenPos.z = 0.f; // 직교 투영이므로 깊이는 무시

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vOutScreenPos);

	if (m_fTimeAcc < 0.1f)
		m_pTransformCom->Scaling(SkullScale1);
	else
		m_pTransformCom->Scaling(SkullScale2);

	return EVN_NONE;
}

void CSkull::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSkull::Render()
{

	return __super::Render();

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//_float4x4 mat{};
	//m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &mat);
	//m_pGraphic_Device->SetTransform(D3DTS_PROJECTION,&m_Orthomat);
	//	
	//m_pTransformCom->Bind_Resource();

	////if (FAILED(m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard_Y())))
	////	return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_Resource(0)))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Bind_Buffers()))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Render()))
	//	return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &mat);
}

CSkull* CSkull::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkull* pInstance = new CSkull(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkull");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkull::Clone(void* pArg)
{
	CSkull* pInstance = new CSkull(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CSkull");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkull::Free()
{
	__super::Free();
}