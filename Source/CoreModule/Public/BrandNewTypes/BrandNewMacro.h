#pragma once

#include "CoreMinimal.h"

#define TEAM_PLAYER 0
#define TEAM_ENEMY 1
#define TEAM_NEUTRAL 3

#define ECC_Player ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Enemy ECollisionChannel::ECC_GameTraceChannel2
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel3
#define ECC_Ground ECollisionChannel::ECC_GameTraceChannel4
