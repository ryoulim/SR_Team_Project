#include "Statue.h"

BEGIN(Client)

class CCanopy final : public CStatue
{
private:
	CCanopy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCanopy(const CCanopy& Prototype);
	virtual ~CCanopy() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg)override;

private:
	CCollider* m_pSubColliderCom[2] = { nullptr };

public:
	static CCanopy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


