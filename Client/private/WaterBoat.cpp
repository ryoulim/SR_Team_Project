#include "WaterBoat.h"
#include "GameInstance.h"


CWaterBoat::CWaterBoat(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CWaterBoat::CWaterBoat(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CWaterBoat::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fFrame = GetRandomFloat(0.f, m_fAnimationMaxFrame);

	if (m_fMin == 0 && m_fMax == 0)
	{
		m_fMin = -120;
		m_fMax = 120;
	}

	return S_OK;
}

HRESULT CWaterBoat::Ready_Particle()
{

	m_dwFVF = Particle::FVF;
	m_dwVtxSize = sizeof(Particle);


	HRESULT hr;
	hr = m_pGraphic_Device->CreateVertexBuffer(
		m_vbSize * m_dwVtxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0
	);

	return S_OK;
}

HRESULT CWaterBoat::Ready_Components(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	//Ʈ������ ������Ʈ ����
	CTransform::DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransForm), &TransformDesc)))
		return E_FAIL;

	//�ؽ�ó ������Ʈ ����
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + pDesc->szTextureTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	//���̴� ����
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CWaterBoat::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	// 1. ��ġ: �� ��ó (ĳ���� �߾� ���� �¿�� ��¦ ������)
	int sideOffset = (rand() % 2 == 0) ? -1 : 1;
	attribute->_Position = m_vPosition;
	attribute->_Position.x += float(sideOffset * 8);
	attribute->_Position.y += -10.f;
	attribute->_Position.z += +10.f;

	// 2. ����: �¿�� �������� ������ ȿ��
	float xSpeed = (sideOffset >= 0)
		? GetRandomFloat(10.f, m_fMax) : GetRandomFloat(m_fMin, -10.f);

	attribute->_Velocity = { xSpeed, GetRandomFloat(10.f, 20.f), GetRandomFloat(m_fNum - 200, m_fNum)};

	// 3. �߷�: ���� ���ϰ� �Ʒ��� ���� (Ȥ�� ����)
	attribute->_Accelerator = { 0.0f, -60.0f, 0.0f };

	// 4. ���־�
	attribute->_Size = GetRandomFloat(5.0f, 8.0f);  // ����� ũ��

	// 5. ����
	attribute->_Age = 0.0f;
	attribute->_LifeTime = GetRandomFloat(0.05f, m_fLifeTime);
}

void CWaterBoat::SetPosition(_float3 Position)
{
	m_vPosition = Position;
}

EVENT CWaterBoat::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//������ ��ƼŬ�� �����Ѵ�.
		if (i->_isAlive)
		{
			i->_Velocity += i->_Accelerator * timeDelta;
			i->_Position += i->_Velocity * timeDelta;


			i->_Age += timeDelta;

			if (i->_Age > i->_LifeTime)
			{
				resetParticle(&(*i));
			}
		}
	}

	if (m_bDead)
	{
		int a = 0;
		return EVN_DEAD;
	}

	return EVN_NONE;
}

HRESULT CWaterBoat::SetUp_RenderState()
{
	//�⺻ ����
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_fSize));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.1f));
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//������ : ���̽� �� ���� ������ �־����ϴ�.
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�Ÿ��� ���� ��ƼŬ ũ������
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(1.0f));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.05f));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(0.005f));



#pragma region �ؽ�ó�� ����(��/����)
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
#pragma endregion


	//���� ��ȯ
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

	return S_OK;
}
HRESULT CWaterBoat::Render()
{
	//���� ���¸� �����Ѵ�.
	SetUp_RenderState();
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	if (!m_Particles.empty())
	{
		//m_pTextureCom->Bind_Resource(0);
		m_pGraphic_Device->SetTexture(0, nullptr);
		m_pGraphic_Device->SetFVF(Particle::FVF);
		m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(Particle));

		//���ؽ� ���۸� ��� ��� ó������ �����Ѵ�.
		if (m_vbOffset >= m_vbSize)
			m_vbOffset = 0;

		Particle* v = 0;

		m_pVB->Lock(
			m_vbOffset * sizeof(Particle),
			m_vbBatchSize * sizeof(Particle),
			(void**)&v,
			m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);


		DWORD numParticlesInBatch = 0;

		//��� ��ƼŬ�� ������ �� ����

		list<Attribute>::iterator i;
		for (i = m_Particles.begin(); i != m_Particles.end(); i++)
		{
			i->_Color = D3DXCOLOR(
				GetRandomFloat(10.f, 100.f) / 255.f,   // R:  ���� ���ϰų�, û�Ͽ� ����� ����
				GetRandomFloat(100.f, 200.f) / 255.f,  // G:  û��~Ǫ�� �ʷ�
				GetRandomFloat(180.f, 255.f) / 255.f,  // B:  �Ķ� ���� ����
				1.f
			);


			if (i->_isAlive)
			{
				//�� �ܰ��� ���׸�Ʈ���� ������ ��ƼŬ�� ���� ���ؽ� ���� ���׸�Ʈ�� �����Ѵ�.
				v->_Position = i->_Position;
				v->_Color = (D3DCOLOR)i->_Color;
				v->_Size = i->_Size;
				v++;
				numParticlesInBatch++;

				//���� �ܰ��� ���ؽ����۰� ��� ä�����ִ°�?
				if (numParticlesInBatch == m_vbBatchSize)
				{
					//���ؽ� ���۷� ����� ������ �ܰ��� ��ƼŬ�� �׸���.
					m_pVB->Unlock();
					
					m_pGraphic_Device->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, m_vbBatchSize);

					//�ܰ谡 �׷����� ���� ���� �ܰ踦 ��ƼŬ�� ä���.
					m_vbOffset += m_vbBatchSize;

					//���ؽ� ������ ��踦 �Ѵ� �޸𸮷� ������ ����X , ���� ��� ó������
					if (m_vbOffset >= m_vbSize)
						m_vbOffset = 0;

					m_pVB->Lock(
						m_vbOffset * sizeof(Particle),
						m_vbBatchSize * sizeof(Particle),
						(void**)&v,
						m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; //���� �ܰ踦 ���� ����
				}
			}
		}

		m_pVB->Unlock();

		//������ �ܰ谡 �����������ʴ� ����� ����ó��

		if (numParticlesInBatch)
		{
			m_pGraphic_Device->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, numParticlesInBatch);
		}

		//�������
		m_vbOffset += m_vbBatchSize;

	}

	//��������
	Release_RenderState();

	return S_OK;

}
HRESULT CWaterBoat::Release_RenderState()
{

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���� ��带 �⺻������ ���� (SRCALPHA �� �⺻���� SRC)
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	return S_OK;
}

CWaterBoat* CWaterBoat::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CWaterBoat* pInstance = new CWaterBoat(pGraphicDev, _strObjName);

	//��ƼŬ ����
	pInstance->m_vbSize = 100;				//  GPU�� �ѹ��� �׸� �� �ִ� ��ƼŬ ����, CPU�� GPU�� ��ƼŬ ���� ���ۿ� ���� �� �ִ� ����
	pInstance->m_fSize = 10.f;				//  ��ƼŬ�� ũ��
	pInstance->m_vbOffset = 0;				//  ���׸�Ʈ�� ��ġ����� �ű涧 ���� ������(0����)
	pInstance->m_vbBatchSize = 100;			//  ���׸�Ʈ ��ġ������ ũ��(�ѹ��� �ű� �� �ִ� �������� ����)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  �ٿ���ڽ��� �ּ�ũ��
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  �ٿ���ڽ��� �ִ�ũ��

	return pInstance;
}

CGameObject* CWaterBoat::Clone(void* pArg)
{
	CWaterBoat* pInstance = new CWaterBoat(*this);
	pInstance->m_isClone = true;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CWaterBoat");
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWaterBoat::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
}
