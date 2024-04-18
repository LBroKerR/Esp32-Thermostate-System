// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.0
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"
///////////////////// VARIABLES ////////////////////

// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_mainPanel;
void ui_event_WtmpARC(lv_event_t * e);
lv_obj_t * ui_WtmpARC;
void ui_event_WtmpLabel(lv_event_t * e);
lv_obj_t * ui_WtmpLabel;
void ui_event_TmpLabel(lv_event_t * e);
lv_obj_t * ui_TmpLabel;
void ui_event_HmdLabel(lv_event_t * e);
lv_obj_t * ui_HmdLabel;
void ui_event_HeatButton(lv_event_t * e);
lv_obj_t * ui_HeatButton;
lv_obj_t * ui_Label10;
void ui_event_ProgButton(lv_event_t * e);
lv_obj_t * ui_ProgButton;
lv_obj_t * ui_ProgLabel;
void ui_event_wifiLabel(lv_event_t * e);
lv_obj_t * ui_wifiLabel;
void ui_event_serverlabel(lv_event_t * e);
lv_obj_t * ui_serverlabel;
void ui_event_TimeLabel(lv_event_t * e);
lv_obj_t * ui_TimeLabel;
lv_obj_t * ui_Label1;


// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
lv_obj_t * ui_Screen2;
lv_obj_t * ui_ProgPanel;
void ui_event_wtmpSLider(lv_event_t * e);
lv_obj_t * ui_wtmpSLider;
lv_obj_t * ui_ProgTmpLabel;
void ui_event_Button3(lv_event_t * e);
lv_obj_t * ui_Button3;
lv_obj_t * ui_Label15;
void ui_event_Roller1(lv_event_t * e);
lv_obj_t * ui_Roller1;
void ui_event_Roller2(lv_event_t * e);
lv_obj_t * ui_Roller2;
lv_obj_t * ui_TmpChart;
lv_obj_t * ui_WTMPSliderLable;


// SCREEN: ui_Screen3
void ui_Screen3_screen_init(void);
lv_obj_t * ui_Screen3;
lv_obj_t * ui_HeatPanel;
void ui_event_Back2Button(lv_event_t * e);
lv_obj_t * ui_Back2Button;
lv_obj_t * ui_Back2Label;
void ui_event_HeatSetting(lv_event_t * e);
lv_obj_t * ui_HeatSetting;
lv_obj_t * ui_HeatScreenLabel;
void ui_event_offsetSlider(lv_event_t * e);
lv_obj_t * ui_offsetSlider;
lv_obj_t * ui_offsetlabelText;
lv_obj_t * ui_offsetNumberLabel;
lv_obj_t * ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

void ui_event_WtmpARC(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        set_WTMP(e);
        //lv_label_set_text(ui_wifiLabel, "Conneted!");
        get_Wtmp(e);
    }
}
void ui_event_WtmpLabel(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        _ui_arc_increment(ui_WtmpARC, 1);
    }
}
void ui_event_TmpLabel(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        Set_Temp(e);
    }
}
void ui_event_HmdLabel(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        setting_Hmd(e);
    }
}
void ui_event_HeatButton(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_MOVE_LEFT, 10, 0, &ui_Screen3_screen_init);
    }
}
void ui_event_ProgButton(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
      lv_roller_set_selected(ui_Roller2, (uint16_t) 12, LV_ANIM_OFF);
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 10, 0, &ui_Screen2_screen_init);
    }
}
void ui_event_wifiLabel(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        WifiConneted(e);
    }
}
void ui_event_serverlabel(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        ClientConneted(e);
    }
}
void ui_event_TimeLabel(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        setClock(e);
    }
}
void ui_event_wtmpSLider(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        SetWantedTMPSCR2(e);
    }
}
void ui_event_Button3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
      lv_roller_set_selected(ui_Roller2, (uint16_t) 0, LV_ANIM_OFF);
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_MOVE_LEFT, 10, 0, &ui_Screen1_screen_init);
    }
}
void ui_event_Roller1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        setProgNum(e);
    }
}
void ui_event_Roller2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        setProgHour(e);
    }
}
void ui_event_Back2Button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 10, 0, &ui_Screen1_screen_init);
    }
}
void ui_event_HeatSetting(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        ChangeHeatSetting(e);
    }
}
void ui_event_offsetSlider(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        setoffsetNumber(e);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui_Screen2_screen_init();
    ui_Screen3_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen1);
}