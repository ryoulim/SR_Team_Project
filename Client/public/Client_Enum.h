#pragma once


namespace Client
{
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_RACE, LEVEL_TEST, LEVEL_END };
	enum COLLIDER_GROUP { CG_PAWN, CG_MONSTER, CG_BLOCK, CG_PBULLET, CG_MBULLET, CG_END };

	enum COLLIDER_ID {
		CI_PLAYER_START,																			// �÷��̾�
		CI_PLAYER, 
		CI_PLAYER_END,		

		CI_MONBODY_START,																			// ���� ����
		CI_MON_BODY, 
		CI_MONBODY_END,																				

		CI_MONHEAD_START,																			// ���� �Ӹ�(����)
		CI_MON_HEAD, 
		CI_MONHEAD_END,			

		CI_WEAPON_START,																			// �÷��̾� ����
		CI_LOVERBOY, CI_CHAINGUN, CI_DISPENSOR_SHELL, CI_DISPENSOR_GRENADE,							
		CI_WEAPON_END,

		CI_BLOCK_START,																				// ����
		CI_BLOCK_COMMON, CI_BLOCK_INVISIBLE,														
		CI_BLOCK_END,

		CI_MBULLET_START,																			// ���� ����ü
		CI_MONSTER_BULLET,
		CI_MBULLET_END
	};

#define CI_PLAYER( ID )		(CI_PLAYER_START < ID && CI_PLAYER_END > ID )
#define CI_MONBODY( ID )	(CI_MONBODY_START < ID && CI_MONBODY_END > ID )
#define CI_MONHEAD( ID )	(CI_MONHEAD_START < ID && CI_MONHEAD_END > ID )
#define CI_WEAPON( ID )		(CI_WEAPON_START < ID && CI_WEAPON_END > ID )
#define CI_BLOCK( ID )		(CI_BLOCK_START < ID && CI_BLOCK_END > ID )
#define CI_MBULLET( ID )	(CI_MBULLET_START < ID && CI_MBULLET_END > ID )
}
