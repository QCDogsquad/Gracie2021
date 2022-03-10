#ifndef AUTONOMOUS_H
#define AUTONOMOUS_H

//~ Auto selector

const char *MatchButtonsMap[] = {
 "None",
 "Auto A",
 "Auto B",
 "Auto C",
 "Auto D",
 "Auto E",
 "",
};

const char *SkillsButtonsMap[] = {
 "Auto A",
 "",
};

enum auto_mode {
 Auto_None    = 0,
 
 MatchAuto_A  = 1,
 MatchAuto_B  = 2,
 MatchAuto_C  = 3,
 MatchAuto_D  = 4,
 MatchAuto_E  = 5,
 
 SkillsAuto_A = 6,
 
 // NOTE(Tyler): Always keep at the end
 Auto_TOTAL
};

enum auto_selector_tab {
 AutoSelectorTab_None   = 0,
 AutoSelectorTab_Match  = 1,
 AutoSelectorTab_Skills = 2,
};

struct auto_selector {
 auto_mode Selected;
 
 lv_obj_t *Tabview;
 
 lv_obj_t *MatchPage;
 lv_obj_t *MatchMatrix;
 
 lv_obj_t *SkillsPage;
 lv_obj_t *SkillsMatrix;
};

#endif //AUTONOMOUS_H
