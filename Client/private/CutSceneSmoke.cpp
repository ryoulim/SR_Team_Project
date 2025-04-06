#include "CutSceneSmoke.h"
#include "GameInstance.h"

CCutSceneSmoke::CCutSceneSmoke(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CCutSceneSmoke::CCutSceneSmoke(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CCutSceneSmoke::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCutSceneSmoke::Ready_Particle()
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

HRESULT CCutSceneSmoke::Ready_Components(void* pArg)
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

void CCutSceneSmoke::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	//�߽ɿ��� �������� ���� ��� (0 ~ 2PI)
	float angle = GetRandomFloat(0.0f, D3DX_PI * 2.0f);
	float speed = GetRandomFloat(100.0f, 300.0f); // ���� Ȯ�� �ӵ�

	//��ƼŬ ��ġ = �߽�
	attribute->_Position = m_vPosition;

	//���� �������� ������ �ӵ�
	attribute->_Velocity.x = cos(angle) * speed;
	attribute->_Velocity.y = GetRandomFloat(10.0f, 100.0f); // ��¦ ���� �߰�
	attribute->_Velocity.z = sin(angle) * speed;

	//�߷� (�ణ ������ ��)
	attribute->_Accelerator = _float3(0.0f, -20.0f, 0.0f);

	//���� �� ���� ����
	attribute->_Age = 0.f;
	attribute->_Color = WHITE;
	attribute->_ColorFade = WHITE;
	attribute->_LifeTime = GetRandomFloat(0.5f, 1.5f);

}

EVENT CCutSceneSmoke::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//������ ��ƼŬ�� �����Ѵ�.
		if (i->_isAlive)
		{
			/* [ ��ƼŬ ������ �ִϸ��̼� ���� ] */
			FrameUpdateAge(timeDelta, i->_Animation, i->_Age);

			i->_Velocity += i->_Accelerator * timeDelta;
			i->_Position += i->_Velocity * timeDelta;

			i->_Age += timeDelta;
			if (i->_Age > i->_LifeTime)
			{
				i->_isAlive = false;
				//resetParticle(&(*i));
			}
		}
	}

	removeDeadParticle();
	if (m_Particles.empty())
	{
		return EVN_DEAD;
	}

	return EVN_NONE;
}

HRESULT CCutSceneSmoke::Render()
{
	if (!m_Particles.empty())
	{
		//���� ���¸� �����Ѵ�.
		SetUp_RenderState();

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
		m_pShaderCom->Begin(CShader::SMOKE);
		list<Attribute>::iterator i;
		for (i = m_Particles.begin(); i != m_Particles.end(); i++)
		{
			/* [ ��ƼŬ ������ �ִϸ��̼� ���� ] */
			m_pTextureCom->Bind_Resource((_uint)i->_Animation);

			D3DXVECTOR4 texelSize(1.0f / 64.f, 1.0f / 64.f, 0.0f, 0.0f);
			m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "g_Texture", (_uint)i->_Animation);
			m_pShaderCom->SetVector("g_vTexelSize", &texelSize);

			//�� ����
			float fBlurAmount = 1.f;
			m_pShaderCom->SetFloat("g_fBlurAmount", fBlurAmount);

			//���̿� ���� ����
			float age = i->_Age;
			float maxAge = i->_LifeTime;
			float fLifeRatio = 1.0f - (age / maxAge);
			m_pShaderCom->SetFloat("g_fLifeRatio", fLifeRatio);

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

	m_pShaderCom->End();
	return S_OK;

}


CCutSceneSmoke* CCutSceneSmoke::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CCutSceneSmoke* pInstance = new CCutSceneSmoke(pGraphicDev, _strObjName);

	//pInstance->m_vPosition = _Origin;
	pInstance->m_vbSize = 2048;				//  GPU�� �ѹ��� �׸� �� �ִ� ��ƼŬ ����, CPU�� GPU�� ��ƼŬ ���� ���ۿ� ���� �� �ִ� ����
	pInstance->m_fSize = 5.f;				//  ��ƼŬ�� ũ��
	pInstance->m_vbOffset = 0;				//  ���׸�Ʈ�� ��ġ����� �ű涧 ���� ������(0����)
	pInstance->m_vbBatchSize = 1;			//  ���׸�Ʈ ��ġ������ ũ��(�ѹ��� �ű� �� �ִ� �������� ����)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  �ٿ���ڽ��� �ּ�ũ��
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  �ٿ���ڽ��� �ִ�ũ��

	return pInstance;
}

float CCutSceneSmoke::GetRandomColor(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void CCutSceneSmoke::SetPosition(_float3 Position)
{
	m_vPosition = Position;
}

CGameObject* CCutSceneSmoke::Clone(void* pArg)
{
	CCutSceneSmoke* pInstance = new CCutSceneSmoke(*this);
	pInstance->m_isClone = true;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCutSceneSmoke");
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCutSceneSmoke::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
