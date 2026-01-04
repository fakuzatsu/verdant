#include "global.h"
#include "battle.h"
#include "test/test.h"
#include "dex/gf_string.h"
#include "constants/moves.h"
#include "item.h"

extern const struct Item gItemsInfo[];

TEST("DEX: Items")
{
    DebugPrintf("===DEBUG OUTPUT START===");

    for (u32 i = 0; i < ITEMS_COUNT; i++)
    {
        // Skip ITEM_NONE (usually index 0)
        if (i == ITEM_NONE)
            continue;

        const struct Item *item = &gItemsInfo[i];

        DebugPrintf("- itemId: %u", i);

        if (item)
        {
            char itemBuf[256];
            ConvertGfStringToUtf8(item->name, itemBuf);
            DebugPrintf("  Name: %s", itemBuf);

            char descBuf[256];
            ConvertGfStringToUtf8(item->description, descBuf);
            DebugPrintf("  Description: %s", descBuf);
        }
        // for (int i = 0; i < 256 && item->name[i] != 0xFF; i++)
        // { // 0xFF = end of string
        //     DebugPrintf("  Bytes: %02X ", item->name[i]);
        // }
    }

    DebugPrintf("===DEBUG OUTPUT END===");
}
