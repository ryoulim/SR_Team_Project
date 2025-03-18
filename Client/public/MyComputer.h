#include "Statue.h"

BEGIN(Client)

class CMyComputer final : public CStatue
{
private:
	CMyComputer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMyComputer(const CMyComputer& Prototype);
	virtual ~CMyComputer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CMyComputer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


