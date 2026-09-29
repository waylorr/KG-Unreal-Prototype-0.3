#pragma once
#include "CoreMinimal.h"

namespace KW {
// Workbench navigation state only. Data drafts, component values and preview time live elsewhere.
struct WorkbenchNavigation {
 bool KitScope=false;
 // One Workbench sidebar; KitScope remains the internal resource-editing boundary.
 int Section=0; // Library, legacy slots, Themes, Motion, Ambient, Reaction, Events.
 bool DrawerOpen=true;
 bool DesignOpen=true;
 int SelectedLayer=-1;
 int Tab=0,Category=0,ControlPage=0,PresetPage=0;
 bool LayersInspector=false;
 float KitScroll[7]={},ComponentScroll[7]={};
 int KitCategory=0,ComponentCategory=0;
 void OpenComponents(){if(KitScope)KitCategory=Category;KitScope=false;Category=ComponentCategory;ControlPage=0;}
 void OpenKit(){if(!KitScope)ComponentCategory=Category;KitScope=true;Category=FMath::Clamp(KitCategory,0,6);if(Category==5)Category=4;ControlPage=0;}
 void SelectInspector(int Next){Tab=FMath::Clamp(Next,0,2);ControlPage=0;}
 void SelectCategory(int Next){Category=FMath::Clamp(Next,0,6);if(KitScope)KitCategory=Category;else ComponentCategory=Category;LayersInspector=false;ControlPage=0;}
 void OpenProgressPart(){Category=5;ComponentCategory=5;Tab=2;LayersInspector=false;ControlPage=0;}
 void SelectSection(int Next){Section=FMath::Clamp(Next,0,7);}
};
}
