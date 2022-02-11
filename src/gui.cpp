
//~ Callbacks
// NOTE(Tyler): strcmp returns 0 if the two strings are the same
#define AUTONOMOUS_SELECTOR_TEST_MODE(Name, Mode, Index) \
if(strcmp(Text, Name) == 0){ \
AutonomousSelector.Selected = Mode; \
return LV_RES_OK; \
} 

lv_res_t AutonomousSelectorCallback(lv_obj_t *Matrix, const char *Text){
 const char **Buttons = lv_btnm_get_map(Matrix);
 u16 ActiveTab = lv_tabview_get_tab_act(AutonomousSelector.Tabview);
 
 if(ActiveTab == AutonomousSelectorTab_Match){
  AUTONOMOUS_SELECTOR_TEST_MODE("None",   Autonomous_None,   0);
  AUTONOMOUS_SELECTOR_TEST_MODE("Auto A", MatchAutonomous_A, 1);
  AUTONOMOUS_SELECTOR_TEST_MODE("Auto B", MatchAutonomous_B, 2);
  AUTONOMOUS_SELECTOR_TEST_MODE("Auto C", MatchAutonomous_C, 3);
 }else if(ActiveTab == AutonomousSelectorTab_Skills){
  AUTONOMOUS_SELECTOR_TEST_MODE("Auto A", SkillsAutonomous_A, 0);
 }else{
  AutonomousSelector.Selected = Autonomous_None;
 }
 
 return LV_RES_OK;
}

#undef AUTONOMOUS_SELECTOR_TEST_MODE

//~ Helpers
#if 0
lv_obj_t *MakeButton(lv_obj_t *Parent, u32 ID, const char *Text, lv_action_t Action, lv_point_t P, lv_point_t Size){
 lv_obj_t *Result = lv_btn_create(Parent, 0);
 lv_obj_set_free_num(Result, ID);
 lv_btn_set_action(Result, LV_BTN_ACTION_CLICK, Action); 
 lv_obj_set_size(Result, Size.x, Size.y);
 lv_obj_align(Result, NULL, LV_ALIGN_IN_TOP_LEFT, P.x, P.y);
 
 lv_obj_t *Label = lv_label_create(Result, 0);
 lv_label_set_text(Label, Text);
 
 return Result;
}
#endif

lv_obj_t *MakeButtonMatrix(lv_obj_t *Parent, const char **Map, lv_btnm_action_t Action){
 lv_coord_t Width = lv_obj_get_width(Parent);
 lv_obj_t *Result = lv_btnm_create(Parent, 0);
 lv_btnm_set_map(Result, Map);
 lv_obj_set_width(Result, Width);
 lv_btnm_set_action(Result, Action);
 
 //lv_btnm_set_style(AutonomousButtons, LV_BTNM_STYLE_BTN_REL, &ReleasedButtonStyle);
 //lv_btnm_set_style(AutonomousButtons, LV_BTNM_STYLE_BTN_PR,  &PressedButtonStyle);
 
 return Result;
 
}

//~ 
void InitGUI(){
#if 0
 lv_style_copy(&ReleasedButtonStyle, &lv_style_plain);
 ReleasedButtonStyle.body.main_color = LV_COLOR_MAKE(150, 0, 0);
 ReleasedButtonStyle.body.grad_color = LV_COLOR_MAKE(0, 0, 150);
 ReleasedButtonStyle.body.radius = 0;
 ReleasedButtonStyle.text.color = LV_COLOR_MAKE(255, 255, 255);
#endif
 
 AutonomousSelector.Tabview = lv_tabview_create(lv_scr_act(), 0);
 lv_tabview_add_tab(AutonomousSelector.Tabview, "None");
 
 AutonomousSelector.MatchPage   = lv_tabview_add_tab(AutonomousSelector.Tabview, "Match");
 AutonomousSelector.MatchMatrix = MakeButtonMatrix(AutonomousSelector.MatchPage, 
                                                   MatchButtonsMap, 
                                                   AutonomousSelectorCallback);
 
 AutonomousSelector.SkillsPage   = lv_tabview_add_tab(AutonomousSelector.Tabview, "Skills");
 AutonomousSelector.SkillsMatrix = MakeButtonMatrix(AutonomousSelector.SkillsPage, 
                                                    SkillsButtonsMap, 
                                                    AutonomousSelectorCallback);
 
 lv_tabview_set_tab_act(AutonomousSelector.Tabview, AutonomousSelectorTab_Match, false);
}