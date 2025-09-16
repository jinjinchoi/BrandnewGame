
#include "BrandNewTypes/BrandNewGamePlayTag.h"

namespace BrandNewGamePlayTag
{
	/**
	 * Attribute Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary_Strength, "Attribute.Primary.Strength")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary_Intelligence, "Attribute.Primary.Intelligence")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary_Dexterity, "Attribute.Primary.Dexterity")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary_Vitality, "Attribute.Primary.Vitality")
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Experience_Level, "Attribute.Experience.Level")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Experience_XP, "Attribute.Experience.XP")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Experience_AttributePoint, "Attribute.Experience.AttributePoint")

	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_MaxHealth, "Attribute.Secondary.MaxHealth")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Vital_CurrentHealth, "Attribute.Vital.CurrentHealth")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondry_MaxMana, "Attribute.Secondary.MaxMana")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Vital_CurrentMana, "Attribute.Vital.CurrentMana")

	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondry_PhysicalAttackPower, "Attribute.Secondary.PhysicalAttackPower")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondry_MagicAttackPower, "Attribute.Secondary.MagicAttackPower")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondry_PhysicalDefensePower, "Attribute.Secondary.PhysicalDefensePower")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondry_MagicDefensePower, "Attribute.Secondary.MagicDefensePower")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondry_CriticalChance, "Attribute.Secondary.CriticalChance")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondry_CriticalMagnitude, "Attribute.Secondary.CriticalMagnitude")
	
	
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
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_SpawnMagicSphere, "Input.Ability.SpawnMagicSphere");

	UE_DEFINE_GAMEPLAY_TAG(Input_UI_OpenInGameMenu, "Input.UI.OpenInGameMenu")
	
	
	/**
	 * Ability Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_EquipWeapon, "Ability.Player.Combat.EquipWeapon")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_UnequipWeapon, "Ability.Player.Combat.UnequipWeapon")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_NormalAttack, "Ability.Player.Combat.NormalAttack")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_SpawnMagicSphere, "Ability.Player.Combat.SpawnMagicSphere")
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Enemy_Combat_MeleeAttack, "Ability.Enemy.Combat.MeleeAttack")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Enemy_Combat_RangeAttack, "Ability.Enemy.Combat.RangeAttack")
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Shared_React_Hit, "Ability.Shared.React.Hit")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Shared_React_Death, "Ability.Shared.React.Death")
	
	/**
	 * Event Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Event_EquipWeapon, "Event.EquipWeapon")
	UE_DEFINE_GAMEPLAY_TAG(Event_UnequipWeapon, "Event.UnequipWeapon")
	
	UE_DEFINE_GAMEPLAY_TAG(Event_Attack_End, "Event.Attack.End")
	UE_DEFINE_GAMEPLAY_TAG(Event_ProjectileSpawn, "Event.ProjectileSpawn")
	
	UE_DEFINE_GAMEPLAY_TAG(Event_Hit_Melee, "Event.Hit.Melee")

	UE_DEFINE_GAMEPLAY_TAG(Event_HitReact, "Event.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(Event_HitReact_Front, "Event.HitReact.Front")
	UE_DEFINE_GAMEPLAY_TAG(Event_HitReact_Back, "Event.HitReact.Back")
	UE_DEFINE_GAMEPLAY_TAG(Event_HitReact_Left, "Event.HitReact.Left")
	UE_DEFINE_GAMEPLAY_TAG(Event_HitReact_Right, "Event.HitReact.Right")

	
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

	/**
	 * Status Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Status_Shared_Hit, "Status.Shared.Hit")
	UE_DEFINE_GAMEPLAY_TAG(Status_Shared_Dead, "Status.Shared.Dead")
	UE_DEFINE_GAMEPLAY_TAG(Status_Shared_Strafing, "Status.Shared.Strafing")

	/**
	 * Gameplay Cue Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Widget_Damage_Normal, "GameplayCue.Widget.Damage.Normal")

	/**
	 * Block Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Block_Player_Movement, "Block.Player.Movement");
	
}