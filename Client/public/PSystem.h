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
protected:
	CPSystem() = delete;
	CPSystem(LPDIRECT3DDEVICE9 pGraphic_Device) = delete;


	CPSystem(LPDIRECT3DDEVICE9 pGraphic_Device, wstring _strObjName);
	CPSystem(const CPSystem& Prototype);
	virtual ~CPSystem() = default;


public:	//이니셜라이즈
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;


public:
	virtual EVENT	Update(_float fTimeDelta);
	virtual void	Late_Update();


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

protected: //청소
	virtual void removeDeadParticle();




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

	//파티클 크기, 초당 개수
	_float					m_fSize;
	_float					m_fEmitRate;
	
	LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;

	DWORD					m_vbSize;
	DWORD					m_vbOffset;
	DWORD					m_vbBatchSize;

	D3DXVECTOR3				m_vMin;
	D3DXVECTOR3				m_vMax;

	////////////////////////////////////////////////////////

};

