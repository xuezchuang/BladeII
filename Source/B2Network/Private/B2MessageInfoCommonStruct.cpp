
#include "B2MessageInfoCommonStruct.h"
#include "B2NetworkPlugin.h"
namespace b2network {

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GlobalAccountInfo)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(FString, nickname)
		REFLECTION_REGISTER_FIELD(int32, type)							// 계정타입, @see B2AccountType
		REFLECTION_REGISTER_FIELD(int32, status)						// 계정상태, @see B2AccountStatus
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2AccountInfo)
		REFLECTION_REGISTER_FIELD(FString, name)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(int32, cash)
		REFLECTION_REGISTER_FIELD(int32, money)
		REFLECTION_REGISTER_FIELD(int32, blade_point)
		REFLECTION_REGISTER_FIELD(int32, blade_point_max)
		REFLECTION_REGISTER_FIELD(int32, blade_point_charge_lefttime_second)
		REFLECTION_REGISTER_FIELD(int32, inventory_aether_buy_count)			// 에테르 인벤토리 구매 횟수
		REFLECTION_REGISTER_FIELD(int32, inventory_consumable_slot_count)		// 소모품 인벤토리 슬롯 수
		REFLECTION_REGISTER_FIELD(int32, dark_crystal)
		REFLECTION_REGISTER_FIELD(int32, stage_boost_ticket)					// 3배 모험 이용권
		REFLECTION_REGISTER_FIELD(int32, shop_ten_lottery_ticket)				// 장비소환 10개 뽑기 티켓 수
		REFLECTION_REGISTER_FIELD(int64, guild_id)
		REFLECTION_REGISTER_FIELD(int32, fame_point)
		REFLECTION_REGISTER_FIELD(int32, social_point)
		REFLECTION_REGISTER_FIELD(int32, last_play_stage_id)
		REFLECTION_REGISTER_FIELD(int32, last_clear_stage_id_normal)
		REFLECTION_REGISTER_FIELD(int32, last_clear_stage_id_hard)
		REFLECTION_REGISTER_FIELD(int32, last_clear_stage_id_very_hard)
		REFLECTION_REGISTER_FIELD(int32, last_clear_stage_id_hell)
		REFLECTION_REGISTER_FIELD(int32, attendance_day)
		REFLECTION_REGISTER_FIELD(int32, attendance_type)
		REFLECTION_REGISTER_FIELD(int64, nextAttendance_time)
		REFLECTION_REGISTER_FIELD(int32, daily_play_time_in_sec)
		REFLECTION_REGISTER_FIELD(int32, daily_play_time_reward_index)
		REFLECTION_REGISTER_FIELD(int64, daily_play_time_update_time)
		REFLECTION_REGISTER_FIELD(int64, rest_time)
		REFLECTION_REGISTER_FIELD(int64, leave_guild_time)
		REFLECTION_REGISTER_FIELD(int64, kicked_guild_id)
		REFLECTION_REGISTER_FIELD(int64, gold_buff_end_time)
		REFLECTION_REGISTER_FIELD(int64, exp_buff_end_time)
		REFLECTION_REGISTER_FIELD(int64, gold_buff_ticket)
		REFLECTION_REGISTER_FIELD(int64, exp_buff_ticket)
		REFLECTION_REGISTER_FIELD(int32, stage_auto_clear_ticket)
		REFLECTION_REGISTER_FIELD(int64, next_fairy_gift_time)
	REFLECTION_REGISTER_END(35)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2LightAccount)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(int32, cash)
		REFLECTION_REGISTER_FIELD(int32, money)
		REFLECTION_REGISTER_FIELD(int32, blade_point)
		REFLECTION_REGISTER_FIELD(int32, dark_crystal)
		REFLECTION_REGISTER_FIELD(int32, social_point)
		REFLECTION_REGISTER_FIELD(int32, stage_boost_ticket)
		REFLECTION_REGISTER_FIELD(int32, shop_ten_lottery_ticket)
		REFLECTION_REGISTER_FIELD(int32, fame_point)
		REFLECTION_REGISTER_FIELD(int32, stage_auto_clear_ticket)
	REFLECTION_REGISTER_END(10)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2SealSlotState)
		REFLECTION_REGISTER_FIELD(int32, seal_slot)
		REFLECTION_REGISTER_FIELD(bool, opened)
		REFLECTION_REGISTER_FIELD(int32, option_id)
		REFLECTION_REGISTER_FIELD(float, option_value)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2FactoryLimitStatus)
		REFLECTION_REGISTER_FIELD(int32, limit_id)
		REFLECTION_REGISTER_FIELD(int32, limit_type)
		REFLECTION_REGISTER_FIELD(int32, limit_time_type)
		REFLECTION_REGISTER_FIELD(int32, left_count)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2FactoryPeriodicStatus)
		REFLECTION_REGISTER_FIELD(int32, factory_id)
		REFLECTION_REGISTER_FIELD(int64, preview_time)
		REFLECTION_REGISTER_FIELD(int64, start_time)
		REFLECTION_REGISTER_FIELD(int64, end_time)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2FactoryResult)
		REFLECTION_REGISTER_FIELD(int32, factory_result_type)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, make_item)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, bonus_item)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, refund_item)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2UnsealBoxReward)
		REFLECTION_REGISTER_FIELD(int32, reward_type)
		REFLECTION_REGISTER_FIELD(int32, amount)
		REFLECTION_REGISTER_FIELD(int32, template_id)
		REFLECTION_REGISTER_FIELD(int32, effect_grade)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, item)
		REFLECTION_REGISTER_FIELD(B2TotemServerInfoPtr, totem)
		REFLECTION_REGISTER_FIELD(B2AetherServerInfoPtr, aether)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdFactoryInfo)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, factory_type)
		REFLECTION_REGISTER_FIELD(int32, factory_tab)
		REFLECTION_REGISTER_FIELD(int32, group_id)
		REFLECTION_REGISTER_FIELD(int32, group_sort)
		REFLECTION_REGISTER_FIELD(int32, item_sort)
		REFLECTION_REGISTER_FIELD(int32, limit_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, gold_cost)
		REFLECTION_REGISTER_FIELD(int32, gem_cost)
		REFLECTION_REGISTER_FIELD(int32, dark_crystal_cost)
		REFLECTION_REGISTER_FIELD(int32, success_rate)
		REFLECTION_REGISTER_FIELD(int32, huge_success_rate)
		REFLECTION_REGISTER_FIELD(int32, fail_rate)
		REFLECTION_REGISTER_FIELD(int32, huge_success_add_item_template_id)
		REFLECTION_REGISTER_FIELD(int32, huge_success_add_item_count)
		REFLECTION_REGISTER_FIELD(int32, result_item_template_id)
		REFLECTION_REGISTER_FIELD(int32, result_item_count)
		REFLECTION_REGISTER_FIELD(int32, main_material_template_id)
		REFLECTION_REGISTER_FIELD(int32, main_material_cond_reinforce)
		REFLECTION_REGISTER_FIELD(int32, main_material_cond_level)
		REFLECTION_REGISTER_FIELD(int32, main_material_count)
		REFLECTION_REGISTER_FIELD(int32, main_material_fail_refund_rate)
		REFLECTION_REGISTER_FIELD(int32, material_template_id1)
		REFLECTION_REGISTER_FIELD(int32, material_count1)
		REFLECTION_REGISTER_FIELD(int32, material_fail_refund_rate1)
		REFLECTION_REGISTER_FIELD(int32, material_template_id2)
		REFLECTION_REGISTER_FIELD(int32, material_count2)
		REFLECTION_REGISTER_FIELD(int32, material_fail_refund_rate2)
		REFLECTION_REGISTER_FIELD(int32, material_template_id3)
		REFLECTION_REGISTER_FIELD(int32, material_count3)
		REFLECTION_REGISTER_FIELD(int32, material_fail_refund_rate3)
		REFLECTION_REGISTER_FIELD(int32, material_template_id4)
		REFLECTION_REGISTER_FIELD(int32, material_count4)
		REFLECTION_REGISTER_FIELD(int32, material_fail_refund_rate4)
		REFLECTION_REGISTER_FIELD(int32, visible_type)
	REFLECTION_REGISTER_END(36)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdFactoryMaterialTransInfo)
		REFLECTION_REGISTER_FIELD(int32, factory_id)
		REFLECTION_REGISTER_FIELD(int32, trans_main_material_template_id)
		REFLECTION_REGISTER_FIELD(int32, trans_main_material_count)
		REFLECTION_REGISTER_FIELD(int32, trans_material_template_id1)
		REFLECTION_REGISTER_FIELD(int32, trans_material_count1)
		REFLECTION_REGISTER_FIELD(int32, trans_material_template_id2)
		REFLECTION_REGISTER_FIELD(int32, trans_material_count2)
		REFLECTION_REGISTER_FIELD(int32, trans_material_template_id3)
		REFLECTION_REGISTER_FIELD(int32, trans_material_count3)
		REFLECTION_REGISTER_FIELD(int32, trans_material_template_id4)
		REFLECTION_REGISTER_FIELD(int32, trans_material_count4)
	REFLECTION_REGISTER_END(11)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdFactoryLimitInfo)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, limit_type)
		REFLECTION_REGISTER_FIELD(int32, limit_count)
		REFLECTION_REGISTER_FIELD(int32, limit_time_type)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSealBoxInfo)
		REFLECTION_REGISTER_FIELD(int32, seal_box_template_id)
		REFLECTION_REGISTER_FIELD(int32, reward_slot1)
		REFLECTION_REGISTER_FIELD(int32, reward_slot2)
		REFLECTION_REGISTER_FIELD(int32, reward_slot3)
		REFLECTION_REGISTER_FIELD(int32, reward_slot4)
		REFLECTION_REGISTER_FIELD(int32, reward_slot5)
		REFLECTION_REGISTER_FIELD(bool, is_rate_visible)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSealBoxRewardInfo)
		REFLECTION_REGISTER_FIELD(int32, seal_box_reward_id)
		REFLECTION_REGISTER_FIELD(int32, min_fixed_count)
		REFLECTION_REGISTER_FIELD(int32, max_fixed_count)
		REFLECTION_REGISTER_FIELD(bool, is_recycle)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSealBoxRewardRateInfo)
		REFLECTION_REGISTER_FIELD(int32, seal_box_reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_type)
		REFLECTION_REGISTER_FIELD(int32, template_id)
		REFLECTION_REGISTER_FIELD(int32, min_amount)
		REFLECTION_REGISTER_FIELD(int32, max_amount)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, group_id)
		REFLECTION_REGISTER_FIELD(int32, equip_category)
		REFLECTION_REGISTER_FIELD(int32, equip_position)
		REFLECTION_REGISTER_FIELD(int32, quality)
		REFLECTION_REGISTER_FIELD(int32, enhance_level)
		REFLECTION_REGISTER_FIELD(int32, surpass_count)
		REFLECTION_REGISTER_FIELD(int32, reward_rate)
		REFLECTION_REGISTER_FIELD(bool, is_fixed_reward)
		REFLECTION_REGISTER_FIELD(int32, fixed_reward_rate)
		REFLECTION_REGISTER_FIELD(int32, effect_grade)
	REFLECTION_REGISTER_END(16)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ItemServerInfo)
		REFLECTION_REGISTER_FIELD(int64, id)
		REFLECTION_REGISTER_FIELD(int32, template_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, inventory_type)
		REFLECTION_REGISTER_FIELD(int32, equip_position)
		REFLECTION_REGISTER_FIELD(int32, amount)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, quality)
		REFLECTION_REGISTER_FIELD(int32, random_option_id1)
		REFLECTION_REGISTER_FIELD(int32, random_option_id2)
		REFLECTION_REGISTER_FIELD(int32, random_option_id3)
		REFLECTION_REGISTER_FIELD(float, random_option_value1)
		REFLECTION_REGISTER_FIELD(float, random_option_value2)
		REFLECTION_REGISTER_FIELD(float, random_option_value3)
		REFLECTION_REGISTER_FIELD(int32, level)						// [1, 40]
		REFLECTION_REGISTER_FIELD(float, exp)						// [0, 100), decimalpoint is 2
		REFLECTION_REGISTER_FIELD(int32, enhance_level)				// [0, 100), decimalpoint is 2
		REFLECTION_REGISTER_FIELD(int32, surpass_count)				// [0, 5]
		REFLECTION_REGISTER_FIELD(bool, locked)
		REFLECTION_REGISTER_FIELD(float, power)
		REFLECTION_REGISTER_FIELD(bool, is_new)
		REFLECTION_REGISTER_FIELD(float, md_main_attribute_value)
		REFLECTION_REGISTER_FIELD(int32, open_seal_slot_count)
		REFLECTION_REGISTER_FIELD(TArray<B2SealSlotStatePtr>, seal_slot_states)
	REFLECTION_REGISTER_END(24)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ItemPreset)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, preset_id)
		REFLECTION_REGISTER_FIELD(int32, equip_position)
		REFLECTION_REGISTER_FIELD(int64, item_id)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2CostumeServerInfo)
		REFLECTION_REGISTER_FIELD(int64, id)
		REFLECTION_REGISTER_FIELD(int32, template_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, inventory_type)
		REFLECTION_REGISTER_FIELD(int32, equip_position)
		REFLECTION_REGISTER_FIELD(int32, amount)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, quality)
		REFLECTION_REGISTER_FIELD(int32, random_option_id1)
		REFLECTION_REGISTER_FIELD(int32, random_option_id2)
		REFLECTION_REGISTER_FIELD(int32, random_option_id3)
		REFLECTION_REGISTER_FIELD(float, random_option_value1)
		REFLECTION_REGISTER_FIELD(float, random_option_value2)
		REFLECTION_REGISTER_FIELD(float, random_option_value3)
		REFLECTION_REGISTER_FIELD(int32, level)						// [1, 40]
		REFLECTION_REGISTER_FIELD(float, exp)						// [0, 100), decimalpoint is 2
		REFLECTION_REGISTER_FIELD(int32, enhance_level)				// [0, 100), decimalpoint is 2
		REFLECTION_REGISTER_FIELD(int32, surpass_count)				// [0, 5]
		REFLECTION_REGISTER_FIELD(bool, locked)
		REFLECTION_REGISTER_FIELD(float, power)
		REFLECTION_REGISTER_FIELD(bool, is_new)
		REFLECTION_REGISTER_FIELD(float, md_main_attribute_value)
	REFLECTION_REGISTER_END(22)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2AetherServerInfo)
		REFLECTION_REGISTER_FIELD(int64, id)
		REFLECTION_REGISTER_FIELD(int32, type)
		REFLECTION_REGISTER_FIELD(int32, slot)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, tier)
		REFLECTION_REGISTER_FIELD(int32, main_option_id)
		REFLECTION_REGISTER_FIELD(float, main_option_value)
		REFLECTION_REGISTER_FIELD(int32, sub_option_id1)
		REFLECTION_REGISTER_FIELD(int32, sub_option_id2)
		REFLECTION_REGISTER_FIELD(int32, sub_option_id3)
		REFLECTION_REGISTER_FIELD(float, sub_option_value1)
		REFLECTION_REGISTER_FIELD(float, sub_option_value2)
		REFLECTION_REGISTER_FIELD(float, sub_option_value3)
		REFLECTION_REGISTER_FIELD(int32, sub_option_enhance_count1)
		REFLECTION_REGISTER_FIELD(int32, sub_option_enhance_count2)
		REFLECTION_REGISTER_FIELD(int32, sub_option_enhance_count3)
		REFLECTION_REGISTER_FIELD(bool, locked)
		REFLECTION_REGISTER_FIELD(bool, is_new)
		REFLECTION_REGISTER_FIELD(float, calc_main_option_value)
		REFLECTION_REGISTER_FIELD(float, calc_sub_option_value1)
		REFLECTION_REGISTER_FIELD(float, calc_sub_option_value2)
		REFLECTION_REGISTER_FIELD(float, calc_sub_option_value3)
		REFLECTION_REGISTER_FIELD(int32, calc_tier)
	REFLECTION_REGISTER_END(25)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2TotemRefineState)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, status)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2TotemSubOptionDetail)
		REFLECTION_REGISTER_FIELD(int32, option_id)
		REFLECTION_REGISTER_FIELD(float, option_value)
		REFLECTION_REGISTER_FIELD(int32, refine_count)
		REFLECTION_REGISTER_FIELD(TArray<B2TotemRefineStatePtr>, refine_states)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2TotemServerInfo)
		REFLECTION_REGISTER_FIELD(int64, id)
		REFLECTION_REGISTER_FIELD(int32, template_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, equip_slot)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, main_option_id)
		REFLECTION_REGISTER_FIELD(float, main_option_value)
		REFLECTION_REGISTER_FIELD(TArray<B2TotemSubOptionDetailPtr>, sub_option_details)
		REFLECTION_REGISTER_FIELD(int32, refine_level)
		REFLECTION_REGISTER_FIELD(int32, refine_try_count)
		REFLECTION_REGISTER_FIELD(int32, gold_success_rate)
		REFLECTION_REGISTER_FIELD(int32, piece_success_rate)
		REFLECTION_REGISTER_FIELD(bool, locked)
		REFLECTION_REGISTER_FIELD(float, power)
		REFLECTION_REGISTER_FIELD(bool, is_new)
	REFLECTION_REGISTER_END(15)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2CharacterSetOptionInfo)
		REFLECTION_REGISTER_FIELD(int32, selected_offense_set_option_id)
		REFLECTION_REGISTER_FIELD(int32, selected_defense_set_option_id)
		REFLECTION_REGISTER_FIELD(TArray<int32>, defense_set_option_ids)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2CharacterPower)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, total_power)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2SkillServerInfo)
		REFLECTION_REGISTER_FIELD(int32, skill_id)
		REFLECTION_REGISTER_FIELD(int32, skill_level)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2SkillQuickSlot)
		REFLECTION_REGISTER_FIELD(int32, slot)
		REFLECTION_REGISTER_FIELD(int32, left_skill_point)
		REFLECTION_REGISTER_FIELD(float, skill_total_power)
		REFLECTION_REGISTER_FIELD(int32, using_skill_id_1)
		REFLECTION_REGISTER_FIELD(int32, using_skill_id_2)
		REFLECTION_REGISTER_FIELD(int32, using_skill_id_3)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSkillInfo)
		REFLECTION_REGISTER_FIELD(int32, skill_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, learn_level)
		REFLECTION_REGISTER_FIELD(int32, max_skill_level)
		REFLECTION_REGISTER_FIELD(bool, usable)
		REFLECTION_REGISTER_FIELD(int32, level_up_cost_factor)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSkillOption)
		REFLECTION_REGISTER_FIELD(int32, skill_id)
		REFLECTION_REGISTER_FIELD(int32, skill_level)
		REFLECTION_REGISTER_FIELD(int32, option_id)
		REFLECTION_REGISTER_FIELD(float, option_value)
		REFLECTION_REGISTER_FIELD(float, power)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSkillLevelUp)
		REFLECTION_REGISTER_FIELD(int32, skill_id)
		REFLECTION_REGISTER_FIELD(int32, skill_level)
		REFLECTION_REGISTER_FIELD(int32, req_skill_point)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdItemLevelupExpFactor)
		REFLECTION_REGISTER_FIELD(FString, type_name)
		REFLECTION_REGISTER_FIELD(int32, key_code)
		REFLECTION_REGISTER_FIELD(double, factor)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdItemOptionAddValue)
		REFLECTION_REGISTER_FIELD(int32, option_id)
		REFLECTION_REGISTER_FIELD(float, add_value)
		REFLECTION_REGISTER_FIELD(float, power_factor)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdItemLevelupFactor)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(double, factor)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdItemLevelupCost)
		REFLECTION_REGISTER_FIELD(int32, inventory_type)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, price_grade1)
		REFLECTION_REGISTER_FIELD(int32, price_grade2)
		REFLECTION_REGISTER_FIELD(int32, price_grade3)
		REFLECTION_REGISTER_FIELD(int32, price_grade4)
		REFLECTION_REGISTER_FIELD(int32, price_grade5)
		REFLECTION_REGISTER_FIELD(int32, price_grade6)
		REFLECTION_REGISTER_FIELD(int32, price_grade7)
		REFLECTION_REGISTER_FIELD(int32, price_grade8)
		REFLECTION_REGISTER_FIELD(int32, price_grade9)
		REFLECTION_REGISTER_FIELD(int32, price_grade10)
		REFLECTION_REGISTER_FIELD(int32, price_grade11)
		REFLECTION_REGISTER_FIELD(int32, price_grade12)
	REFLECTION_REGISTER_END(14)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdItemEnhanceCost)
		REFLECTION_REGISTER_FIELD(int32, enhance_level)
		REFLECTION_REGISTER_FIELD(int32, enhance_rate)
		REFLECTION_REGISTER_FIELD(int32, enhance_scroll_cost)
		REFLECTION_REGISTER_FIELD(int32, protection_scroll_cost)
		REFLECTION_REGISTER_FIELD(int32, money_cost)
		REFLECTION_REGISTER_FIELD(int32, exchange_cost_dark_crystal)
		REFLECTION_REGISTER_FIELD(int32, exchange_cost_gem)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdCostumeEnhanceCost)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, enhance_level)
		REFLECTION_REGISTER_FIELD(int32, money_cost)
		REFLECTION_REGISTER_FIELD(int32, weapon_material_id)
		REFLECTION_REGISTER_FIELD(int32, weapon_amount)
		REFLECTION_REGISTER_FIELD(int32, weapon_restore_amount)
		REFLECTION_REGISTER_FIELD(int32, armor_material_id)
		REFLECTION_REGISTER_FIELD(int32, armor_amount)
		REFLECTION_REGISTER_FIELD(int32, armor_restore_amount)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdItemEnhanceEffectInfo)
		REFLECTION_REGISTER_FIELD(int32, enhance_level)
		REFLECTION_REGISTER_FIELD(double, set_effect)
		REFLECTION_REGISTER_FIELD(int32, item_aura)
		REFLECTION_REGISTER_FIELD(float, channel_alpha)
		REFLECTION_REGISTER_FIELD(float, channel_red)
		REFLECTION_REGISTER_FIELD(float, channel_green)
		REFLECTION_REGISTER_FIELD(float, channel_blue)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdItemSurpassLevel)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, increase_level)
		REFLECTION_REGISTER_FIELD(int32, max_surpass_level)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdItemDismantleInfo)
		REFLECTION_REGISTER_FIELD(int32, inventory_type)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, material1)
		REFLECTION_REGISTER_FIELD(int32, material_amount1)
		REFLECTION_REGISTER_FIELD(int32, material2)
		REFLECTION_REGISTER_FIELD(int32, material_amount2)
		REFLECTION_REGISTER_FIELD(int32, material3)
		REFLECTION_REGISTER_FIELD(int32, material_amount3)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdResurrectionInfo)
		REFLECTION_REGISTER_FIELD(int32, mode_type)
		REFLECTION_REGISTER_FIELD(int32, count)
		REFLECTION_REGISTER_FIELD(int32, price)
		REFLECTION_REGISTER_FIELD(int32, attribute_change1)
		REFLECTION_REGISTER_FIELD(int32, change_value1)
		REFLECTION_REGISTER_FIELD(int32, attribute_change2)
		REFLECTION_REGISTER_FIELD(int32, change_value2)
		REFLECTION_REGISTER_FIELD(int32, attribute_change3)
		REFLECTION_REGISTER_FIELD(int32, change_value3)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2CharacterServerInfo)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, style_face)
		REFLECTION_REGISTER_FIELD(int32, style_hair)
		REFLECTION_REGISTER_FIELD(int32, style_skin)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int64, exp)
		REFLECTION_REGISTER_FIELD(int64, exp_max)
		REFLECTION_REGISTER_FIELD(int32, skill_point_buy_count)
		REFLECTION_REGISTER_FIELD(int32, inventory_item_buy_count)
		REFLECTION_REGISTER_FIELD(int32, inventory_item_slot_count)
		REFLECTION_REGISTER_FIELD(int32, rank)
		REFLECTION_REGISTER_FIELD(int32, rank_node_index)
		REFLECTION_REGISTER_FIELD(bool, rank_node_state)
		REFLECTION_REGISTER_FIELD(int32, wing_grade)
		REFLECTION_REGISTER_FIELD(int32, wing_level)
		REFLECTION_REGISTER_FIELD(bool, wing_visible)
		REFLECTION_REGISTER_FIELD(bool, helmet_visible)
		REFLECTION_REGISTER_FIELD(int32, aether_offense_set_option_id)
		REFLECTION_REGISTER_FIELD(int32, aether_defense_set_option_id)
		REFLECTION_REGISTER_FIELD(int32, remain_skill_point)
		REFLECTION_REGISTER_FIELD(TArray<B2SkillQuickSlotPtr>, skill_quick_slots)
		REFLECTION_REGISTER_FIELD(FString, name)
		REFLECTION_REGISTER_FIELD(TArray<B2SkillServerInfoPtr>, skills)
		REFLECTION_REGISTER_FIELD(bool,  unity_skill)
		REFLECTION_REGISTER_FIELD(int32, total_power)
		REFLECTION_REGISTER_FIELD(bool, costume_weapon_visible)
		REFLECTION_REGISTER_FIELD(bool, costume_hat_visible)
		REFLECTION_REGISTER_FIELD(bool, costume_armor_visible)
		REFLECTION_REGISTER_FIELD(bool, unity_skill_awaken)
	REFLECTION_REGISTER_END(29)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2LightCharacter)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int64, exp)
		REFLECTION_REGISTER_FIELD(int64, exp_max)
		REFLECTION_REGISTER_FIELD(bool, changed_level)
		REFLECTION_REGISTER_FIELD(int32, remain_skill_point)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2Reward)
		REFLECTION_REGISTER_FIELD(B2RawRewardPtr, raw_reward)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, item)
		REFLECTION_REGISTER_FIELD(B2CostumeServerInfoPtr, costume)
		REFLECTION_REGISTER_FIELD(B2AetherServerInfoPtr, aether)
		REFLECTION_REGISTER_FIELD(B2LightAccountPtr, light_account)
		REFLECTION_REGISTER_FIELD(TArray<B2LightCharacterPtr>, light_characters)
	REFLECTION_REGISTER_END(6)
	
	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2Relic)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, relic_id)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, enhance_level)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2WingOptionInfo)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, option_index)
		REFLECTION_REGISTER_FIELD(int32, level)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2FairyStatus)
		REFLECTION_REGISTER_FIELD(int32, fairy_type)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, enhance_point)
		REFLECTION_REGISTER_FIELD(int32, activated_bless_option_id)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildSkill)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(bool, enabled)
		REFLECTION_REGISTER_FIELD(int64, buff_end_time)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2PresetNum)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, item_preset_num)
		REFLECTION_REGISTER_FIELD(int32, skill_preset_num)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2UnitySkill)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, unity_character_type)
		REFLECTION_REGISTER_FIELD(int32, step)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2FindAccountInfo)
		REFLECTION_REGISTER_FIELD(FString, nickname)
		REFLECTION_REGISTER_FIELD(TArray<B2CharacterServerInfoPtr>, characters)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, equip_items)
		REFLECTION_REGISTER_FIELD(TArray<B2CostumeServerInfoPtr>, equip_costumes)
		REFLECTION_REGISTER_FIELD(TArray<B2TotemServerInfoPtr>, equip_totems)
		REFLECTION_REGISTER_FIELD(TArray<B2AetherServerInfoPtr>, mount_aethers)
		REFLECTION_REGISTER_FIELD(TArray<B2RelicPtr>, relics)
		REFLECTION_REGISTER_FIELD(TArray<B2WingOptionInfoPtr>, wing_options)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildSkillPtr>, guild_skills)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemPresetPtr>, item_presets)
		REFLECTION_REGISTER_FIELD(TArray<B2PresetNumPtr>, character_presets)
		REFLECTION_REGISTER_FIELD(TArray<B2UnitySkillPtr>, unity_skills)
		REFLECTION_REGISTER_FIELD(TArray<B2FairyStatusPtr>, fairies)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
		REFLECTION_REGISTER_FIELD(int32, assault_match_grade)
		REFLECTION_REGISTER_FIELD(int32, assault_match_star_count)
	REFLECTION_REGISTER_END(17)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2FindAccountQuery)
		REFLECTION_REGISTER_FIELD(FString, nickname)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(TArray<int32>, character_types)
		REFLECTION_REGISTER_FIELD(int32, item_preset_id)
		REFLECTION_REGISTER_FIELD(int32, skill_slot)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2CharacterSummary)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(FString, account_name)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, character_level)
		REFLECTION_REGISTER_FIELD(int32, power)
		REFLECTION_REGISTER_FIELD(bool, is_online)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2User)
		REFLECTION_REGISTER_FIELD(B2CharacterSummaryPtr, character_summary)
		REFLECTION_REGISTER_FIELD(FString, guild_name)
		REFLECTION_REGISTER_FIELD(int32, mark_index)
		REFLECTION_REGISTER_FIELD(int32, mark_color)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_index)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_color)
		REFLECTION_REGISTER_FIELD(int64, last_logout_time)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2Friend)
		REFLECTION_REGISTER_FIELD(B2UserPtr, user)
		REFLECTION_REGISTER_FIELD(int64, last_social_point_send_time)
		REFLECTION_REGISTER_FIELD(bool, is_new)
		REFLECTION_REGISTER_FIELD(int64, raid_penalty_remove_time)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2FriendInviteRewardInfo)
		REFLECTION_REGISTER_FIELD(int32, platform_type)
		REFLECTION_REGISTER_FIELD(int32, invite_count)
		REFLECTION_REGISTER_FIELD(int32, joiner_count)
		REFLECTION_REGISTER_FIELD(int32, invite_reward_mask)
		REFLECTION_REGISTER_FIELD(int32, joiner_reward_mask)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2InventoryCountServerInfo)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, count)
		REFLECTION_REGISTER_FIELD(int32, max_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2StageClearInfo)
		REFLECTION_REGISTER_FIELD(int32, stage_id)
		REFLECTION_REGISTER_FIELD(int32, clear_grade)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ActRewardInfo)
		REFLECTION_REGISTER_FIELD(int32, difficulty)
		REFLECTION_REGISTER_FIELD(int32, act_id)
		REFLECTION_REGISTER_FIELD(int32, reward_index)
		REFLECTION_REGISTER_FIELD(int32, require_star_count)
		REFLECTION_REGISTER_FIELD(int32, reward_gold)
		REFLECTION_REGISTER_FIELD(int32, reward_gem)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
		REFLECTION_REGISTER_FIELD(bool, received)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ActRepeatClearRewardInfo)
		REFLECTION_REGISTER_FIELD(int32, difficulty)
		REFLECTION_REGISTER_FIELD(int32, act_id)
		REFLECTION_REGISTER_FIELD(int32, clear_count)
		REFLECTION_REGISTER_FIELD(int32, require_clear_count)
		REFLECTION_REGISTER_FIELD(int32, reward_gold)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2StageClearRewardItemInfo)
		REFLECTION_REGISTER_FIELD(int32, group_id)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, type)
		REFLECTION_REGISTER_FIELD(int32, main_attribute)
		REFLECTION_REGISTER_FIELD(float, main_attribute_value_min)
		REFLECTION_REGISTER_FIELD(float, main_attribute_value_max)
		REFLECTION_REGISTER_FIELD(int32, random_option_count)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2StageClearRewardAetherInfo)
		REFLECTION_REGISTER_FIELD(int32, type)
		REFLECTION_REGISTER_FIELD(TArray<int32>, tiers)
		REFLECTION_REGISTER_FIELD(TArray<int32>, grades)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(B2Endpoint)
        REFLECTION_REGISTER_FIELD(FString, host)
        REFLECTION_REGISTER_FIELD(int32, port)
    REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2MatchInfo)
		REFLECTION_REGISTER_FIELD(FString, match_token)
		REFLECTION_REGISTER_FIELD(int32, opponent_type)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2MatchResult)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(FString, match_token)
		REFLECTION_REGISTER_FIELD(int32, result)
		REFLECTION_REGISTER_FIELD(bool, disconnect)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2MatchStatus)
		REFLECTION_REGISTER_FIELD(int32, battle_grade)
		REFLECTION_REGISTER_FIELD(int32, battle_star_count)
		REFLECTION_REGISTER_FIELD(int32, continue_win)
		REFLECTION_REGISTER_FIELD(int32, continue_win_max)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2Ranking)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(int32, ranking)
		REFLECTION_REGISTER_FIELD(int32, total_ranker_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2MatchRanker)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(FString, nickname)
		REFLECTION_REGISTER_FIELD(int32, battle_grade)
		REFLECTION_REGISTER_FIELD(int32, battle_star_count)
		REFLECTION_REGISTER_FIELD(int32, best_floor)
		REFLECTION_REGISTER_FIELD(int32, clear_time)
		REFLECTION_REGISTER_FIELD(int32, total_power)
		REFLECTION_REGISTER_FIELD(int32, point)
		REFLECTION_REGISTER_FIELD(TArray<B2MatchRankerEntry>, entries)
        REFLECTION_REGISTER_FIELD(FString, pvp_replay_url)
        REFLECTION_REGISTER_FIELD(int32, replay_size_compressed)
        REFLECTION_REGISTER_FIELD(int32, replay_size_uncompressed)
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2HallOfFame)
		REFLECTION_REGISTER_FIELD(int32, mode)
		REFLECTION_REGISTER_FIELD(int32, ranking)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(int32, praised_count)
		REFLECTION_REGISTER_FIELD(int32, match_grade)
		REFLECTION_REGISTER_FIELD(int32, match_start_count)
		REFLECTION_REGISTER_FIELD(int32, mvp_count)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2HallOfFameUser)
		REFLECTION_REGISTER_FIELD(B2UserPtr, user)
		REFLECTION_REGISTER_FIELD(TArray<B2ItemServerInfoPtr>, equip_items)
		REFLECTION_REGISTER_FIELD(TArray<B2CostumeServerInfoPtr>, equip_costumes)
		REFLECTION_REGISTER_FIELD(TArray<B2TotemServerInfoPtr>, equip_totems)
		REFLECTION_REGISTER_FIELD(int32, wing_grade)
		REFLECTION_REGISTER_FIELD(int32, wing_level)
		REFLECTION_REGISTER_FIELD(bool, wing_visible)
		REFLECTION_REGISTER_FIELD(bool, helmet_visible)
		REFLECTION_REGISTER_FIELD(bool, costume_weapon_visible)
		REFLECTION_REGISTER_FIELD(bool, costume_hat_visible)
		REFLECTION_REGISTER_FIELD(bool, costume_armor_visible)
	REFLECTION_REGISTER_END(11)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2HallOfFameReward)
		REFLECTION_REGISTER_FIELD(int32, mode)
		REFLECTION_REGISTER_FIELD(int32, ranking)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2HallOfFamePraise)
		REFLECTION_REGISTER_FIELD(int32, mode)
		REFLECTION_REGISTER_FIELD(int32, praise_count)
		REFLECTION_REGISTER_FIELD(int32, max_count)
		REFLECTION_REGISTER_FIELD(int64, target_account_id)
	REFLECTION_REGISTER_END(4)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(B2RoomLocation)
        REFLECTION_REGISTER_FIELD(B2EndpointPtr, channel_endpoint)
        REFLECTION_REGISTER_FIELD(FString, room_id)
    REFLECTION_REGISTER_END(2)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(B2RoomParticipant)
        REFLECTION_REGISTER_FIELD(int32, id)
        REFLECTION_REGISTER_FIELD(int64, account_id)
        REFLECTION_REGISTER_FIELD(B2FindAccountInfoPtr, account_info)
    REFLECTION_REGISTER_END(3)

    //////////////////////////////////////////////////////////////////////////
    REFLECTION_REGISTER_BEGIN(B2RoomProperty)
        REFLECTION_REGISTER_FIELD(FString, key)
        REFLECTION_REGISTER_FIELD(FString, value)
    REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ConfigData)
		REFLECTION_REGISTER_FIELD(int32, skill_point_buy_price_gem)					// 스킬 포인트 구매에 필요한 젬
		REFLECTION_REGISTER_FIELD(int32, max_skill_point_buy_count)					// 스킬 포인트 최대 구매 횟수
		REFLECTION_REGISTER_FIELD(int32, skill_point_reset_price_gem)				// 스킬 초기화에 필요한 젬
		REFLECTION_REGISTER_FIELD(int32, need_cash_for_extend_inventory)			// 인벤토리 1회 확장에 필요한 젬
		REFLECTION_REGISTER_FIELD(int32, increase_inventory_slot_per_buy_count)		// 인벤토리 1회 확장당 슬롯 늘어나는 수
		REFLECTION_REGISTER_FIELD(int32, max_inventory_slot_count)					// 인벤토리 최대 슬롯 수
		REFLECTION_REGISTER_FIELD(int32, max_friend_count)							// 최대 친구 수
		REFLECTION_REGISTER_FIELD(int32, max_send_ask_friend)						// 보낼 수 있는 친구 요청 최대 수
		REFLECTION_REGISTER_FIELD(int32, max_receive_ask_friend)					// 받을 수 있는 친구 요청 최대 수
		REFLECTION_REGISTER_FIELD(int32, cooltime_for_send_social_point)			// 우정 포인트 다시 보낼 수 있는 시간
		REFLECTION_REGISTER_FIELD(int32, max_delete_friend_count_for_day)			// 하루에 친구 삭제 최대 수
		REFLECTION_REGISTER_FIELD(int32, max_social_point_value)					// 우정 포인트 최대값
		REFLECTION_REGISTER_FIELD(int32, add_social_point_for_send_to_friend)		// 하트 보냈을때 내가 얻게 되는 우정 포인트
		REFLECTION_REGISTER_FIELD(int32, add_social_point_for_receive_from_friend)	// 하트 받았을때(우편에서 받음) 내가 얻게 되는 우정 포인트
		REFLECTION_REGISTER_FIELD(int32, stage_buff_rate_for_increase_exp)			// 스테이지 선택 버프 경험치 증가 비율
		REFLECTION_REGISTER_FIELD(int32, stage_buff_rate_for_increase_gold)			// 스테이지 선택 버프 골드 증가 비율
		REFLECTION_REGISTER_FIELD(int32, stage_buff_rate_for_boost_game_speed)		// 스테이지 선택 버프 게임 속도 증가 비율
		REFLECTION_REGISTER_FIELD(int32, guild_set_up_gold_cost)					// 길드 창설 비용
		REFLECTION_REGISTER_FIELD(int32, default_max_guild_member_count)			// 기본 길드원 최대수
		REFLECTION_REGISTER_FIELD(int32, max_invite_guild_count_for_account)		// 계정마다 길드 초대 신청을 받을수 있는 최대 수
		REFLECTION_REGISTER_FIELD(int32, invite_guild_expire_period_by_day)			// 길드 초대 만료 기간 (일)
		REFLECTION_REGISTER_FIELD(int32, max_raid_count_per_day)					// 레이드 하루 최대 도전횟수
		REFLECTION_REGISTER_FIELD(int32, update_guild_gold_cost)					// 길드 편집 골드 비용
		REFLECTION_REGISTER_FIELD(int32, guild_master_logout_limit_period_day)		// 길드장의 비접속 제한 기간. 이기간 이후 부길드장이 길드장 권한 가져올수 있음
		REFLECTION_REGISTER_FIELD(int32, guild_battle_time_limit_sec_client)		// 길드전 전투 제한 시간 (초)
		REFLECTION_REGISTER_FIELD(int32, assault_resurrection_max_count)			// 점령전 최대부활 횟수
		REFLECTION_REGISTER_FIELD(int32, hero_tower_sweep_reward_count_per_floor)
		REFLECTION_REGISTER_FIELD(int32, assault_resurrection_max_count)
		REFLECTION_REGISTER_FIELD(int32, pvp_match_point_max)
		REFLECTION_REGISTER_FIELD(int32, pvp_match_point_charge_period_second)
		REFLECTION_REGISTER_FIELD(int32, team_match_point_max)
		REFLECTION_REGISTER_FIELD(int32, team_match_point_charge_period_second)
		REFLECTION_REGISTER_FIELD(int32, item_random_option_change_cost_dark_crystal)
		REFLECTION_REGISTER_FIELD(int32, item_preset_max_count)
		REFLECTION_REGISTER_FIELD(int32, skill_quick_slot_max_count)
		REFLECTION_REGISTER_FIELD(int32, assault_mvp_point_to_open_reward)
		REFLECTION_REGISTER_FIELD(int32, duel_reward_challenge_max_count)
		REFLECTION_REGISTER_FIELD(int32, guild_support_cool_time_hour)
		REFLECTION_REGISTER_FIELD(int32, guild_support_max_count_for_day)
		REFLECTION_REGISTER_FIELD(int32, rest_max_time_second)
		REFLECTION_REGISTER_FIELD(int32, rest_time_second_per_one_point)
		REFLECTION_REGISTER_FIELD(int32, guild_mercenary_donation_max_count_for_day)
		REFLECTION_REGISTER_FIELD(int32, guild_rejoin_wait_time_min)
		REFLECTION_REGISTER_FIELD(int32, character_level_max)
		REFLECTION_REGISTER_FIELD(int32, match_continuous_win_count_for_bonus)
		REFLECTION_REGISTER_FIELD(int32, hero_tower_free_ticket_count_max)
		REFLECTION_REGISTER_FIELD(int32, counter_dungeon_daily_enter_count_max)
		REFLECTION_REGISTER_FIELD(int32, max_chat_block_count)
		REFLECTION_REGISTER_FIELD(int32, min_floor_for_hero_tower_ranking)
		REFLECTION_REGISTER_FIELD(int32, raid_guild_bonus_count_member_2)
		REFLECTION_REGISTER_FIELD(int32, raid_guild_bonus_count_member_3)
		REFLECTION_REGISTER_FIELD(int32, raid_guild_bonus_count_member_4)
		REFLECTION_REGISTER_FIELD(int32, show_review_inducement_popup)
		REFLECTION_REGISTER_FIELD(int32, max_open_stage_difficulty)
		REFLECTION_REGISTER_FIELD(int32, item_random_option_change_cost_cash)
		REFLECTION_REGISTER_FIELD(int32, event_dice_free_count)
		REFLECTION_REGISTER_FIELD(int32, event_dice_point_per_one_dice)
		REFLECTION_REGISTER_FIELD(int32, max_costume_inventory_slot_count)
		REFLECTION_REGISTER_FIELD(int32, guild_master_change_period_day)
		REFLECTION_REGISTER_FIELD(int32, exchange_count_gold)
		REFLECTION_REGISTER_FIELD(int32, exchange_count_gem)
		REFLECTION_REGISTER_FIELD(int32, lobby_style)
		REFLECTION_REGISTER_FIELD(int32, advanced_rank_level)
	REFLECTION_REGISTER_END(62)

	REFLECTION_REGISTER_BEGIN(B2mdStageInfo)
		REFLECTION_REGISTER_FIELD(int32, stage_id)
		REFLECTION_REGISTER_FIELD(int32, difficulty)
		REFLECTION_REGISTER_FIELD(int32, difficulty_scale)
		REFLECTION_REGISTER_FIELD(int32, map_id)
		REFLECTION_REGISTER_FIELD(int32, clear_time_grade_high)
		REFLECTION_REGISTER_FIELD(int32, clear_time_grade_medium)
		REFLECTION_REGISTER_FIELD(int32, recommend_power)
		REFLECTION_REGISTER_FIELD(int32, recommend_attack)
		REFLECTION_REGISTER_FIELD(int32, recommend_defense)
		REFLECTION_REGISTER_FIELD(int32, gold_cost_exp_buff)
		REFLECTION_REGISTER_FIELD(int32, gold_cost_gold_buff)
		REFLECTION_REGISTER_FIELD(int32, gold_cost_auto_skill_buff)
		REFLECTION_REGISTER_FIELD(int32, gold_cost_boost_buff)
		REFLECTION_REGISTER_FIELD(int32, aether_type)
	REFLECTION_REGISTER_END(14)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2BuySkillPointRequestInfo)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, skill_point_buy_count)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2BuySkillPointResponseInfo)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, left_skill_point)
		REFLECTION_REGISTER_FIELD(int32, total_skill_point_buy_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2BattleFormation)
		REFLECTION_REGISTER_FIELD(int32, formation_type)
		REFLECTION_REGISTER_FIELD(int32, formation_level)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2BattleFormationBuff)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, buff_type1)
		REFLECTION_REGISTER_FIELD(float, buff_value1)
		REFLECTION_REGISTER_FIELD(int32, buff_type2)
		REFLECTION_REGISTER_FIELD(float, buff_value2)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2TeamBattleEntry)
		REFLECTION_REGISTER_FIELD(int32, formation_type)
		REFLECTION_REGISTER_FIELD(int32, character_slot_1)
		REFLECTION_REGISTER_FIELD(int32, character_slot_2)
		REFLECTION_REGISTER_FIELD(int32, character_slot_3)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdTeamBattleFormationInfo)
		REFLECTION_REGISTER_FIELD(int32, formation_type)
		REFLECTION_REGISTER_FIELD(int32, formation_level)
		REFLECTION_REGISTER_FIELD(int32, enhance_gold_cost)
		REFLECTION_REGISTER_FIELD(TArray<B2BattleFormationBuffPtr>, buffs)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2Team)
		REFLECTION_REGISTER_FIELD(int32, debuff_value)
		REFLECTION_REGISTER_FIELD(B2FindAccountInfoPtr, account)
		REFLECTION_REGISTER_FIELD(B2BattleFormationPtr, formation)
		REFLECTION_REGISTER_FIELD(B2TeamBattleEntryPtr, entry)
		REFLECTION_REGISTER_FIELD(B2MatchStatusPtr, match_status)
		REFLECTION_REGISTER_FIELD(FString, guild_name)
		REFLECTION_REGISTER_FIELD(int32, mark_index)
		REFLECTION_REGISTER_FIELD(int32, mark_color)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_index)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_color)
	REFLECTION_REGISTER_END(10)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2Matcher)
		REFLECTION_REGISTER_FIELD(int32, debuff_value)
		REFLECTION_REGISTER_FIELD(B2FindAccountInfoPtr, account)
		REFLECTION_REGISTER_FIELD(B2MatchStatusPtr, match_status)
		REFLECTION_REGISTER_FIELD(FString, guild_name)
		REFLECTION_REGISTER_FIELD(int32, mark_index)
		REFLECTION_REGISTER_FIELD(int32, mark_color)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_index)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_color)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdRankInfo)
		REFLECTION_REGISTER_FIELD(int32, rank)
		REFLECTION_REGISTER_FIELD(int32, node_index)
		REFLECTION_REGISTER_FIELD(int32, open_level)
		REFLECTION_REGISTER_FIELD(int32, attribute_change)
		REFLECTION_REGISTER_FIELD(float, change_value)
		REFLECTION_REGISTER_FIELD(int32, need_rank_promotion_point)
		REFLECTION_REGISTER_FIELD(int32, need_activate_gold)
		REFLECTION_REGISTER_FIELD(int32, rate)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDailyMatchReward)
		REFLECTION_REGISTER_FIELD(int32, match_type)
		REFLECTION_REGISTER_FIELD(int32, require_count)
		REFLECTION_REGISTER_FIELD(int32, reward_gold)
		REFLECTION_REGISTER_FIELD(int32, reward_bladepoint)
		REFLECTION_REGISTER_FIELD(int32, reward_gem)
		REFLECTION_REGISTER_FIELD(int32, reward_item_template_id)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2RawReward)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, count)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2RewardRandomCount)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, min_count)
		REFLECTION_REGISTER_FIELD(int32, max_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdMatchRankingReward)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, ranking_min)
		REFLECTION_REGISTER_FIELD(int32, ranking_max)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2RankingReward)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, star_count)
		REFLECTION_REGISTER_FIELD(int32, ranking)
		REFLECTION_REGISTER_FIELD(int32, ranker_count)
		REFLECTION_REGISTER_FIELD(int32, initial_grade)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2DimensionRankingReward)
		REFLECTION_REGISTER_FIELD(int32, best_difficulty_level)
		REFLECTION_REGISTER_FIELD(int32, best_clear_time_millis)
		REFLECTION_REGISTER_FIELD(int32, ranking)
		REFLECTION_REGISTER_FIELD(int32, ranker_count)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdMainQuestInfo)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, main_quest_type)
		REFLECTION_REGISTER_FIELD(int32, act_id)
		REFLECTION_REGISTER_FIELD(int32, difficulty)
		REFLECTION_REGISTER_FIELD(int32, event_type)
		REFLECTION_REGISTER_FIELD(int32, main_view_condition)
		REFLECTION_REGISTER_FIELD(int32, condition_1)
		REFLECTION_REGISTER_FIELD(int32, condition_2)
		REFLECTION_REGISTER_FIELD(int32, condition_3)
		REFLECTION_REGISTER_FIELD(int32, condition_4)
		REFLECTION_REGISTER_FIELD(int32, goal_count)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDailyQuestInfo)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, event_type)
		REFLECTION_REGISTER_FIELD(int32, condition_1)
		REFLECTION_REGISTER_FIELD(int32, condition_2)
		REFLECTION_REGISTER_FIELD(int32, condition_3)
		REFLECTION_REGISTER_FIELD(int32, condition_4)
		REFLECTION_REGISTER_FIELD(int32, goal_count)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdRewardInfo)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, reward_type)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_push_type)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2RankInfo)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, node_index)
		REFLECTION_REGISTER_FIELD(int32, honor_point)
		REFLECTION_REGISTER_FIELD(int32, node_state)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2MailItemInfo)
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
		REFLECTION_REGISTER_FIELD(int32, item_amount)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(B2ItemServerInfoPtr, item)
		REFLECTION_REGISTER_FIELD(B2CostumeServerInfoPtr, costume)
		REFLECTION_REGISTER_FIELD(B2TotemServerInfoPtr, totem)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2MailInfo)
		REFLECTION_REGISTER_FIELD(int64, id)
		REFLECTION_REGISTER_FIELD(int32, mail_category)
		REFLECTION_REGISTER_FIELD(int32, mail_attachType)
		REFLECTION_REGISTER_FIELD(B2MailItemInfoPtr, mail_item_info)
		REFLECTION_REGISTER_FIELD(int32, sender_type)
		REFLECTION_REGISTER_FIELD(int32, source_type)
		REFLECTION_REGISTER_FIELD(FString, sender_name)
		REFLECTION_REGISTER_FIELD(int64, expire_time)
		REFLECTION_REGISTER_FIELD(int32, gold)
		REFLECTION_REGISTER_FIELD(int32, gem)
		REFLECTION_REGISTER_FIELD(int32, blade_point)
		REFLECTION_REGISTER_FIELD(int32, social_point)
		REFLECTION_REGISTER_FIELD(TArray<B2MailItemInfoPtr>, selective_item_infos)
		REFLECTION_REGISTER_FIELD(int32, general_lottery_id)
		REFLECTION_REGISTER_FIELD(int32, fixed_grade_lottery_id)
		REFLECTION_REGISTER_FIELD(int32, selective_lottery_id)
		REFLECTION_REGISTER_FIELD(int32, hero_piece)
		REFLECTION_REGISTER_FIELD(int32, rank_promotion_point)
		REFLECTION_REGISTER_FIELD(int32, dark_crystal)
		REFLECTION_REGISTER_FIELD(int32, stage_boost_ticket)
		REFLECTION_REGISTER_FIELD(int32, hot_time_gold_buff_ticket)
		REFLECTION_REGISTER_FIELD(int32, hot_time_exp_buff_ticket)
		REFLECTION_REGISTER_FIELD(int32, shop_ten_lottery_ticket)
		REFLECTION_REGISTER_FIELD(FString, message)
		REFLECTION_REGISTER_FIELD(int32, package_id)
		REFLECTION_REGISTER_FIELD(int32, fame_point)
		REFLECTION_REGISTER_FIELD(int32, selective_aether_lottery_id)
		REFLECTION_REGISTER_FIELD(int32, prefix_selective_lottery_id)
		REFLECTION_REGISTER_FIELD(int32, stage_auto_clear_ticket)
		REFLECTION_REGISTER_FIELD(int32, totem_prefix_lottery_id)
	REFLECTION_REGISTER_END(31)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2KillMonsterInfo)
		REFLECTION_REGISTER_FIELD(int32, monster_id)
		REFLECTION_REGISTER_FIELD(int32, monster_grade)
		REFLECTION_REGISTER_FIELD(int32, kill_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2QuestInfo)
		REFLECTION_REGISTER_FIELD(int32, slot)
		REFLECTION_REGISTER_FIELD(int32, type)
		REFLECTION_REGISTER_FIELD(int32, quest_id)
		REFLECTION_REGISTER_FIELD(int32, progress_count)
		REFLECTION_REGISTER_FIELD(int32, state)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdCounterDungeon)
		REFLECTION_REGISTER_FIELD(int32, dungeon_id)
		REFLECTION_REGISTER_FIELD(int32, dungeon_type)
		REFLECTION_REGISTER_FIELD(int32, difficulty)
		REFLECTION_REGISTER_FIELD(int32, difficulty_level)
		REFLECTION_REGISTER_FIELD(int32, difficulty_scale)
		REFLECTION_REGISTER_FIELD(int32, recommend_power)
		REFLECTION_REGISTER_FIELD(int32, recommend_attack)
		REFLECTION_REGISTER_FIELD(int32, recommend_defense)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardRandomCountPtr>, rewards)
		REFLECTION_REGISTER_FIELD(int32, drop_gold_min)
		REFLECTION_REGISTER_FIELD(int32, drop_gold_max)
	REFLECTION_REGISTER_END(11)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdCounterDungeonPeriod)
		REFLECTION_REGISTER_FIELD(int32, dungeon_type)
		REFLECTION_REGISTER_FIELD(int32, day_of_week)
		REFLECTION_REGISTER_FIELD(int32, begin_hour)
		REFLECTION_REGISTER_FIELD(int32, begin_min)
		REFLECTION_REGISTER_FIELD(int32, end_hour)
		REFLECTION_REGISTER_FIELD(int32, end_min)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdCounterDungeonWeekReward)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, require_count)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(3)

	REFLECTION_REGISTER_BEGIN(B2mdDimensionInfo)                                  
		REFLECTION_REGISTER_FIELD(int32, difficulty_level)                   
		REFLECTION_REGISTER_FIELD(int32, difficulty_scale)                   
		REFLECTION_REGISTER_FIELD(int32, fixed_drop_item_id1)                
		REFLECTION_REGISTER_FIELD(int32, fixed_drop_count1)                  
		REFLECTION_REGISTER_FIELD(int32, drop_totem_count_min)               
		REFLECTION_REGISTER_FIELD(int32, drop_totem_count_max)               
		REFLECTION_REGISTER_FIELD(int32, recommend_power)                    
		REFLECTION_REGISTER_FIELD(int32, recommend_attack)                   
		REFLECTION_REGISTER_FIELD(int32, recommend_defense)                  
	REFLECTION_REGISTER_END(9)                                                
                                                                            
	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDimensionWeekReward)                        
		REFLECTION_REGISTER_FIELD(int32, index)                              
		REFLECTION_REGISTER_FIELD(int32, ranking_min)                        
		REFLECTION_REGISTER_FIELD(int32, ranking_max)                        
		REFLECTION_REGISTER_FIELD(TArray<B2RawRewardPtr>, rewards)
	REFLECTION_REGISTER_END(4)


	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2Guild)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
		REFLECTION_REGISTER_FIELD(FString, guild_name)
		REFLECTION_REGISTER_FIELD(int64, master_account_id)
		REFLECTION_REGISTER_FIELD(FString, master_name)
		REFLECTION_REGISTER_FIELD(int32, max_member_count)
		REFLECTION_REGISTER_FIELD(FString, notice)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, exp)
		REFLECTION_REGISTER_FIELD(int32, join_option)
		REFLECTION_REGISTER_FIELD(int32, mark_index)
		REFLECTION_REGISTER_FIELD(int32, mark_color)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_index)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_color)
		REFLECTION_REGISTER_FIELD(int32, cur_member_count)
		REFLECTION_REGISTER_FIELD(int64, create_time)
		REFLECTION_REGISTER_FIELD(int32, season_obtain_medal)
		REFLECTION_REGISTER_FIELD(int32, season_ranking)
		REFLECTION_REGISTER_FIELD(int32, season_ranking_total)
		REFLECTION_REGISTER_FIELD(int32, members_avg_power)
		REFLECTION_REGISTER_FIELD(int32, total_result)
		REFLECTION_REGISTER_FIELD(bool, join_guild_battle)
		REFLECTION_REGISTER_FIELD(int32, turn_obtain_medal)
		REFLECTION_REGISTER_FIELD(int64, master_elected_time)
	REFLECTION_REGISTER_END(23)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildMember)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
		REFLECTION_REGISTER_FIELD(int32, contribution)
		REFLECTION_REGISTER_FIELD(int32, rank)
		REFLECTION_REGISTER_FIELD(B2CharacterSummaryPtr, character_summary)
		REFLECTION_REGISTER_FIELD(int64, last_logout_time)
		REFLECTION_REGISTER_FIELD(int32, total_support_count)
		REFLECTION_REGISTER_FIELD(int32, today_support_count)
		REFLECTION_REGISTER_FIELD(int32, total_donation_count)
		REFLECTION_REGISTER_FIELD(int32, today_donation_count)
		REFLECTION_REGISTER_FIELD(int64, raid_penalty_remove_time)
	REFLECTION_REGISTER_END(10)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdMissionInfo)
		REFLECTION_REGISTER_FIELD(int32, mission_id)
		REFLECTION_REGISTER_FIELD(int32, mission_type)
		REFLECTION_REGISTER_FIELD(int32, event_type)
		REFLECTION_REGISTER_FIELD(int32, condition1)
		REFLECTION_REGISTER_FIELD(int32, condition2)
		REFLECTION_REGISTER_FIELD(int32, condition3)
		REFLECTION_REGISTER_FIELD(int32, condition4)
		REFLECTION_REGISTER_FIELD(int32, goal_count)
		REFLECTION_REGISTER_FIELD(int32, complete_point)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(11)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSerialMissionInfo)
		REFLECTION_REGISTER_FIELD(int32, mission_id)
		REFLECTION_REGISTER_FIELD(int32, mission_step)
		REFLECTION_REGISTER_FIELD(int32, event_type)
		REFLECTION_REGISTER_FIELD(int32, condition1)
		REFLECTION_REGISTER_FIELD(int32, condition2)
		REFLECTION_REGISTER_FIELD(int32, condition3)
		REFLECTION_REGISTER_FIELD(int32, condition4)
		REFLECTION_REGISTER_FIELD(int32, goal_count)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(10)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdMissionCompletePointInfo)
		REFLECTION_REGISTER_FIELD(int32, mission_type)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, require_point)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2MissionInfo)
		REFLECTION_REGISTER_FIELD(int32, mission_type)
		REFLECTION_REGISTER_FIELD(int32, mission_state_type)
		REFLECTION_REGISTER_FIELD(int32, mission_id)
		REFLECTION_REGISTER_FIELD(int32, progress_count)
		REFLECTION_REGISTER_FIELD(int32, completed_step)
		REFLECTION_REGISTER_FIELD(int32, rewarded_step)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2MissionCompletePointInfo)
		REFLECTION_REGISTER_FIELD(int32, mission_type)
		REFLECTION_REGISTER_FIELD(int32, completePoint)
		REFLECTION_REGISTER_FIELD(TArray<int32>, reward_indexs)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdWingEnhance)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, need_item_template_id)
		REFLECTION_REGISTER_FIELD(int32, need_item_count)
		REFLECTION_REGISTER_FIELD(int32, enhance_point)
		REFLECTION_REGISTER_FIELD(int32, need_enhance_point)
		REFLECTION_REGISTER_FIELD(int32, gold_cost)
		REFLECTION_REGISTER_FIELD(int32, option_type1)
		REFLECTION_REGISTER_FIELD(float, option_value1)
		REFLECTION_REGISTER_FIELD(int32, option_type2)
		REFLECTION_REGISTER_FIELD(float, option_value2)
		REFLECTION_REGISTER_FIELD(int32, option_type3)
		REFLECTION_REGISTER_FIELD(float, option_value3)
	REFLECTION_REGISTER_END(13)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdWingUpgrade)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, need_item_template_id)
		REFLECTION_REGISTER_FIELD(int32, need_item_count)
		REFLECTION_REGISTER_FIELD(int32, rate)
		REFLECTION_REGISTER_FIELD(int32, gold_cost)
		REFLECTION_REGISTER_FIELD(int32, fail_point_max)
		REFLECTION_REGISTER_FIELD(int32, option_type1)
		REFLECTION_REGISTER_FIELD(float, option_value1)
		REFLECTION_REGISTER_FIELD(int32, option_type2)
		REFLECTION_REGISTER_FIELD(float, option_value2)
		REFLECTION_REGISTER_FIELD(int32, option_type3)
		REFLECTION_REGISTER_FIELD(float, option_value3)
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdWingOptionValue)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, option_index)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, option_type)
		REFLECTION_REGISTER_FIELD(float, option_value)
		REFLECTION_REGISTER_FIELD(int32, need_item_template_id)
		REFLECTION_REGISTER_FIELD(int32, need_item_count)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdWingOptionOpen)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, option_index)
		REFLECTION_REGISTER_FIELD(int32, need_grade)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdWingOptionMaxLevel)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, option_index)
		REFLECTION_REGISTER_FIELD(int32, wing_grade)
		REFLECTION_REGISTER_FIELD(int32, max_level)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdHeroTowerFloor)
		REFLECTION_REGISTER_FIELD(int32, floor)
		REFLECTION_REGISTER_FIELD(int32, recommend_power)
		REFLECTION_REGISTER_FIELD(int32, recommend_attack)
		REFLECTION_REGISTER_FIELD(int32, recommend_defense)
		REFLECTION_REGISTER_FIELD(int32, clear_limit_time)
		REFLECTION_REGISTER_FIELD(int32, map_id)
		REFLECTION_REGISTER_FIELD(int32, difficulty_scale)
		REFLECTION_REGISTER_FIELD(int32, monster_level)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, first_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(10)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdRelicInfo)
		REFLECTION_REGISTER_FIELD(int32, relic_id)
		REFLECTION_REGISTER_FIELD(int32, open_stage_id)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdRelicGradeInfo)
		REFLECTION_REGISTER_FIELD(int32, relic_id)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, option_id_1)
		REFLECTION_REGISTER_FIELD(int32, option_value_1)
		REFLECTION_REGISTER_FIELD(int32, option_id_2)
		REFLECTION_REGISTER_FIELD(int32, option_value_2)
		REFLECTION_REGISTER_FIELD(int32, option_id_3)
		REFLECTION_REGISTER_FIELD(int32, option_value_3)
		REFLECTION_REGISTER_FIELD(int32, promotion_rate)
		REFLECTION_REGISTER_FIELD(int32, promotion_gold)
		REFLECTION_REGISTER_FIELD(int32, promotion_boss_piece)
		REFLECTION_REGISTER_FIELD(int32, boss_piece_id)
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdRelicLevelInfo)
		REFLECTION_REGISTER_FIELD(int32, relic_id)
		REFLECTION_REGISTER_FIELD(int32, enhance_level)
		REFLECTION_REGISTER_FIELD(int32, mode_option_id_1)
		REFLECTION_REGISTER_FIELD(int32, mode_option_value_1)
		REFLECTION_REGISTER_FIELD(int32, mode_option_id_2)
		REFLECTION_REGISTER_FIELD(int32, mode_option_value_2)
		REFLECTION_REGISTER_FIELD(int32, mode_option_id_3)
		REFLECTION_REGISTER_FIELD(int32, mode_option_value_3)
		REFLECTION_REGISTER_FIELD(int32, enhance_rate)
		REFLECTION_REGISTER_FIELD(int32, need_gold)
		REFLECTION_REGISTER_FIELD(int32, need_hero_piece)
	REFLECTION_REGISTER_END(11)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopCashBonus)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, increase_rate)
		REFLECTION_REGISTER_FIELD(int64, start_time)
		REFLECTION_REGISTER_FIELD(int64, end_time)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopFundsSale)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, discount_rate)
		REFLECTION_REGISTER_FIELD(int64, start_time)
		REFLECTION_REGISTER_FIELD(int64, end_time)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2MagicShop)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(bool, is_purchased)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ShopLottery)
		REFLECTION_REGISTER_FIELD(int32, draw_price_type)
		REFLECTION_REGISTER_FIELD(bool, is_continuous)
		REFLECTION_REGISTER_FIELD(int64, next_free_time)
		REFLECTION_REGISTER_FIELD(int32, lottery_count)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopLotteryPrice)
		REFLECTION_REGISTER_FIELD(bool, is_continuous)
	    REFLECTION_REGISTER_FIELD(bool, is_again)
		REFLECTION_REGISTER_FIELD(int32, gem)
		REFLECTION_REGISTER_FIELD(int32, calculated_gem)
		REFLECTION_REGISTER_FIELD(int32, social_point)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopLotteryPossibleGrade)
		REFLECTION_REGISTER_FIELD(int32, draw_price_type)
		REFLECTION_REGISTER_FIELD(bool, is_continuous)
		REFLECTION_REGISTER_FIELD(int32, min_grade)
		REFLECTION_REGISTER_FIELD(int32, max_grade)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopLotteryDailyLimit)
		REFLECTION_REGISTER_FIELD(int32, draw_price_type)
		REFLECTION_REGISTER_FIELD(bool, is_continuous)
		REFLECTION_REGISTER_FIELD(int32, limit_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopLotterySale)
		REFLECTION_REGISTER_FIELD(int32, draw_price_type)
		REFLECTION_REGISTER_FIELD(bool, is_continuous)
		REFLECTION_REGISTER_FIELD(int32, discount_rate)
		REFLECTION_REGISTER_FIELD(int64, start_time)
		REFLECTION_REGISTER_FIELD(int64, end_time)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopLotterySpecialGrade)
		REFLECTION_REGISTER_FIELD(int32, min_grade)
		REFLECTION_REGISTER_FIELD(int32, max_grade)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdItemInfo)
		REFLECTION_REGISTER_FIELD(int32, md_template_id)
		REFLECTION_REGISTER_FIELD(int32, md_group_id)
		REFLECTION_REGISTER_FIELD(int32, md_set_id)
		REFLECTION_REGISTER_FIELD(int32, md_type)
		REFLECTION_REGISTER_FIELD(int32, md_grade)
		REFLECTION_REGISTER_FIELD(int32, md_inventory_type)
		REFLECTION_REGISTER_FIELD(int32, md_equip_position)
		REFLECTION_REGISTER_FIELD(int32, md_max_levelup)
		REFLECTION_REGISTER_FIELD(int32, md_main_attribute)
		REFLECTION_REGISTER_FIELD(float, md_level_factor)
		REFLECTION_REGISTER_FIELD(int32, md_unique_option_id1)
		REFLECTION_REGISTER_FIELD(float, md_unique_option_value1)
		REFLECTION_REGISTER_FIELD(int32, md_unique_option_id2)
		REFLECTION_REGISTER_FIELD(float, md_unique_option_value2)
		REFLECTION_REGISTER_FIELD(int32, md_unique_option_id3)
		REFLECTION_REGISTER_FIELD(float, md_unique_option_value3)
		REFLECTION_REGISTER_FIELD(int32, md_unique_option_id4)
		REFLECTION_REGISTER_FIELD(float, md_unique_option_value4)
		REFLECTION_REGISTER_FIELD(int32, md_unique_skill_id)
		REFLECTION_REGISTER_FIELD(int32, seal_slot)
		REFLECTION_REGISTER_FIELD(int32, seal_slot_open)
		REFLECTION_REGISTER_FIELD(int32, md_max_grade)
		REFLECTION_REGISTER_FIELD(int32, md_random_option_count)
		REFLECTION_REGISTER_FIELD(int32, md_next_grade_template_id)
		REFLECTION_REGISTER_FIELD(int32, md_buy_price)
		REFLECTION_REGISTER_FIELD(int32, md_sell_price)
		REFLECTION_REGISTER_FIELD(bool, is_stackable)
		REFLECTION_REGISTER_FIELD(bool, is_saleable)
		REFLECTION_REGISTER_FIELD(bool, is_equipable)
		REFLECTION_REGISTER_FIELD(bool, is_levelupable)
		REFLECTION_REGISTER_FIELD(bool, is_enhanceable)
		REFLECTION_REGISTER_FIELD(bool, is_upgradable)
		REFLECTION_REGISTER_FIELD(bool, is_composable)
		REFLECTION_REGISTER_FIELD(bool, is_surpassable)
		REFLECTION_REGISTER_FIELD(bool, is_sealable)
	REFLECTION_REGISTER_END(35)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdCostumeInfo)
		REFLECTION_REGISTER_FIELD(int32, md_template_id)
		REFLECTION_REGISTER_FIELD(int32, md_group_id)
		REFLECTION_REGISTER_FIELD(int32, md_set_id)
		REFLECTION_REGISTER_FIELD(int32, md_type)
		REFLECTION_REGISTER_FIELD(int32, md_grade)
		REFLECTION_REGISTER_FIELD(int32, md_inventory_type)
		REFLECTION_REGISTER_FIELD(int32, md_equip_position)
		REFLECTION_REGISTER_FIELD(int32, md_max_levelup)
		REFLECTION_REGISTER_FIELD(int32, md_main_attribute)
		REFLECTION_REGISTER_FIELD(float, md_level_factor)
		REFLECTION_REGISTER_FIELD(int32, md_unique_option_id1)
		REFLECTION_REGISTER_FIELD(float, md_unique_option_value1)
		REFLECTION_REGISTER_FIELD(int32, md_unique_option_id2)
		REFLECTION_REGISTER_FIELD(float, md_unique_option_value2)
		REFLECTION_REGISTER_FIELD(int32, md_unique_option_id3)
		REFLECTION_REGISTER_FIELD(float, md_unique_option_value3)
		REFLECTION_REGISTER_FIELD(int32, md_unique_option_id4)
		REFLECTION_REGISTER_FIELD(float, md_unique_option_value4)
		REFLECTION_REGISTER_FIELD(int32, md_unique_skill_id)
		REFLECTION_REGISTER_FIELD(int32, md_max_grade)
		REFLECTION_REGISTER_FIELD(int32, md_random_option_count)
		REFLECTION_REGISTER_FIELD(int32, md_next_grade_template_id)
		REFLECTION_REGISTER_FIELD(int32, md_buy_price)
		REFLECTION_REGISTER_FIELD(int32, md_sell_price)
		REFLECTION_REGISTER_FIELD(bool, is_stackable)
		REFLECTION_REGISTER_FIELD(bool, is_saleable)
		REFLECTION_REGISTER_FIELD(bool, is_equipable)
		REFLECTION_REGISTER_FIELD(bool, is_levelupable)
		REFLECTION_REGISTER_FIELD(bool, is_enhanceable)
		REFLECTION_REGISTER_FIELD(bool, is_upgradable)
		REFLECTION_REGISTER_FIELD(bool, is_composable)
		REFLECTION_REGISTER_FIELD(bool, is_surpassable)
	REFLECTION_REGISTER_END(32)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ShopProduct)
		REFLECTION_REGISTER_FIELD(int32, product_type)
		REFLECTION_REGISTER_FIELD(int32, product_id)
		REFLECTION_REGISTER_FIELD(int32, product_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ShopMileage)
		REFLECTION_REGISTER_FIELD(int32, shop_mileage_type)
		REFLECTION_REGISTER_FIELD(int32, mileage)
		REFLECTION_REGISTER_FIELD(int32, reward_index)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ShopBuyManage)
		REFLECTION_REGISTER_FIELD(int32, product_id)
		REFLECTION_REGISTER_FIELD(int32, buy_count)
		REFLECTION_REGISTER_FIELD(int64, updated_time)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ShopProductMark)
		REFLECTION_REGISTER_FIELD(int32, product_id)
		REFLECTION_REGISTER_FIELD(int32, mark_type)
		REFLECTION_REGISTER_FIELD(int64, sales_start_time)
		REFLECTION_REGISTER_FIELD(int64, sales_end_time)
	REFLECTION_REGISTER_END(4)
		
	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopProduct)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, shop_type)
		REFLECTION_REGISTER_FIELD(int32, shop_order)
		REFLECTION_REGISTER_FIELD(int32, product_type)
		REFLECTION_REGISTER_FIELD(int32, product_order)
		REFLECTION_REGISTER_FIELD(int32, product_id)
		REFLECTION_REGISTER_FIELD(int32, product_count)
		REFLECTION_REGISTER_FIELD(int32, bonus_product_type)
		REFLECTION_REGISTER_FIELD(int32, bonus_product_id)
		REFLECTION_REGISTER_FIELD(int32, bonus_product_count)
		REFLECTION_REGISTER_FIELD(int32, calculated_bonus_count)
		REFLECTION_REGISTER_FIELD(int32, buy_price_type)
		REFLECTION_REGISTER_FIELD(int32, price)
		REFLECTION_REGISTER_FIELD(int32, calculated_price)
		REFLECTION_REGISTER_FIELD(int32, restric_type)
		REFLECTION_REGISTER_FIELD(int32, buy_limit_count)
	REFLECTION_REGISTER_END(16)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopPackage)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, category)
		REFLECTION_REGISTER_FIELD(int32, tab_type)
		REFLECTION_REGISTER_FIELD(FString, name)
		REFLECTION_REGISTER_FIELD(int32, pay_period)
		REFLECTION_REGISTER_FIELD(int32, period_bonus_reward_id)
		REFLECTION_REGISTER_FIELD(int32, period_bonus_reward_count)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopPackageProduct)
		REFLECTION_REGISTER_FIELD(int32, package_id)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(B2ShopProductPtr, product)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopMileageReward)
		REFLECTION_REGISTER_FIELD(int32, mileage_type)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, mileage)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2CollectionItem)
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
		REFLECTION_REGISTER_FIELD(int32, collect_state)
		REFLECTION_REGISTER_FIELD(bool, is_new_item)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2CollectionSetItem)
		REFLECTION_REGISTER_FIELD(int32, group_id)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, character_type)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdItemSet)
		REFLECTION_REGISTER_FIELD(int32, set_id)
		REFLECTION_REGISTER_FIELD(FString, name)
		REFLECTION_REGISTER_FIELD(int32, set_option_id_set2)
		REFLECTION_REGISTER_FIELD(float, set_value_set2)
		REFLECTION_REGISTER_FIELD(int32, set_option_id_set3)
		REFLECTION_REGISTER_FIELD(float, set_value_set3)
		REFLECTION_REGISTER_FIELD(int32, set_option_id_set4)
		REFLECTION_REGISTER_FIELD(float, set_value_set4)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSelectItemOption)
		REFLECTION_REGISTER_FIELD(int32, option_id)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, equip_position)
		REFLECTION_REGISTER_FIELD(float, prob)
		REFLECTION_REGISTER_FIELD(int32, decimal_point)
		REFLECTION_REGISTER_FIELD(float, min)
		REFLECTION_REGISTER_FIELD(float, max)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSelectItemSealOption)
		REFLECTION_REGISTER_FIELD(int32, option_id)
		REFLECTION_REGISTER_FIELD(int32, slot)
		REFLECTION_REGISTER_FIELD(float, prob)
		REFLECTION_REGISTER_FIELD(int32, decimal_point)
		REFLECTION_REGISTER_FIELD(float, min)
		REFLECTION_REGISTER_FIELD(float, max)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdItemSealCost)
		REFLECTION_REGISTER_FIELD(int32, slot)
		REFLECTION_REGISTER_FIELD(int32, open_price_type)
		REFLECTION_REGISTER_FIELD(int32, open_price)
		REFLECTION_REGISTER_FIELD(int32, template_id1)
		REFLECTION_REGISTER_FIELD(int32, amount1)
		REFLECTION_REGISTER_FIELD(int32, template_id2)
		REFLECTION_REGISTER_FIELD(int32, amount2)
		REFLECTION_REGISTER_FIELD(int32, template_id3)
		REFLECTION_REGISTER_FIELD(int32, amount3)
		REFLECTION_REGISTER_FIELD(int32, template_id4)
		REFLECTION_REGISTER_FIELD(int32, amount4)
		REFLECTION_REGISTER_FIELD(int32, seal_price_type)
		REFLECTION_REGISTER_FIELD(int32, seal_price)
	REFLECTION_REGISTER_END(13)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdCollectionReward)
		REFLECTION_REGISTER_FIELD(int32, collection_category)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2RaidRankingReward)
		REFLECTION_REGISTER_FIELD(int32, ranking)	
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
		REFLECTION_REGISTER_FIELD(int32, item_count)
		REFLECTION_REGISTER_FIELD(bool, is_bonus)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2DimensionStatus)                            
		REFLECTION_REGISTER_FIELD(int32, best_difficulty_level)               
		REFLECTION_REGISTER_FIELD(int32, best_clear_time_millis)              
		REFLECTION_REGISTER_FIELD(int32, best_character_type)                 
		REFLECTION_REGISTER_FIELD(int32, best_character_level)                
		REFLECTION_REGISTER_FIELD(int32, best_total_power)                    
		REFLECTION_REGISTER_FIELD(int32, best_record_time)                    
		REFLECTION_REGISTER_FIELD(int32, top_difficulty_level)                
		REFLECTION_REGISTER_FIELD(int32, last_difficulty_level)               
		REFLECTION_REGISTER_FIELD(int32, last_character_type)                 
		REFLECTION_REGISTER_FIELD(int32, daily_clear_count)                   
		REFLECTION_REGISTER_FIELD(int32, daily_clear_count_max)               
		REFLECTION_REGISTER_FIELD(int32, daily_count_init_time)               
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDailyAttendanceInfo)
		REFLECTION_REGISTER_FIELD(int32, attendance_type)
		REFLECTION_REGISTER_FIELD(int32, day)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildBattleCharacterSlot)
		REFLECTION_REGISTER_FIELD(int32, slot_num)
		REFLECTION_REGISTER_FIELD(int32, entry_code)
		REFLECTION_REGISTER_FIELD(int32, entry_type)
		REFLECTION_REGISTER_FIELD(int32, level)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildBattleEntry)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildBattleCharacterSlotPtr>, slots)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildBattleMember)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(FString, nickname)
		REFLECTION_REGISTER_FIELD(int32, power_index)
		REFLECTION_REGISTER_FIELD(int32, left_attack)
		REFLECTION_REGISTER_FIELD(int32, left_medal)
		REFLECTION_REGISTER_FIELD(int32, season_obtain_medal)
		REFLECTION_REGISTER_FIELD(int32, total_power)
		REFLECTION_REGISTER_FIELD(B2GuildBattleEntryPtr, entry)
		REFLECTION_REGISTER_FIELD(bool, is_attacked)
		REFLECTION_REGISTER_FIELD(TArray<B2CharacterPowerPtr>, character_powers)
	REFLECTION_REGISTER_END(10)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildBattleAttack)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(int32, attack_count)
		REFLECTION_REGISTER_FIELD(int32, power_index)
		REFLECTION_REGISTER_FIELD(FString, nickname)
		REFLECTION_REGISTER_FIELD(FString, target_nickname)
		REFLECTION_REGISTER_FIELD(int32, target_power_index)
		REFLECTION_REGISTER_FIELD(int32, obtain_medal)
		REFLECTION_REGISTER_FIELD(int64, battle_end_time)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildBattleGuild)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
		REFLECTION_REGISTER_FIELD(FString, guild_name)
		REFLECTION_REGISTER_FIELD(int32, mark_index)
		REFLECTION_REGISTER_FIELD(int32, mark_color)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_index)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_color)
		REFLECTION_REGISTER_FIELD(int32, obtain_medal)
		REFLECTION_REGISTER_FIELD(int32, attack_count)
		REFLECTION_REGISTER_FIELD(int32, guild_level)
		REFLECTION_REGISTER_FIELD(int32, guild_exp)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildBattleMemberPtr>, members)
		REFLECTION_REGISTER_FIELD(TArray<B2GuildBattleAttackPtr>, attacks)
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGuildBattleScheduleInfo)
		REFLECTION_REGISTER_FIELD(int32, turn)
		REFLECTION_REGISTER_FIELD(int32, state)
		REFLECTION_REGISTER_FIELD(int32, start_day)
		REFLECTION_REGISTER_FIELD(int32, start_hour)
		REFLECTION_REGISTER_FIELD(int32, start_min)
		REFLECTION_REGISTER_FIELD(int32, end_day)
		REFLECTION_REGISTER_FIELD(int32, end_hour)
		REFLECTION_REGISTER_FIELD(int32, end_min)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGuildBattleResultReward)
		REFLECTION_REGISTER_FIELD(int32, reward_grade)
		REFLECTION_REGISTER_FIELD(int32, req_count_min)
		REFLECTION_REGISTER_FIELD(int32, req_count_max)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGuildBattleSeasonReward)
		REFLECTION_REGISTER_FIELD(int32, order)
		REFLECTION_REGISTER_FIELD(int32, ranking_min)
		REFLECTION_REGISTER_FIELD(int32, ranking_max)
		REFLECTION_REGISTER_FIELD(float, ranking_rate_min)
		REFLECTION_REGISTER_FIELD(float, ranking_rate_max)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildBattleState)
		REFLECTION_REGISTER_FIELD(int32, season_id)
		REFLECTION_REGISTER_FIELD(int32, turn)
		REFLECTION_REGISTER_FIELD(int32, current_state)
		REFLECTION_REGISTER_FIELD(int64, begin_time)
		REFLECTION_REGISTER_FIELD(int64, end_time)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildBattleHistory)
		REFLECTION_REGISTER_FIELD(int32, obtain_medal)
		REFLECTION_REGISTER_FIELD(int32, medal_total_count)
		REFLECTION_REGISTER_FIELD(int32, result)
		REFLECTION_REGISTER_FIELD(int64, battle_end_time)
		REFLECTION_REGISTER_FIELD(int64, target_guild_id)
		REFLECTION_REGISTER_FIELD(FString, target_guild_name)
		REFLECTION_REGISTER_FIELD(int32, target_obtain_medal)
		REFLECTION_REGISTER_FIELD(int32, target_mark)
		REFLECTION_REGISTER_FIELD(int32, target_mark_color)
		REFLECTION_REGISTER_FIELD(int32, target_mark_bg)
		REFLECTION_REGISTER_FIELD(int32, target_mark_bg_color)
		REFLECTION_REGISTER_FIELD(int32, target_medal_total_count)
		REFLECTION_REGISTER_FIELD(int32, member_count)
		REFLECTION_REGISTER_FIELD(int32, attack_count)
		REFLECTION_REGISTER_FIELD(int32, target_member_count)
		REFLECTION_REGISTER_FIELD(int32, target_attack_count)
	REFLECTION_REGISTER_END(16)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildBattleRecord)
		REFLECTION_REGISTER_FIELD(int32, win_count)
		REFLECTION_REGISTER_FIELD(int32, draw_count)
		REFLECTION_REGISTER_FIELD(int32, lose_count)
		REFLECTION_REGISTER_FIELD(int32, total_obtain_medal)
		REFLECTION_REGISTER_FIELD(int32, ranking)
		REFLECTION_REGISTER_FIELD(int32, ranking_total_count)
		REFLECTION_REGISTER_FIELD(int32, season_id)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildRanking)
		REFLECTION_REGISTER_FIELD(int32, ranking)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
		REFLECTION_REGISTER_FIELD(FString, guild_name)
		REFLECTION_REGISTER_FIELD(int32, total_obtain_medal)
		REFLECTION_REGISTER_FIELD(int32, total_result)
		REFLECTION_REGISTER_FIELD(int32, members_avg_power)
		REFLECTION_REGISTER_FIELD(int64, create_time)
		REFLECTION_REGISTER_FIELD(int32, mark)
		REFLECTION_REGISTER_FIELD(int32, mark_color)
		REFLECTION_REGISTER_FIELD(int32, mark_bg)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_color)
		REFLECTION_REGISTER_FIELD(FString, gm_nickname)
		REFLECTION_REGISTER_FIELD(int32, max_member_count)
		REFLECTION_REGISTER_FIELD(int32, cur_member_count)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, exp)
	REFLECTION_REGISTER_END(16)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGuildSkill)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, upgrade_cost_gold)
		REFLECTION_REGISTER_FIELD(int32, upgrade_req_guild_level)
		REFLECTION_REGISTER_FIELD(int32, buy_cost_gold)
		REFLECTION_REGISTER_FIELD(int32, buff_option_type)
		REFLECTION_REGISTER_FIELD(float, buff_option_value)
		REFLECTION_REGISTER_FIELD(int32, buff_time_sec)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGuildExp)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, exp_max)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdRaidInfo)
		REFLECTION_REGISTER_FIELD(int32, raid_type)
		REFLECTION_REGISTER_FIELD(int32, step)
		REFLECTION_REGISTER_FIELD(int32, difficulty_scale)
		REFLECTION_REGISTER_FIELD(int32, recommend_power)
		REFLECTION_REGISTER_FIELD(int32, recommend_attack)
		REFLECTION_REGISTER_FIELD(int32, recommend_defense)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_min_count)
		REFLECTION_REGISTER_FIELD(int32, reward_max_count)
		REFLECTION_REGISTER_FIELD(int32, main_reward_id)
		REFLECTION_REGISTER_FIELD(int32, main_reward_min_count)
		REFLECTION_REGISTER_FIELD(int32, main_reward_max_count)
		REFLECTION_REGISTER_FIELD(int32, reward_item_min_grade)
		REFLECTION_REGISTER_FIELD(int32, reward_item_max_grade)
	REFLECTION_REGISTER_END(14)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdMonsterInfo)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(FString, name)
		REFLECTION_REGISTER_FIELD(FString, symbol)
		REFLECTION_REGISTER_FIELD(FString, grade)
		REFLECTION_REGISTER_FIELD(FString, type)
		REFLECTION_REGISTER_FIELD(FString, desc)
		REFLECTION_REGISTER_FIELD(int32, hp_scale)
		REFLECTION_REGISTER_FIELD(int32, attack_defense_level_scale)
		REFLECTION_REGISTER_FIELD(int32, max_armor_override)
	REFLECTION_REGISTER_END(9)

	REFLECTION_REGISTER_BEGIN(B2GuildMemberCandidate)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(int32, invite_state)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildInvite)
		REFLECTION_REGISTER_FIELD(B2GuildPtr, guild)
		REFLECTION_REGISTER_FIELD(int64, expire_time)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGeneralLottery)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, equip_category)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSelectiveLottery)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, equip_category)
		REFLECTION_REGISTER_FIELD(int32, item_grade)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdFixedGradeLottery)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, equip_category)
		REFLECTION_REGISTER_FIELD(int32, item_grade)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdPrefixSelectiveLottery)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, group_id)
		REFLECTION_REGISTER_FIELD(int32, equip_category)
		REFLECTION_REGISTER_FIELD(int32, item_grade)
		REFLECTION_REGISTER_FIELD(int32, surpass_level)
		REFLECTION_REGISTER_FIELD(int32, quality)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdAdditionalMatchCost)
		REFLECTION_REGISTER_FIELD(int32, count)
		REFLECTION_REGISTER_FIELD(int32, cost)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2MatchOpenPeriodInfo)
		REFLECTION_REGISTER_FIELD(int32, day_of_week)
		REFLECTION_REGISTER_FIELD(int32, begin_hour)
		REFLECTION_REGISTER_FIELD(int32, begin_min)
		REFLECTION_REGISTER_FIELD(int32, end_hour)
		REFLECTION_REGISTER_FIELD(int32, end_min)
		REFLECTION_REGISTER_FIELD(int32, raid_type)
		REFLECTION_REGISTER_FIELD(int32, assault_type)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdStageFixedDrop)
		REFLECTION_REGISTER_FIELD(int32, stage_id)
		REFLECTION_REGISTER_FIELD(int32, idx)
		REFLECTION_REGISTER_FIELD(int32, template_id)
		REFLECTION_REGISTER_FIELD(int32, min)
		REFLECTION_REGISTER_FIELD(int32, max)
		REFLECTION_REGISTER_FIELD(int32, rate)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2DropData)
		REFLECTION_REGISTER_FIELD(int32, template_id)
		REFLECTION_REGISTER_FIELD(int32, count)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2DuelRewardBoxStatus)
		REFLECTION_REGISTER_FIELD(bool, is_reward_state)
		REFLECTION_REGISTER_FIELD(int32, box_grade)
		REFLECTION_REGISTER_FIELD(int32, win_count)
		REFLECTION_REGISTER_FIELD(int32, upgrade_win_count)
		REFLECTION_REGISTER_FIELD(int32, defeat_count)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2DuelRewardBoxInfoResponseRewardBox)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, win_count)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2DuelRewardBoxGainableListResponseRewardItem)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, min_count)
		REFLECTION_REGISTER_FIELD(int32, max_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdAssaultVictoryRewardInfo)
		REFLECTION_REGISTER_FIELD(int32, match_result)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdAssaultMvpRewardInfo)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_min_count)
		REFLECTION_REGISTER_FIELD(int32, reward_max_count)
	REFLECTION_REGISTER_END(3)


	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdEventAttendance)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(FString, title)
		REFLECTION_REGISTER_FIELD(int32, total_attendance_days)
		REFLECTION_REGISTER_FIELD(int64, start_time)
		REFLECTION_REGISTER_FIELD(int64, end_time)
		REFLECTION_REGISTER_FIELD(FString, explain)
		REFLECTION_REGISTER_FIELD(FString, event_url)
		REFLECTION_REGISTER_FIELD(int32, bonus_type)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdEventAttendanceReward)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, day)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDailyPlayTimeReward)
		REFLECTION_REGISTER_FIELD(int32, reward_index)
		REFLECTION_REGISTER_FIELD(int32, play_time_inmin)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2EventAttendanceStatus)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, attendance_day)
		REFLECTION_REGISTER_FIELD(int64, last_attendance_time)
		REFLECTION_REGISTER_FIELD(int64, next_attendance_time)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2BeginnerAttendanceStatus)
		REFLECTION_REGISTER_FIELD(int32, total_attendance_days)
		REFLECTION_REGISTER_FIELD(int32, attendance_day)
		REFLECTION_REGISTER_FIELD(int64, last_attendance_time)
		REFLECTION_REGISTER_FIELD(int64, next_attendance_time)
		REFLECTION_REGISTER_FIELD(int64, finish_time)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ActIdentifier)
		REFLECTION_REGISTER_FIELD(int32, difficulty)
		REFLECTION_REGISTER_FIELD(int32, act_id)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildInfo)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
		REFLECTION_REGISTER_FIELD(FString, guild_name)
		REFLECTION_REGISTER_FIELD(int32, mark_index)
		REFLECTION_REGISTER_FIELD(int32, mark_color)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_index)
		REFLECTION_REGISTER_FIELD(int32, mark_bg_color)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2IncreaseCharacterExpInfo)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, exp)
		REFLECTION_REGISTER_FIELD(int32, exp_max)
		REFLECTION_REGISTER_FIELD(bool, level_changed)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdTutorialInfo)
		REFLECTION_REGISTER_FIELD(int32, tutorial_id)
		REFLECTION_REGISTER_FIELD(int32, prev_tutorial_id)
		REFLECTION_REGISTER_FIELD(int32, character_level)
		REFLECTION_REGISTER_FIELD(int32, act_cleared)
		REFLECTION_REGISTER_FIELD(int32, cash)
		REFLECTION_REGISTER_FIELD(int32, gold)
		REFLECTION_REGISTER_FIELD(int32, reward_id1)
		REFLECTION_REGISTER_FIELD(int32, reward_count1)
		REFLECTION_REGISTER_FIELD(int32, reward_id2)
		REFLECTION_REGISTER_FIELD(int32, reward_count2)
		REFLECTION_REGISTER_FIELD(int32, reward_id3)
		REFLECTION_REGISTER_FIELD(int32, reward_count3)
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdAetherOptionInfo)
		REFLECTION_REGISTER_FIELD(int32, option_id)
		REFLECTION_REGISTER_FIELD(int32, option_group)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(float, default_value)
		REFLECTION_REGISTER_FIELD(float, increase_value_per_level)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdAetherSlotInfo)
		REFLECTION_REGISTER_FIELD(int32, slot_id)
		REFLECTION_REGISTER_FIELD(int32, option_id)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdAetherEnhanceInfo)
		REFLECTION_REGISTER_FIELD(int32, enhance_level)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, aether_piece_cost)
		REFLECTION_REGISTER_FIELD(int32, enhance_rate)
		REFLECTION_REGISTER_FIELD(int32, dismantle_amount)
		REFLECTION_REGISTER_FIELD(int32, sell_price)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdAetherEnhanceOptionInfo)
		REFLECTION_REGISTER_FIELD(int32, enhance_level)
		REFLECTION_REGISTER_FIELD(int32, tier)
		REFLECTION_REGISTER_FIELD(int32, state_main_option)
		REFLECTION_REGISTER_FIELD(int32, state_sub_option)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdAetherSetOptionInfo)
		REFLECTION_REGISTER_FIELD(int32, type)
		REFLECTION_REGISTER_FIELD(int32, tier)
		REFLECTION_REGISTER_FIELD(int32, set_option_id)
		REFLECTION_REGISTER_FIELD(int32, min_apply_count)
		REFLECTION_REGISTER_FIELD(int32, trigger_rate)
		REFLECTION_REGISTER_FIELD(int32, cooldown_second)
		REFLECTION_REGISTER_FIELD(int32, damage_rate)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDuelModeReward)
		REFLECTION_REGISTER_FIELD(int32, duel_mode)
		REFLECTION_REGISTER_FIELD(int32, match_result)
		REFLECTION_REGISTER_FIELD(TArray<B2RewardPtr>, rewards)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdRaidExp)
		REFLECTION_REGISTER_FIELD(int32, raid_type)
		REFLECTION_REGISTER_FIELD(int32, step)
		REFLECTION_REGISTER_FIELD(int32, exp)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildChatData)
		REFLECTION_REGISTER_FIELD(int64, id)
		REFLECTION_REGISTER_FIELD(int64, guild_id)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(FString, nickname)
		REFLECTION_REGISTER_FIELD(int64, create_time)
		REFLECTION_REGISTER_FIELD(int32, chat_type)
		REFLECTION_REGISTER_FIELD(FString, message)
		REFLECTION_REGISTER_FIELD(int32, chat_notice_type)
		REFLECTION_REGISTER_FIELD(TArray<FString>, notice_args)
		REFLECTION_REGISTER_FIELD(int32, support_item_template_id)
		REFLECTION_REGISTER_FIELD(int32, support_count)
		REFLECTION_REGISTER_FIELD(int32, support_max_count)
		REFLECTION_REGISTER_FIELD(int64, update_time)
		REFLECTION_REGISTER_FIELD(int64, expire_time)
		REFLECTION_REGISTER_FIELD(int32, character_level_max)
	REFLECTION_REGISTER_END(15)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGuildSupportItem)
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
		REFLECTION_REGISTER_FIELD(int32, support_max_count)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2UnitySkillMission)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, unity_character_type)
		REFLECTION_REGISTER_FIELD(int32, step)
		REFLECTION_REGISTER_FIELD(int32, state)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdUnitySkillMission)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, unity_character_type)
		REFLECTION_REGISTER_FIELD(int32, step)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, event_type)
		REFLECTION_REGISTER_FIELD(int32, condition1)
		REFLECTION_REGISTER_FIELD(int32, condition2)
		REFLECTION_REGISTER_FIELD(int32, condition3)
		REFLECTION_REGISTER_FIELD(int32, condition4)
		REFLECTION_REGISTER_FIELD(int32, goal_count)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2UnitySkillAwakenMission)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, mission_id)
		REFLECTION_REGISTER_FIELD(int32, try_count)
		REFLECTION_REGISTER_FIELD(bool, completed)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdUnitySkillAwakenMission)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, mission_id)
		REFLECTION_REGISTER_FIELD(int32, req_item_id_1)
		REFLECTION_REGISTER_FIELD(int32, req_item_count_1)
		REFLECTION_REGISTER_FIELD(int32, req_item_id_2)
		REFLECTION_REGISTER_FIELD(int32, req_item_count_2)
		REFLECTION_REGISTER_FIELD(int32, req_gold)
		REFLECTION_REGISTER_FIELD(int32, success_rate)
		REFLECTION_REGISTER_FIELD(int32, success_rate_add)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdRestReward)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(float, reward_count_per_point)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GuildMercenary)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, exp)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGuildMercenary)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, mastery_type)
		REFLECTION_REGISTER_FIELD(int32, max_armor_override)
		REFLECTION_REGISTER_FIELD(int32, skill_id_1)
		REFLECTION_REGISTER_FIELD(int32, skill_id_2)
		REFLECTION_REGISTER_FIELD(int32, skill_id_3)
		REFLECTION_REGISTER_FIELD(int32, max_use_count)
		REFLECTION_REGISTER_FIELD(bool, is_usable)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGuildMercenarySkill)
		REFLECTION_REGISTER_FIELD(int32, skill_id)
		REFLECTION_REGISTER_FIELD(int32, require_mercenary_level)
		REFLECTION_REGISTER_FIELD(int32, cool_time_sec)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGuildMercenaryMastery)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, max_exp)
		REFLECTION_REGISTER_FIELD(int32, option_id_1)
		REFLECTION_REGISTER_FIELD(float, option_value_1)
		REFLECTION_REGISTER_FIELD(int32, option_id_2)
		REFLECTION_REGISTER_FIELD(float, option_value_2)
		REFLECTION_REGISTER_FIELD(int32, option_id_3)
		REFLECTION_REGISTER_FIELD(float, option_value_3)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGuildMercenaryDonation)
		REFLECTION_REGISTER_FIELD(int32, donation_type)
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
		REFLECTION_REGISTER_FIELD(int32, donation_amount)
		REFLECTION_REGISTER_FIELD(int32, exp_increment)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2AssaultBattleInfo)
		REFLECTION_REGISTER_FIELD(int32, kill_count)
		REFLECTION_REGISTER_FIELD(int32, death_count)
		REFLECTION_REGISTER_FIELD(int32, damage)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2RoomCharacter)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, character_power)
		REFLECTION_REGISTER_FIELD(int32, character_level)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2AdminMessage)
        REFLECTION_REGISTER_FIELD(FString, message)
        REFLECTION_REGISTER_FIELD(int32, display_time)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2PointEventStatus)
		REFLECTION_REGISTER_FIELD(int32, event_id)
        REFLECTION_REGISTER_FIELD(int32, point)
        REFLECTION_REGISTER_FIELD(int32, reward_state_value)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdEventInfo)
		REFLECTION_REGISTER_FIELD(int32, event_id)
        REFLECTION_REGISTER_FIELD(int32, event_category)
        REFLECTION_REGISTER_FIELD(FString, event_tab_name)
        REFLECTION_REGISTER_FIELD(FString, title)
        REFLECTION_REGISTER_FIELD(int64, start_time)
        REFLECTION_REGISTER_FIELD(int64, end_time)
        REFLECTION_REGISTER_FIELD(FString, explain)
        REFLECTION_REGISTER_FIELD(FString, banner_url)
        REFLECTION_REGISTER_FIELD(FString, link_text)
        REFLECTION_REGISTER_FIELD(FString, link_url)
		REFLECTION_REGISTER_FIELD(int32, tab_order)
		REFLECTION_REGISTER_FIELD(int32, event_sub_category)
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdPointEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
        REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, event_type)
		REFLECTION_REGISTER_FIELD(int32, condition1)
		REFLECTION_REGISTER_FIELD(int32, condition2)
		REFLECTION_REGISTER_FIELD(int32, condition3)
		REFLECTION_REGISTER_FIELD(int32, condition4)
        REFLECTION_REGISTER_FIELD(int32, point)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdPointEventReward)
		REFLECTION_REGISTER_FIELD(int32, event_id)
        REFLECTION_REGISTER_FIELD(int32, index)
        REFLECTION_REGISTER_FIELD(int32, reward_id)
        REFLECTION_REGISTER_FIELD(int32, reward_count)
        REFLECTION_REGISTER_FIELD(int32, need_point)
		REFLECTION_REGISTER_FIELD(int32, tab_group_id)
		REFLECTION_REGISTER_FIELD(int32, buy_limit_count)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2LevelUpEventInfo)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, state)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdLevelUpEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdHotTimeEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, day_of_week)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, buff_type)
		REFLECTION_REGISTER_FIELD(int32, begin_hour)
		REFLECTION_REGISTER_FIELD(int32, begin_min)
		REFLECTION_REGISTER_FIELD(int32, end_hour)
		REFLECTION_REGISTER_FIELD(int32, end_min)
		REFLECTION_REGISTER_FIELD(int32, increase_rate)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSeasonMissionEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id) 
		REFLECTION_REGISTER_FIELD(int32, day)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, event_type)
		REFLECTION_REGISTER_FIELD(int32, condition1)
		REFLECTION_REGISTER_FIELD(int32, condition2)
		REFLECTION_REGISTER_FIELD(int32, condition3)
		REFLECTION_REGISTER_FIELD(int32, condition4)
		REFLECTION_REGISTER_FIELD(int32, goal_count)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
		REFLECTION_REGISTER_FIELD(int32, tutorial_id)
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2SeasonMissionInfo)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, day)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, progress_count)
		REFLECTION_REGISTER_FIELD(int32, state)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2SeasonMissionEventStatus)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, attendance_day)
		REFLECTION_REGISTER_FIELD(int64, next_attendance_time)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2StageClearEventStatus)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, state)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdStageClearEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, stage)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdKakaoFriendInviteReward)
		REFLECTION_REGISTER_FIELD(int32, invite_reward_type)
		REFLECTION_REGISTER_FIELD(int32, reward_index)
		REFLECTION_REGISTER_FIELD(int32, goal_count)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdHotTimeProduct)
		REFLECTION_REGISTER_FIELD(int32, buff_type)
		REFLECTION_REGISTER_FIELD(int32, period_hour)
		REFLECTION_REGISTER_FIELD(int32, price)
		REFLECTION_REGISTER_FIELD(int32, increaseRate)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdMagicShopProduct)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, magic_shop_group_type)
		REFLECTION_REGISTER_FIELD(int32, product_type)
		REFLECTION_REGISTER_FIELD(int32, product_detail)
		REFLECTION_REGISTER_FIELD(int32, product_count)
		REFLECTION_REGISTER_FIELD(int32, price_type)
		REFLECTION_REGISTER_FIELD(int32, price)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdMagicShopRenewalCost)
		REFLECTION_REGISTER_FIELD(int32, renewal_count)
		REFLECTION_REGISTER_FIELD(int32, gem_cost)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2RollingBanner)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, type)
		REFLECTION_REGISTER_FIELD(int32, sub_type)
		REFLECTION_REGISTER_FIELD(FString, banner_img_url)
		REFLECTION_REGISTER_FIELD(int64, start_time)
		REFLECTION_REGISTER_FIELD(int64, end_time)
		REFLECTION_REGISTER_FIELD(int32, banner_duration)
		REFLECTION_REGISTER_FIELD(FString, web_url)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2MarketPurchaseInfo)
		REFLECTION_REGISTER_FIELD(int32, product_id)
		REFLECTION_REGISTER_FIELD(FString, market_product_id)
		REFLECTION_REGISTER_FIELD(FString, developer_payload)
		REFLECTION_REGISTER_FIELD(int32, purchase_state)
		REFLECTION_REGISTER_FIELD(int32, bonus_increase_rate)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdMarketProduct)
		REFLECTION_REGISTER_FIELD(int32, shop_product_id)
		REFLECTION_REGISTER_FIELD(FString, market_product_id)
		REFLECTION_REGISTER_FIELD(FString, currency_code)
		REFLECTION_REGISTER_FIELD(float, price)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2FlatRatePackageBonusState)
		REFLECTION_REGISTER_FIELD(int32, current_index)
		REFLECTION_REGISTER_FIELD(bool, weekly_state)
		REFLECTION_REGISTER_FIELD(bool, monthly_state)
		REFLECTION_REGISTER_FIELD(bool, reward_state)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDuelModeSettleSchedule)
		REFLECTION_REGISTER_FIELD(int32, mode_type)
		REFLECTION_REGISTER_FIELD(int32, ranking_settle_state)
		REFLECTION_REGISTER_FIELD(int32, day)
		REFLECTION_REGISTER_FIELD(int32, end_hour)
		REFLECTION_REGISTER_FIELD(int32, end_min)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2PeriodicPackageState)
		REFLECTION_REGISTER_FIELD(int32, package_id)
		REFLECTION_REGISTER_FIELD(int64, end_pay_time)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2LevelUpPackageMission)
		REFLECTION_REGISTER_FIELD(int32, package_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, state)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ReachLevelPackageMission)
		REFLECTION_REGISTER_FIELD(int32, package_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, state)
		REFLECTION_REGISTER_FIELD(int64, expire_time)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopLevelUpPackage)
		REFLECTION_REGISTER_FIELD(int32, package_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopReachLevelPackage)
		REFLECTION_REGISTER_FIELD(int32, package_id)
		REFLECTION_REGISTER_FIELD(int32, character_type)
		REFLECTION_REGISTER_FIELD(int32, reach_level)
		REFLECTION_REGISTER_FIELD(int32, limit_level)
		REFLECTION_REGISTER_FIELD(int32, buy_reward_id)
		REFLECTION_REGISTER_FIELD(int32, buy_reward_count)
		REFLECTION_REGISTER_FIELD(int32, complete_reward_id)
		REFLECTION_REGISTER_FIELD(int32, complete_reward_count)
		REFLECTION_REGISTER_FIELD(int32, expire_day)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopFlatRatePackageBonus)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdShopPackageTab)
		REFLECTION_REGISTER_FIELD(int32, tab_type)
		REFLECTION_REGISTER_FIELD(int32, sort_order)
		REFLECTION_REGISTER_FIELD(bool, is_open)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdUnitySkillOption)
		REFLECTION_REGISTER_FIELD(int32, main_character_type)
		REFLECTION_REGISTER_FIELD(int32, unity_character_type)
		REFLECTION_REGISTER_FIELD(int32, step)
		REFLECTION_REGISTER_FIELD(int32, option_id)
		REFLECTION_REGISTER_FIELD(double, option_value)
		REFLECTION_REGISTER_FIELD(double, power)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2BuyShopResultInfo)
		REFLECTION_REGISTER_FIELD(TArray<B2ShopProductPtr>, add_products)
		REFLECTION_REGISTER_FIELD(TArray<B2ShopBuyManagePtr>, shop_buy_manages)
		REFLECTION_REGISTER_FIELD(int32, buy_product_type)
		REFLECTION_REGISTER_FIELD(B2PeriodicPackageStatePtr, periodic_package_state)
		REFLECTION_REGISTER_FIELD(B2FlatRatePackageBonusStatePtr, flat_rate_package_bonus_state)
		REFLECTION_REGISTER_FIELD(TArray<B2LevelUpPackageMissionPtr>, level_up_package_missions)
		REFLECTION_REGISTER_FIELD(B2ReachLevelPackageMissionPtr, reach_level_package_mission)
		REFLECTION_REGISTER_FIELD(int32, mileage_type)
		REFLECTION_REGISTER_FIELD(int32, current_mileage)
		REFLECTION_REGISTER_FIELD(TArray<B2CostumeServerInfoPtr>, costumes)
		REFLECTION_REGISTER_FIELD(TArray<B2TotemServerInfoPtr>, totems)
	REFLECTION_REGISTER_END(11)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ContentsModeInfo)
		REFLECTION_REGISTER_FIELD(int32, mode)
		REFLECTION_REGISTER_FIELD(int32, state)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdWebUrl)
		REFLECTION_REGISTER_FIELD(FString, key)
		REFLECTION_REGISTER_FIELD(FString, url)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2PointShopEventStatus)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, point)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2PointShopEventRewardHistory)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, total_reward_count)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2BlockChatUser)
		REFLECTION_REGISTER_FIELD(int64, account_id)
		REFLECTION_REGISTER_FIELD(FString, nickname)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2TenLotteryEventStatus)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, lottery_count)
		REFLECTION_REGISTER_FIELD(TArray<int32>, rewarded_indexs)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdTenLotteryEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, goal_lottery_count)
		REFLECTION_REGISTER_FIELD(TArray<B2RawRewardPtr>, rewards)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSpendGemEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, goal_spend_count)
		REFLECTION_REGISTER_FIELD(TArray<B2RawRewardPtr>, rewards)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2SpendGemEventStatus)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, spend_gem_count)
		REFLECTION_REGISTER_FIELD(int32, reward_state_value)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdGemPurchaseEventReward)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, goal_count)
		REFLECTION_REGISTER_FIELD(TArray<B2RawRewardPtr>, rewards)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2GemPurchaseEventStatus)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, gem_count)
		REFLECTION_REGISTER_FIELD(int32, reward_state_value)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdTenLotterySavingsEvent)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, gain_point)
		REFLECTION_REGISTER_FIELD(int32, need_point)
		REFLECTION_REGISTER_FIELD(TArray<B2RawRewardPtr>, rewards)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDiceEventMission)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, event_type)
		REFLECTION_REGISTER_FIELD(int32, goal_count)
		REFLECTION_REGISTER_FIELD(int32, point)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDiceEventCell)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, cell_number)
		REFLECTION_REGISTER_FIELD(int32, cell_type)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
		REFLECTION_REGISTER_FIELD(int32, effect_value)
	REFLECTION_REGISTER_END(6)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDiceEventFinishReward)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, number)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, reward_count)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2DiceEventMission)
		REFLECTION_REGISTER_FIELD(int32, event_type)
		REFLECTION_REGISTER_FIELD(int32, progress_count)
		REFLECTION_REGISTER_FIELD(int32, state)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2DiceEventStatus)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, point)
		REFLECTION_REGISTER_FIELD(int32, free_dice)
		REFLECTION_REGISTER_FIELD(int32, position)
		REFLECTION_REGISTER_FIELD(TArray<B2DiceEventMissionPtr>, missions)
		REFLECTION_REGISTER_FIELD(int32, finish_count)
		REFLECTION_REGISTER_FIELD(int32, buy_point_count)
		REFLECTION_REGISTER_FIELD(int32, roll_dice_count)
		REFLECTION_REGISTER_FIELD(int32, mileage_reward_value)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2ItemSpotSaleOption)
		REFLECTION_REGISTER_FIELD(TArray<int32>, inventory_types)
		REFLECTION_REGISTER_FIELD(int32, item_grade)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2SoldItemInfo)
		REFLECTION_REGISTER_FIELD(int32, template_id)
		REFLECTION_REGISTER_FIELD(int32, quality)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2AetherSpotSaleOption)
		REFLECTION_REGISTER_FIELD(int32, aether_grade)
	REFLECTION_REGISTER_END(1)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2SoldAetherInfo)
		REFLECTION_REGISTER_FIELD(int32, aether_grade)
		REFLECTION_REGISTER_FIELD(int32, tier)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDiceEventBuyPoint)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, buy_count)
		REFLECTION_REGISTER_FIELD(int32, gem)
		REFLECTION_REGISTER_FIELD(int32, point)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDiceEventMileage)
		REFLECTION_REGISTER_FIELD(int32, event_id)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(int32, dice_count)
		REFLECTION_REGISTER_FIELD(TArray<B2RawRewardPtr>, rewards)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2DonationPointRewardState)
		REFLECTION_REGISTER_FIELD(int32, index)
		REFLECTION_REGISTER_FIELD(bool, rewarded)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2DonationPresent)
		REFLECTION_REGISTER_FIELD(int32, item_template_id)
		REFLECTION_REGISTER_FIELD(int32, amount)
		REFLECTION_REGISTER_FIELD(int32, donation_consumable_type)	// 기부 보상 : 소모품 타입 정보 (UNDEFINED 이면 장비 아이템)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDonationEventInfo)
		REFLECTION_REGISTER_FIELD(int32, donation_id)
		REFLECTION_REGISTER_FIELD(int32, cost_type)
		REFLECTION_REGISTER_FIELD(int32, cost)
		REFLECTION_REGISTER_FIELD(int32, extra_cost)
		REFLECTION_REGISTER_FIELD(int32, cut_off_point)
		REFLECTION_REGISTER_FIELD(int32, obtain_point)
		REFLECTION_REGISTER_FIELD(int32, bundle_cost)
		REFLECTION_REGISTER_FIELD(int32, extra_bundle_cost)
		REFLECTION_REGISTER_FIELD(int32, bundle_obtain_point)
		REFLECTION_REGISTER_FIELD(int32, serial_donation_count)
		REFLECTION_REGISTER_FIELD(int64, start_time)
		REFLECTION_REGISTER_FIELD(int64, end_time)
		REFLECTION_REGISTER_FIELD(int64, expire_time)
		REFLECTION_REGISTER_FIELD(FString, title)
		REFLECTION_REGISTER_FIELD(FString, description)
		REFLECTION_REGISTER_FIELD(FString, banner_url)
		REFLECTION_REGISTER_FIELD(FString, link_text)
		REFLECTION_REGISTER_FIELD(FString, link_url)
		REFLECTION_REGISTER_FIELD(int32, donation_template_id)
	REFLECTION_REGISTER_END(19)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDonationPointRankingRewardInfo)
		REFLECTION_REGISTER_FIELD(int32, donation_id)
		REFLECTION_REGISTER_FIELD(int32, rank)
		REFLECTION_REGISTER_FIELD(int32, reward_id1)
		REFLECTION_REGISTER_FIELD(int32, reward_count1)
		REFLECTION_REGISTER_FIELD(int32, reward_id2)
		REFLECTION_REGISTER_FIELD(int32, reward_count2)
		REFLECTION_REGISTER_FIELD(int32, reward_id3)
		REFLECTION_REGISTER_FIELD(int32, reward_count3)
		REFLECTION_REGISTER_FIELD(int32, reward_id4)
		REFLECTION_REGISTER_FIELD(int32, reward_count4)
		REFLECTION_REGISTER_FIELD(int32, reward_id5)
		REFLECTION_REGISTER_FIELD(int32, reward_count5)
	REFLECTION_REGISTER_END(12)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDonationBundleFixedRewardInfo)
		REFLECTION_REGISTER_FIELD(int32, donation_id)
		REFLECTION_REGISTER_FIELD(int32, reward_id)
		REFLECTION_REGISTER_FIELD(int32, min_amount)
		REFLECTION_REGISTER_FIELD(int32, max_amount)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdDonationPointStepRewardInfo)
		REFLECTION_REGISTER_FIELD(int32, donation_id)
		REFLECTION_REGISTER_FIELD(int32, step)
		REFLECTION_REGISTER_FIELD(int32, point)
		REFLECTION_REGISTER_FIELD(int32, reward_id1)
		REFLECTION_REGISTER_FIELD(int32, reward_count1)
		REFLECTION_REGISTER_FIELD(int32, reward_id2)
		REFLECTION_REGISTER_FIELD(int32, reward_count2)
		REFLECTION_REGISTER_FIELD(int32, reward_id3)
		REFLECTION_REGISTER_FIELD(int32, reward_count3)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdFairyInfo)
		REFLECTION_REGISTER_FIELD(int32, fairy_type)
		REFLECTION_REGISTER_FIELD(int32, option_id)
		REFLECTION_REGISTER_FIELD(int32, bless_level)
		REFLECTION_REGISTER_FIELD(float, option_value)
		REFLECTION_REGISTER_FIELD(int32, level)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdFairyLevelupCost)
		REFLECTION_REGISTER_FIELD(int32, level)
		REFLECTION_REGISTER_FIELD(int32, material_cost)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdFairyOpenInfo)
		REFLECTION_REGISTER_FIELD(int32, fairy_type)
		REFLECTION_REGISTER_FIELD(int32, stage_id)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2CharacterPowerSpecific)
		REFLECTION_REGISTER_FIELD(int32, category)
		REFLECTION_REGISTER_FIELD(int32, power)
	REFLECTION_REGISTER_END(2)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdTotemInfo)
		REFLECTION_REGISTER_FIELD(int32, template_id)
		REFLECTION_REGISTER_FIELD(int32, group_id)
		REFLECTION_REGISTER_FIELD(int32, bonus_id)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, sell_price)
		REFLECTION_REGISTER_FIELD(int32, equip_slot)
		REFLECTION_REGISTER_FIELD(int32, refine_limit_count1)
		REFLECTION_REGISTER_FIELD(int32, refine_limit_count2)
		REFLECTION_REGISTER_FIELD(int32, refine_limit_count3)
	REFLECTION_REGISTER_END(9)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdSelectTotemOption)
		REFLECTION_REGISTER_FIELD(int32, option_id)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(float, min_main_value)
		REFLECTION_REGISTER_FIELD(float, max_main_value)
		REFLECTION_REGISTER_FIELD(float, min_sub_value)
		REFLECTION_REGISTER_FIELD(float, max_sub_value)
		REFLECTION_REGISTER_FIELD(int32, decimal_point)
	REFLECTION_REGISTER_END(7)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdTotemRefineInfo)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, refine_chance)
		REFLECTION_REGISTER_FIELD(int32, gold_cost)
		REFLECTION_REGISTER_FIELD(int32, dimensional_piece_cost)
	REFLECTION_REGISTER_END(4)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdTotemRefineRateInfo)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, base_rate)
		REFLECTION_REGISTER_FIELD(int32, gold_decrease_rate)
		REFLECTION_REGISTER_FIELD(int32, gold_increase_rate)
		REFLECTION_REGISTER_FIELD(int32, piece_decrease_rate)
		REFLECTION_REGISTER_FIELD(int32, piece_increase_rate)
		REFLECTION_REGISTER_FIELD(int32, min_rate)
		REFLECTION_REGISTER_FIELD(int32, max_rate)
	REFLECTION_REGISTER_END(8)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdTotemDismantleInfo)
		REFLECTION_REGISTER_FIELD(int32, refine_level)
		REFLECTION_REGISTER_FIELD(int32, grade)
		REFLECTION_REGISTER_FIELD(int32, amount)
	REFLECTION_REGISTER_END(3)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdTotemBonusOptionInfo)
		REFLECTION_REGISTER_FIELD(int32, bonus_id)
		REFLECTION_REGISTER_FIELD(int32, condition1)
		REFLECTION_REGISTER_FIELD(int32, bonus_option_id1)
		REFLECTION_REGISTER_FIELD(float, bonus_value1)
		REFLECTION_REGISTER_FIELD(int32, condition2)
		REFLECTION_REGISTER_FIELD(int32, bonus_option_id2)
		REFLECTION_REGISTER_FIELD(float, bonus_value2)
		REFLECTION_REGISTER_FIELD(int32, condition3)
		REFLECTION_REGISTER_FIELD(int32, bonus_option_id3)
		REFLECTION_REGISTER_FIELD(float, bonus_value3)
	REFLECTION_REGISTER_END(10)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdTotemPrefixLotteryInfo)
		REFLECTION_REGISTER_FIELD(int32, id)
		REFLECTION_REGISTER_FIELD(int32, base_prefix)
		REFLECTION_REGISTER_FIELD(int32, grade)
	REFLECTION_REGISTER_END(3)


	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdExchangeSystemInfo)
			REFLECTION_REGISTER_FIELD(int32, id)
			REFLECTION_REGISTER_FIELD(int32, template_id)
			REFLECTION_REGISTER_FIELD(int32, request_count)
			REFLECTION_REGISTER_FIELD(int32, exchange_template_id)
			REFLECTION_REGISTER_FIELD(int32, result_count)
	REFLECTION_REGISTER_END(5)

	//////////////////////////////////////////////////////////////////////////
	REFLECTION_REGISTER_BEGIN(B2mdExtendMessage)
		REFLECTION_REGISTER_FIELD(TArray<B2mdTotemInfoPtr>, md_totem_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSelectTotemOptionPtr>, md_select_totem_options)
		REFLECTION_REGISTER_FIELD(TArray<B2mdTotemRefineInfoPtr>, md_totem_refine_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdTotemRefineRateInfoPtr>, md_totem_refine_rate_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdTotemDismantleInfoPtr>, md_totem_dismantle_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdTotemBonusOptionInfoPtr>, md_totem_bonus_option_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDimensionInfoPtr>, md_dimension_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdDimensionWeekRewardPtr>, md_dimension_week_rewards)
		REFLECTION_REGISTER_FIELD(TArray<B2mdTotemPrefixLotteryInfoPtr>, md_totem_prefix_lottery_infos)
		REFLECTION_REGISTER_FIELD(TArray<B2mdItemSealCostPtr>, md_item_seal_cost)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSelectItemSealOptionPtr>, md_select_item_seal_option)
		REFLECTION_REGISTER_FIELD(TArray<B2mdCostumeEnhanceCostPtr>, md_costume_enhance_cost)
		REFLECTION_REGISTER_FIELD(TArray<B2mdExchangeSystemInfoPtr>, md_exchange_system_info)
		REFLECTION_REGISTER_FIELD(TArray<B2mdExchangeSystemCostPtr>, md_exchange_system_cost)
		REFLECTION_REGISTER_FIELD(TArray<B2mdQualityTransRateInfoPtr>, md_quality_trans_rate_info)
		REFLECTION_REGISTER_FIELD(TArray<B2mdLotteryShopRateQualityInfoPtr>, md_lottery_shop_rate_quality_info)
		REFLECTION_REGISTER_FIELD(TArray<B2mdFactoryInfoPtr>, md_factory_info)
		REFLECTION_REGISTER_FIELD(TArray<B2mdFactoryMaterialTransInfoPtr>, md_factory_material_trans_info)
		REFLECTION_REGISTER_FIELD(TArray<B2mdFactoryLimitInfoPtr>, md_factory_limit_info)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSealBoxInfoPtr>, md_seal_box_info)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSealBoxRewardInfoPtr>, md_seal_box_reward_info)
		REFLECTION_REGISTER_FIELD(TArray<B2mdSealBoxRewardRateInfoPtr>, md_seal_box_reward_rate_info)
	REFLECTION_REGISTER_END(22)
};