#include "PreCompileHeader.h"


//Chain
#define BOSS_OLDCROW_CHAINPIVOTCOUNT 4   //체인 피봇의 개수

#define BOSS_OLDCROW_MEGADASHPATTERNEND 1.0f  //메가 대쉬 패턴 끝나는 시간
#define BOSS_OLDCROW_CHAINCOUNT 4            //패턴에 사용되는 체인 개수
#define BOSS_OLDCROW_CREATECHAINTIME 0.75f      //패턴 내 체인 생성하는 시간
#define BOSS_OLDCROW_INCHAINRENDERCOUNT 100     //체인 렌더러 내 개수

#define BOSS_OLDCROW_CHAINSPEED 7500.0f   //체인 속도

//Dash
#define BOSS_OLDCROW_DASHSPEED 1000.0f
#define BOSS_OLDCROW_DASHSPEED2 BOSS_OLDCROW_DASHSPEED * 0.75f

#define BOSS_OLDCROW_MEGADASHSPEED 7000.0f

//CrowHead
#define BOSS_OLDCROW_CROWHEADSPEED 100.0f

//Jump
#define BOSS_OLDCROW_JUMPFORCE 2000.0f

//Slam
#define BOSS_OLDCROW_SLAMFORCE 4000.0f