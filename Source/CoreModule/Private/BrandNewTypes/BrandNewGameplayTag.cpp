
#include "BrandNewTypes/BrandNewGamePlayTag.h"

namespace BrandNewGamePlayTag
{
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
	 * Input Tags
	 */ 
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move")
	UE_DEFINE_GAMEPLAY_TAG(Input_Jump, "Input.Jump")
	UE_DEFINE_GAMEPLAY_TAG(Input_Look, "Input.Look")
	UE_DEFINE_GAMEPLAY_TAG(Input_Walk, "Input.Walk")
	UE_DEFINE_GAMEPLAY_TAG(Input_Run, "Input.Run")
	
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Equip, "Input.Ability.Equip")
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Unequip, "Input.Ability.Unequip")
	
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Combo, "Input.Ability.Combo");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Combo_NormalAttack, "Input.Ability.Combo.NormalAttack");
	
	/**
	 * Ability Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_EquipWeapon, "Ability.Player.Combat.EquipWeapon")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_UnequipWeapon, "Ability.Player.Combat.UnequipWeapon")
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_NormalAttack, "Ability.Player.Combat.NormalAttack")
	
	/**
	 * Event Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Event_EquipWeapon, "Event.EquipWeapon")
	UE_DEFINE_GAMEPLAY_TAG(Event_UnequipWeapon, "Event.UnequipWeapon")
	
	UE_DEFINE_GAMEPLAY_TAG(Event_ComboWindow_Open, "Event.ComboWindow.Open")
	UE_DEFINE_GAMEPLAY_TAG(Event_Attack_End, "Event.Attack.End")
	
	UE_DEFINE_GAMEPLAY_TAG(Event_Hit_Melee, "Event.Hit.Melee")
	
	/**
	 * Damage Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Damage_Type_Physical, "Damage.Type.Physical")
	UE_DEFINE_GAMEPLAY_TAG(Damage_Type_Magic, "Damage.Type.Magic")
	
	UE_DEFINE_GAMEPLAY_TAG(Damage_Element_NoneElemental, "Damage.Element.NoneElemental")
	UE_DEFINE_GAMEPLAY_TAG(Damage_Element_Fire, "Damage.Element.Fire")
	UE_DEFINE_GAMEPLAY_TAG(Damage_Element_Ice, "Damage.Element.Ice")
	UE_DEFINE_GAMEPLAY_TAG(Damage_Element_Light, "Damage.Element.Light")
	UE_DEFINE_GAMEPLAY_TAG(Damage_Element_Darkness, "Damage.Element.Darkness")

	
	

	
	
}