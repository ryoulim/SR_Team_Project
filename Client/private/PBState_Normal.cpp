#include "PBState_Normal.h"
#include "PBState_Lerp.h"

CPBState_Normal::CPBState_Normal()
{
}

void CPBState_Normal::Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
	
}

void CPBState_Normal::Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
	m_fPosZ = pPlayerOnBoat->Compute_CurPosZ();

	pPlayerOnBoat->Go_Straight(fTimeDelta);
	pPlayerOnBoat->Key_Input(fTimeDelta);

	if (m_fPosZ > 9000.f)
		Exit(pPlayerOnBoat);
}

void CPBState_Normal::Exit(CPlayerOnBoat* pPlayerOnBoat)
{
	m_fPosZ = 0.f;

	pPlayerOnBoat->Set_State(CPlayerOnBoat::LERP);
}

void CPBState_Normal::Free()
{
	__super::Free();
}
