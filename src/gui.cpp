
//~ Callbacks
// NOTE(Tyler): strcmp returns 0 if the two strings are the same
#define AUTONOMOUS_SELECTOR_TEST_MODE(Mode, Index) \
if(strcmp(Text, lv_btnm_get_map(Matrix)[Index]) == 0){ \
AutonomousSelector.Selected = Mode; \
lv_btnm_set_toggle(Matrix, true, Index); \
return LV_RES_OK; \
} 

lv_res_t AutonomousSelectorCallback(lv_obj_t *Matrix, const char *Text){
 const char **Buttons = lv_btnm_get_map(Matrix);
 u16 ActiveTab = lv_tabview_get_tab_act(AutonomousSelector.Tabview);
 
 if(ActiveTab == AutonomousSelectorTab_Match){
  lv_obj_t *Matrix = AutonomousSelector.MatchMatrix;
  AUTONOMOUS_SELECTOR_TEST_MODE(Autonomous_None,   0);
  AUTONOMOUS_SELECTOR_TEST_MODE(MatchAutonomous_A, 1);
  AUTONOMOUS_SELECTOR_TEST_MODE(MatchAutonomous_B, 2);
  AUTONOMOUS_SELECTOR_TEST_MODE(MatchAutonomous_C, 3);
  AUTONOMOUS_SELECTOR_TEST_MODE(MatchAutonomous_D, 4);
  AUTONOMOUS_SELECTOR_TEST_MODE(MatchAutonomous_E, 5);
 }else if(ActiveTab == AutonomousSelectorTab_Skills){
  lv_obj_t *Matrix = AutonomousSelector.SkillsMatrix;
  AUTONOMOUS_SELECTOR_TEST_MODE(SkillsAutonomous_A, 0);
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
 
 return Result;
 
}

void InitStyle(lv_style_t *Style, lv_color_t MainColor){
 lv_style_copy(Style, &lv_style_plain);
 Style->body.main_color = MainColor;
}

//~ 
void InitGUI(){
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
 
 // @default_auto
 lv_tabview_set_tab_act(AutonomousSelector.Tabview, AutonomousSelectorTab_Match, false);
 AutonomousSelector.Selected = MatchAutonomous_A;
 lv_btnm_set_toggle(AutonomousSelector.MatchMatrix, true, 1);
 
 
}