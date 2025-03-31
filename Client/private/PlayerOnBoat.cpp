#include "PlayerOnBoat.h"
#include "CameraManager.h"

CPlayerOnBoat::CPlayerOnBoat(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPawn { pGraphic_Device }
{
}

CPlayerOnBoat::CPlayerOnBoat(const CPlayerOnBoat& Prototype)
	: CPawn (Prototype)
{
}

HRESULT CPlayerOnBoat::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerOnBoat::Initialize(void* pArg)
{
	m_szTextureID = TEXT("PlayerOnBoat");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//TPSī�޶�� Layer_Camera�� 3��°�� �ֽ��ϴ�.
	CGameObject* FPS_Camera = m_pGameInstance->Find_Object(LEVEL_STATIC, TEXT("Layer_Camera"), 2);
	m_pCameraTransform = static_cast<CTransform*>(FPS_Camera->Find_Component(TEXT("Com_Transform")));
	Safe_AddRef(m_pCameraTransform);

	return S_OK;
}

void CPlayerOnBoat::Priority_Update(_float fTimeDelta)
{
	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z > 10000.f
		|| m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z < 100.f)
	{
		m_pTransformCom->Go_Straight(fTimeDelta  * 5.f);

		//�������״� �ð� �� �ʱ�ȭ
		if (m_fTime != 0.f)
			m_fTime = 0.f;

		//�ʱ�ȭ
		if (m_fStartPosX != 0.f)
			m_fStartPosX = 0.f;
	}

	else if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z > 9000.f)
	{
		//���������� ���� �������� x�� ����, �������� z���� 9000.f�� �����ǹǷ� ������ �ʿ� ����
		if (m_fStartPosX == 0.f)
		{
			m_fStartPosX = m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x;
		}
		
		//�ð��� �����ؼ� ����
		m_fTime += fTimeDelta * 0.7f;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			{ LerpToHole(m_fStartPosX, 450.f, m_fTime),
			17.f,
			LerpToHole(9000.f, 10000.f, m_fTime) });
	}
	
	else
	{
		Key_Input(fTimeDelta);
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	
	__super::Priority_Update(fTimeDelta);
}

EVENT CPlayerOnBoat::Update(_float fTimeDelta)
{
	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x > 600.f)
	{
		
	}

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x < 300.f)
	{

	}

	Update_Camera_Link();

	return __super::Update(fTimeDelta);
}

void CPlayerOnBoat::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayerOnBoat::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return __super::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CPlayerOnBoat::Key_Input(_float fTimeDelta)
{
	if (KEY_PRESSING(DIK_LEFT))
	{
		//m_pGravityCom->Go_Left_On_Terrain(fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_RIGHT))
	{
		//m_pGravityCom->Go_Right_On_Terrain(fTimeDelta);
		m_pTransformCom->Go_Right(fTimeDelta);
	}
}

void CPlayerOnBoat::Update_Camera_Link()
{	
	//�÷��̾� �����ϰ��� �����´�
	_float3 Scale = m_pCameraTransform->Compute_Scaled();

	//ī�޶��� ��ġ�� (�÷��̾� ��ġ + @)
	m_pCameraTransform->Set_State(CTransform::STATE_POSITION, *m_pTransformCom->Get_State(CTransform::STATE_POSITION) 
		+ _float3(0.f, 20.f, -80.f));// -20 50
}

_float CPlayerOnBoat::LerpToHole(_float StartPos, _float TargetPos, _float fTimeDelta)
{
	return (TargetPos - StartPos) * fTimeDelta + StartPos;
}

CPlayerOnBoat* CPlayerOnBoat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayerOnBoat* pInstance = new CPlayerOnBoat(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayerOnBoat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerOnBoat::Clone(void* pArg)
{
	CPlayerOnBoat* pInstance = new CPlayerOnBoat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CPlayerOnBoat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerOnBoat::Free()
{
	__super::Free();

	Safe_Release(m_pCameraManager);
	Safe_Release(m_pCameraTransform);
}
