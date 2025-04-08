// �� Ŭ���� �̸� : PlayerMissile
// �θ� Ŭ���� �̸� : Bullet

#pragma once
#include "Bullet.h"

BEGIN(Client)

class CPlayerMissile final : public CBullet
{
public:
	typedef struct tagPlayerMissileDesc : public CBullet::DESC
	{
		_float3 vLook;
	}DESC;
private:
	CPlayerMissile(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerMissile(const CPlayerMissile& Prototype);
	virtual ~CPlayerMissile() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*		m_pShaderCom{ nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg);

private:
	/* �ܻ��� �����غ���.. */
	typedef struct TrailData {
		CTransform* pTransform;
		_float fRandomColor;
	}TRAILDATA;

	vector<TRAILDATA>  m_TrailDatas;
	_float			 m_fTrailDuration = 0.5f; // �ܻ��� ������� �ð�	
	_float			 m_fTrailTimer = {};	  // �ܻ� ���� ������
	_float			 m_fTimeAcc{};

	void Make_TrailData(_float InitScale);
	void Update_TrailData(_float dt);
	void Render_TrailData();
	void Trail_Billboard(_float4x4& matWorld) const;

public:
	static CPlayerMissile* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END