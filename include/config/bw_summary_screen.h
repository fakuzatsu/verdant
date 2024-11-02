#ifndef GUARD_CONFIG_BW_SUMMARY_H
#define GUARD_CONFIG_BW_SUMMARY_H

// turn on and off the bw summary screen
#define BW_SUMMARY_SCREEN                           TRUE

// constants
#define BW_MAX_MOVE_DESCRIPTION_LENGTH              60      // this only needs to be updated if you use auto-formatting
#define BW_IV_EV_HIDDEN                             0       // don't show IVs and EVs
#define BW_IV_EV_GRADED                             1       // show graded values for IVs and EVs
#define BW_IV_EV_PRECISE                            2       // show precise values for IVs and EVs
#define BW_SUMMARY_MON_IDLE_ANIMS_FRAMES            300     // number of frames between each idle anim IF BW_SUMMARY_MON_IDLE_ANIMS is true.
                                                            // for reference, Emerald runs at 60FPS by default

// configs
#define BW_EXTENDED_ABILITY_DESCRIPTIONS            TRUE                // uses ability descriptions that more appropriately fit the bw_summary screen. Conflicts with Pokedex Plus
#define BW_SUMMARY_AUTO_FORMAT_MOVE_DESCRIPTIONS    TRUE                // automatically formats move descriptions to fit the new box size. disable if you want to format them manually
#define BW_SUMMARY_NATURE_COLORS                    TRUE                // color stats increased or reduced by nature, red = boosted, blue = reduced
#define BW_SUMMARY_NATURE_ARROWS                    TRUE                // arrows to show stats increased or reduced by nature
#define BW_SUMMARY_CATEGORY_ICONS                   TRUE                // determines whether category (split) icons are shown or not
#define BW_SUMMARY_IV_EV_DISPLAY                    BW_IV_EV_PRECISE    // determines how to show IVs and EVs
#define BW_SUMMARY_DECAP                            FALSE               // indicates if summary screen-specific strings should be decapitalized
#define BW_SUMMARY_SHOW_FRIENDSHIP                  TRUE                // show a heart that fills up to indicate friendship value
#define BW_SUMMARY_BW_STATUS_ICONS                  TRUE                // use Gen 5 style status icons instead of the default ones.
#define BW_SUMMARY_BW_TYPE_ICONS                    TRUE                // use Gen 5 style type icons instead of the default ones
                                                                        // out of the box the vanilla icons don't fit well, this is mostly a compatibility
                                                                        // option for people who already use custom icons everywhere else

#define BW_SUMMARY_SCROLLING_BG                     TRUE                // enables scrolling animated background
#define BW_SUMMARY_BG_BLEND                         TRUE                // enables alpha blending for the main UI (semi-transparency)
#define BW_SUMMARY_MON_IDLE_ANIMS                   TRUE                // loops the mon animations regularly as an "idle" anim
#define BW_SUMMARY_MON_SHADOWS                      TRUE                // displays a shadow for the mon sprite
#define BW_SUMMARY_SHOW_CONTEST_MOVES               TRUE                // show the contest moves page

#endif // GUARD_CONFIG_BW_SUMMARY_H