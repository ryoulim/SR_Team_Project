#include "Statue.h"

BEGIN(Client)

class CBuildingH final : public CStatue
{
private:
	CBuildingH(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBuildingH(const CBuildingH& Prototype);
	virtual ~CBuildingH() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CBuildingH* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


