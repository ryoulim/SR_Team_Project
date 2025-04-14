#pragma once

#include "Client_Defines.h"
#include "Particle_Define.h"
#include "GameObject.h"
#include <list>

BEGIN(Engine)
class CTexture;
class CTransform;
class CShader;
END

class CPSystem : public CGameObject
{
public:
	typedef struct tagPSystemDesc
	{
		const TCHAR* szTextureTag;
		_float	fLifeTime;
		_float3 vPosition;
		_float	fMaxFrame;
		_float	fSize;
		_float	fMin = 0;
		_float	fMax = 0;
		_float	fNum = 0;
		_int	iNum = 0;
		_int	iParticleNums;
		_bool	isLoop = true;
	}DESC;


protected:
	CPSystem() = delete;
	CPSystem(LPDIRECT3DDEVICE9 pGraphic_Device) = delete;


	CPSystem(LPDIRECT3DDEVICE9 pGraphic_Device, wstring _strObjName);
	CPSystem(const CPSystem& Prototype);
	virtual ~CPSystem() = default;


public:	//이니셜라이즈
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;

	void FrameUpdateAge(float timeDelta, float& fAnimation, float& age);
	void FrameUpdate(_float timeDelta, _float _MaxFrame, _float fSpeed, _bool isLoop);


public:
	virtual EVENT	Update(_float fTimeDelta);
	virtual void	Late_Update(_float fTimeDelta)override;


public: //렌더링
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Render();
	virtual HRESULT Release_RenderState();
	


public: //파티클
	virtual void reset();
	virtual void resetParticle(Attribute* attribute) {}
	virtual void addParticle();


public: //준비
	virtual HRESULT Ready_Particle();


public: //생존
	bool isEmpty();
	bool isDead();

public: //청소
	virtual void removeDeadParticle();
	void SetDead() { m_bDead = true; }



public: //제거
	virtual void Free();
		



protected: //멤버변수

	_ulong							m_dwFVF;
	_ulong							m_dwVtxSize;

	///////////////////////////////////////////////////////

	CTexture*				m_pTextureCom = { nullptr };
	CTransform*				m_pTransForm = { nullptr };
	CShader*				m_pShaderCom = { nullptr };

	///////////////////////////////////////////////////////

	D3DXVECTOR3				m_vPosition = {0.f, 10.f, 0.f};
	list<Attribute>			m_Particles;
	
	//최대 파티클 수
	_int					m_iMaxParticle = 10000;
	_int					m_iNum = 0;
	_float					m_fNum = 0;

	//파티클 크기, 초당 개수
	_float					m_fSize = 1.f;
	_float					m_fEmitRate;
	_float					m_fLifeTime = 0.f;
	_float					m_fAccTime = 0.f;
	
	LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;

	DWORD					m_vbSize;
	DWORD					m_vbOffset;
	DWORD					m_vbBatchSize;

	D3DXVECTOR3				m_vMin;
	D3DXVECTOR3				m_vMax;

	////////////////////////////////////////////////////////
	bool					m_isClone = false;

	_float					m_fMin = 0;
	_float					m_fMax = 0;

	_bool					m_bIsLoop = true;
	_bool					m_bDead = false;

protected: // 애니메이션
	_float	m_fAnimationFrame = 0;
	_float	m_fAnimationMaxFrame = 0;
	_float	m_fAnimationSpeed = 0;

};

