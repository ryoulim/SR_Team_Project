// 내 클래스 이름 : Loading_ToBoss
// 부모 클래스 이름 : LoadingCutscene

#pragma once
#include "LoadingCutscene.h"

BEGIN(Client)

class CLoading_ToBoss final : public CLoadingCutscene
{
public:
	enum TEXNUM {
		TEX_SMALLNOISE = 0,
		TEX_SMALLNOISE_END = 3,

		TEX_SMALLBREAKINGNEWS = 4,

		TEX_SMALLNEWS = 5,
		TEX_SMALLNEWS_END = 8,

		TEX_WIDENOISE = 9,
		TEX_WIDENOISE_END = 12,

		TEX_WIDEBOSS = 13,
		TEX_WIDEREDBOSS = 14,

		TEX_BREAK_END = 15
	};

	enum SEQUENCE {
		SEQUENCE_SMALLNOISE = 0,
		SEQUENCE_SMALLBREAKINGNEWS,
		SEQUENCE_SMALLNEWS,
		SEQUENCE_WIDENOISE,
		SEQUENCE_WIDEBOSS,
		SEQUENCE_WIDEREDBOSS,
		SEQUENCE_END
	};



private:
	CLoading_ToBoss(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoading_ToBoss(const CLoading_ToBoss& Prototype);
	virtual ~CLoading_ToBoss() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Background();

private:
	void Init_Sequence();
	void Update_Animation(_float fTimeDelta);

private:
	SEQUENCE	m_eSequence = SEQUENCE::SEQUENCE_SMALLNOISE;
	_int		m_iTexRepeat = { };
	_bool		m_bNextSequence = { false };
	_float		m_fAnimationStartFrame = {};

public:
	static CLoading_ToBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END