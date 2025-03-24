#include "Statue.h"

BEGIN(Client)

class CBuildingW final : public CStatue
{
private:
	CBuildingW(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBuildingW(const CBuildingW& Prototype);
	virtual ~CBuildingW() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CBuildingW* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


