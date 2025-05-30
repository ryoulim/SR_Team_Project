// 내 클래스 이름 : Weapon_LoverBoy
// 부모 클래스 이름 : Weapon

// 텍스쳐 순서 : 
// 기본 : 0 ~ 6
// 재장전 : 7 ~ 12
// 총 내려가는거 뭐드라 아마 12나 13일듯
// 종 올라오는거 : 13~14
#pragma once
#include "Weapon.h"

BEGIN(Client)

class CWeapon_LoverBoy final : public CWeapon
{
private:
	CWeapon_LoverBoy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWeapon_LoverBoy(const CWeapon_LoverBoy& Prototype);
	virtual ~CWeapon_LoverBoy() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Set_State(STATE State) override;

private:
	_bool						m_bDoOnce{};
	// 석양이진다 용 몬스터 골라내는 맵
	multimap<_float, CCollider*> m_TargetMonsters;
	multimap<_float, CCollider*>::iterator m_CurTarget{ m_TargetMonsters.end() };

private:
	virtual void Opening(_float fTimeDelta) override;
	virtual void Weak_Attack(_float fTimeDelta) override;
	virtual void Strong_Attack(_float fTimeDelta) override;
	virtual void Reload(_float fTimeDelta) override;
	virtual void Ending(_float fTimeDelta) override;

	void Search_Target();
	void Left_Hand_Render();
	virtual HRESULT Ready_Components(void* pArg);

	struct LEFTHAND
	{
		CTexture* pTextureCom = { nullptr };
		CVIBuffer* pVIBufferCom = { nullptr };
		CTransform* pTransformCom = { nullptr };
	} m_LeftHand;

	struct TargetInfo
	{
		_float fDistance;
		CCollider* pCollider;

		bool operator<(const TargetInfo& other) const
		{
			return fDistance < other.fDistance; // 반대 정렬: 큰 값 우선 -> 최대 힙
		}
	};

public:
	static CWeapon_LoverBoy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END