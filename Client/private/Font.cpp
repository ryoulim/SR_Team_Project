// �� Ŭ���� �̸� : Font
// �θ� Ŭ���� �̸� : UI

#include "Font.h"
#include "GameInstance.h"

CFont::CFont(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CFont::CFont(const CFont& Prototype)
	: CUI(Prototype)
{
}

HRESULT CFont::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFont::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	//m_szTextureID = TEXT("Font");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CFont::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFont::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CFont::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFont::Render()
{

	if (FAILED(Bind_Texture_To_Transform()))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
	//	return E_FAIL;

	//if (FAILED(m_pTextureCom->Get_TextureSize(static_cast<_uint>(m_fTextureNum), &m_vSize)))
	//	return E_FAIL;

	//m_pTransformCom->Scaling(m_vSize);

	//if (FAILED(m_pTransformCom->Bind_Resource()))
	//	return E_FAIL;



	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFont::Render_Text(const string& _text,FONTALIGN _align, _float _posX, _float _posY)
{
	m_uiTextWidth = static_cast<_uint>(_text.length());
	
	_float startPosX = {};
	if (_align == LEFT)
		startPosX = _posX;
	else if (_align == CENTER)
		startPosX = _posX - m_uiTextWidth / 2.f * 14.f;


	if (m_eFontType != MEDIUMBLUE)
	{
		for (auto ch : _text)
		{
			/* �ٸ� ��Ʈ �߰� �ʿ� */
		}
	}
	else
	{
		_float fontWidth{};
		for (auto ch : _text)
		{
			if (ch != L' ')
			{
				m_fTextureNum = static_cast<_float>(toascii(ch) - 33);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(startPosX + fontWidth, _posY, 0.f));
				                                 
				/* Scale �ʱ�ȭ �Լ� �ʿ�? */
				_float3 vRight{}, vLook{}, vUp{};
				D3DXVec3Normalize(&vRight, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
				D3DXVec3Normalize(&vLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
				D3DXVec3Normalize(&vUp, m_pTransformCom->Get_State(CTransform::STATE_UP));
				m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
				m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
				m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);

				// SetSamplerState ��� �ϴ��� Ȯ�� .. 
				m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
				m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
				//m_pTransformCom->Scaling(_float3(12.f, 15.f, 1.f));
				Render();
				fontWidth += m_vSize.x;
			}
			else
				fontWidth += 10.f;
		}
	}
	return S_OK;
}

HRESULT CFont::Render_Text(const _int _val, FONTALIGN _align, _float _posX, _float _posY)
{
	_int val = _val;
	m_uiTextWidth = 0;
	if (val != 0)
	{
		while (_val % 10)
		{
			m_uiTextWidth++;
			val /= 10;
		}
	}
	else
		m_uiTextWidth = 1;

	_float startPosX = {};
	if (_align == LEFT)
		startPosX = _posX;
	else if (_align == CENTER)
		startPosX = _posX - m_uiTextWidth / 2.f * 14.f;


	if (m_eFontType != MEDIUMBLUE)
	{
		for (size_t i = 0; i < m_uiTextWidth; i++)
		{
			/* �ٸ� ��Ʈ �߰� �ʿ� */
		}
	}
	else
	{
		_float fontWidth{};
		for (size_t i = 0; i < m_uiTextWidth; i++)
		{
			m_fTextureNum = static_cast<_float>(val % 10);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(startPosX + fontWidth, _posY, 1.f));

			/* Scale �ʱ�ȭ �Լ� �ʿ�? */
			_float3 vRight{}, vLook{}, vUp{};
			D3DXVec3Normalize(&vRight, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
			D3DXVec3Normalize(&vLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			D3DXVec3Normalize(&vUp, m_pTransformCom->Get_State(CTransform::STATE_UP));
			m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
			m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);

			//m_pTransformCom->Scaling(_float3(12.f, 15.f, 1.f));
			Render();
			val /= 10;
			fontWidth += m_vSize.x;
		}
	}
	return S_OK;
}

HRESULT CFont::Bind_Texture_To_Transform()
{
	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Get_TextureSize(static_cast<_uint>(m_fTextureNum), &m_vSize)))
		return E_FAIL;

	m_pTransformCom->Scaling(m_vSize);

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	return S_OK;
}


//CFont* CFont::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
//{
//	CFont* pInstance = new CFont(pGraphic_Device);
//
//	if (FAILED(pInstance->Initialize_Prototype()))
//	{
//		MSG_BOX("Failed to Created : CFont");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

//CGameObject* CFont::Clone(void* pArg)
//{
//	CFont* pInstance = new CFont(*this);
//
//	if (FAILED(pInstance->Initialize(pArg)))
//	{
//		MSG_BOX("Failed to Clone : CFont");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

void CFont::Free()
{
	__super::Free();

}