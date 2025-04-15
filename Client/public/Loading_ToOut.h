// 내 클래스 이름 : Loading_ToOut
// 부모 클래스 이름 : LoadingCutscene

#pragma once
#include "LoadingCutscene.h"

BEGIN(Client)

class CLoading_ToOut final : public CLoadingCutscene
{
private:
	CLoading_ToOut(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoading_ToOut(const CLoading_ToOut& Prototype);
	virtual ~CLoading_ToOut() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Background();
	HRESULT Render_FirstCutscene();
	HRESULT Render_SecondCutscene();
	void	Set_CutPosition(_float fTimeDelta);

	
private:
	typedef struct tagSizePos
	{
		_float3 vSize = {};
		_float3 vPos = {};
	}SIZE_POS;
	array<SIZE_POS, 6> m_arrTexSizePos;
	array<_float3, 6> m_arrTexLastPos;
	_bool	m_isFirstCutscene{ true };
	_uint	m_iCurCut{ 0 };
	_float  m_fAnimationTime{ 0.f };
	_int	iUIX = _int(g_iWinSizeX) / -2;
	_int	iUIY = _int(g_iWinSizeY) / 2;
	_float  m_fWaitForNextCut{ 0.f };
	_bool   m_bRenderOk[6] = {};
	_float  m_fWindVolume{ 0.f };
	_float  m_fWindOutVolume{ 0.f };

public:
	static CLoading_ToOut* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END