
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
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_MaxMana, "Attribute.Secondary.MaxMana")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Vital_CurrentMana, "Attribute.Vital.CurrentMana")

	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_PhysicalAttackPower, "Attribute.Secondary.PhysicalAttackPower")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_MagicAttackPower, "Attribute.Secondary.MagicAttackPower")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_PhysicalDefensePower, "Attribute.Secondary.PhysicalDefensePower")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_MagicDefensePower, "Attribute.Secondary.MagicDefensePower")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_CriticalChance, "Attribute.Secondary.CriticalChance")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_CriticalMagnitude, "Attribute.Secondary.CriticalMagnitude")

	/**
	 * Item Attribute Tag
	 */
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Item_Strength, "Attribute.Item.Strength")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Item_Intelligence, "Attribute.Item.Intelligence")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Item_Dexterity, "Attribute.Item.Dexterity")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Item_Vitality, "Attribute.Item.Vitality")

	
	/**
	 * Input Tags
	 */ 
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move")
	UE_DEFINE_GAMEPLAY_TAG(Input_Jump, "Input.Jump")
	UE_DEFINE_GAMEPLAY_TAG(Input_Look, "Input.Look")
	UE_DEFINE_GAMEPLAY_TAG(Input_Walk, "Input.Walk")
	UE_DEFINE_GAMEPLAY_TAG(Input_Run, "Input.Run")
	UE_DEFINE_GAMEPLAY_TAG(Input_Interact, "Input.Interact")
	
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Equip, "Input.Ability.Equip")
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Unequip, "Input.Ability.Unequip")
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_LockOn, "Input.Ability.LockOn")
	
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Combo, "Input.Ability.Combo");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Combo_NormalAttack, "Input.Ability.Combo.NormalAttack");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_SpawnMagicSphere, "Input.Ability.SpawnMagicSphere");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_FireBall, "Input.Ability.FireBall");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Dodge, "Input.Ability.Dodge");

	UE_DEFINE_GAMEPLAY_TAG(Input_UI_OpenInGameMenu, "Input.UI.OpenInGameMenu")
	
	
	/**
	 * Ability Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat, "Ability.Player.Combat")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_EquipWeapon, "Ability.Player.Combat.EquipWeapon")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_UnequipWeapon, "Ability.Player.Combat.UnequipWeapon")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_NormalAttack, "Ability.Player.Combat.NormalAttack")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_SpawnMagicSphere, "Ability.Player.Combat.SpawnMagicSphere")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Combat_FireBall, "Ability.Player.Combat.FireBall")
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Special_Dodge, "Ability.Player.Special.Dodge")
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Utility_LockOn, "Ability.Player.Utility.LockOn")
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Enemy_Combat_MeleeAttack, "Ability.Enemy.Combat.MeleeAttack")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Enemy_Combat_RangeAttack, "Ability.Enemy.Combat.RangeAttack")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Enemy_Combat_TrackingShard, "Ability.Enemy.Combat.TrackingShard")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Enemy_Combat_ShockLanding, "Ability.Enemy.Combat.ShockLanding")
	
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
	UE_DEFINE_GAMEPLAY_TAG(Status_Shared_Invincible, "Status.Shared.Invincible")
	
	UE_DEFINE_GAMEPLAY_TAG(Status_Player_LockOn, "Status.Player.LockOn")
	UE_DEFINE_GAMEPLAY_TAG(Status_Player_Fire, "Status.Player.Fire")
	UE_DEFINE_GAMEPLAY_TAG(Status_Player_Dodging, "Status.Player.Dodging")

	/**
	 * Gameplay Cue Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Widget_Damage_Normal, "GameplayCue.Widget.Damage.Normal")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_GroundWarning, "GameplayCue.Effect.GroundWarning")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_ArcaneShard, "GameplayCue.Effect.ArcaneShard")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_LandingImpact, "GameplayCue.Effect.LandingImpact")

	/**
	 * Block Tags
	 */
	UE_DEFINE_GAMEPLAY_TAG(Block_Player_Movement, "Block.Player.Movement");
	UE_DEFINE_GAMEPLAY_TAG(Block_Player_GageChange, "Block.Player.GageChange");
	
}