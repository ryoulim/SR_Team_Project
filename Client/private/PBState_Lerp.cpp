#include "PBState_Lerp.h"
#include "PBState_Accel.h"

CPBState_Lerp::CPBState_Lerp()
{
}

void CPBState_Lerp::Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
	m_fStartPosX = pPlayerOnBoat->Compute_StartPosX();
}

void CPBState_Lerp::Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
	m_fPosZ = pPlayerOnBoat->Compute_CurPosZ();

	m_fTime += fTimeDelta * 1.2f;

	pPlayerOnBoat->Move_Lerp(m_fStartPosX, m_fTime);

	if (m_fPosZ > 9800.f)
		Exit(pPlayerOnBoat);
}

void CPBState_Lerp::Exit(CPlayerOnBoat* pPlayerOnBoat)
{
	m_fPosZ = 0.f;
	m_fStartPosX = 0.f;
	m_fTime = 0.f;
	
	pPlayerOnBoat->Set_State(CPlayerOnBoat::ACCEL);
}

void CPBState_Lerp::Free()
{
	__super::Free();
}
