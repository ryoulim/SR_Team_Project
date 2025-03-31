#include "Statue.h"

BEGIN(Client)

class CRock final : public CStatue
{
private:
	CRock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRock(const CRock& Prototype);
	virtual ~CRock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CRock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


