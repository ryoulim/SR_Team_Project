#include "Statue.h"

BEGIN(Client)

class CForest final : public CStatue
{
private:
	CForest(LPDIRECT3DDEVICE9 pGraphic_Device);
	CForest(const CForest& Prototype);
	virtual ~CForest() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CForest* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


