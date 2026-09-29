#pragma once

namespace KW::ActionId {
// Stable hit-test IDs. Keep input mapping here; presentation code uses these names.
inline constexpr int SaveComponent=1,ReloadComponent=2,Close=3;
inline constexpr int GoHome=4,GoWorkbench=5,GoEpisodes=6,GoEditor=7;
inline constexpr int InspectorContent=10,InspectorMotion=11,InspectorStyle=12;
inline constexpr int PausePreview=20,StopPreview=21,ReplayEnter=22,PreviewExit=23,ResetTest=24;
inline constexpr int Expanded=30,Compact=31,Pin=32,Disabled=33,AutoCollapse=34;
inline constexpr int Reward50=40,Reward100=41,NearThreshold=42,ClearRewards=43,Portrait=50;
inline constexpr int OpenComponents=60,OpenKit=61,Pulse=62,Reward150=63,SavePreset=64,ReloadPreset=65,ResetCategory=66,PreviousPresetPage=67,NextPresetPage=68,ApplyPreset=69;
inline constexpr int CategoryBase=70,PreviousControls=76,NextControls=77,Undo=78,Redo=79,SaveAs=80,BarSolo=81,ToggleSound=82;
inline constexpr int TogglePresetPicker=83,DismissPresetPicker=84,PreviousPickerPage=85,NextPickerPage=86,CreateVariant=87,NextVariant=88,PreviousVariant=89;
inline constexpr int SelectProfile=90,ShowLayers=91,ResetPartDefaults=92,OpenProgressPart=93,ComponentSearch=106;
inline constexpr int SectionBase=130,DeletePreset=137,ToggleDrawer=138,EventSection=139,SelectLayerBase=220,OpenDesign=144,BackToProfile=145,PreviewLevelUp=146;
inline constexpr int ProfileName=100,Level=101,CurrentXP=102,NextLevelXP=103,PresetName=104,VariantName=105;
inline constexpr int LinkCategoryBase=120;
inline constexpr int LayerBase=200,SeekPreview=300,ParameterSliderBase=400,ParameterResetBase=500,PresetSlotBase=600,ParameterChoiceBase=800,ParameterValueBase=900,ParameterToggleBase=1100;
}
