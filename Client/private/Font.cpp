// 내 클래스 이름 : Font
// 부모 클래스 이름 : UI

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

void CFont::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CFont::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFont::Render()
{
	return __super::Render();
}

HRESULT CFont::Render_Text(const wstring& _text, FONTTYPE _type, FONTALIGN _align, _float _posX, _float _posY)
{
	m_uiTextWidth = _text.length();
	//Calc_TextWidth(_text);
	if (_type != MEDIUMBLUE)
	{
		for (auto ch : _text)
		{

		}
	}
	else
	{
		_float temp{};
		for (auto ch : _text)
		{
			if (ch != L' ')
			{
				_float3 vRight{}, vLook{}, vUp{};
				D3DXVec3Normalize(&vRight, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
				D3DXVec3Normalize(&vLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
				D3DXVec3Normalize(&vUp, m_pTransformCom->Get_State(CTransform::STATE_UP));
				m_fTextureNum = toascii(ch) - 33;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(10.f + temp, 10.5f, 1.f));
				m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
				m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
				m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
				
				m_pTransformCom->Scaling(_float3(13.f, 18.f, 1.f));
				Render();
				temp += 13.f;
			}
		}
	}
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