#include "Statue.h"

BEGIN(Client)

class CStreetLampHead final : public CStatue
{
public:
	typedef struct tagStreeLampDesc : public CStatue::DESC
	{
		_uint iLightNumber;
	}DESC;

private:
	CStreetLampHead(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStreetLampHead(const CStreetLampHead& Prototype);
	virtual ~CStreetLampHead() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Setting_LightDesc();	
	void Turn_on_Light(_uint LightNumber);
	
private:
	D3DLIGHT9 m_LightDesc = {};
	_uint m_iLightNumber = {};
	_bool m_bLightOn = { true };

public:
	static CStreetLampHead* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


