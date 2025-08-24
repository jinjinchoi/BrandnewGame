
#include "BrandNewTypes/BrandNewGamePlayTag.h"

namespace BrandNewGamePlayTag
{

	/**
	 * Input Tags
	 */ 
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move")
	UE_DEFINE_GAMEPLAY_TAG(Input_Jump, "Input.Jump")
	UE_DEFINE_GAMEPLAY_TAG(Input_Look, "Input.Look")
	UE_DEFINE_GAMEPLAY_TAG(Input_Walk, "Input.Walk")
	UE_DEFINE_GAMEPLAY_TAG(Input_Run, "Input.Run")
	
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Equip, "Input.Ability.Equip")
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Unequip, "Input.Ability.Unequip")

	/**
	 * Attribute Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MaxHealth, "Attribute.MaxHealth")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_CurrentHealth, "Attribute.CurrentHealth")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MaxMana, "Attribute.MaxMana")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_CurrentMana, "Attribute.CurrentMana")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Strength, "Attribute.Strength")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Intelligence, "Attribute.Intelligence")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Dexterity, "Attribute.Dexterity")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Vitality, "Attribute.Vitality")
	
	/**
	 * Ability Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_EquipWeapon, "Ability.Player.EquipWeapon")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_UnequipWeapon, "Ability.Player.UnequipWeapon")
	
	/**
	 * Event Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Event_EquipWeapon, "Event.EquipWeapon")
	UE_DEFINE_GAMEPLAY_TAG(Event_UnequipWeapon, "Event.UnequipWeapon")
	
	
}