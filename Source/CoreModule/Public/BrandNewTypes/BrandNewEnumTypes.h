#pragma once

UENUM(BlueprintType)
enum class  EEquippedWeapon : uint8 
{
	Unequipped,
	Sword
	
};

UENUM(BlueprintType)
enum class EGate : uint8
{
	Walking,
	Jogging,
	Running
};

UENUM(BlueprintType)
enum class ETurnDirection : uint8
{
	None,
	Turn90Right,
	Turn90Left,
	Turn180Right,
	Turn180Left
};

UENUM(BlueprintType)
enum class ERootYawOffsetMode : uint8
{
	Setting,
	BlendOut,
	Hold
};