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


public:	//�̴ϼȶ�����
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;


public:
	virtual EVENT	Update(_float fTimeDelta);
	virtual void	Late_Update();


public: //������
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Render();
	virtual HRESULT Release_RenderState();
	


public: //��ƼŬ
	virtual void reset();
	virtual void resetParticle(Attribute* attribute) {}
	virtual void addParticle();


public: //�غ�
	virtual HRESULT Ready_Particle();


public: //����
	bool isEmpty();
	bool isDead();

protected: //û��
	virtual void removeDeadParticle();




public: //����
	virtual void Free();
		



protected: //�������

	_ulong							m_dwFVF;
	_ulong							m_dwVtxSize;

	///////////////////////////////////////////////////////

	CTexture*				m_pTextureCom = { nullptr };
	CTransform*				m_pTransForm = { nullptr };
	CShader*				m_pShaderCom = { nullptr };

	///////////////////////////////////////////////////////

	D3DXVECTOR3				m_vPosition = {0.f, 10.f, 0.f};
	list<Attribute>			m_Particles;
	
	//�ִ� ��ƼŬ ��
	_int					m_iMaxParticle = 10000;

	//��ƼŬ ũ��, �ʴ� ����
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

