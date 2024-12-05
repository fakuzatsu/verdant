#include "battle_main.h"

const u16 poolItemClauseExclusions[] =
{
    ITEM_ORAN_BERRY,
    ITEM_SITRUS_BERRY,
    ITEM_NONE,              //  This list MUST end with ITEM_NONE
};

const struct PoolRules defaultPoolRules =
{
    .speciesClause = B_POOL_RULE_SPECIES_CLAUSE,
    .excludeForms = B_POOL_RULE_EXCLUDE_FORMS,
    .itemClause = B_POOL_RULE_ITEM_CLAUSE,
    .itemClauseExclusions = B_POOL_RULES_USE_ITEM_EXCLUSIONS,
    .tagLead = POOL_TAG_UNIQUE,
    .tagAce = POOL_TAG_UNIQUE,
    .tagWeatherSetter = POOL_TAG_UNIQUE,
    .tagWeatherAbuser = POOL_TAG_UNIQUE,
    .tagSupport = POOL_TAG_UNIQUE,
};

const struct PoolRules doublesPoolRules = 
{
    .speciesClause = B_POOL_RULE_SPECIES_CLAUSE,
    .excludeForms = B_POOL_RULE_EXCLUDE_FORMS,
    .itemClause = B_POOL_RULE_ITEM_CLAUSE,
    .itemClauseExclusions = B_POOL_RULES_USE_ITEM_EXCLUSIONS,
    .tagLead = POOL_TAG_2_MAX,
    .tagAce = POOL_TAG_UNIQUE,
    .tagWeatherSetter = POOL_TAG_UNIQUE,
    .tagWeatherAbuser = POOL_TAG_UNIQUE,
    .tagSupport = POOL_TAG_UNIQUE,
};

const struct PoolRules singlesWeatherPoolRules =
{
    .speciesClause = B_POOL_RULE_SPECIES_CLAUSE,
    .excludeForms = B_POOL_RULE_EXCLUDE_FORMS,
    .itemClause = B_POOL_RULE_ITEM_CLAUSE,
    .itemClauseExclusions = B_POOL_RULES_USE_ITEM_EXCLUSIONS,
    .tagLead = POOL_TAG_UNIQUE,
    .tagAce = POOL_TAG_UNIQUE,
    .tagWeatherSetter = POOL_TAG_UNIQUE | POOL_TAG_REQUIRED,
    .tagSupport = POOL_TAG_UNIQUE,
};