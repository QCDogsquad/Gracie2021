
//~ Callbacks
// NOTE(Tyler): strcmp returns 0 if the two strings are the same
#define AUTO_SELECTOR_TEST_MODE(Mode, Index) \
if(strcmp(Text, lv_btnm_get_map(Matrix)[Index]) == 0){ \
AutoSelector.Selected = Mode; \
lv_btnm_set_toggle(Matrix, true, Index); \
return LV_RES_OK; \
} 

lv_res_t AutoSelectorCallback(lv_obj_t *Matrix, const char *Text){
 const char **Buttons = lv_btnm_get_map(Matrix);
 u16 ActiveTab = lv_tabview_get_tab_act(AutoSelector.Tabview);
 
 if(ActiveTab == AutoSelectorTab_Match){
  lv_obj_t *Matrix = AutoSelector.MatchMatrix;
  AUTO_SELECTOR_TEST_MODE(Auto_None,   0);
  AUTO_SELECTOR_TEST_MODE(MatchAuto_A, 1);
  AUTO_SELECTOR_TEST_MODE(MatchAuto_B, 2);
  AUTO_SELECTOR_TEST_MODE(MatchAuto_C, 3);
  AUTO_SELECTOR_TEST_MODE(MatchAuto_D, 4);
  AUTO_SELECTOR_TEST_MODE(MatchAuto_E, 5);
 }else if(ActiveTab == AutoSelectorTab_Skills){
  lv_obj_t *Matrix = AutoSelector.SkillsMatrix;
  AUTO_SELECTOR_TEST_MODE(SkillsAuto_A, 0);
 }else{
  AutoSelector.Selected = Auto_None;
 }
 
 return LV_RES_OK;
}

#undef AUTO_SELECTOR_TEST_MODE

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
 AutoSelector.Tabview = lv_tabview_create(lv_scr_act(), 0);
 lv_tabview_add_tab(AutoSelector.Tabview, "None");
 
 AutoSelector.MatchPage   = lv_tabview_add_tab(AutoSelector.Tabview, "Match");
 AutoSelector.MatchMatrix = MakeButtonMatrix(AutoSelector.MatchPage, 
                                             MatchButtonsMap, 
                                             AutoSelectorCallback);
 AutoSelector.SkillsPage   = lv_tabview_add_tab(AutoSelector.Tabview, "Skills");
 AutoSelector.SkillsMatrix = MakeButtonMatrix(AutoSelector.SkillsPage, 
                                              SkillsButtonsMap, 
                                              AutoSelectorCallback);
 
 // @default_auto
 lv_tabview_set_tab_act(AutoSelector.Tabview, AutoSelectorTab_Match, false);
 AutoSelector.Selected = MatchAuto_A;
 lv_btnm_set_toggle(AutoSelector.MatchMatrix, true, 1);
 
 
}