#pragma once

#include "libraries.h"
#include "data.h"

uint8_t mui_hrule(mui_t *ui, uint8_t msg);

constexpr muif_t muif_list[] = {
    /* normal text style */
    MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),

    /* bold text style */
    MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvB08_tr),

    /* horizontal line (hrule) */
    MUIF_RO("HR", mui_hrule),

    /* main menu */
    MUIF_RO("GP", mui_u8g2_goto_data),
    MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),

    /* DRY RUN FIELDS */
    MUIF_VARIABLE("DE", &dry_run_cutoff_protection, mui_u8g2_u8_opt_line_wa_mud_pi),
    MUIF_U8G2_U8_MIN_MAX("DA", &dry_run_cutoff_power_1, 0, 1, mui_u8g2_u8_min_max_wm_mud_pi),
    MUIF_U8G2_U8_MIN_MAX("DB", &dry_run_cutoff_power_2, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
    MUIF_U8G2_U8_MIN_MAX("DC", &dry_run_cutoff_power_3, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
    MUIF_U8G2_U8_MIN_MAX("DD", &dry_run_cutoff_power_4, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
    MUIF_U8G2_U8_MIN_MAX("DS", &dry_run_cutoff_delay, 0, 200, mui_u8g2_u8_min_max_wm_mud_pi),

    /*TANK FULL PROTECTION FIELDS*/
    MUIF_VARIABLE("TP", &tank_full_cutoff_protection, mui_u8g2_u8_opt_line_wa_mud_pi),
    MUIF_U8G2_U8_MIN_MAX("TL", &tank_full_cutoff_level, 1, 8, mui_u8g2_u8_min_max_wm_mud_pi),
    MUIF_U8G2_U8_MIN_MAX("TD", &tank_full_cutoff_delay, 0, 200, mui_u8g2_u8_min_max_wm_mud_pi),

    /* Dashboard & LED STRIP */
    MUIF_VARIABLE("PS", &dashboard_style, mui_u8g2_u8_radio_wm_pi),
    MUIF_VARIABLE("LS", &led_strip_style, mui_u8g2_u8_radio_wm_pi),

    MUIF_GOTO(mui_u8g2_btn_goto_w1_pi),
    MUIF_U8G2_LABEL(),
};

constexpr fds_t fds_data[] =
    /* top level main menu */
    MUI_FORM(0)
        MUI_STYLE(1)
            MUI_LABEL(5, 10, "Configuration Mode")
                MUI_XY("HR", 0, 13)
                    MUI_STYLE(0)

                        MUI_DATA("GP",
                                 MUI_10 "Pump Dry Run Protection|" MUI_11 "Tank Full Protection|" MUI_12 "Dashboard Selection|" MUI_13 "Scheduler Settings|" MUI_14 "Level Strip Settings|" MUI_15 "Reset controller|")
                            MUI_XYA("GC", 5, 25, 0)
                                MUI_XYA("GC", 5, 37, 1)
                                    MUI_XYA("GC", 5, 49, 2)
                                        MUI_XYA("GC", 5, 61, 3)

    /* Pump Dry Run Protection */
    MUI_FORM(10)
        MUI_STYLE(1)
            MUI_LABEL(5, 10, "Pump Dry Run Settings")
                MUI_XY("HR", 0, 13)
                    MUI_STYLE(0)
                        MUI_LABEL(0, 27, "Protection:")
                            MUI_XYAT("DE", 76, 27, 40, "Enabled|Disabled")
                                MUI_LABEL(0, 41, "Cutoff Power:")
                                    MUI_XY("DA", 76, 41)
                                        MUI_XY("DB", 85, 41)
                                            MUI_XY("DC", 94, 41)
                                                MUI_XY("DD", 103, 41)
                                                    MUI_LABEL(112, 41, "W")
                                                        MUI_LABEL(0, 55, "Cutoff Delay:")
                                                            MUI_XY("DS", 76, 55)
                                                                MUI_LABEL(102, 55, "Sec")

    /* Tank Protection selection */
    MUI_FORM(11)
        MUI_STYLE(1)
            MUI_LABEL(5, 10, "Tank full Protection")
                MUI_XY("HR", 0, 13)
                    MUI_STYLE(0)
                        MUI_LABEL(0, 27, "Protection:")
                            MUI_XYAT("TP", 76, 27, 40, "Enabled|Disabled")
                                MUI_LABEL(0, 41, "Cutoff Level:")
                                    MUI_XY("TL", 76, 41)
                                        MUI_LABEL(102, 41, "watts")
                                            MUI_LABEL(0, 55, "Cutoff Delay:")
                                                MUI_XY("TD", 76, 55)
                                                    MUI_LABEL(102, 55, "Sec")

    /* Dashboard  */
    MUI_FORM(12)
        MUI_STYLE(1)
            MUI_LABEL(5, 10, "Dashboard Selection")
                MUI_XY("HR", 0, 13)
                    MUI_STYLE(0)

                        MUI_XYAT("PS", 10, 28, 0, "Dash-1")
                            MUI_XYAT("PS", 10, 40, 1, "Dash-2")

                                MUI_XYAT("PS", 65, 28, 2, "Dash-3")
                                    MUI_XYAT("PS", 65, 40, 3, "Dash-4")

    /* Scheduler*/
    MUI_FORM(13)
        MUI_STYLE(1)
            MUI_LABEL(5, 10, "Scheduler Settings")
                MUI_XY("HR", 0, 13)
                    MUI_STYLE(0)

    /* LED STRIP SETINGS */
    MUI_FORM(14)
        MUI_STYLE(1)
            MUI_LABEL(5, 10, "Level Strip Settings")
                MUI_XY("HR", 0, 13)
                    MUI_STYLE(0)
                        MUI_XYAT("LS", 10, 28, 0, "Type-1")
                            MUI_XYAT("LS", 10, 40, 1, "Type-2")
                                MUI_XYAT("LS", 65, 28, 2, "Type-3")
                                    MUI_XYAT("LS", 65, 40, 3, "Type-4")

    /* Reset Setings */
    MUI_FORM(15)
        MUI_STYLE(1)
            MUI_LABEL(5, 10, "Reset Controller")
                MUI_XY("HR", 0, 13)
                    MUI_STYLE(0)
                        MUI_LABEL(5, 30, "Press SELECT to reset");
;
