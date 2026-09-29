#pragma once
#include "KitParameters.h"

namespace KW {
// Inspector grouping is presentation metadata. Parameter IDs and persistence stay unchanged.
struct FKitParameterGroup {const TCHAR* Title;TArray<int> Items;int Toggle=-1;};
inline const TArray<FKitParameterGroup>& ParameterGroups(int Category){
 static const TArray<FKitParameterGroup> Entrance={{TEXT("REVEAL CHOREOGRAPHY"),{0,3}},{TEXT("TIMING & EASING"),{1,33}},{TEXT("LAYER ASSEMBLY"),{2}},{TEXT("DISPLACEMENT"),{36},36},{TEXT("SIGNAL EDGE"),{38},38},{TEXT("ENERGY & GLITCH"),{EffectExtraBase+EntryBloom,EffectExtraBase+EntryParticles,EffectExtraBase+EntryGlitch}}};
 static const TArray<FKitParameterGroup> Exit={{TEXT("RETRACTION CHOREOGRAPHY"),{4}},{TEXT("TIMING & EASING"),{5,34}},{TEXT("FRAGMENTATION"),{37},37},{TEXT("SIGNAL EDGE"),{39},39},{TEXT("ENERGY & GLITCH"),{EffectExtraBase+ExitBloom,EffectExtraBase+ExitParticles,EffectExtraBase+ExitGlitch}}};
 static const TArray<FKitParameterGroup> Ambient={
  {TEXT("GLOBAL LOOP"),{7,8,12},6},
  {TEXT("GLASS REFLECTION"),{9,EffectExtraBase+GlassSpeed},9},
  {TEXT("PARTICLES"),{10,31,32,EffectExtraBase+ParticleSpeed},10},
  {TEXT("TEXT INTERFERENCE"),{11,EffectExtraBase+TextSpeed},11},
  {TEXT("WHOLE-PROFILE GLITCH"),{17,18,22,23,24},17},
  {TEXT("SCAN & PERIMETER"),{20,21,EffectExtraBase+PerimeterSpeed}},{TEXT("DEPTH"),{EffectExtraBase+ParallaxDepth}}};
 static const TArray<FKitParameterGroup> Reaction={{TEXT("REACTION TIMING"),{13,14},13},{TEXT("PARTICLE BURST"),{15},15}};
 static const TArray<FKitParameterGroup> Appearance={{TEXT("FRAME & ORNAMENTS"),{25,26}},{TEXT("SIGNAL LIGHTING"),{19,16}},{TEXT("WHOLE COMPONENT"),{EffectExtraBase+ThemeCustom,EffectExtraBase+ThemeOpacity}},{TEXT("FRAME COLOR"),{EffectExtraBase+FrameHue,EffectExtraBase+FrameSaturation,EffectExtraBase+FrameBrightness,EffectExtraBase+FrameOpacity}},{TEXT("TEXT COLOR"),{EffectExtraBase+TextHue,EffectExtraBase+TextSaturation,EffectExtraBase+TextBrightness,EffectExtraBase+TextOpacity}},{TEXT("GLASS COLOR"),{EffectExtraBase+GlassHue,EffectExtraBase+GlassSaturation,EffectExtraBase+GlassBrightness,EffectExtraBase+GlassOpacity}},{TEXT("ACCENT COLOR"),{EffectExtraBase+AccentHue,EffectExtraBase+AccentSaturation,EffectExtraBase+AccentBrightness,EffectExtraBase+AccentOpacity}},{TEXT("XP FILL COLOR"),{EffectExtraBase+ProgressHue,EffectExtraBase+ProgressSaturation,EffectExtraBase+ProgressBrightness,EffectExtraBase+ProgressOpacity}},{TEXT("LEVEL COLOR"),{EffectExtraBase+LevelHue,EffectExtraBase+LevelSaturation,EffectExtraBase+LevelBrightness,EffectExtraBase+LevelOpacity}}};
 static const TArray<FKitParameterGroup> Progress={{TEXT("TRACK GEOMETRY"),{27,28,35}},{TEXT("FILL LIGHT & SWEEP"),{29,30}}};
 static const TArray<FKitParameterGroup> Event={{TEXT("XP GAIN / BAR"),{EffectExtraBase+EventIntensity,EffectExtraBase+EventDuration,EffectExtraBase+BarKick}},{TEXT("XP POPUP COMPOSITION"),{EffectExtraBase+PopupScale,EffectExtraBase+PopupRise,EffectExtraBase+PopupOpacity}},{TEXT("XP PARTICLES"),{EffectExtraBase+EventBurst},EffectExtraBase+EventBurst},{TEXT("LEVEL UP"),{EffectExtraBase+LevelFlash,EffectExtraBase+LevelParticles}}};
 switch(Category){case 0:return Entrance;case 1:return Exit;case 2:return Ambient;case 3:return Reaction;case 4:return Appearance;case 6:return Event;default:return Progress;}
}
inline float ParameterContentHeight(int Category){float H=14;for(const auto& Group:ParameterGroups(Category))H+=44+Group.Items.Num()*67+14;return H;}
}
