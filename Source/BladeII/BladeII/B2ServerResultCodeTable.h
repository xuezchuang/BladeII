#pragma once

#ifndef SERVER_RESULT_CODE_TABLE

namespace B2ResultCode
{	
	
	// Success result
	const int SUCCESS                                                      =     0;	// Ok
	const int SUCCESS_NEW_ACCOUNT                                          =     1;	// New account created.
	const int SUCCESS_ACHIEVE_NEW_RECORD                                   =     2;	// New record achieved.
	
	// System
	
	const int FAILURE_INTERNAL_ERROR                                       =    10;	// Server internal error
	const int FAILURE_INTERNAL_DATABASE_ERROR                              =    11;	// Server internal database error.
	const int FAILURE_TRY_AGAIN_ERROR                                      =    12;	// Try again later
	const int FAILURE_MASTER_DATA_ERROR                                    =    13;	// Master data not found
	const int FAILURE_INVALID_MARKET                                       =    14;	// InvalidMarketException error.
	const int FAILURE_MAPPER_PARAMETER_ERROR                               =    15;	// mapper parameter error.
	const int FAILURE_PARAMETERS_CHECKSUM_ERROR                            =    16;	// parameters checksum error.
	const int FAILURE_RANDOM_FAILURE_COUNT_INSERT                          =    17;	// random failure count couldn't be created.
	const int FAILURE_RANDOM_FAILURE_COUNT_UPDATE                          =    18;	// random failure count couldn't be updated.
	
	
	// platform api
	const int FAILURE_PLATFORM_API_ERROR                                   =    20;	// Platform server api processing error
	const int FAILURE_PLATFORM_API_REQUEST_ERROR                           =    21;	// Platform server api processing error, caused invalid requesting
	const int FAILURE_PLATFORM_API_SERVER_ERROR                            =    22;	// Platform server api processing error, caused server-side problem
	const int FAILURE_PLATFORM_NOT_ALLOWED_PLATFORM                        =    23;	// Not allowed request for the platform
	
	// Global Repository
	
	// Auth Database
	
	const int FAILURE_DUPLICATED_USER                                      =   100;	// UserId already exists.
	const int FAILURE_DUPLICATED_NICKNAME                                  =   101;	// Account Nickname already exists.
	const int FAILURE_ACCOUNT_NOT_FOUND                                    =   102;	// Account not found
	const int FAILURE_NO_ACCOUNT_NICKNAME                                  =   103;	// Account nickname is empty
	const int FAILURE_SESSION_TOKEN_NOT_FOUND                              =   110;	// Session Token not found
	const int FAILURE_SESSION_TOKEN_MISMATCH                               =   111;	// Session token mismatch
	const int FAILURE_SESSION_TOKEN_DUPLICATED                             =   112;	// Session token duplicated
	const int FAILURE_NICKNAME_LENGTH_NOT_AVAILABLE                        =   113;	// Nickname length is not available
	const int FAILURE_DISAPPROVED_ACCOUNT_WITHDRAW                         =   114;	// Disapproved account, it's withdrawing
	const int FAILURE_ACCOUNT_BLOCK_STATE                                  =   115;	// Account is blocked
	const int FAILURE_ACCOUNT_NOT_WITHDRAW_STATE                           =   116;	// Account is not withdraw state.
	
	// World Repository
	
	// Game Database
	
	// master data
	const int FAILURE_MASTER_DATA_LOAD                                     =   400;	// Not exists master data
	const int FAILURE_DUPLICATED_MD_KEY                                    =   401;	// Master data key already exist.
	
	const int FAILURE_DUPLICATED_MD_ITEM_DISMANTLE_INFO                    =   410;	// md_item_dismantle_info key already exists.
	const int FAILURE_NOT_EXIST_MD_ITEM_DISMANTLE_INFO                     =   411;	// not exist md_item_dismantle_info.
	const int FAILURE_DUPLICATED_MD_FAIRY_INFO                             =   412;	// md_fairy_info key already exists.
	const int FAILURE_NOT_EXIST_MD_FAIRY_INFO                              =   413;	// not exist md_fairy_info.
	const int FAILURE_DUPLICATED_MD_FAIRY_LEVELUP_COST                     =   414;	// md_fairy_levelup_cost key already exists.
	const int FAILURE_NOT_EXIST_MD_FAIRY_LEVELUP_COST                      =   415;	// not exist md_fairy_levelup_cost.
	const int FAILURE_DUPLICATED_MD_FAIRY_OPEN_INFO                        =   416;	// md_fairy_open_info key already exists.
	const int FAILURE_NOT_EXIST_MD_FAIRY_OPEN_INFO                         =   417;	// not exist md_fairy_open_info.
	
	const int FAILURE_DUPLICATED_MD_STAGE_MATERIAL_DROP                    =   429;	// md_stage_material_drop key already exist.
	const int FAILURE_NOT_EXIST_MD_STAGE_MATERIAL_DROP                     =   430;	// not exist md_stage_material_drop.
	const int FAILURE_DUPLICATED_MD_WEB_URL                                =   431;	// md_web_url key already exists.
	const int FAILURE_NOT_EXIST_MD_WEB_URL                                 =   432;	// not exist md_web_url.
	const int FAILURE_DUPLICATED_MD_STAGE_ITEM_QUALITY_INFO                =   433;	// md_stage_item_quality_info already exists.
	const int FAILURE_NOT_EXIST_MD_STAGE_ITEM_QUALITY_INFO                 =   434;	// not exist md_stage_item_quality_info.
	const int FAILURE_DUPLICATED_MD_MARKET_PRODUCT                         =   435;	// md_market_product already exists.
	const int FAILURE_NOT_EXIST_MD_MARKET_PRODUCT                          =   436;	// not exist md_market_product.
	const int FAILURE_DUPLICATED_MD_ITEM_COMPOSE_QUALITY_RATE_INFO         =   437;	// md_item_compose_quality_rate already exists.
	const int FAILURE_NOT_EXIST_MD_ITEM_COMPOSE_QUALITY_RATE_INFO          =   438;	// not exist md_item_compose_quality_rate.
	const int FAILURE_DUPLICATED_MD_SHOP_LOTTERY_SALE_INFO                 =   439;	// md_lottery_shop_sale already exists.
	const int FAILURE_NOT_EXIST_MD_SHOP_LOTTERY_SALE_INFO                  =   440;	// not exist md_lottery_shop_sale.
	const int FAILURE_DUPLICATED_MD_SHOP_CASH_BONUS_INFO                   =   441;	// md_shop_cash_bonus already exists.
	const int FAILURE_NOT_EXIST_MD_SHOP_CASH_BONUS_INFO                    =   442;	// not exist md_shop_cash_bonus
	const int FAILURE_DUPLICATED_MD_SHOP_FUNDS_SALE_INFO                   =   443;	// md_shop_funds_sale already exists.
	const int FAILURE_NOT_EXIST_MD_SHOP_FUNDS_SALE_INFO                    =   444;	// not exist md_shop_funds_sale
	const int FAILURE_DUPLICATED_MD_HOT_TIME_PRODUCT                       =   445;	// md_hot_time_product already exists.
	const int FAILURE_NOT_EXIST_MD_HOT_TIME_PRODUCT                        =   446;	// not exist md_hot_time_product.
	const int FAILURE_DUPLICATED_MD_KAKAO_FRIEND_REWARD                    =   447;	// md_kakao_friend_reward already exists.
	const int FAILURE_NOT_EXIST_MD_KAKAO_FRIEND_REWARD                     =   448;	// not exist md_kakao_friend_reward.
	const int FAILURE_DUPLICATED_MD_GUILD_BATTLE_MATCH_RANKING             =   449;	// md_guild_battle_match_ranking key already exist.
	const int FAILURE_NOT_EXIST_MD_GUILD_BATTLE_MATCH_RANKING              =   450;	// not exist md_guild_battle_match_ranking.
	const int FAILURE_DUPLICATED_MD_GUILD_MERCENARY_SKILL                  =   451;	// md_guild_mercenary_skill key already exist.
	const int FAILURE_NOT_EXIST_MD_GUILD_MERCENARY_SKILL                   =   452;	// not exist md_guild_mercenary_skill.
	const int FAILURE_DUPLICATED_MD_GUILD_MERCENARY_MASTERY                =   453;	// md_guild_mercenary_mastery key already exist.
	const int FAILURE_NOT_EXIST_MD_GUILD_MERCENARY_MASTERY                 =   454;	// not exist md_guild_mercenary_mastery.
	const int FAILURE_DUPLICATED_MD_GUILD_MERCENARY_DONATION               =   455;	// md_guild_mercenary_donation key already exist.
	const int FAILURE_NOT_EXIST_MD_GUILD_MERCENARY_DONAION                 =   456;	// not exist md_guild_mercenary_donation.
	const int FAILURE_DUPLICATED_MD_GUILD_MERCENARY                        =   457;	// md_guild_mercenary key already exist.
	const int FAILURE_NOT_EXIST_MD_GUILD_MERCENARY                         =   458;	// not exist md_guild_mercenary.
	const int FAILURE_DUPLICATED_MD_GUILD_BATTLE_PARTICIPATION_REWARD      =   459;	// md_guild_battle_reward_participation key already exists.
	const int FAILURE_NOT_EXIST_MD_GUILD_BATTLE_PARTICIPATION_REWARD       =   460;	// not exist md_guild_battle_reward_participation.
	const int FAILURE_DUPLICATED_MD_AETHER_SET_OPTION_INFO                 =   461;	// md_aether_set_option already exists.
	const int FAILURE_NOT_EXIST_MD_AETHER_SET_OPTION_INFO                  =   462;	// not exist md_aether_set_option.
	const int FAILURE_NOT_EXIST_MD_GUILD_SUPPORT_ITEM                      =   463;	// not exist md_guild_support_item.
	const int FAILURE_DUPLICATED_MD_GUILD_SUPPORT_ITEM                     =   464;	// md_guild_support_item already exists.
	const int FAILURE_NOT_EXIST_MD_STAGE_AETHER_REWARD_INFO                =   465;	// not exist md_stage_aether_reward.
	const int FAILURE_DUPLICATED_MD_AETHER_SLOT_INFO                       =   466;	// md_aether_slot already exists.
	const int FAILURE_NOT_EXIST_MD_AETHER_SLOT_INFO                        =   467;	// not exist md_aether_slot.
	const int FAILURE_DUPLICATED_MD_AETHER_OPTION_INFO                     =   468;	// md_aether_option already exists.
	const int FAILURE_NOT_EXIST_MD_AETHER_OPTION_INFO                      =   469;	// not exist md_aether_option.
	const int FAILURE_DUPLICATED_MD_AETHER_ENHANCE_INFO                    =   470;	// md_aether_enhance already exists.
	const int FAILURE_NOT_EXIST_MD_AETHER_ENHANCE_INFO                     =   471;	// not exist md_aether_enhance.
	const int FAILURE_DUPLICATED_MD_AETHER_ENHANCE_OPTION_INFO             =   472;	// md_aether_enhance_option already exists.
	const int FAILURE_NOT_EXIST_MD_AETHER_ENHANCE_OPTION_INFO              =   473;	// not exist md_aether_enhance_option.
	const int FAILURE_NOT_EXIST_MD_DAILY_PLAYTIME_REWARD                   =   474;	// not exist md_daily_reward_play_time.
	const int FAILURE_DUPLICATED_MD_DAILY_PLAYTIME_REWARD                  =   475;	// md_daily_reward_play_time key already exist.
	const int FAILURE_NOT_EXIST_MD_EVENT_ATTENDANCE_REWARD                 =   476;	// not exist md_event_attendance_reward.
	const int FAILURE_DUPLICATED_MD_EVENT_ATTENDANCE_REWARD                =   477;	// md_event_attendance_reward key already exist.
	const int FAILURE_NOT_EXIST_MD_EVENT_ATTENDANCE                        =   478;	// not exist md_event_attendance.
	const int FAILURE_DUPLICATED_MD_EVENT_ATTENDANCE                       =   479;	// md_event_attendance key already exist.
	const int FAILURE_DUPLICATED_MD_REWARD_BOX_INFO                        =   480;	// md_reward_box_duel key already exist.
	const int FAILURE_NOT_EXIST_MD_REWARD_BOX_INFO                         =   481;	// not exist md_reward_box_duel.
	const int FAILURE_DUPLICATED_MD_RANDOM_REWARD_GROUP_RATE               =   482;	// md_random_reward_group_rate key already exist.
	const int FAILURE_NOT_EXIST_MD_RANDOM_REWARD_GROUP_RATE                =   483;	// not exist md_random_reward_group_rate.
	const int FAILURE_DUPLICATED_MD_STAGE_FIXED_DROP                       =   484;	// md_stage_fixed_drop key already exist.
	const int FAILURE_NOT_EXIST_MD_STAGE_FIXED_DROP                        =   485;	// not exist md_stage_fixed_drop.
	const int FAILURE_DUPLICATED_MD_GUILD_EXP                              =   486;	// md_guild_exp key already exist.
	const int FAILURE_NOT_EXIST_MD_GUILD_EXP                               =   487;	// not exist md_guild_exp.
	const int FAILURE_DUPLICATED_MD_GUILD_SKILL                            =   488;	// md_guild_skill key already exist.
	const int FAILURE_NOT_EXIST_MD_GUILD_SKILL                             =   489;	// not exist md_guild_skill.
	const int FAILURE_DUPLICATED_MD_GUILD_BATTLE_SCHEDULE                  =   490;	// md_guild_battle_schedule key already exists.
	const int FAILURE_NOT_EXIST_MD_GUILD_BATTLE_SCHEDULE                   =   491;	// not exist md_guild_battle_schedule.
	const int FAILURE_DUPLICATED_MD_EVENT_SPEND_GEM_INFO                   =   492;	// md_event_sped_gem_reward key already exists.
	
	const int FAILURE_DUPLICATED_MD_GUILD_BATTLE_RESULT_REWARD             =   494;	// md_guild_battle_reward_result key already exists.
	const int FAILURE_NOT_EXIST_MD_GUILD_BATTLE_RESULT_REWARD              =   495;	// not exist md_guild_battle_reward_result.
	const int FAILURE_DUPLICATED_MD_GUILD_BATTLE_SEASON_REWARD             =   496;	// md_guild_battle_reward_season key already exists.
	const int FAILURE_NOT_EXIST_MD_GUILD_BATTLE_SEASON_REWARD              =   497;	// not exist md_guild_battle_reward_season.
	const int FAILURE_DUPLICATED_MD_ITEM_LEVELUP_FACTOR                    =   498;	// md_item_levelup_factor key already exists.
	const int FAILURE_NOT_EXIST_MD_ITEM_LEVELUP_FACTOR                     =   499;	// Not exist md_item_levelup_factor.
	
	const int FAILURE_DUPLICATED_MD_ITEMINFO                               =   500;	// md_item templateId already exists.
	const int FAILURE_NOT_EXIST_MD_ITEMINFO                                =   501;	// Not exist md_item.
	const int FAILURE_INVALID_MD_ITEM_TYPE                                 =   502;	// invalid md item type
	const int FAILURE_OVERFLOW_MD_ITEM_INFO                                =   503;	// overflow md item info
	
	const int FAILURE_DUPLICATED_MD_ITEM_SURPASS_COST                      =   508;	// md_item_surpass_cost key already exists.
	const int FAILURE_NOT_EXIST_MD_ITEM_SURPASS_COST                       =   509;	// Not exist md_item_surpass_cost.
	const int FAILURE_DUPLICATED_MD_ITEMOPTION                             =   510;	// md_item_option optionId already exists.
	const int FAILURE_NOT_EXIST_MD_ITEMOPTION                              =   511;	// Not exist md_item_option.
	const int FAILURE_DUPLICATED_MD_ITEM_SURPASS_LEVEL                     =   512;	// md_item_surpass_level key already exists.
	const int FAILURE_NOT_EXIST_MD_ITEM_SURPASS_LEVEL                      =   513;	// Not exist md_item_surpass_level.
	const int FAILURE_DUPLICATED_MD_SELECTIVE_AETHER_LOTTERY_INFO          =   514;	// md_lottery_selective_aether key already exists.
	const int FAILURE_NOT_EXIST_MD_SELECTIVE_AETHER_LOTTERY_INFO           =   515;	// not exist md_lottery_selective_aether.
	const int FAILURE_INVALID_MD_SELECTIVE_AETHER_LOTTERY_TIER_TYPE        =   516;	// invalid md_lottery_selective_aether tier type.
	const int FAILURE_DUPLICATED_MD_SELECTIVE_PREFIX_LOTTERY_INFO          =   517;	// md_lottery_selective_prefix key already exists.
	const int FAILURE_NOT_EXIST_MD_SELECTIVE_PREFIX_LOTTERY_INFO           =   518;	// not exist md_lottery_selective_prefix.
	const int FAILURE_INVALID_MD_SELECTIVE_PREFIX_LOTTERY_GROUPID          =   519;	// invalid md_lottery_selective_prefix groudId.
	
	const int FAILURE_DUPLICATED_MD_ITEMSETINFO                            =   520;	// md_item_set setId already exists.
	const int FAILURE_NOT_EXIST_MD_ITEMSETINFO                             =   521;	// Not exist md_item_set.
	
	const int FAILURE_DUPLICATED_MD_EVENT_DICE_MISSION                     =   522;	// md_event_dice_mission key already exist.
	const int FAILURE_DUPLICATED_MD_EVENT_DICE_CELL                        =   523;	// md_event_dice_cell key already exist.
	const int FAILURE_DUPLICATED_MD_EVENT_DICE_FINISH_REWARD               =   524;	// md_event_dice_finish_reward key already exist.
	const int FAILURE_NOT_EXIST_MD_EVENT_DICE_MISSION                      =   525;	// Not exist  md_event_dice_mission
	
	const int FAILURE_DUPLICATED_MD_CONFIG                                 =   530;	// md_config key already exists.
	const int FAILURE_NOT_EXIST_MD_CONFIG                                  =   531;	// Not exist config key.
	const int FAILURE_DUPLICATED_MD_SHOP_PACKAGE_TAB                       =   532;	// md_shop_package_tab exists.
	
	const int FAILURE_DUPLICATED_MD_ITEM_LEVELUP_EXP                       =   540;	// md_item_levelup_exp key already exists.
	const int FAILURE_NOT_EXIST_MD_ITEM_LEVELUP_EXP                        =   541;	// Not exist md_item_levelup_exp.
	
	const int FAILURE_DUPLICATED_MD_ITEM_LEVELUP_COST                      =   550;	// md_item_levelup_cost key already exists.
	const int FAILURE_NOT_EXIST_MD_ITEM_LEVELUP_COST                       =   551;	// Not exist md_item_levelup_cost.
	
	const int FAILURE_DUPLICATED_MD_ITEMUPGRADECOST                        =   560;	// md_item_upgrade_cost key already exists.
	const int FAILURE_NOT_EXIST_MD_ITEMUPGRADECOST                         =   561;	// Not exist md_item_upgrade_cost.
	const int FAILURE_DUPLICATED_MD_RAID_ITEM_QUALITY_INFO                 =   562;	// md_raid_item_quality_info already exists
	
	const int FAILURE_DUPLICATED_MD_ITEMCOMPOSECOST                        =   570;	// mdCreateItemCompsoeCost key already exists.
	const int FAILURE_NOT_EXIST_MD_ITEMCOMPOSECOST                         =   571;	// Not exist itemComposeCost.
	const int FAILURE_DUPLICATED_MD_ITEM_ENHANCE_COST                      =   572;	// md_item_enhance_cost key already exists.
	const int FAILURE_NOT_EXIST_MD_ITEM_ENHANCE_COST                       =   573;	// Not exist md_item_enhance_cost.
	const int FAILURE_DUPLICATED_MD_ITEM_ENHANCE_EFFECT_INFO               =   574;	// md_item_enhance_effect key already exists.
	const int FAILURE_NOT_EXIST_MD_ITEM_ENHANCE_EFFECT_INFO                =   575;	// Not exist md_item_enhance_effect.
	const int FAILURE_DUPLICATED_MD_ITEM_ENHANCE_SCROLL_INFO               =   576;	// md_item_enhance_scroll key already exists.
	const int FAILURE_NOT_EXIST_MD_ITEM_ENHANCE_SCROLL_INFO                =   577;	// Not exist md_item_enhance_scroll.
	const int FAILURE_NOT_EXIST_MD_UNITY_SKILL_OPTION                      =   578;	// Not exist md_unity_skill_option.
	
	const int FAILURE_DUPLICATED_MD_CHARACTER_EXP_INFO                     =   580;	// md_character_exp key already exists.
	const int FAILURE_NOT_EXIST_MD_CHARACTER_EXP_INFO                      =   581;	// Not exist md_character_exp.
	
	const int FAILURE_DUPLICATED_MD_STAGEINFO                              =   590;	// md_stage key already exists.
	const int FAILURE_NOT_EXIST_MD_STAGEINFO                               =   591;	// Not exist stageInfo.
	
	const int FAILURE_DUPLICATED_MD_STAGEGOLDINFO                          =   600;	// md_stage_gold key already exists.
	const int FAILURE_NOT_EXIST_MD_STAGEGOLDINFO                           =   601;	// Not exist stageGoldInfo.
	const int FAILURE_DUPLICATED_MD_HOF_PRAISED_RANKING_REWARD             =   602;	// md_hof_praised_ranking_reward key already exists.
	const int FAILURE_NOT_EXIST_MD_HOF_PRAISED_RANKING_REWARD              =   603;	// Not exist md_hof_praised_ranking_reward.
	const int FAILURE_DUPLICATED_MD_HOF_PRAISE_REWARD                      =   604;	// md_hof_praise_reward key already exists.
	const int FAILURE_NOT_EXIST_MD_HOF_PRAISE_REWARD                       =   605;	// Not exist md_hof_praise_reward.
	const int FAILURE_DUPLICATED_MD_HOF_RANKING_REWARD                     =   606;	// md_hof_ranking_reward key already exists.
	const int FAILURE_NOT_EXIST_MD_HOF_RANKING_REWARD                      =   607;	// Not exist md_hof_ranking_reward.
	
	const int FAILURE_DUPLICATED_MD_DROPITEMINFO                           =   610;	// md_item_drop key already exists.
	const int FAILURE_NOT_EXIST_MD_DROPITEMINFO                            =   611;	// Not exist dropItemInfo.
	
	const int FAILURE_NOT_EXIST_MD_EVENT_DICE_MILEAGE                      =   613;	// Not exist md_event_dice_mileage.
	const int FAILURE_DUPLICATED_MD_MAGIC_SHOP_RENEWAL_COST                =   614;	// md_magic_shop_renewal_cost key already exists.
	const int FAILURE_NOT_EXIST_MD_MAGIC_SHOP_RENEWAL_COST                 =   615;	// Not exist md_magic_shop_renewal_cost.
	const int FAILURE_DUPLICATED_MD_EVENT_TEN_LOTTERY_SAVINGS              =   616;	// md_event_ten_lottery_savings key already exists.
	const int FAILURE_DUPLICATED_MD_EVENT_DICE_BUY_POINT                   =   617;	// md_event_dice_buy_point key already exists.
	const int FAILURE_DUPLICATED_MD_EVENT_DICE_MILEAGE                     =   618;	// md_event_dice_mileage key already exists.
	
	const int FAILURE_DUPLICATED_MD_REWARDITEMINFO                         =   620;	// mdRewardItemInfo key already exists.
	const int FAILURE_NOT_EXIST_MD_REWARDITEMINFO                          =   621;	// Not exist rewardItemInfo.
	const int FAILURE_DUPLICATED_MD_SHOP_FLAT_RATE_PACKAGE_BONUS           =   622;	// md_shop_flat_rate_package_bonus already exists.
	const int FAILURE_NOT_MD_RAID_BASE_INFO                                =   623;	// md_raid not exists.
	const int FAILURE_INVALID_MD_RAID_TYPE                                 =   624;	// invalid md raid type
	const int FAILURE_INVALID_MD_MAGIC_SHOP_PRODUCT_TYPE                   =   625;	// invalid md magic shop product type
	const int FAILURE_DUPLICATED_MD_MAGIC_SHOP_PRODUCT                     =   626;	// md_magic_shop_product key already exists.
	const int FAILURE_NOT_EXIST_MD_MAGIC_SHOP_PRODUCT                      =   627;	// Not exist md_magic_shop_product.
	const int FAILURE_DUPLICATED_MD_MAGIC_SHOP_GROUP_INFO                  =   628;	// md_magic_shop_group_info key already exists.
	const int FAILURE_NOT_EXIST_MD_MAGIC_SHOP_GROUP_INFO                   =   629;	// Not exist md_magic_shop_group_info.
	const int FAILURE_DUPLICATED_MD_ACT_CLEAR_REWARDINFO                   =   630;	// md_stage_act_clear_reward key already exists.
	const int FAILURE_NOT_EXIST_MD_ACT_CLEAR_REWARDINFO                    =   631;	// Not exist actClearRewardInfo.
	const int FAILURE_DUPLICATED_MD_REST_REWARD                            =   632;	// md_reward_rest already exists.
	const int FAILURE_NOT_EXIST_MD_SHOP_REACH_LEVEL_PACKAGE                =   633;	// md_shop_reach_level_package not exists.
	const int FAILURE_DUPLICATED_MD_EVENT_INFO                             =   634;	// md_event_info already exists.
	const int FAILURE_DUPLICATED_MD_SEASON_MISSION_EVENT                   =   635;	// md_season_mission_event already exists.
	const int FAILURE_DUPLICATED_MD_SHOP_PACKAGE_PRODUCT                   =   636;	// md_shop_package_product already exists.
	const int FAILURE_DUPLICATED_MD_SHOP_LEVEL_UP_PACKAGE                  =   637;	// md_shop_level_up_package already exists.
	const int FAILURE_DUPLICATED_MD_SHOP_REACH_LEVEL_PACKAGE               =   638;	// md_shop_reach_level_package already exists.
	const int FAILURE_NOT_EXIST_MD_SHOP_LEVEL_UP_PACKAGE                   =   639;	// md_shop_level_up_package not exists.
	const int FAILURE_DUPLICATED_MD_ACT_REPEATCLEAR_REWARDINFO             =   640;	// mdActRepeatClearRewardInfo key already exists.
	const int FAILURE_NOT_EXIST_MD_ACT_REPEATCLEAR_REWARDINFO              =   641;	// Not exist actRepeatClearRewardInfo.
	const int FAILURE_INVALID_MD_QUEST_REWARD_TYPE                         =   643;	// invalid md quest reward type
	const int FAILURE_DUPLICATED_MD_MAIN_QUEST_INFO                        =   644;	// md_quest_main already exists.
	const int FAILURE_DUPLICATED_MD_DAILY_QUEST_INFO                       =   645;	// md_daily_quest already exists.
	const int FAILURE_DUPLICATED_MD_QUEST_REWARD_INFO                      =   646;	// mdQuestRewardInfo already exists.
	const int FAILURE_NOT_EXIST_MD_MAIN_QUEST_INFO                         =   647;	// Not exist md main quest info
	const int FAILURE_NOT_EXIST_MD_DAILY_QUEST_INFO                        =   648;	// Not exist md daily quest info
	const int FAILURE_INVALID_MD_GAME_EVENT_TYPE                           =   649;	// Not invalid md game event type
	const int FAILURE_DUPLICATED_MD_SKILLINFO                              =   650;	// md_skill key already exists.
	const int FAILURE_NOT_EXIST_MD_SKILLINFO                               =   651;	// Not exist md_skill.
	const int FAILURE_DUPLICATED_MD_SKILLOPTION                            =   660;	// md_skill_option key already exists.
	const int FAILURE_NOT_EXIST_MD_SKILLOPTION                             =   661;	// Not exist md_skill_option.
	const int FAILURE_DUPLICATED_MD_SKILL_LEVELUP                          =   662;	// md_skill_levelup key already exists.
	const int FAILURE_NOT_EXIST_MD_SKILL_LEVELUP                           =   663;	// Not exist md_skill_levelup.
	const int FAILURE_DUPLICATED_MD_MATCH_PRESERVE                         =   664;	// md_match_preserve key already exists.
	const int FAILURE_NOT_EXIST_MD_MATCH_PRESERVE                          =   665;	// Not exist md_match_preserve.
	const int FAILURE_DUPLICATED_MD_DUEL_MODE_SETTLE_SCHEDULE              =   666;	// md_duel_mode_settle_schedule key already exists.
	const int FAILURE_NOT_EXIST_MD_DUEL_MODE_SETTLE_SCHEDULE               =   667;	// Not exist md_duel_mode_settle_schedule.
	const int FAILURE_DUPLICATED_MD_RESURRECTIONINFO                       =   670;	// md_resurrection key already exists.
	const int FAILURE_NOT_EXIST_MD_RESURRECTIONINFO                        =   671;	// Not exist md_resurrection.
	const int FAILURE_INVALID_MD_RESURRECTION_MODE_TYPE                    =   680;	// invalid md resurrection mode Type
	const int FAILURE_INVALID_MD_RESURRECTION_BUFF_TYPE                    =   681;	// invalid md resurrection buff Type
	const int FAILURE_DUPLICATED_MD_TEAMBATTLE_FORMATION                   =   690;	// md_team_battle_formation key already exists.
	const int FAILURE_NOT_EXIST_MD_TEAMBATTLE_FORMATION                    =   691;	// Not exist md_team_battle_formation.
	const int FAILURE_INVALID_MD_TEAMBATTLE_FORMATION_TYPE                 =   692;	// invalid mdTeamBattleFormaion Type
	const int FAILURE_INVALID_MD_TEAMBATTLE_FORMATION_BUFF_TYPE            =   693;	// invalid mdTeamBattleFormaion buff Type
	const int FAILURE_NOT_EXIST_MD_FIXED_GRADE_LOTTERY_INFO                =   698;	// Not exist md fixed grade lottery info
	const int FAILURE_DUPLICATED_MD_FIXED_GRADE_LOTTERY_INFO               =   699;	// md_lottery_fixed_grade already exists.
	const int FAILURE_DUPLICATED_MD_RANKINFO                               =   700;	// md_rank key already exists.
	const int FAILURE_NOT_EXIST_MD_RANKINFO                                =   701;	// not exist md_rank.
	const int FAILURE_INVALID_MD_RANK_TYPE                                 =   702;	// invalid md rank type
	const int FAILURE_INVALID_MD_RANK_STATUS_TYPE                          =   703;	// invalid md rank status type
	const int FAILURE_DUPLICATED_MD_GENERAL_LOTTERY_INFO                   =   705;	// md_lottery_general already exists.
	const int FAILURE_INVALID_MD_LOTTERY_GRADE_TYPE                        =   706;	// invalid md lottery grade type
	const int FAILURE_INVALID_MD_LOTTERY_EQUIP_CATEGORY_TYPE               =   707;	// invalid md lottery equip category type
	const int FAILURE_DUPLICATED_MD_SELECTIVE_LOTTERY_INFO                 =   708;	// md_lottery_selective already exists.
	const int FAILURE_DUPLICATED_MD_LOTTERY_GRADE_RATE_INFO                =   709;	// md_lottery_rate_grade already exists.
	const int FAILURE_DUPLICATED_MD_LOTTERY_GROUP_ID_RATE_INFO             =   710;	// md_lottery_rate_group_id already exists.
	const int FAILURE_DUPLICATED_MD_LOTTERY_ITEM_TYPE_RATE_INFO            =   711;	// md_lottery_rate_item_type already exists.
	const int FAILURE_DUPLICATED_MD_LOTTERY_QUALITY_RATE_INFO              =   712;	// md_lottery_rate_quality already exists.
	const int FAILURE_NOT_EXIST_MD_GENERAL_LOTTERY_INFO                    =   713;	// Not exist md general lottery info
	const int FAILURE_NOT_EXIST_MD_SELECT_LOTTERY_INFO                     =   714;	// Not exist md select lottery info
	const int FAILURE_DUPLICATED_MD_PVP_RANKING_REWARD                     =   715;	// md_pvp_reward_ranking key already exists.
	const int FAILURE_NOT_EXIST_MD_PVP_RANKING_REWARD                      =   716;	// not exist md_pvp_reward_ranking.
	const int FAILURE_DUPLICATED_MD_TEAM_RANKING_REWARD                    =   717;	// md_team_battle_reward_ranking key already exists.
	const int FAILURE_NOT_EXIST_MD_TEAN_RANKING_REWARD                     =   718;	// not exist md_team_battle_reward_ranking.
	const int FAILURE_DUPLICATED_MD_DAILY_MATCH_REWARD                     =   719;	// md_daily_reward_match key already exists.
	const int FAILURE_NOT_EXIST_MD_DAILY_MATCH_REWARD                      =   720;	// not exist md_daily_reward_match.
	const int FAILURE_DUPLICATED_MD_COUNTER_DUNGEON_INFO                   =   721;	// md_counter_dungeon key already exists.
	const int FAILURE_NOT_EXIST_MD_COUNTER_DUNGEON_INFO                    =   722;	// not exist md_counter_dungeon.
	const int FAILURE_DUPLICATED_MD_COUNTER_DUNGEON_WEEK_REWARD            =   723;	// md_counter_dungeon_reward_weekly key already exists.
	const int FAILURE_NOT_EXIST_MD_COUNTER_DUNGEON_WEEK_REWARD             =   724;	// not exist md_counter_dungeon_reward_weekly.
	const int FAILURE_DUPLICATED_MD_COUNTER_DUNGEON_PERIOD                 =   725;	// md_counter_dungeon_period key already exists.
	const int FAILURE_NOT_EXIST_MD_COUNTER_DUNGEON_PERIOD                  =   726;	// not exist md_counter_dungeon_period.
	const int FAILURE_INVALID_MD_DAY_OF_WEEK                               =   727;	// not invalid day of week
	const int FAILURE_DUPLICATED_MD_DAILY_QUEST_LIST_TYPE_INFO             =   728;	// mdDailyQuestListTypeInfo already exists
	const int FAILURE_DUPLICATED_MD_COUNTER_DUNGEON_TICKET_PRICE           =   729;	// md_counter_dungeon_ticket_price key already exists.
	const int FAILURE_NOT_EXIST_MD_COUNTER_DUNGEON_TICKET_PRICE            =   730;	// not exist md_counter_dungeon_ticket_price.
	const int FAILURE_DUPLICATED_MD_DAILY_QUEST_DAY_LIST_TYPE              =   731;	// md_daily_quest_day_list_type already exists
	const int FAILURE_INVALID_MD_MISSION_TYPPE                             =   732;	// invalid md mission type
	const int FAILURE_NOT_EXIST_MD_PERIOD_MISSION_INFO                     =   733;	// failure not exist md period mission info
	const int FAILURE_NOT_EXIST_MD_SERIAL_MISSION_INFO                     =   734;	// failure not exist md serial mission info
	const int FAILURE_NOT_EXIST_MD_MISSION_COMPLETE_POINT_INFO             =   735;	// failure not exist md mission complete point info
	const int FAILURE_DUPLICATED_MD_WING_ENHANCE_INFO                      =   736;	// mdWingEnhancekey already exists.
	const int FAILURE_DUPLICATED_MD_WING_UPGRADE_INFO                      =   737;	// mdWingUpgradekey already exists.
	const int FAILURE_DUPLICATED_MD_WING_OPTION_VALUE_INFO                 =   738;	// mdWingOptionValuekey already exists.
	const int FAILURE_DUPLICATED_MD_WING_OPTION_OPEN_INFO                  =   739;	// mdWingOptionOpenkey already exists.
	const int FAILURE_DUPLICATED_MD_WING_OPTION_MAXLEVEL_INFO              =   740;	// mdWingOptionMaxLevelkey already exists.
	const int FAILURE_NOT_EXIST_MD_WING_ENHANCE_INFO                       =   741;	// not exist mdWingEnhanceInfo
	const int FAILURE_NOT_EXIST_MD_WING_UPGRADE_INFO                       =   742;	// not exist mdWingUpgradeInfo
	const int FAILURE_NOT_EXIST_MD_WING_OPTION_MAXLEVEL_INFO               =   743;	// not exist mdWingOptionMaxLevelInfo
	const int FAILURE_NOT_EXIST_MD_WING_OPTION_VALUE_INFO                  =   744;	// not exist mdWingOptionValuelInfo
	const int FAILURE_INVALID_MD_OPTION_TYPE                               =   745;	// Invalid md option type
	const int FAILURE_NOT_EXIST_MD_WING_OPTION_OPEN_INFO                   =   746;	// not exist mdWingOptionOpenInfo
	const int FAILURE_INVALID_MD_RELIC_OPTION_TYPE                         =   747;	// Invalid mdRelic option type
	const int FAILURE_INVALID_MD_RELIC_MODE_OPTION_TYPE                    =   748;	// Invalid mdRelic mode option type
	const int FAILURE_DUPLICATED_MD_HERO_TOWER_FLOOR                       =   750;	// md_hero_tower_floor already exists.
	const int FAILURE_NOT_EXIST_MD_HERO_TOWER_FLOOR                        =   751;	// not exist md_hero_tower_floor.
	const int FAILURE_DUPLICATED_MD_HERO_TOWER_WEEKLY_REWARD               =   752;	// mdHeroTowerWeeklyReward already exists.
	const int FAILURE_NOT_EXIST_MD_HERO_TOWER_WEEKLY_REWARD                =   753;	// not exist mdHeroTowerWeeklyReward.
	const int FAILURE_DUPLICATED_MD_RELIC_GRADE_INFO                       =   754;	// md_relic_grade already exists.
	const int FAILURE_NOT_EXIST_MD_RELIC_GRADE_INFO                        =   755;	// not exist md_relic_grade.
	const int FAILURE_DUPLICATED_MD_RELIC_LEVEL_INFO                       =   756;	// md_relic_level already exists.
	const int FAILURE_NOT_EXIST_MD_RELIC_LEVEL_INFO                        =   757;	// not exist md_relic_level.
	const int FAILURE_DUPLICATED_MD_RELIC_INFO                             =   758;	// md_relic_level already exists.
	const int FAILURE_NOT_EXIST_MD_RELIC_INFO                              =   759;	// not exist md_relic_level.
	const int FAILURE_INVALID_MD_SHOP_DRAW_PRICE_TYPE                      =   762;	// invalid md shop draw price type
	const int FAILURE_INVALID_MD_STAGE_DIFFICULTY_TYPE                     =   763;	// invalid md stage difficulty type
	const int FAILURE_INVALID_MD_SHOP_PRODUCT_TYPE                         =   764;	// invalid md shop product type
	const int FAILURE_INVALID_MD_SHOP_PRICE_TYPE                           =   765;	// invalid md shop price type
	const int FAILURE_INVALID_MD_SHOP_MILEAGE_TYPE                         =   766;	// invalid md shop mileage type
	const int FAILURE_DUPLICATED_MD_SHOP_LOTTERY_PRICE                     =   767;	// md_lottery_shop_price already exists.
	const int FAILURE_DUPLICATED_MD_SHOP_LOTTERY_GRADE_RATE                =   768;	// md_lottery_shop_rate_grade already exists.
	const int FAILURE_DUPLICATED_MD_SHOP_LOTTERY_GROUP_ID_RATE             =   769;	// md_lottery_shop_rate_group_id already exists.
	const int FAILURE_DUPLICATED_MD_SHOP_LOTTERY_ITEM_TYPE_RATE            =   770;	// md_lottery_shop_rate_item_type already exists.
	const int FAILURE_DUPLICATED_MD_SHOP_LOTTERY_QUALITY_RATE              =   771;	// md_lottery_shop_rate_quality already exists.
	const int FAILURE_DUPLICATED_MD_SHOP_PRODUCT                           =   772;	// md_shop_product already exists.
	const int FAILURE_DUPLICATED_MD_SHOP_MILEAGE_PRODUCT                   =   773;	// mdShopProductMileage already exists.
	const int FAILURE_DUPLICATED_MD_SHOP_LOTTERY_FREE_TIME                 =   774;	// mdShopLotteryFreeTime already exists.
	const int FAILURE_NOT_EXIST_MD_SHOP_LOTTERY_FREE_TIME                  =   775;	// not exist mdShopLotteryFreeTime.
	const int FAILURE_NOT_EXIST_MD_SHOP_LOTTERY_PRICE                      =   776;	// not exist md_lottery_shop_price.
	const int FAILURE_INVALID_MD_SHOP_SPECIAL_LOTTERY_TYPE                 =   777;	// invalid md shop special lottery type
	const int FAILURE_NOT_EXIST_MD_SHOP_MILEAGE_REWARD                     =   778;	// not exist mdShopMileageReward.
	const int FAILURE_DUPLICATED_MD_SHOP_PACKAGE                           =   779;	// md_shop_package already exists.
	const int FAILURE_NOT_EXIST_MD_SHOP_PRODUCT                            =   780;	// not exist mdShopProdcut.
	const int FAILURE_NOT_EXIST_MD_SHOP_PACKAGE                            =   781;	// not exist md_shop_package.
	const int FAILURE_DUPLICATED_MD_COLLECTION_REWARD                      =   782;	// md_collection_reward already exists.
	const int FAILURE_NOT_EXIST_MD_COLLECTION_REWARD                       =   783;	// not exist md_collection_reward.
	const int FAILURE_NOT_EXIST_MD_RAID_ITEM_RATE                          =   784;	// not exist md_raid_item_rate.
	const int FAILURE_NOT_EXIST_MD_RAID_ITEM_COUNT                         =   785;	// not exist md_raid_item_count.
	const int FAILURE_DUPLICATED_MD_DAILY_ATTENDANCE                       =   786;	// md_daily_attendance already exists.
	const int FAILURE_NOT_EXIST_MD_DAILY_ATTENDANCE_INFO                   =   787;	// not exist md_daily_attendance.
	const int FAILURE_DUPLICATED_MD_RAID_INFO                              =   788;	// md_raid already exists.
	const int FAILURE_DUPLICATED_MD_MONSTER_INFO                           =   789;	// md_monster already exists.
	const int FAILURE_DUPLICATED_MD_SELECTIVE_ITEM_LOTTERY                 =   790;	// mdMSelectiveItemLottery already exists.
	const int FAILURE_DUPLICATED_MD_ADDITIONAL_MATCH_COST                  =   791;	// mdMdAdditionalMatchCost already exists
	const int FAILURE_DUPLICATED_MD_ASSAULT_RANKING_REWARD                 =   792;	// md_assault_reward_ranking key already exists.
	const int FAILURE_DUPLICATED_MD_ASSAULT_PERIOD                         =   793;	// md_assault_period key already exists.
	const int FAILURE_DUPLICATED_MD_RAID_PERIOD                            =   794;	// md_raid_period already exists.
	const int FAILURE_DUPLICATED_MD_ASSAULT_VICTORY_REWARD                 =   795;	// md_assault_reward_victory already exists.
	const int FAILURE_DUPLICATED_MD_ASSAULT_MVP_REWARD                     =   796;	// md_assault_reward_mvp already exists.
	const int FAILURE_DUPLICATED_MD_DUEL_MODE_REWARD                       =   797;	// md_duel_mode_reward already exists.
	const int FAILURE_DUPLICATED_MD_RAID_EXP                               =   798;	// md_raid_exp already exists.
	
	const int FAILURE_MD_VERIFY                                            =   799;	// verify result is fail.
	
	// common
	const int FAILURE_QUERY_NO_RESULT                                      =   800;	// No result.
	const int FAILURE_INVALID_PARAMETER                                    =   801;	// InvalidParameterException occur.
	const int FAILURE_UPDATE_SIGN_IN_INFO                                  =   803;	// Failed to update sign in info.
	
	const int FAILURE_CONTENTS_BLOCK_ENTER_STATE                           =   810;	// contents is block enter state.
	const int FAILURE_CONTENTS_MODE_FINISH_STATE                           =   811;	// contents is mode finish state.
	const int FAILURE_CONTENTS_SYSTEM_CHECK_STATE                          =   812;	// contents is system check state.
	const int FAILURE_CONTENTS_BLOCK_NOT_AVAILABLE_MODE                    =   813;	// contents mode is not available block.
	
	const int FAILURE_NAME_INVALID_CHAR                                    =   895;	// name has invalid character
	const int FAILURE_NAME_LENGTH_TOO_SHORT                                =   896;	// name is too short
	const int FAILURE_NAME_LENGTH_TOO_LONG                                 =   897;	// name is too long
	const int FAILURE_NAME_INCLUDE_SWEAR_WORD                              =   898;	// name is including swear word
	const int FAILURE_NAME_INCLUDE_PROHIBITED_NAME                         =   899;	// name is including prohibited name
	
	// account
	const int FAILURE_DUPLICATED_ACCOUNTID                                 =   900;	// AccountId already exists.
	const int FAILURE_NOT_EXIST_ACCOUNT                                    =   901;	// Not exist account.
	const int FAILURE_DELETED_ACCOUNT                                      =   902;	// It's deleted account.
	const int FAILURE_NOT_FINISH_BUILD_ACCOUNT                             =   903;	// Not finish build account.
	const int FAILURE_ALREADY_DONE_BUILD_ONCE                              =   904;	// Already done build once.
	const int FAILURE_NOT_FOUND_PLATFORM_LINK                              =   905;	// Not found platform info for the account.
	
	const int FAILURE_CASH_UNDERFLOW                                       =   910;	// Cash is zero.
	const int FAILURE_MONEY_UNDERFLOW                                      =   911;	// Money is zero.
	const int FAILURE_BLADEPOINT_UNDERFLOW                                 =   913;	// BladePoint is zero.
	const int FAILURE_SOCIALPOINT_UNDERFLOW                                =   914;	// SocialPoint is zero.
	const int FAILURE_SOCIALPOINT_OVERFLOW                                 =   915;	// SocialPoint is over max.
	const int FAILURE_RAID_COUNT_UNDERFLOW                                 =   916;	// Raid count is underflow
	const int FAILURE_PVP_MATCH_POINT_UNDERFLOW                            =   917;	// PvP Match Point is underflow
	const int FAILURE_TEAM_MATCH_POINT_UNDERFLOW                           =   918;	// Team Match Point is underflow
	const int FAILURE_DARK_CRYSTAL_UNDERFLOW                               =   919;	// Not enough dark crystal
	const int FAILURE_DARK_CRYSTAL_DECREASE                                =   920;	// Failed to decrease dark crystal
	const int FAILURE_CASH_DECREASE                                        =   921;	// Failed to decrease Cash
	const int FAILURE_CASH_SUM_OVERFLOW                                    =   922;	// The Sum of Cash(=Gem) is negative, which did not intend so.
	const int FAILURE_CASH_FIELD_OVERFLOW                                  =   923;	// One of cash(=gem) is negative, which did not intend so.
	const int FAILURE_DARK_CRYSTAL_SUM_OVERFLOW                            =   924;	// Dark Crystal Paid is negative, which did not intend so.
	const int FAILURE_DARK_CRYSTAL_FIELD_OVERFLOW                          =   925;	// One of dark Crystal is negative, which did not intend so.
	const int FAILURE_INCREASE_GOLD_BUFF_TIME                              =   926;	// Failed increase gold buff time
	const int FAILURE_INCREASE_EXP_BUFF_TIME                               =   927;	// Failed increase exp buff time
	const int FAILURE_DECREASE_GOLD_BUFF_TICKET                            =   928;	// Failed decrease gold buff ticket
	const int FAILURE_DECREASE_EXP_BUFF_TICKET                             =   929;	// Failed decrease exp buff ticket
	const int FAILURE_INCREASE_GOLD_BUFF_TICKET                            =   930;	// Failed increase gold buff ticket
	const int FAILURE_INCREASE_EXP_BUFF_TICKET                             =   931;	// Failed increase exp buff ticket
	const int FAILURE_FAME_POINT_UNDERFLOW                                 =   932;	// Fame point is underflow
	
	
	// character
	const int FAILURE_DUPLICATED_CHARACTER                                 =  1000;	// CharacterId already exists.
	const int FAILURE_NOT_EXIST_CHARACTER                                  =  1001;	// Not exist character.
	const int FAILURE_DELETED_CHARACTER                                    =  1002;	// It's deleted character.
	const int FAILURE_TRANSACTION_WHILE_INCREASE_EXP                       =  1003;	// Character exp not same for start value.
	const int FAILURE_INVALID_CHARACTER_TYPE                               =  1004;	// invalid character type.
	
	const int FAILURE_CHARACTER_NAME_TOO_SHORT                             =  1010;	// Character name is too short
	const int FAILURE_CHARACTER_NAME_TOO_LONG                              =  1011;	// Character name is too long
	const int FAILURE_CHARACTER_NAME_INVALID_CHAR                          =  1012;	// Character name has invalid character
	
	const int FAILURE_CHARACTER_LEVEL_OVERFLOW                             =  1021;	// Character level is max.
	const int FAILURE_SKILLPOINT_OVERFLOW                                  =  1031;	// SkillPoint is max.
	const int FAILURE_SKILLPOINT_UNDERFLOW                                 =  1032;	// SkillPoint is zero.
	const int FAILURE_SKILLPOINTBUYCOUNT_OVERFLOW                          =  1033;	// SkillPointBuyCount is max.
	
	const int FAILURE_INVEN_AETHER_BUYCOUNT_OVERFLOW                       =  1040;	// Inventory aether buy count is max.
	const int FAILURE_INVEN_ITEM_BUYCOUNT_OVERFLOW                         =  1041;	// Inventory item buy count is max.
	const int FAILURE_INVENTORYTYPE_WRONG                                  =  1044;	// Inventory type is wrong.
	
	const int FAILURE_NO_MORE_TUTORIAL                                     =  1051;	// All tutorials are opened. No more contents to open.
	const int FAILURE_UPDATE_TUTORIAL_STATUS                               =  1052;	// Failed to update tutorial status.
	const int FAILURE_TUTORIAL_CONDITION_LEVEL                             =  1053;	// Largest character level is smaller than master data.
	const int FAILURE_TUTORIAL_CONDITION_ACT_CLEARED                       =  1054;	// Largest act cleared is smaller than master data.
	const int FAILURE_TUTORIAL_CONDITION_FINISHED                          =  1055;	// Current tutorial not finished yet and has not skip command.
	const int FAILURE_TUTORIAL_NOT_PREPARED                                =  1056;	// Gems, cash or items was not given to player for tutorial.
	const int FAILURE_TUTORIAL_ALREADY_PREPARED                            =  1057;	// Gems, cash or items was already given to player for tutorial.
	const int FAILURE_TUTORIAL_NOTHING_TO_DRAW                             =  1058;	// Nothing was defined for draw in this stage.
	const int FAILURE_TUTORIAL_UPDATE_INFO                                 =  1059;	// Cannot retrieve character's largest character level and act cleared.
	const int FAILURE_TUTORIAL_ALREADY_DRAWN                               =  1060;	// Tutorial Lottery is already drawn.
	const int FAILURE_TUTORIAL_ALREADY_FINISHED                            =  1061;	// This tutorial is already finished.
	const int FAILURE_TUTORIAL_NO_PROGRESS                                 =  1062;	// Requires created tutorial progress but no progress found.
	const int FAILURE_NO_PLAYER_STATUS_FOR_PROGRESS                        =  1063;	// No player info found to calculate contents open status.
	const int FAILURE_NO_STACKABLE_ITEM                                    =  1064;	// Tutorial items at draw stage has been set with stackable.
	const int FAILURE_NO_CONTENTS_OPEN_INFO                                =  1065;	// No contents open info found with given key.
	
	const int FAILURE_POWER_UPPER_LIMIT_VIOLATION                          =  1071;	// Character power to update is over the upper limit.
	const int FAILURE_POWER_ALREADY_OVER_UPPER_LIMIT                       =  1072;	// Character power in character was already over power limit.
	const int FAILURE_POWER_SUSPECT_GRADE_UPDATE                           =  1073;	// Failed to update character power suspect grade.
	const int FAILURE_POWER_ALREADY_HIGHER_SUSPECT                         =  1074;	// Failed to update character power suspect grade since it's already higher suspect.
	const int FAILURE_POWER_UPDATE_NO_CHANGE                               =  1075;	// Failed to update character power: there was no change.
	
	// item
	const int FAILURE_DUPLICATED_ITEM                                      =  2000;	// ItemId already exists.
	const int FAILURE_NOT_EXIST_ITEM                                       =  2001;	// Not exist item.
	const int FAILURE_NOT_EXIST_MATERIAL_ITEM                              =  2002;	// Not exist material item.
	const int FAILURE_LOCKED_ITEM                                          =  2004;	// It's locked item.
	
	// for item levelup
	const int FAILURE_REQUIRE_MATERIAL_ITEM_FOR_LEVELUP                    =  2005;	// Require material items for levelup item.
	const int FAILURE_TOO_MANY_MATERIAL_ITEM_FOR_LEVELUP                   =  2006;	// Too many material items for levelup item.
	const int FAILURE_REQUIRE_NOT_EQUIPPED_ITEM                            =  2007;	// Equipped item can't use for material item of levelup.
	const int FAILURE_NOT_SAME_CHARACTERCLASS_FOR_LEVELUP                  =  2008;	// Require same character type for levelup.
	const int FAILURE_NOT_SAME_INVENTORYTYPE_FOR_LEVELUP                   =  2009;	// Require same inventory type for levelup.
	const int FAILURE_NOT_EQUIP_ITEM                                       =  2010;	// It's not equip item.
	// for item surpass
	const int FAILURE_SURPASSCOUNT_OVERFLOW                                =  2011;	// Surpass count is max.
	const int FAILURE_REQUIRE_MAXGRADE_ITEM_FOR_SURPASS                    =  2012;	// Require max grade item for surpass.
	const int FAILURE_REQUIRE_MAX_LEVEL_FOR_SURPASS                        =  2013;	// Require max level for surpass.
	const int FAILURE_NEED_SAME_ITEM_FOR_SURPASS                           =  2014;	// Need same item for surpass.
	// for item upgrade
	const int FAILURE_ALREADY_MAX_GRADE                                    =  2015;	// Already item's grade is maximum.
	const int FAILURE_REQUIRE_MAX_LEVEL_FOR_UPGRADE                        =  2016;	// Require max level for upgrade.
	const int FAILURE_NO_NEXT_GRADE_INFO                                   =  2017;	// Not exist next grade info.
	const int FAILURE_NEED_MORE_UPGRADE_STONE                              =  2018;	// Need more upgrade stone.
	const int FAILURE_ITEM_AMOUNT_UNDERFLOW                                =  2019;	// Item amount is not enough.
	const int FAILURE_ITEM_AMOUNT_OVERFLOW                                 =  2020;	// Item amount is max.
	const int FAILURE_OPTION_RECALCULATE_FOR_UPGRADE                       =  2021;	// fail to recalculate item optinos.
	// for item compose
	const int FAILURE_REQUIRE_MAX_LEVEL_FOR_COMPOSE                        =  2022;	// Require max level for compose.
	const int FAILURE_NOT_SAME_INVENTORYTYPE_FOR_COMPOSE                   =  2023;	// Require same inventory type to mainitem.
	const int FAILURE_NOT_SAME_GRADE_FOR_COMPOSE                           =  2024;	// Require same grade to mainitem.
	const int FAILURE_REQUIRE_NOT_EQUIPPED_FOR_COMPOSE                     =  2025;	// Require not equipped item.
	
	const int FAILURE_NOT_SAME_MATERIAL_ITEM_GROUP_ID_FOR_SURPASS          =  2030;	// Not same material item group id for surpass.
	
	// for item values calculate
	const int FAILURE_CALCULATE_ITEM_VALUES                                =  2100;	// Fail to calculate item values.
	const int FAILURE_EQUIP_ITEM_CREATE_COUNT_OVERFLOW                     =  2200;	// Equip item create count overflow.
	const int FAILURE_NONEQUIP_CREATE_COUNT_OVERFLOW                       =  2201;	// Non equip item create count overflow.
	const int FAILURE_NEED_MORE_ITEM_AMOUNT                                =  2210;	// Need more item amount.
	const int FAILURE_EQUIP_ITEM_ALREADY_EXIST_SAME_POS                    =  2211;	// Equip item already exist same pos.
	// for item option change
	const int FAILURE_NO_OPTION_LIST_FOR_CHANGE                            =  2212;	// No item option found for that item.
	const int FAILURE_LOW_GRADE_TO_CHANGE_OPTION                           =  2213;	// The grade of this item is too low to change option.
	const int FAILURE_INVALID_GRADE_TO_CHANGE_OPTION                       =  2214;	// The grade of item is over its max.
	const int FAILURE_LOW_OPTION_CHANGE_COST                               =  2215;	// Option change cost set to 0 or less.
	const int FAILURE_FAILED_TO_UPDATE_ITEM_OPTION                         =  2216;	// Item option to change was not written to DB.
	const int FAILURE_FAILED_TO_REMOVE_USED_ITEM_OPTION                    =  2217;	// Item option exists but failed to remove afeter appliance.
	const int FAILURE_INVALID_ITEM_SLOT                                    =  2218;	// Random option change must specifiy slot between 1 and 3.
	const int FAILURE_INVALID_RANDOM_OPTION_RANGE                          =  2219;	// Random option value is not in range.
	const int FAILURE_INVALID_PRICE_TYPE                                   =  2220;	// Given price type for random option change is not dark crystal nor cash.
	// for item enhance
	const int FAILURE_NEED_MORE_ENHANCE_SCROLL                             =  2300;	// Need more enhance scroll.
	const int FAILURE_NEED_MORE_PROTECTION_SCROLL                          =  2301;	// Need more Protection scroll.
	const int FAILURE_INVALID_TEMPLATE_ID_PROTECTION_SCROLL                =  2302;	// Invalid template id by protection scroll.
	const int FAILURE_OVERFLOW_ITEM_ENHANCE_LEVEL                          =  2303;	// Overflow item enhance level.
	const int FAILURE_UNDERFLOW_ITEM_ENHANCE_LEVEL                         =  2304;	// Underflow item enhance level.
	// for item isNew
	const int FAILURE_NOT_EXIST_NEW_ITEM_BY_TARGET                         =  2305;	// Not exist new item by target.
	
	const int FAILURE_INVENTORY_TYPE_MISMATCH_FROM_SCROLL                  =  2306;	// Inventory type mismatch from Scroll.
	const int FAILURE_NOT_ENOUGH_CONSUMABLE_ITEM_COUNT                     =  2307;	// Not enough consumable item count.
	
	// for item sell
	const int FAILURE_TOO_MANY_ITEM_FOR_SELL                               =  2308;	// Too many item for sell.
	const int FAILURE_SELL_ITEM_LIST_EMPTY                                 =  2309;	// SellItemList is empty.
	
	// for item equip/unequip
	const int FAILURE_EQUIP_ITEM                                           =  2310;	// equip fail.
	const int FAILURE_UNEQUIP_ITEM                                         =  2311;	// unequip fail.
	const int FAILURE_ITEM_PRESET_ID_OUT_OF_RANGE                          =  2312;	// item preset id is out of range.
	const int FAILURE_USE_CAN_NOT_PROTECTION_SCROLL                        =  2313;	// Use can't a Protection Scroll.
	
	// for item dismantle
	const int FAILURE_TOO_MANY_ITEM_FOR_DISMANTLE                          =  2314;	// Too many item for dismantle.
	const int FAILURE_DISMANTLE_ITEM_LIST_EMPTY                            =  2315;	// DismantleItemList is empty.
	
	// for lock item
	const int FAILURE_UNABLE_TO_LOCK_ITEM                                  =  2320;	// Unable to lock item.
	
	// for properties item
	const int FAILURE_UNABLE_TO_STACK_ITEM                                 =  2321;	// Unable to stack item.
	const int FAILURE_UNABLE_TO_SALE_ITEM                                  =  2322;	// Unable to sale item.
	const int FAILURE_UNABLE_TO_EQUIP_ITEM                                 =  2323;	// Unable to equip item.
	const int FAILURE_UNABLE_TO_LEVELUP_ITEM                               =  2324;	// Unable to levelup item.
	const int FAILURE_UNABLE_TO_ENHANCE_ITEM                               =  2325;	// Unable to enhance item.
	const int FAILURE_UNABLE_TO_UPGRADE_ITEM                               =  2326;	// Unable to upgrade item.
	const int FAILURE_UNABLE_TO_COMPOSE_ITEM                               =  2327;	// Unable to compose item.
	const int FAILURE_UNABLE_TO_SURPASS_ITEM                               =  2328;	// Unable to surpass item.
	const int FAILURE_UNABLE_TO_SALE_STACKABLE_ITEM                        =  2329;	// Unable to sale stackable item.
	const int FAILURE_UNABLE_TO_SALE_EQUIP_ITEM                            =  2330;	// Unable to sale equip item.
	
	const int FAILURE_CAN_NOT_EXCEED_MAX_ENHANCELEVEL                      =  2331;	// Can't exceed max enhanceLevel 
	const int FAILURE_UPDATE_HELMET_VISIBLE                                =  2332;	// fail update helmet visible
	const int FAILURE_ITEM_ENHANCE_LEVEL_UPDATE                            =  2333;	// failed to update item enhance level.
	const int FAILURE_CHARACTER_CLASS_MISMATCH_EXCHANGE_ENHANCE_LEVEL      =  2334;	// Require same character type for enhance level exchange.
	const int FAILURE_INVENTORY_TYPE_MISMATCH_EXCHANGE_ENHANCE_LEVEL       =  2335;	// Require same inventory type for enhance level exchange.
	const int FAILURE_SAME_ENHANCE_LEVEL_FOR_EXCHANGE                      =  2336;	// Requires different item enhance level.
	
	const int FAILURE_NOT_CONSUMABLE_ITEM                                  =  2400;	// Not a consumable item.
	const int FAILURE_INVALID_CREATE_ITEM                                  =  2401;	// Invalid create item.
	const int FAILURE_NOT_ALLOWED_GET_ALL_ACCOUNT_ITEMS                    =  2402;	// Not allowed get all account items.
	const int FAILURE_INCREASE_ITEM_AMOUNT                                 =  2403;	// Failure increase item amount.
	const int FAILURE_DECREASE_ITEM_AMOUNT                                 =  2404;	// Failure decrease item amount.
	const int FAILURE_UPDATE_ITEM_ENHANCE_FAILURE_COUNT                    =  2405;	// Failed to update item enhance failure count.

	// costume
	const int FAILURE_INVALID_CREATE_COSTUME                               =  2500;	// Invalid create costume.
	const int FAILURE_COSTUME_CREATE_COUNT_OVERFLOW                        =  2501;	// Costume create count overflow.
	const int FAILURE_EQUIPPED_COSTUME_ANOTHER_POSITION                    =  2502;	// Equipped costume another position.
	const int FAILURE_NOT_EXIST_COSTUME                                    =  2503;	// Not exist costume.
	const int FAILURE_EQUIP_COSTUME                                        =  2504;	// Failure equip costume.
	const int FAILURE_UNEQUIP_COSTUME_FAILED                               =  2506;	// The unequip costume failed.
	const int FAILURE_ALREADY_EXISTS_COSTUME_TARGET_POSITION               =  2507;	// Already exists costume to target quipPosition.
	const int FAILURE_NOT_EXIST_NEW_COSTUME_BY_TARGET                      =  2510;	// Not exist new costume by target.
	const int FAILURE_NOT_ENOUGH_COSTUME_INVENTORY_LEFT_SLOT               =  2511;	// Not enough costume inventory left slot.
	
	// aether (2600 ~ 2800)
	const int FAILURE_DUPLICATED_AETHER                                    =  2600;	// AetherId already exists.
	const int FAILURE_NOT_EXIST_AETHER                                     =  2601;	// Not exist aether.
	const int FAILURE_LOCKED_AETHER                                        =  2602;	// It's locked aether.
	const int FAILURE_INVALID_GRADE_RANGE_AETHER                           =  2603;	// Invalid grade range of aether.
	const int FAILURE_MOUNT_DUPLICATED_SLOT_AETHER                         =  2604;	// Mount failure due to duplicated slot.
	const int FAILURE_MOUNT_OPERATION_FAILED                               =  2605;	// The mount operation failed.
	const int FAILURE_UNMOUNT_OPERATION_FAILED                             =  2606;	// The unmount operation failed.
	const int FAILURE_NOT_EXIST_NEW_AETHER_BY_TARGET                       =  2607;	// Not exist new aether by target.
	const int FAILURE_ALREADY_EXISTS_AETHER_TARGET_SLOT                    =  2608;	// Already exists aether to target slot.
	const int FAILURE_ALREADY_MOUNTED_AETHER                               =  2609;	// Already mounted aether.
	const int FAILURE_MOUNT_LIST_EMPTY                                     =  2610;	// MountList is empty.
	const int FAILURE_UNMOUNT_LIST_EMPTY                                   =  2611;	// UnmountList is empty.
	const int FAILURE_DISMANTLE_LIST_EMPTY                                 =  2612;	// DismantleList is empty(aether).
	const int FAILURE_DISMANTLE_OPERATION_FAILED                           =  2613;	// The dismantle operation failed.
	const int FAILURE_NOT_MATCHED_SET_OPTION                               =  2614;	// Not matched set option.
	const int FAILURE_TOO_MANY_AETHER_FOR_DISMANTLE                        =  2615;	// Too many aether for dismantle.
	
	// aether enhance
	const int FAILURE_NEED_MORE_AETHER_PIECE                               =  2630;	// Need more aether piece.
	const int FAILURE_OVERFLOW_AETHER_ENHANCE_LEVEL                        =  2631;	// Overflow aether enhance level.
	const int FAILURE_OVERFLOW_AETHER_SUB_OPTION_COUNT                     =  2632;	// Overflow aether subOption count.
	const int FAILURE_AETHER_SUB_OPTION_OUT_OF_RANGE                       =  2633;	// Aether subOption out of range.
	const int FAILURE_UPDATE_AETHER_ENHANCE_FAILURE_COUNT                  =  2634;	// Aether subOption out of range.
	
	// for aether values calculate
	const int FAILURE_CALCULATE_AETHER_VALUES                              =  2700;	// Failed to update aether enhance failure count.
	
	// skill
	const int FAILURE_DUPLICATED_SKILL                                     =  3000;	// Skill already exists.
	const int FAILURE_NOT_EXIST_SKILL                                      =  3001;	// Not exist skill.
	const int FAILURE_DELETED_SKILL                                        =  3002;	// It's deleted skill.
	const int FAILURE_SKILL_LEVEL_OVERFLOW                                 =  3003;	// Skill Level is max.
	const int FAILURE_DUPLICATED_SKILL_PRESET                              =  3004;	// Skill Preset already exists.
	const int FAILURE_NOT_EXIST_SKILL_QUICK_SLOT                           =  3005;	// Not exist skill preset.
	const int FAILURE_NOT_ENOUGH_SKILL_POINT                               =  3006;	// Not enough skill point.
	const int FAILURE_ALREADY_SKILL_LEVEL_MAX                              =  3007;	// Already skill level is max.
	const int FAILURE_NOT_ENOUGH_LEARN_CHARACTER_LEVEL                     =  3008;	// Character level is low.
	const int FAILURE_NOT_USABLE_CHARACTER_TYPE                            =  3009;	// Not usable character type.
	const int FAILURE_OVERFLOW_SKILL_POINT_BUY_COUNT                       =  3010;	// SkillPoint buy count is overflow.
	const int FAILURE_NOT_ACTIVE_SKILL                                     =  3011;	// Not active skill.
	const int FAILURE_UNDERFLOW_LEFT_SKILL_POINT                           =  3012;	// Underflow left skill point.
	const int FAILURE_NOT_SAME_BEFORE_SKILL_LEVEL                          =  3013;	// Not same before skill level.
	const int FAILURE_SKILL_PRESET_ID_OUT_OF_RANGE                         =  3014;	// skill preset id is out of range.
	
	// stagePlayInfo
	const int FAILURE_NEED_HIGHER_DIFFICULTY_FOR_CONTINUOUS_BATTLE         =  3999;	// Need higher difficulty for continuous battle.
	const int FAILURE_NOTEXIST_STAGE_PLAY_INFO                             =  4000;	// Not exist stage play info.
	const int FAILURE_DUPLICATED_ACT_REWARD_HISTORY                        =  4001;	// Act reward history already exists.
	const int FAILURE_DUPLICATED_STAGE_CLEAR_HISTORY                       =  4002;	// Stage clear history already exists.
	const int FAILURE_NOTEXIST_STAGE_CLEAR_HISTORY                         =  4003;	// Not exist stage clear info.
	const int FAILURE_RESURRECT_COUNT_OVERFLOW                             =  4010;	// Resurrect count is overflow.
	const int FAILURE_PLAY_INFO_NOT_STAGE_START_STATE                      =  4011;	// Play info is not start state.
	const int FAILURE_CLEARHISTORY_INCREASE_COUNT_BELOW_ZERO               =  4013;	// Clear history increase count is below zero.
	const int FAILURE_NOTEXIST_ACT_REWARD_HISTORY                          =  4014;	// Not exist act reward history info.
	const int FAILURE_NOT_ENOUGH_INVENTORY_LEFT_SLOT                       =  4015;	// Not enough inventory left slot.
	const int FAILURE_NEED_DIFFERENT_CHARACTER_TYPE                        =  4016;	// Need different character type.
	const int FAILURE_NEED_CLEAR_GRADE_HIGH                                =  4017;	// Need clear grade high.
	const int FAILURE_NOT_ENOUGH_STAGE_BOOST_TICKET                        =  4018;	// Not Enough stage boost ticket
	const int FAILURE_UPDATE_ACT_REWARD_HISTORY                            =  4019;	// Failure update act reward history
	
	// clear stage
	const int FAILURE_NOT_SAME_STAGE_ID_FOR_CLEAR_STAGE                    =  4020;	// Not same stage id for clear stage.
	const int FAILURE_NOT_SAME_TOKEN_FOR_CLEAR_STAGE                       =  4021;	// Not same token for clear stage.
	const int FAILURE_NOT_SAME_CHARACTER_TYPE_FOR_CLEAR_STAGE              =  4022;	// Not same character type for clear stage.
	const int FAILURE_WRONG_ITEM_ACCOUNT_ID                                =  4023;	// Drop item's accountId is wrong.
	
	// stage play - extension
	const int FAILURE_NOT_ENOUGH_AETHER_INVENTORY_LEFT_SLOT                =  4030;	// Not enough aether inventory left slot.
	const int FAILURE_START_STAGE_TRY_AGAIN_LATER                          =  4031;	// Please try play again later. caused fast re-starting.
	const int FAILURE_NOT_ENOUGH_STAGE_SWEEP_TICKET = 4032; //
	
	// stage buff
	const int FAILURE_NOT_ENOUGH_EXP_BUFF_TICKET                           =  4040;	// Not enough exp buff ticket
	const int FAILURE_NOT_ENOUGH_GOLD_BUFF_TICKET                          =  4041;	// Not enough gold buff ticket
	const int FAILURE_NOT_ENOUGH_AUTO_SKILL_BUFF_TICKET                    =  4042;	// Not enough auto skill buff ticket
	const int FAILURE_NOT_ENOUGH_BOOST_BUFF_TICKET                         =  4043;	// Not enough boost buff ticket
	
	// counter dungeon
	const int FAILURE_NOT_EXIST_COUNTER_DUNGEON_INFO                       =  4100;	// Not exist counter dungeon info.
	const int FAILURE_NOT_OPENED_COUNTER_DUNGEON                           =  4101;	// not opened counter dungeon.
	const int FAILURE_NOT_ENOUGH_COUNTER_DUNGEON_TICKET                    =  4102;	// not enough counter dungeon ticket.
	const int FAILURE_NOT_SAME_COUNTER_DUNGEON_ID_TO_START                 =  4103;	// not same dungeon id to start.
	const int FAILURE_NOT_SAME_COUNTER_DUNGEON_TOKEN                       =  4104;	// not same counter dungeon token.
	const int FAILURE_OVERFLOW_COUNTER_DUNGEON_BUY_TICKET_MAX_COUNT        =  4105;	// Overflow max counter dungeon buy ticket count.
	const int FAILURE_NOT_SAME_COUNTER_DUNGEON_BUY_TICKET_COUNT            =  4106;	// not same counter dungeon buy count.
	const int FAILURE_NOT_ENOUGH_COUNTER_DUNGEON_WEEKLY_CLEAR_COUNT        =  4107;	// not enough counter dungeon clear count.
	const int FAILURE_ALREADY_RECEIVED_COUNTER_DUNGEON_WEEKLY_REWARD       =  4108;	// already received counter dungeon weekly reward.
	const int FAILURE_NOT_RECEIVED_COUNTER_DUNGEON_PREV_WEEKLY_REWARD      =  4109;	// not received counter dungeon previous weekly reward.
	const int FAILURE_NOT_SAME_COUNTER_DUNGEON_REWARD_RECEIVE_COUNT        =  4110;	// not same counter dungeon reward receive count.
	const int FAILURE_NOT_ENTERABLE_CHARACTER_TYPE_FOR_COUNTER_DUNGEON     =  4111;	// not enterable character type for counter dungeon.
	const int FAILURE_UPDATE_COUNTER_DUNGEON_TICKET_COUNT                  =  4112;	// Failure updating counter dungeon ticket count.
	const int FAILURE_LOW_DIFFICULTY_LEVEL_COUNTER_DUNGEON_SWEEP           =  4113;	// Low difficulty level for counter dungeon sweep.
	
	// hero tower
	const int FAILURE_NOT_EXIST_HERO_TOWER_INFO                            =  4200;	// Not exist hero tower info.
	const int FAILURE_ALREADY_BOUGHT_HERO_TOWER_RETRY_TICKET               =  4201;	// Already bought hero tower retry ticket.
	const int FAILURE_HERO_TOWER_CONTINUE_PLAY_FOR_PLAY_TOKEN_WRONG        =  4203;	// HeroTower continue play fail for playToken.
	const int FAILURE_HERO_TOWER_CONTINUE_PLAY_FOR_FLOOR_WRONG             =  4204;	// HeroTower continue play fail for floor.
	const int FAILURE_HERO_TOWER_ENTER_FLOOR_IS_WRONG                      =  4205;	// HeroTower enter floor is wrong.
	const int FAILURE_HERO_TOWER_REMAIN_RETRY_COUNT                        =  4206;	// HeroTower remain retry count.
	const int FAILURE_HERO_TOWER_HERO_PIECE_UNDERFLOW                      =  4207;	// Hero piece count is underflow.
	const int FAILURE_HERO_TOWER_FINISH_FOR_PLAY_TOKEN_WRONG               =  4208;	// HeroTower finish fail for playToken.
	const int FAILURE_HERO_TOWER_FINISH_FLOOR_IS_WRONG                     =  4209;	// HeroTower finish floor is wrong.
	const int FAILURE_HERO_TOWER_OVER_DAILY_FREE_RETRY_COUNT               =  4211;	// Over the heroTower daily free retry count.
	const int FAILURE_ALREADY_RECEIVED_SWEEP_REWARD                        =  4212;	// Already received sweep reward.
	const int FAILURE_HERO_TOWER_CLEAR_FLOOR_UNDERFLOW                     =  4213;	// HeroTower clearFloor zero.
	const int FAILURE_NOT_EXIST_HERO_TOWER_SWEEP_INFO                      =  4214;	// Not exist hero tower sweep info.
	const int FAILURE_DELETED_HERO_TOWER_SWEEP_INFO                        =  4215;	// It's deleted hero tower sweep info.
	
	// relic
	const int FAILURE_NOT_EXIST_RELIC_INFO                                 =  4300;	// Not exist relic info.
	const int FAILURE_NOT_EXIST_TARGET_RELIC_GRADE                         =  4301;	// Not exist target relic grade.
	const int FAILURE_NOT_EXIST_TARGET_RELIC_ENHANCE_LEVEL                 =  4302;	// Not exist target relic enhance level.
	const int FAILURE_ALREADY_TARGET_ENHANCE_LEVEL_MAX                     =  4303;	// Already target enhance level is max.
	const int FAILURE_ALREADY_TARGET_GRADE_MAX                             =  4304;	// Already target grade is max.
	const int FAILURE_RELIC_ENHANCE_LEVEL_WRONG                            =  4305;	// Relic enhance level is wrong.
	const int FAILURE_DUPLICATE_RELIC_INFO                                 =  4306;	// Relic info already exists.
	const int FAILURE_RELIC_GRADE_WRONG                                    =  4307;	// Relic grade is wrong.
	const int FAILURE_TARGET_NOT_MAXIMUM_ENHANCE_LEVEL                     =  4308;	// Target is not at maximum enhance level.
	const int FAILURE_NEED_MORE_BOSS_PIECE_ITEM_AMOUNT                     =  4309;	// Need more bossPiece item amount.
	
	// guild
	const int FAILURE_DUPLICATED_GUILD_ID                                  =  5000;	// Guild Id already exists.
	const int FAILURE_DUPLICATED_GUILD_NAME                                =  5001;	// Guild Name already exists.
	const int FAILURE_DUPLICATED_ACCOUNTID_GUILDMEMBER                     =  5002;	// Account Id already exists in guild_member
	const int FAILURE_NOT_EXIST_GUILD_ID                                   =  5003;	// Guild Id not exist.
	const int FAILURE_DUPLICATED_GM_ACCOUNT_ID                             =  5004;	// GmAccountId already exists another guild.
	const int FAILURE_ALREADY_INVITED_ACCOUNT                              =  5005;	// Already invited account.
	const int FAILURE_ALREADY_JOINED_SOME_GUILD                            =  5006;	// Already joined some guild.
	const int FAILURE_OVERFLOW_INVITE_GUILD_COUNT                          =  5007;	// Overflow max invite guild count.
	const int FAILURE_OVERFLOW_GUILD_MEMBER_COUNT                          =  5008;	// Overflow max guild member count.
	const int FAILURE_RESET_GUILDINFO_FOR_NOT_SAME_GUILD_ID                =  5009;	// Reset fail for not same guildId.
	const int FAILURE_NEED_HIGHER_RANK_FOR_INVITE_GUILD                    =  5010;	// Need higher rank for invite guild.
	const int FAILURE_NOT_EXIST_INVITE_FROM_GUILD                          =  5011;	// Not exist invite from guild.
	const int FAILURE_NOT_EXIST_GUILD_NAME                                 =  5012;	// Guild name is not exist.
	
	const int FAILURE_NEED_MORE_TIME_FOR_REJOIN_GUILD                      =  5014;	// Need more time for rejoin guild.
	const int FAILURE_NOT_EXIST_GUILDMEMBER                                =  5015;	// Not exist guild member.
	const int FAILURE_ONLY_GUILD_MASTER_CAN_DELETE_GUILD                   =  5016;	// Only guild master can delete guild.
	const int FAILURE_LOW_RANK_FOR_APPOINT_SUB_MASTER                      =  5017;	// Low rank for appoint sub guild master.
	const int FAILURE_LOW_RANK_FOR_DISMISS_SUB_MASTER                      =  5018;	// Low rank for dismiss sub guild master.
	const int FAILURE_NEED_LOW_RANK_FOR_TO_BE_A_SUB_GUILD_MASTER           =  5019;	// Need low rank for to be a sub guild master.
	const int FAILURE_NOT_SAME_BEFORE_RANK_FOR_CHANGE_RANK                 =  5020;	// Not same before rank for change rank.
	const int FAILURE_NOT_SAME_GUILD                                       =  5021;	// Not same guild.
	const int FAILURE_WRONG_RANK_FOR_DISMISSED_SUB_MASTER                  =  5022;	// Wrong rank for dismissed sub master.
	const int FAILURE_NEED_HIGHER_RANK_FOR_KICK_GUILD_MEMBER               =  5023;	// Need higher rank for kick guild member.
	const int FAILURE_NOT_CHANGED_UPDATE_GUILD_VALUES                      =  5024;	// Not changed update guild values.
	const int FAILURE_NEED_HIGHER_RANK_FOR_UPDATE_GUILD                    =  5025;	// Need higher rank for update guild.
	const int FAILURE_NEED_HIGHER_RANK_FOR_UPDATE_GUILD_NOTICE             =  5026;	// Need higher rank for update guild notice.
	const int FAILURE_NOT_UPDATED_GUILD_NOTICE                             =  5027;	// Not updated guild notice.
	const int FAILURE_TOO_LONG_NOTICE_MESSAGE                              =  5028;	// Too long notice message.
	const int FAILURE_NEED_HIGHER_RANK_FOR_OBTAIN_GUILD_MASTER             =  5029;	// Need sub guild master rank for obtain guild master.
	const int FAILURE_NEED_MORE_TIME_FOR_OBTAIN_GUILD_MASTER               =  5030;	// Need more time for obtain guild master.
	
	const int FAILURE_GUILD_SUPPORT_ALREADY_EXIST                          =  5050;	// Already exist guild support.
	const int FAILURE_GUILD_SUPPORT_IN_COOL_TIME                           =  5051;	// Wait more time for cool time.
	const int FAILURE_NOT_EXIST_GUILD_SUPPORT                              =  5052;	// Not exist guild support.
	const int FAILURE_TODAY_SUPPORT_COUNT_OVERFLOW                         =  5053;	// Today support count overflow.
	const int FAILURE_NEED_MORE_ITEM_FOR_SUPPORT                           =  5054;	// Need more item for support.
	const int FAILURE_UPDATE_GUILD_SUPPORT_CHAT_DATA                       =  5055;	// Update guild support fail.
	const int FAILURE_TARGET_ACCOUNT_ID_NOT_SAME_FROM_CLIENT               =  5056;	// Target account id is not same from client.
	const int FAILURE_TARGET_ACCOUNT_IS_NOT_GUILD_MEMBER                   =  5057;	// Target account is not guild member.
	const int FAILURE_TARGET_SUPPORT_IS_COMPLETED                          =  5058;	// Target support is already completed.
	const int FAILURE_TARGET_SUPPORT_IS_EXPIRED                            =  5059;	// Target support is already expired.
	
	const int FAILURE_TODAY_MERCENARY_DONATION_COUNT_OVERFLOW              =  5070;	// Today mercenary donation count is over.
	const int FAILURE_TRANSACTION_WHILE_INCREASE_MERCENARY_EXP             =  5071;	// Increase mercenary exp fail.
	const int FAILURE_MERCENARY_ALREADY_MAX_LEVEL                          =  5072;	// Mercenary is already max level.
	
	// guild battle
	const int FAILURE_NOT_AVAILABLE_STATE_FOR_EXECUTE_JOB                  =  5101;	// Not available state for execute job.
	const int FAILURE_NOT_CHANGED_GUILD_BATTLE_STATE                       =  5102;	// Not changed guild battle state.
	const int FAILURE_NEXT_STATE_TYPE_IS_WRONG                             =  5103;	// Next state type is wrong.
	const int FAILURE_RENAME_TABLE_ALREADY_EXIST                           =  5104;	// Rename table already exist.
	const int FAILURE_NOT_ENABLE_BACKUP_AND_SNAP_FOR_TABLE_TYPE            =  5105;	// Not enable backup and snap for table type.
	const int FAILURE_NOT_ENABLE_BACKUP_FOR_TABLE_TYPE                     =  5106;	// Not enable backup for table type.
	const int FAILURE_UPDATE_MATCHING_GUILD_INFO                           =  5107;	// Update matching guild info.
	const int FAILURE_SET_RECEIVED_TURN_REWARD                             =  5108;	// Set received turn reward.
	const int FAILURE_SET_RECEIVED_SEASON_REWARD                           =  5109;	// Set received season reward.
	const int FAILURE_COPY_TABLE_ALREADY_EXIST                             =  5110;	// Copy table already exist.
	const int FAILURE_NOT_ENABLE_STATE                                     =  5111;	// Not enable state.
	const int FAILURE_LOW_RANK_FOR_PARTICIPATION_GUILDBATTLE               =  5112;	// Low rank for participation guild battle.
	const int FAILURE_NEED_MORE_MEMBER_FOR_GUILDBATTLE                     =  5113;	// Need more member for guildbattle.
	const int FAILURE_ALREADY_PARTICIPATION_GUILDBATTLE                    =  5114;	// Already participation guildbattle.
	const int FAILURE_NOT_EXIST_TARGET_GUILD                               =  5115;	// Not exist target guild.
	const int FAILURE_NOT_EXIST_TARGET_GUILDBATTLE_MEMBER                  =  5116;	// Not exist target guildbattle member.
	const int FAILURE_NOT_EXIST_MY_GUILDBATTLE_MEMBER                      =  5117;	// Not exist my guildbattle member.
	const int FAILURE_NOT_ENOUGH_ATTACK_COUNT                              =  5118;	// Not enough attack count.
	const int FAILURE_NOT_ENOUGH_MEDAL_COUNT_TARGET                        =  5119;	// Not enough medal count.
	const int FAILURE_ALREADY_ALLY_ATTACKED                                =  5120;	// Already ally attacked.
	const int FAILURE_NOT_SAME_BATTLE_TOKEN                                =  5121;	// Not same battle token.
	const int FAILURE_NOT_SAME_TARGET_ACCOUNT                              =  5122;	// Not same target account.
	const int FAILURE_ALREADY_ATTACKED_TARGET                              =  5123;	// Already attacked target.
	const int FAILURE_GUILD_BATTLE_ROUND_WIN_COUNT_WRONG                   =  5124;	// GuildBattle round win count is wrong.
	const int FAILURE_LEAVE_GUILD_AFTER_BATTLE_STARTED_BY_LAST_MEMBER      =  5125;	// Cant leave guild after battle started by last guild member.
	const int FAILURE_ATTACK_TARGET_MEMBER_FOR_DELETED_ACCOUNT             =  5126;	// Cant attack target member for deleted account.
	const int FAILURE_KICK_MEMBER_AFTER_BATTLE_STARTED                     =  5127;	// Cant kick member after battle started.
	const int FAILURE_NOT_EXIST_TURN_RESULT                                =  5128;	// Not exist turn result.
	const int FAILURE_ALREADY_RECEIVED_TURN_REWARD                         =  5129;	// Already received turn reward.
	const int FAILURE_NOT_EXIST_SEASON_RANKING                             =  5130;	// Not exist season ranking.
	const int FAILURE_ALREADY_RECEIVED_SEASON_REWARD                       =  5131;	// Already received season reward.
	const int FAILURE_GUILD_BATTLE_TIME_LIMIT_OVER                         =  5132;	// Guild battle time limit over.
	
	const int FAILURE_INCREASE_GUILD_EXP                                   =  5150;	// Can't increase guild exp.
	const int FAILURE_LEVEL_UP_GUILD_SKILL_ALREADY_MAX                     =  5160;	// Already skill level max.
	const int FAILURE_LEVEL_UP_GUILD_SKILL_REQ_GUILD_LEVEL                 =  5161;	// Guild Level is row.
	const int FAILURE_LEVEL_UP_GUILD_SKILL                                 =  5162;	// Can't level up guild skill for db error.
	
	const int FAILURE_BUY_GUILD_SKILL_NOT_ACTIVE_SKILL                     =  5165;	// Not active skill id.
	const int FAILURE_BUY_GUILD_SKILL_NOT_LEARN_SKILL                      =  5166;	// Not learn guild skill id.
	const int FAILURE_BUY_GUILD_SKILL                                      =  5167;	// Can't buy guild skill for db error.
	
	const int FAILURE_GUILD_BATTLE_ENTRY_COUNT_IS_WRONG                    =  5171;	// GuildBattle entry count is wrong.
	const int FAILURE_GUILD_BATTLE_ENTRY_SLOT_NUM_IS_WRONG                 =  5172;	// GuildBattle entry slot number is wrong.
	const int FAILURE_GUILD_BATTLE_ENTRY_CODE_NUM_IS_WRONG                 =  5173;	// GuildBattle entry code number is wrong.
	const int FAILURE_GUILD_BATTLE_ENTRY_MERCENARY_IS_NOT_USABLE           =  5174;	// GuildBattle entry mercenary is not usable.
	const int FAILURE_GUILD_BATTLE_ENTRY_CHARACTER_DUPLICATED              =  5175;	// GuildBattle entry character is duplicated.
	const int FAILURE_GUILD_BATTLE_ENTRY_MERCENARY_USE_COUNT_OVERFLOW      =  5176;	// GuildBattle entry mercenary's use count is overflow.
	const int FAILURE_GUILD_BATTLE_TEAM_ENTRY_COUNT_IS_WRONG               =  5177;	// GuildBattle team entry count is wrong.
	const int FAILURE_GUILD_BATTLE_ENTRY_UPDATE                            =  5178;	// GuildBattle entry update fail.
	
	// friend
	const int FAILURE_DUPLICATED_FRIEND_ACCOUNT                            =  5500;	// Friend is already exists.
	const int FAILURE_NOT_EXIST_FRIEND                                     =  5501;	// Not exist friend.
	const int FAILURE_OVERFLOW_FRIEND_COUNT                                =  5502;	// Overflow max friend count.
	const int FAILURE_ALREADY_SEND_ASK_FRIEND                              =  5503;	// Already send ask friend.
	const int FAILURE_OVERFLOW_SEND_ASK_FRIEND_COUNT                       =  5504;	// Overflow max send ask friend count.
	const int FAILURE_ALREADY_RECEIVE_ASK_FRIEND                           =  5505;	// Already receive ask friend.
	const int FAILURE_OVERFLOW_RECEIVE_ASK_FRIEND_COUNT                    =  5506;	// Overflow max receive ask friend count.
	const int FAILURE_ALREADY_FRIEND                                       =  5507;	// Already friend
	const int FAILURE_NOT_EXIST_RECEIVE_ASK_FRIEND                         =  5508;	// Not exist receive ask friend.
	const int FAILURE_NOT_EXIST_SEND_ASK_FRIEND                            =  5509;	// Not exist send ask friend.
	const int FAILURE_OVERFLOW_MAX_DELETE_FRIEND_COUNT_FOR_DAY             =  5510;	// Overflow max delete friend count for day.
	const int FAILURE_NEED_MORE_TIME_TO_SEND_SOCIAL_POINT                  =  5511;	// Need more time to send social point.
	const int FAILURE_FIND_USER_ID_SAME_TO_MINE                            =  5512;	// Find user id is same to mine.
	const int FAILURE_OVERFLOW_TARGET_FRIEND_COUNT                         =  5513;	// Overflow max friend count for target user.
	const int FAILURE_NOT_EXIST_PLATFORM_FRIEND_INVITE_REWARD              =  5514;	// Not exist platform friend invite reward info.
	const int FAILURE_DUPLICATED_PLATFORM_FRIEND_INVITE_REWARD             =  5515;	// fail duplicated platform friend invite reward info.
	const int FAILURE_PLATFORM_FRIEND_INVITE_REWARD_MISMATCH               =  5516;	// fail platform friend invite reward info mismatch.
	const int FAILURE_NOT_SUPPORTED_PLATFORM_FRIEND                        =  5517;	// Not supported platform friend
	const int FAILURE_INVALID_FRIEND_INVITE_REWARD_TYPE                    =  5518;	// Invalid friend invite reward type.
	const int FAILURE_ALREADY_RECEIVED_FRIEND_INVITE_REWARD                =  5519;	// fail already received friend invite reward.
	const int FAILURE_NOT_ENOUGH_COUNT_FRIEND_INVITE_REWARD                =  5520;	// fail fail not enough count friend invite reward.
	
	// mail
	const int FAILURE_NOT_EXIST_MAIL                                       =  6000;	// Not Exist Mail
	const int FAILURE_EXPIRED_MAIL                                         =  6001;	// Expired Mail
	const int FAILURE_OPEN_PRESENT_MAIL                                    =  6002;	// fail open present mail
	const int FAILURE_LOTTERY_SELECT_ITEM_INVALID_RATE_INFOS               =  6003;	// lottery select item invalid random value
	const int FAILURE_INVALID_MAIL_ATTACH_TYPE                             =  6004;	// invalid mail attach type
	const int FAILURE_NOT_SUPPORTED_CATEGORY_ADD_MAIL                      =  6005;	// not supported mail category to add mail
	const int FAILURE_INVALID_TEMPLATE_ID_TO_SELECT_LOTTERY                =  6006;	// invalid template id to select lottery
	const int FAILURE_INVALID_TEMPLATE_ID_TO_SELECT_ITEM_LOTTERY           =  6007;	// invalid template id to select item lottery
	const int FAILURE_CREATE_MAIL                                          =  6008;	// fail create mail
	const int FAILURE_EXCEED_MAX_MAIL_COUNT                                =  6009;	// fail exceed max mail count
	const int FAILURE_NOT_EXIST_TO_OPEN_MAIL                               =  6010;	// fail not exist to open mail
	
	//act reward
	const int FAILURE_NOT_EXIST_REPEAT_REWARD_HISTORY                      =  6100;	// Not exist repeat reward history
	const int FAILURE_NOT_ENOUGH_REPEAT_CLEAR_COUNT_TO_REWARD              =  6101;	// Not enough clear count
	const int FAILURE_UPDATE_ACT_REPEAT_CLEAR_HISTORY                      =  6102;	// Failure update repeat clear history
	
	// reward box
	const int FAILURE_EMPTY_CONFIGURATION_DUEL_REWARD_BOX                  =  6120;	// No any item for the reward box.
	const int FAILURE_NOT_EXIST_ANY_DUEL_REWARD_BOX                        =  6121;	// No any reward box info for the deul mode.
	const int FAILURE_NOT_ENABLE_OPEN_DUEL_REWARD_BOX                      =  6122;	// The reward box is unopened.
	const int FAILURE_NOT_EXIST_DUEL_REWARD_BOX                            =  6123;	// Not exist duel reward box
	const int FAILURE_INVALID_DUEL_MODE                                    =  6124;	// Invalid duel mode type.
	const int FAILURE_UPDATE_DUEL_REWARD_BOX                               =  6125;	// Failure update duel reward box.
	const int FAILURE_INVALID_DUEL_REWARD_BOX_GRADE                        =  6126;	// Invalid duel reward box grade.
	
	//actClearReward
	const int FAILURE_ALREADY_ALL_RECEIVED_ACT_CLEAR_REWARD                =  6300;	// fail already all receive act clear reward
	const int FAILURE_NOT_ENOUGH_COUNT_ACT_CLEAR_REWARD                    =  6301;	// fail not enough count act clear reward
	
	// match
	const int FAILURE_NOT_EXIST_TEAM_BATTLE_FORMATION                      =  6500;	// Not exist team battle formation.
	const int FAILURE_OVERFLOW_TEAM_BATTLE_FORMATION_LEVEL                 =  6501;	// Overflow team battle formation level.
	const int FAILURE_NOT_EXIST_TEAM_BATTLE_INFO                           =  6502;	// Not exist team battle info.
	const int FAILURE_NOT_EXIST_TEAM_BATTLE_ENTRY                          =  6503;	// Not exist team battle entry.
	const int FAILURE_SAME_CHARACTER_EXIST_FOR_TEAMBATTLE_ENTRY            =  6504;	// Same character exists.
	const int FAILURE_NOT_SAME_CURRENT_FORMATION_LEVEL                     =  6505;	// Not same current formation level.
	const int FAILURE_NOT_START_STATE_MATCH_INFO                           =  6506;	// Not start state in match info.
	const int FAILURE_NOT_EXIST_MATCH_INFO                                 =  6507;	// Not exist match info.
	const int FAILURE_NOT_SAME_MATCH_TOKEN                                 =  6508;	// Not same match token.
	const int FAILURE_NOT_EXIST_TEAM_BATTLE_STATUS                         =  6509;	// Not exist team battle status.
	const int FAILURE_ALREADY_RECEIVED_REWARD                              =  6510;	// Already received reward.
	const int FAILURE_NOT_EXIST_IN_RANKING_LIST                            =  6511;	// Not exist in ranking list.
	const int FAILURE_NOT_EXIST_MD_RANKING_REWARD                          =  6512;	// Not exist ranking reward masterdata.
	const int FAILURE_NOT_AVAILABLE_MD_PVP_RANKING_REWARD                  =  6513;	// Not available pvp ranking reward masterdata.
	const int FAILURE_NOT_AVAILABLE_MD_TEAM_RANKING_REWARD                 =  6514;	// Not available team ranking reward masterdata.
	const int FAILURE_NOT_ENOUGH_TEAM_MATCH_POINT                          =  6515;	// Not enough team match point.
	const int FAILURE_INVALID_COUNT_TO_USE_ADDITIONAL_MATCH                =  6516;	// Invalid count to use additional match
	const int FAILURE_UPDATE_ACCOUNT_BATTLE_STATUS                         =  6517;	// Failure update account battle status
	const int FAILURE_NOT_AVAILABLE_MD_ASSAULT_RANKING_REWARD              =  6518;	// Not available assault ranking reward masterdata.
	const int FAILURE_UPDATE_ACCOUNT_PVP_MATCH_GRADE                       =  6519;	// fail update account pvp match grade
	const int FAILURE_UPDATE_ACCOUNT_ASSAULT_MATCH_GRADE                   =  6520;	// fail update account assault match grade
	const int FAILURE_NOT_AVAILABLE_MD_ASSAULT_VICTORY_REWARD              =  6521;	// Not available assault victory reward masterdata
	const int FAILURE_NOT_AVAILABLE_MD_ASSAULT_MVP_REWARD                  =  6522;	// Not available assault mvp reward masterdata
	const int FAILURE_NOT_ENOUGH_ASSAULT_MVP_POINT                         =  6523;	// Not enough assault mvp point
	const int FAILURE_PROCESS_LOSE_ASSAULT                                 =  6524;	// fail process lose assault
	const int FAILURE_NOT_EXIST_MD_TEAM_BATTLE_REWARD                      =  6525;	// Not exist md team battle reward
	const int FAILURE_NOT_EXIST_MD_PVP_BATTLE_REWARD                       =  6526;	// Not exist md pvp battle reward
	const int FAILURE_NOT_EXIST_MD_ASSAULT_BATTLE_REWARD                   =  6527;	// Not exist md assault battle reward
	const int FAILURE_NOT_EXIST_TEAM_BATTLE_OPPONENT                       =  6528;	// Not exist team battle opponent
	
	// hall of fame
	const int FAILURE_ALREADY_RECEIVED_RANKER_REWARD                       =  6550;	// Already received ranker reward
	const int FAILURE_CANT_PRAISE_SELF                                     =  6551;	// Cant praise self
	const int FAILURE_NOT_EXIST_PRAISE_TARGET                              =  6552;	// Not exist praise target
	const int FAILURE_CANT_RESET_PRAISE_COUNT                              =  6553;	// Cant reset praise count
	const int FAILURE_WRONG_MODE_TYPE_FOR_PRAISE                           =  6554;	// Wrong mode type for praise
	const int FAILURE_OVERFLOW_DAILY_PRAISE_COUNT                          =  6555;	// Overflow daily praise count
	const int FAILURE_CANT_INCREASE_PRAISE_COUNT                           =  6556;	// Cant increase praise count
	
	//rank
	const int FAILURE_NOT_ENOUGH_RANK_PROMOTION_POINT_TO_ENHANCE_RANK_NODE =  6600;	// not enough rank promotion point to rank node
	const int FAILURE_ALREADY_MAX_POINT_TO_ENHANCE_RANK_NODE               =  6601;	// already max point to rank node
	const int FAILURE_NOT_CLEARED_STAGE_TO_ENHANCE_RANK_NODE               =  6602;	// not cleared stage to rank node
	const int FAILURE_STATE_THAT_CAN_NO_LONGER_BE_ENHANCED                 =  6603;	// State that can no longer be enhanced
	const int FAILURE_NOT_EXIST_RANK_INFO                                  =  6604;	// not exist rank info
	const int FAILURE_ENHANCE_NODE_STATE_NOT_MATCHED_FIRST                 =  6605;	// enhance node state not matched first
	const int FAILURE_NOT_REACHED_REQUIRE_LEVEL_ENHANCE                    =  6607;	// not reached require level enhance
	const int FAILURE_INCREASE_RANK_PROMOTION_POINT                        =  6608;	// fail increase rank promotion point
	
	//quest
	const int FAILURE_NOT_READY_TO_REWARD_QUEST                            =  6700;	// fail not ready to reward quest
	const int FAILURE_OVER_COUNT_DAILY_QUEST_PER_DAY                       =  6701;	// fail over count daily quest
	const int FAILURE_NOT_EXIST_QUEST_DATA                                 =  6702;	// fail not exist quest data
	const int FAILURE_INVALID_REWARD_ID                                    =  6703;	// fail invaild reward id
	const int FAILURE_DUPLICATED_QUEST_DATA                                =  6704;	// quest data already exists
	const int FAILURE_FAIL_UPDATE_QUEST_COUNT                              =  6705;	// fail update quest count
	const int FAILURE_INVALID_QUEST_SLOT_TYPE                              =  6706;	// fail invalid quest slot type
	const int FAILURE_INVALID_CREATE_MAIN_QUEST_COUNT                      =  6707;	// fail invalid create main quest
	const int FAILURE_INVALID_QUEST_TYPE_TO_ACCEPT_QUEST                   =  6708;	// fail invalid quest type to accpet quest
	const int FAILURE_INVALID_QUEST_TYPE                                   =  6709;	// fail invalid quest type
	const int FAILURE_INVALID_QUEST_STATE                                  =  6710;	// invalid quest state
	const int FAILURE_NOT_FOUNDED_NEXT_DAILY_QUEST                         =  6711;	// not founded next daily quest
	const int FAILURE_INIT_DAILY_QUEST                                     =  6712;	// Failure init daily quest
	const int FAILURE_GIVE_UP_QUEST_NOT_PROGRESS_STATE                     =  6713;	// fail give up quest because not progress state.
	const int FAILURE_GIVE_UP_QUEST_INVALID_SLOT_TYPE                      =  6714;	// fail give up quest because invalid slot type.
	
	//donaiton
	const int FAILURE_NOT_EXIST_DONATION_STATUS							   = 6800; // 扁何 惑怕 沥焊啊 粮犁窍瘤 臼阑 版快
	const int	FAILURE_DUPLICATED_DONATION_STATUS						   = 6801; //	吝汗等 扁何 惑怕 沥焊啊 粮犁 且 版快
	const int	FAILURE_NOT_FREE_DONATION_STATE							   = 6802; //	公丰扁何 矫 公丰扁何 冉荐甫 葛滴 家柳沁芭唱 弓澜扁何狼 惑怕夸没捞 粮犁且 版快
	const int	FAILURE_NOT_MATCHED_LAST_DONATION_KEY_VALUE				   = 6803; //`茄锅 歹` 扁何 八刘侩 虐蔼捞 蜡瓤窍瘤 臼阑 版快
	const int	FAILURE_UPDATE_DAILY_FREE_COUNT							   = 6804; //老老 公丰 扁何 俺荐 诀单捞飘啊 角菩沁阑 版快
	const int	FAILURE_UPDATE_DONATION_POINT							   = 6805; //	扁何 器牢飘 诀单捞飘啊 角菩沁阑 版快
	const int	FAILURE_UPDATE_LAST_DONATION_KEY_VALUE					   = 6806; //`茄锅 歹` 扁何 八刘侩 虐 诀单捞飘啊 角菩沁阑 版快
	const int	FAILURE_INVALID_DONATION_ID								   = 6807; //扁何 捞亥飘 酒捞叼啊 蜡瓤窍瘤 臼篮 版快
	const int	FAILURE_ALREADY_REWARDED_DONATION_POINT_REWARD		   	   = 6808; //	捞固 荐飞茄 扁何 器牢飘 焊惑阑 促矫 夸没茄 版快
	const int	FAILURE_UPDATE_DONATION_POINT_REWARD_STATE				   = 6809; //扁何 器牢飘 焊惑 惑怕 沥焊 诀单捞飘啊 角菩沁阑 版快
	const int	FAILURE_NOT_ENOUGH_POINT_RECEIVE_REWARD					   = 6810; //	扁何 器牢飘 喊 焊惑 罐扁 矫 器牢飘啊 何练且 版快
	const int	FAILURE_DUPLICATE_DONATION_EVENT_IN_PROGRESS			   = 6813; //	柳青吝牢 扁何 捞亥飘啊 2俺 捞惑老 版快
	const int	FAILURE_YET_REMAIN_FREE_DONATION_COUNT					   = 6814; //蜡丰扁何 矫 公丰扁何 冉荐啊 巢酒 乐阑 版快
	const int	FAILURE_THIS_DONATION_EVENT_ALREADY_ENDED				   = 6815; //扁何 矫 扁何 捞亥飘啊 捞固 辆丰等 版快
	const int	FAILURE_NOT_YET_NEXT_DONATION_TIME						   = 6822;				// 扁何 Request 1檬 力茄
	//mission
	const int FAILURE_PUSH_REWARD_NOT_COMPLETED                            =  7000;	// fail push reward not completed
	const int FAILURE_PUSH_REWARD_ALREADY_REWARDED                         =  7001;	// fail push reward already rewarded
	const int FAILURE_PUSH_REWARD_NOT_EXIST_MISSION                        =  7002;	// fail push reward don't exist mission
	const int FAILURE_NOT_EXIST_MISSION                                    =  7003;	// fail don't exist mission
	const int FAILURE_UPDATE_MISSION_COMPLETE_POINT                        =  7004;	// fail update mission complete point
	const int FAILURE_UPDATE_SERIAL_MISSION_REWARD_STEP                    =  7005;	// fail update serial mission step
	const int FAILURE_MISSION_STATE_NOT_COMPLETED                          =  7006;	// fail mission state not completed
	const int FAILURE_UPDATE_MISSION_STATE                                 =  7007;	// fail update mission state
	const int FAILURE_NOT_EXIST_COMPLETE_POINT_DATA                        =  7008;	// fail not exist complete point data
	const int FAILURE_NOT_ENOUGH_COMPLETE_POINT                            =  7009;	// fail not exist complete point data
	const int FAILURE_UPDATE_MISSION_COMPLETE_POINT_DATA                   =  7010;	// fail update mission complete point data
	const int FAILURE_UPDATE_SERIAL_MISSION_COMPETE_STEP                   =  7011;	// fail serial mission complete step
	const int FAILURE_INVALID_MISSION_TYPE                                 =  7012;	// fail invalid mission type
	const int FAILURE_NOT_EXIST_MISSION_INIT_TIME                          =  7013;	// fail not exist mission init time
	const int FAILURE_UPDATE_MISSION_COMPLETE_INIT_TIME                    =  7014;	// fail update mission complete init time
	const int FAILURE_UPDATE_MISSION_COMPLETE_POINT_REWARD                 =  7015;	// fail update mission complete point reward
	const int FAILURE_UPDATE_MISSION_DAILY_INIT_DATE                       =  7016;	// fail update mission daily init date
	const int FAILURE_UPDATE_MISSION_WEEKY_INIT_DATE                       =  7017;	// fail update mission weekly init date
	const int FAILURE_UPDATE_MISSION_MONTHLY_INIT_DATE                     =  7018;	// fail update mission monthly init date
	const int FAILURE_SERIAL_MISSION_NOT_ENOUGH_COMPLETE_STEP              =  7019;	// fail serial mission not enough complete step
	const int FAILURE_INCREASE_MISSION_COMPLETE_POINT                      =  7020;	// fail increase mission complete point
	const int FAILURE_NOT_COMPLETED_STATE_TO_RECEIVE_COMPLETE_POINT_REWARD =  7021;	// fail not completed state to receive complete point reward
	const int FAILURE_NOT_EXIST_MISSION_INIT_DATE                          =  7022;	// fail not exist mission init date
	const int FAILURE_NOT_REWARDABLE_MISSION_TYPE                          =  7023;	// fail not rewardable mission type
	const int FAILURE_INVAILD_MISSION_STATE_TYPE                           =  7024;	// fail invalid mission state type
	
	//wing
	const int FAILURE_INVALID_LEVEL_TO_ENHANCE_WING                        =  7100;	// fail invalid level to enhance wing
	const int FAILURE_INVALID_LEVEL_TO_UPGRADE_WING                        =  7101;	// fail invalid level to upgrade wing
	const int FAILURE_NOT_ENOUGH_WING_GRADE                                =  7102;	// fail not enough wing grade
	const int FAILURE_ALREADY_MAX_LEVEL_WING_OPTION                        =  7103;	// fail already max level wing option
	const int FAILURE_INCREASE_WING_ENHANCE_POINT                          =  7104;	// fail increase wing enhance point
	const int FAILURE_UPDATE_WING_ENHANCE_POINT                            =  7105;	// fail update wing enhance point
	const int FAILURE_INCREASE_WING_ENHANCE_LEVEL                          =  7106;	// fail increase wing enhance level
	const int FAILURE_INCREASE_WING_ENHANCE_GRADE                          =  7107;	// fail increase wing enhance grade
	const int FAILURE_UPDATE_WING_LEVEL                                    =  7108;	// fail update wing level
	const int FAILURE_INCREASE_WING_FAIL_POINT                             =  7109;	// fail increase wing fail point
	const int FAILURE_UPDATE_WING_FAIL_POINT                               =  7110;	// fail update wing fail point
	const int FAILURE_ALREADY_MAX_GRADE_TO_UPGRADE_WING                    =  7111;	// fail already max grade to upgrade wing
	const int FAILURE_UPDATE_WING_GRADE                                    =  7112;	// fail update wing grade
	const int FAILURE_DUPLICATE_WING_OPTION_DATA                           =  7113;	// fail duplicate wing option data
	const int FAILURE_NOT_EXIST_WING_OPTION_DATA                           =  7114;	// fail not exist wing option data
	const int FAILURE_UPDATE_WING_OPTION_LEVEL                             =  7115;	// fail update wing option level
	const int FAILURE_UPDATE_WING_VISIBLE                                  =  7116;	// fail update wing visible
	
	//fairy
	const int FAILURE_DUPLICATE_FAIRY_STATUS_DATA                          =  7151;	// fail duplicate fairy status data
	const int FAILURE_NOT_EXIST_FAIRY_STATUS_DATA                          =  7152;	// fail not exist fairy status data
	const int FAILURE_INVALID_BLESS_TO_OPTION_FAIRY                        =  7153;	// fail invalid bless to option fairy
	const int FAILURE_NOT_EXIST_FAIRY                                      =  7154;	// Not exist fairy.
	const int FAILURE_INVALID_LEVEL_TO_ENHANCE_FAIRY                       =  7155;	// fail invalid level to enhance fairy
	const int FAILURE_OVERFLOW_FAIRY_LEVEL                                 =  7156;	// Overflow fairy level.
	
	//shop
	const int FAILURE_INVALID_SHOP_DRAW_PRICE_TYPE                         =  7200;	// fail invalid shop draw price type
	const int FAILURE_NOT_EXIST_SHOP_MILEAGE                               =  7201;	// fail not exist shop mileage
	const int FAILURE_NOT_ENOUGH_SHOP_MILEAGE                              =  7202;	// fail not enough shop mileage
	const int FAILURE_RESET_SHOP_MILEAGE                                   =  7203;	// fail reset shop mileage
	const int FAILURE_INCREASE_SHOP_MILEAGE_REWARD_INDEX                   =  7204;	// fail increase shop mileage reward index
	const int FAILURE_INCREASE_SHOP_LOTTERY_COUNT                          =  7205;	// fail increase shop lottery count
	const int FAILURE_CREATE_SHOP_LOTTERY_FREE_TIME                        =  7206;	// fail create shop lottery free time
	const int FAILURE_UPDATE_SHOP_LOTTERY_FREE_TIME                        =  7207;	// fail update shop lottery free time
	const int FAILURE_INVALID_SHOP_PRICE_TYPE                              =  7208;	// fail invalid shop price type
	const int FAILURE_INVALID_SHOP_PACKAGE_INFO                            =  7209;	// fail invalid shop package info
	const int FAILURE_UPDATE_SHOP_MILEAGE_EXIST_REWARD                     =  7210;	// fail update shop mileage exist reward
	const int FAILURE_EXCEED_DAILY_LOTTERY_COUNT                           =  7211;	// fail exceed daily lottery count
	const int FAILURE_CAN_NOT_PURCHASE_ANYMORE                             =  7212;	// Can not purchase anymore
	const int FAILURE_RESET_SHOP_BUY_COUNT                                 =  7213;	// fail reset shop buy count
	const int FAILURE_INCREASE_SHOP_BUY_COUNT                              =  7214;	// fail increase shop buy count
	const int FAILURE_NOT_ENDED_PREVIOUS_BUFF_TIME                         =  7215;	// not ended previous buff time
	const int FAILURE_PRODUCT_IS_NOT_DISCOUNTED                            =  7216;	// This product is not discounted.
	const int FAILURE_PRODUCT_IS_NOT_SUBJECT_BONUS_INCREASES               =  7217;	// This product is not subject to bonus increases.
	const int FAILURE_PERIODIC_PAY_PACKAGE_PRODUCT_NOT_ENDED               =  7218;	// fail periodic pay package product not ended.
	const int FAILURE_UPDATE_PERIODIC_PAY_PACKAGE_PAY_TIME                 =  7219;	// fail update periodic pay package pay time.
	const int FAILURE_UPDATE_PERIODIC_PAY_PACKAGE_STATE                    =  7220;	// fail update periodic pay package state.
	const int FAILURE_ALREADY_RECEIVED_PERIODIC_PACKAGE_BONUS              =  7221;	// fail already received periodic package bonus.
	const int FAILURE_NOT_BUY_PERIODIC_PACKAGE                             =  7222;	// fail not buy periodic package
	const int FAILURE_UPDATE_PERIODIC_PACKAGE_REWARD_STATE                 =  7223;	// fail update periodic package reward state.
	const int FAILURE_REWARD_LEVEL_UP_PACKAGE_NOT_COMPLETED                =  7224;	// fail reward level up package not complete.
	const int FAILURE_UPDATE_LEVEL_UP_PACKAGE_MISSION_STATE                =  7225;	// fail update level up package mission state.
	const int FAILURE_REWARD_REACH_LEVEL_PACKAGE_NOT_COMPLETED             =  7226;	// fail reward reach level package not complete.
	const int FAILURE_UPDATE_REACH_LEVEL_PACKAGE_MISSION_STATE             =  7227;	// fail update reach level package mission state.
	const int FAILURE_UPDATE_REACH_LEVEL_BY_LIMIT_LEVEL                    =  7228;	// fail update reach level by limit level.
	const int FAILURE_UPDATE_REACH_LEVEL_BY_EXPIRE_TIME                    =  7229;	// fail update reach level by expire time.
	const int FAILURE_UPSERT_SHOP_LEVEL_UP_PACKAGE_MISSION                 =  7230;	// fail upsert shop level up package mission.
	const int FAILURE_UPSERT_SHOP_REACH_LEVEL_PACKAGE_MISSION              =  7231;	// fail upsert shop reach level package mission.
	const int FAILURE_SHOP_TEN_LOTTERY_TICKET_STILL_REMAIN                 =  7232;	// fail shopTenLotteryTicket still remain.
	const int FAILURE_NOT_ENOUGH_SHOP_TEN_LOTTERY_TICKET                   =  7233;	// Not Enough shopTenLotteryTicket.
	const int FAILURE_NOT_MATCHED_SHOP_LAST_LOTTERY_ID                     =  7234;	// Not matched shop last lottery id
	const int FAILURE_UPDATE_SHOP_LAST_LOTTERY_ID                          =  7235;	// fail update shop last lottery id
	const int FAILURE_NOT_EXIST_SHOP_PERIODIC_PACKAGE_STATE                =  7236;	// fail not exist shop periodic package state
	const int FAILURE_NOT_FREE_LOTTERY_STATE                               =  7237;	// fail not free lottery state
	
	// shop-iap ( 7250 ~ 7279 reserved )
	const int FAILURE_DUPLICATED_MARKET_PURCHASE_INFO                      =  7250;	// fail already exist market purchase info for the product.
	const int FAILURE_UPDATE_MARKET_PURCHASE_INFO_STATE                    =  7251;	// fail update market purchase state.
	const int FAILURE_MARKET_PRODUCT_ID_MISMATCH                           =  7252;	// fail market productid mismatch.
	const int FAILURE_NOT_EXIST_MARKET_PURCHASE_INFO                       =  7253;	// Not exist market purchase progressing info.
	const int FAILURE_MARKET_PURCHASE_INFO_MISMATCH                        =  7254;	// fail market purchase info mismatch.
	const int FAILURE_MARKET_TYPE_MISMATCH                                 =  7255;	// fail market type mismatch.
	const int FAILURE_MARKET_PURCHASE_ALREADY_COMPLETED                    =  7256;	// already completed market purchase info.
	const int FAILURE_MARKET_PURCHASE_RECEIPT_VERIFICATION                 =  7257;	// fail not passed receipt verification.
	const int FAILURE_MARKET_PURCHASE_RECEIPT_READ_INFO                    =  7258;	// fail read product info with purchase receipt.
	const int FAILURE_MARKET_PURCHASE_RECEIPT_MISMATCH_REQUEST             =  7259;	// The receipt and the purchase information do not match.
	const int FAILURE_MARKET_PURCHASE_RECEIPT_NOT_FOR_OURS                 =  7260;	// Not a receipt for this app.
	const int FAILURE_MARKET_PURCHASE_RECEIPT_MISMATCH_ENVIRONMENT         =  7261;	// The receipt verification is other environment.
	const int FAILURE_MARKET_PURCHASE_RECEIPT_TRY_AGAIN_LATER              =  7262;	// The receipt verification must try again later.
	const int FAILURE_MARKET_PURCHASE_RECEIPT_ALREADY_APPLIED              =  7263;	// The receipt is already applied.
	const int FAILURE_MARKET_PURCHASE_RECEIVE_ERROR_OCCURED                =  7264;	// fail only receiving products.
	
	// magic shop (7280~)
	const int FAILURE_OVERFLOW_MAGIC_SHOP_PRODUCT_LIMIT_COUNT              =  7280;	// fail overflow magic shop product limit count
	const int FAILURE_EARLIER_MAGIC_SHOP_FREE_RENEWAL                      =  7281;	// Earlier magic shop free renewal
	const int FAILURE_OVERFLOW_MAGIC_SHOP_RENEWAL_LIMIT_COUNT              =  7282;	// fail overflow magic shop renewal limit count
	const int FAILURE_INVALID_MAGIC_SHOP_PRODUCT_ID                        =  7283;	// Invalid magic shop product id
	const int FAILURE_NOT_EXIST_ACCOUNT_MAGIC_SHOP_INFO                    =  7284;	// Not exist account magic shop info
	const int FAILURE_ALREADY_PURCHASED_PRODUCT_MAGIC_SHOP                 =  7285;	// Already purchased product magic shop
	
	//collection
	const int FAILURE_NOT_ENOUGH_SET_ITEM_COUNT                            =  7300;	// fail not enough setitem count
	const int FAILURE_ALREADY_RECEIVE_SET_ITEM_REWARD                      =  7301;	// fail already receive setitem reward
	const int FAILURE_CREATE_COLLECTION_SET_ITEM_REWARD                    =  7302;	// fail already receive setitem reward
	const int FAILURE_DUPLICATED_COLLECTION_ITEM                           =  7303;	// fail duplicated collection item
	const int FAILURE_DUPLICATED_COLLECTION_SET_ITEM_REWARD                =  7304;	// fail duplicated collection set item reward
	const int FAILURE_UPDATE_COLLECTION_SET_ITEM_REWARD                    =  7305;	// fail update collection set item reward
	const int FAILURE_UPDATE_COLLECTION_ITEM_STATE                         =  7306;	// fail update collection item state
	
	//raid
	const int FAILURE_NOT_HAS_MAP_ID_TO_ROOM_CLOSE                         =  7400;	// fail not has map id to room close
	const int FAILURE_NOT_HAS_DIFFICULTY_TO_ROOM_CLOSE                     =  7401;	// fail not has difficulty to room close
	const int FAILURE_NOT_EXIST_RAID_REWARD_INFO                           =  7402;	// fail not exist raid reward info
	const int FAILURE_INVALID_RAID_REWARD_COUNT                            =  7403;	// fail invalid raid reward count
	const int FAILURE_NOT_EXIST_RAID_INFO                                  =  7404;	// fail not exist raid info
	const int FAILURE_ALREADY_RAID_REWARD                                  =  7405;	// fail already raid reward
	const int FAILURE_SELECT_RAID_REWARD                                   =  7406;	// fail select raid reward
	const int FAILURE_NOT_ENOUGH_RAID_COUNT                                =  7407;	// fail not enough raid count
	const int FAILURE_EXCEED_RAID_RESURRECTION_COUNT                       =  7408;	// fail exceed raid resurrection count
	const int FAILURE_INCREASE_RAID_RESURRECTION_COUNT                     =  7409;	// fail increase raid resurrection count
	const int FAILURE_NOT_OPENED_RAID_MATCH                                =  7410;	// fail not opened raid match
	const int FAILURE_UPDATE_RAID_OPEN_TIME                                =  7411;	// fail update raid open time
	const int FAILURE_NOT_EXIST_RAID_BATTLE_CHARACTER                      =  7412;	// fail not exist raid battle character
	const int FAILURE_MATCHING_RAID_PENALTY_TIME                           =  7413;	// fail matching raid penalty time
	const int FAILURE_NOT_HAS_ROOM_ID_TO_ROOM_CLOSE                        =  7414;	// fail not has room id to room close
	const int FAILURE_UPDATE_RAID_REWARDED                                 =  7415;	// fail update raid rewarded
	const int FAILURE_NOT_HAS_ENDPOINT_OF_REJOIN                           =  7416;	// fail not has endPoint of rejoin
	
	//attendance
	const int FAILURE_NOT_PASSED_ATTENDANCE_TIME                           =  7500;	// not passed attendance time
	const int FAILURE_NOT_ACTIVE_EVENT_ATTENDANCE                          =  7502;	// fail not active event attendance
	const int FAILURE_NOT_EXIST_ACCOUNT_EVENT_ATTENDANCE_STATUS            =  7503;	// fail not exist event attendance status info
	const int FAILURE_OVER_MAX_EVENT_ATTENDANCE_DAY                        =  7504;	// fail over maximum event attendance days
	const int FAILURE_NO_ANY_RECEIVABLE_PLAY_TIME_REWARD                   =  7505;	// fail no any receivable playtime reward.
	const int FAILURE_INITIALIZED_PLAY_TIME_RECORD                         =  7506;	// fail initialize record caused by changeday.
	const int FAILURE_CLOSED_BEGINNER_ATTENDANCE_EVENT                     =  7507;	// fail already closed beginner attendance event.
	const int FAILURE_OVER_MAX_BEGINNER_ATTENDANCE_DAY                     =  7508;	// fail over maximum beginner attendance days
	const int FAILURE_UPDATE_BEGINNER_ATTENDANCE_INFO                      =  7509;	// fail update beginner attendance info
	
	//assault
	const int FAILURE_NOT_EXIST_ASSAULT_INFO                               =  7600;	// fail not exist assault info
	const int FAILURE_EXCEED_ASSAULT_RESURRECTION_COUNT                    =  7601;	// fail exceed assault resurrection count
	const int FAILURE_INCREASE_ASSAULT_RESURRECTION_COUNT                  =  7602;	// fail increase assault resurrection count
	const int FAILURE_NOT_OPENED_ASSAULT_MATCH                             =  7603;	// fail not opened assault match
	const int FAILURE_NOT_EXIST_ASSAULT_BATTLE_STATUS                      =  7604;	// Not exist assault battle status.
	
	//unity skill
	const int FAILURE_NOT_EXIST_UNITY_SKILL_MISSION                        =  7700;	// fail not exist unity skill mission
	const int FAILURE_NOT_PROGRESS_UNITY_SKILL_MISSION                     =  7701;	// fail not progress unity skill mission
	const int FAILURE_NOT_EXIST_MD_UNITY_SKILL_MISSION                     =  7702;	// fail not exist md unity skill mission
	const int FAILURE_VERIFY_COMPLETE_UNITY_MISSION                        =  7703;	// fail verify complete unity skill mission
	const int FAILURE_UPDATE_NEXT_STEP_UNITY_MISSION                       =  7704;	// fail update next step unity skill mission
	const int FAILURE_UPDATE_CHARACTER_UNITY_SKILL                         =  7705;	// fail update character unity skill
	
	//rest reward
	const int FAILURE_NOT_ENOUGH_REST_POINT                                =  7800;	// fail not enough rest point
	const int FAILURE_NOT_NOT_EXIST_REST_REWARD_DATA                       =  7801;	// fail not exist rest reward data
	
	//event
	const int FAILURE_NOT_ACTIVE_EVENT                                     =  7850;	// fail not active event
	const int FAILURE_NOT_EXIST_POINT_EVENT_REWARD_INFO                    =  7851;	// fail not exist point event reward info
	const int FAILURE_ALREADY_RECEIVED_POINT_EVENT_REWARD                  =  7852;	// fail already received point event reward
	const int FAILURE_NOT_EXIST_POINT_EVENT_STATUS                         =  7853;	// fail not exist point event status
	const int FAILURE_NOT_ENOUGH_POINT_TO_RECEIVE_POINT_EVENT_REWARD       =  7854;	// fail not enough point to receive point event reward
	const int FAILURE_NOT_COMPLETED_LEVEL_UP_EVENT                         =  7855;	// fail not completed to receive levelup event reward
	const int FAILURE_NOT_EXIST_SEASON_MISSION_EVENT_INFO                  =  7856;	// fail not exist season mission event info
	const int FAILURE_NOT_COMPLETED_SEASON_MISSION_EVENT                   =  7857;	// fail not completed to receive season mission event reward
	const int FAILURE_NOT_EXIST_LEVEL_UP_EVENT_REWARD_INFO                 =  7858;	// fail not exist levelup event reward info
	const int FAILURE_NOT_EXIST_STAGE_CLEAR_EVENT_REWARD_INFO              =  7859;	// fail not exist stage clear event reward info
	const int FAILURE_NOT_COMPLETED_STAGE_CLEAR_EVENT                      =  7860;	// fail not completed to receive stage clear event reward
	const int FAILURE_NOT_EXIST_POINT_SHOP_EVENT_STATUS                    =  7861;	// fail not exist point shop event status
	const int FAILURE_NOT_ENOUGH_POINT_TO_RECEIVE_POINT_SHOP_EVENT_REWARD  =  7862;	// fail not enough point to receive point shop event reward
	const int FAILURE_DECREASE_POINT_SHOP_EVENT_POINT                      =  7863;	// fail decrease point shop event point
	const int FAILURE_NOT_EXIST_EVENT_TEN_LOTTERY_INFO                     =  7864;	// fail not exist event ten lottery info
	const int FAILURE_CREATE_EVENT_TEN_LOTTERY_REWARD                      =  7865;	// fail create event ten lottery reward
	const int FAILURE_ALREADY_REWARD_EVENT_TEN_LOTTERY                     =  7866;	// fail already reward event ten lottery
	const int FAILURE_INCREASE_EVENT_TEN_LOTTERY_COUNT                     =  7867;	// fail increase event ten lottery count
	const int FAILURE_UPDATE_EVENT_TEN_LOTTERY_EXIST_REWARD                =  7868;	// fail update event ten lottery exist reward
	const int FAILURE_NOT_COMPLETED_EVENT_TEN_LOTTERY                      =  7869;	// fail not completed event ten lottery
	const int FAILURE_NOT_EXIST_GEM_PURCHASE_EVENT_REWARD_INFO             =  7870;	// fail not exist gem purchase event reward info
	const int FAILURE_NOT_EXIST_GEM_PURCHASE_EVENT_STATUS                  =  7871;	// fail not exist gem point event status
	const int FAILURE_NOT_ENOUGH_GEM_PURCHASE_COUNT                        =  7872;	// fail not enough gem purchase count to receive reward
	const int FAILURE_ALREADY_RECEIVED_GEM_PURCHASE_EVENT_REWARD           =  7873;	// fail already received gem purchase event reward
	const int FAILURE_NOT_EXIST_SPEND_GEN_EVENT_STATUS                     =  7874;	// fail not exist spend gem event status
	const int FAILURE_NOT_EXIST_SPEND_GEM_REWARD_INFO                      =  7875;	// fail not exist spend gem reward info
	const int FAILURE_NOT_ENOUGH_COUNT_TO_REWARD_SPEND_GEM_EVENT           =  7876;	// fail not enough count to reward spend gem event
	const int FAILURE_ALREADY_REWARD_EVENT_SPEND_GEM                       =  7877;	// fail already reward event spend gem
	const int FAILURE_INCREASE_EVENT_TEN_LOTTERY_SAVINGS_POINT             =  7878;	// fail increase event ten lottery count
	const int FAILURE_UPDATE_EVENT_TEN_LOTTERY_SAVINGS_EXIST_REWARD        =  7879;	// fail update event ten lottery savings exist reward
	const int FAILURE_NOT_EXIST_EVENT_TEN_LOTTERY_SAVINGS_INFO             =  7880;	// fail not exist event ten lottery savings info
	const int FAILURE_NOT_ENOUGH_TEN_LOTTERY_POINT                         =  7881;	// fail not enough ten lottery point
	const int FAILURE_DECREASE_EVENT_TEN_LOTTERY_SAVINGS_POINT             =  7882;	// fail decrease event ten lottery savings point
	const int FAILURE_NOT_EXIST_DICE_EVENT_STATUS                          =  7883;	// not exist dice event status
	const int FAILURE_NOT_ENOUGH_POINT_TO_DICE                             =  7884;	// fail not enough point to dice
	const int FAILURE_NOT_ENOUGH_FREE_DICE_TO_DICE                         =  7885;	// fail not enough free dice to dice
	const int FAILURE_NOT_EXIST_DICE_EVENT_CELL_INFO                       =  7886;	// fail not exist dice event cell info
	const int FAILURE_UPDATE_DICE_EVENT_POSITION                           =  7887;	// fail update dice event position
	const int FAILURE_CREATE_DICE_EVENT_STATUS                             =  7888;	// fail create dice event status
	const int FAILURE_DECREASE_DICE_EVENT_FREE_DICE                        =  7889;	// fail decrease dice event free dice
	const int FAILURE_DECREASE_DICE_EVENT_POINT                            =  7890;	// fail decrease dice event point
	const int FAILURE_NOT_EXIST_DICE_EVENT_BUY_POINT_INFO                  =  7891;	// not exist dice event buy point info
	const int FAILURE_UPDATE_TO_BUY_DICE_EVENT_POINT                       =  7892;	// fail update to buy dice event point
	const int FAILURE_ALREADY_REWARD_DICE_EVENT_MILEAGE                    =  7893;	// fail already reward dice event mileage
	const int FAILURE_UPDATE_REWARD_DICE_EVENT_MILEAGE_VALUE               =  7894;	// fail update reward dice event mileage value
	const int FAILURE_NOT_ENOUGH_DICE_COUNT_TO_REWARD_MILEAGE              =  7895;	// fail not enough dice count to reward mileage
	const int FAILURE_DICE_EVENT_POINT_UNDERFLOW                           =  7896;	// dice event point underflow
	const int FAILURE_DICE_EVENT_FREE_DICE_UNDERFLOW                       =  7897;	// dice event free dice underflow
	const int FAILURE_OVER_MAX_POINT_SHOP_EVENT_REWARD_COUNT               =  7898;	// fail over maximum point shop event reward count
	const int FAILURE_UPDATE_POINT_SHOP_EVENT_REWARD_COUNT                 =  7899;	// fail update point shop event reward count
	
	//kakao special
	const int FAILURE_KAKAO_MESSAGE_EMPTY_MESSAGEBOX                       =  7900;	// kakako message box is empty
	const int FAILURE_KAKAO_COUPON_ALREADY_USED                            =  7910;	// coupon has already been used
	const int FAILURE_KAKAO_COUPON_EXPIRED                                 =  7911;	// coupon has been expired
	const int FAILURE_KAKAO_COUPON_OVER_MAX_REDEEM_COUNT                   =  7912;	// fail over maximum coupon redeem count
	const int FAILURE_KAKAO_MESSAGE_ALREADY_PROCESSING                     =  7913;	// kakako message is already processing state
	const int FAILURE_KAKAO_CONVERSION_ALREADY_REWARDED                    =  7914;	// fail already received kakao-conversion reward
	const int FAILURE_KAKAO_INVALID_TALK_REWARD_API_KEY                    =  7915;	// invalid kakaotalk reward api key
	
	//user
	const int FAILURE_CREATE_REPORT_USER                                   =  7930;	// fail create report user
	const int FAILURE_CREATE_BLOCK_CHAT_USER                               =  7931;	// fail create block chat user
	const int FAILURE_DELETE_BLOCK_CHAT_USER                               =  7932;	// fail delete block chat user
	const int FAILURE_EXCEED_CHAT_BLOCK_USER_COUNT                         =  7933;	// fail exceed chat block user count
	
	// server command manager
	const int FAILURE_CMD_SYNTAX_WRONG                                     =  7950;	// server command syntax is wrong
	const int FAILURE_CMD_NOT_DEFINED                                      =  7951;	// server command is not defined
	const int FAILURE_CMD_NOT_AVAILABLE_SERVER_GROUP                       =  7952;	// server command is not available for server group
	const int FAILURE_CMD_NOT_AVAILABLE_ACCOUNT_TYPE                       =  7953;	// server command is not available for account type
	
	// unknown
	const int FAILURE_UNKNOWN                                              =  7999;	// Unknown
	
	// NOTE do not use 8000+ as an error code
	// rt-server defined errors
	const int ERROR_SERVICE_UNAVAILABLE                                    =  8000;	// service unavailable
	const int ERROR_API_SERVER_UNAVAILABLE                                 =  8001;	// api server unavailable
	const int ERROR_SESSION_SERVER_UNAVAILABLE                             =  8002;	// session server unavailable
	const int ERROR_CHANNEL_SERVER_UNAVAILABLE                             =  8003;	// channel server unavailable
	
	const int ERROR_CHANNEL_SERVER_ROOM_CLOSED                             =  8005;	// channel server room closed
	
	const int ERROR_VERSION_MISMATCH                                       =  8010;	// version mismatch
	const int ERROR_PROTOCOL_MISMATCH                                      =  8011;	// protocol mismatch
	
	const int ERROR_PROTOCOL_CORRUPTED                                     =  8099;	// protocol corrupted
	
	const int ERROR_UNAUTHENTICATED                                        =  8100;	// unauthenticated
	const int ERROR_INVALID_REQUEST                                        =  8101;	// invalid request
	const int ERROR_ALREADY_AUTHENTICATED                                  =  8102;	// already authenticated
	const int ERROR_UNAUTHORIZED                                           =  8104;	// unauthorized
	const int ERROR_INSECURE_SESSION                                       =  8105;	// insecure session
	
	const int ERROR_PLAYER_NOT_PRESENT                                     =  8200;	// player  not present
	const int ERROR_COULD_NOT_INVITE_ONESELF                               =  8201;	// could not invite oneself
	
	const int ERROR_MATCHMAKING_CANCELED                                   =  8210;	// matchmaking canceled
	const int ERROR_MATCHMAKING_TIMEOUT                                    =  8211;	// matchmaking timeout
	const int ERROR_MATCHMAKING_NOT_FOUND                                  =  8212;	// matchmaking not found
	const int ERROR_NOT_EXIST_RAID_MAP_ID                                  =  8213;	// not exist raid map id
	const int ERROR_NOT_EXIST_RAID_DIFFICULTY                              =  8214;	// not exist raid difficulty
	const int ERROR_EXCEED_CHAT_CHANNEL_COUNT                              =  8215;	// exceed chat channel count
	const int ERROR_INVALID_CHAT_CHANNEL_NUM                               =  8216;	// invalid chat channel num
	
	// cannot use the symbol 'ERROR_NOT_FOUND' cuz it was already defined at 'winerror.h'.
	// const int ERROR_NOT_FOUND                                              =  8404;	// not found
	const int ERROR_BAD_REQUEST                                            =  8405;	// bad request
	const int ERROR_RESPONSE_TIMEOUT                                       =  8406;	// response timeout
	
	
	
	
	
	
	
	
};

#endif //SERVER_RESULT_CODE_TABLE