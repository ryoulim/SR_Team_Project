#include "PBState_Decel.h"
#include "PBState_Normal.h"
#include "PBState_Accel.h"

CPBState_Decel::CPBState_Decel()
{
}

void CPBState_Decel::Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
	if (0.f != pPlayerOnBoat->GetSlidingSpeed())
		m_fDecelSpeed = pPlayerOnBoat->GetSlidingSpeed();

	else
		m_fDecelSpeed = fTimeDelta * 5.f;
}

void CPBState_Decel::Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
	m_fPosZ = pPlayerOnBoat->Compute_CurPosZ();

	m_fDecelSpeed *= 0.98f;

	pPlayerOnBoat->Go_Straight(m_fDecelSpeed);

	if(m_fPosZ > -300.f)
		Exit(pPlayerOnBoat);
}

void CPBState_Decel::Exit(CPlayerOnBoat* pPlayerOnBoat)
{
	m_fPosZ = 0.f;
	m_fDecelSpeed = 0.f;

	pPlayerOnBoat->Set_State(CPlayerOnBoat::NORMAL);
}

void CPBState_Decel::Free()
{
	__super::Free();
}
