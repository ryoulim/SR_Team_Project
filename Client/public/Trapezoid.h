#include "Statue.h"

BEGIN(Client)

class CTrapezoid final : public CStatue
{
private:
	CTrapezoid(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrapezoid(const CTrapezoid& Prototype);
	virtual ~CTrapezoid() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture* m_pTextureComTz = { nullptr };

private:
	HRESULT Render_Rect();

public:
	static CTrapezoid* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


