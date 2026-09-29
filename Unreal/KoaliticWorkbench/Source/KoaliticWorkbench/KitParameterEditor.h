#pragma once
#include "KitParameterGroups.h"
#include "WorkbenchActions.h"
#include "Templates/Function.h"

namespace KW {
// One clipped, scrollable control surface serves shared resources and local designs.
inline void DrawKitParameterEditor(Canvas& C,float Top,float Bottom,float Scroll,const Presentation& Values,int Category,bool GlobalScope,const bool* Overrides,int Edit,const FString& Buffer,
 TFunctionRef<void(int,float,float,float,float,const FString&,bool)> Button,
 TFunctionRef<void(int,float,float,float,float)> Region){
 C.Clip(1446,Top,426,Bottom-Top);
 FLinearColor AccentColor=CategoryColor(Category);
 float Position=Top+8-Scroll;
 for(const auto& Group:ParameterGroups(Category)){
  float HeaderY=Position;
  if(HeaderY+38>=Top&&HeaderY<=Bottom){
   C.Cut(1454,HeaderY,412,37,FLinearColor(.009f,.032f,.049f),A(AccentColor,.35f),4);
   C.Gradient(1458,HeaderY+3,404,16,A(AccentColor,.10f),A(Ink,0));
   C.Box(1454,HeaderY+5,3,27,AccentColor);C.Text(1468,HeaderY+10,Group.Title,10,AccentColor,true);
   if(Category==4&&Group.Items.Num()>=3&&FCString::Strstr(Group.Title,TEXT("COLOR"))){int Hue=Group.Items[0],SatIndex=Group.Items[1],Bright=Group.Items[2];FLinearColor Swatch=FLinearColor::MakeFromHSV8((uint8)FMath::Clamp(GetParam(Values,Hue)*255.f/360.f,0.f,255.f),(uint8)FMath::Clamp(GetParam(Values,SatIndex)*255.f,0.f,255.f),(uint8)FMath::Clamp(GetParam(Values,Bright)*127.f,0.f,255.f));C.Cut(1825,HeaderY+7,32,23,Swatch,A(White,.6f),3);C.Halo(1841,HeaderY+18,19,12,Swatch,.18f);}
   if(Group.Toggle>=0&&HeaderY>=Top&&HeaderY+37<=Bottom)
    Button(ActionId::ParameterToggleBase+Group.Toggle,1776,HeaderY+5,90,27,GetParam(Values,Group.Toggle)>0?TEXT("On"):TEXT("Off"),GetParam(Values,Group.Toggle)>0);
  }
  Position+=44;
  for(int I:Group.Items){float Y=Position;Position+=67;if(Y+61<Top||Y>Bottom)continue;
   bool Interactive=Y>=Top&&Y+61<=Bottom;C.Text(1468,Y+2,Params[I].Label,11,White);
   FString Value=Edit==ActionId::ParameterValueBase+I?Buffer+TEXT("|"):ParamValue(Values,I);
   if(Params[I].Choice){if(Interactive)Button(ActionId::ParameterChoiceBase+I,1468,Y+24,294,31,Value,false);}
   else{
    C.Cut(1768,Y-3,98,23,FLinearColor(.007f,.024f,.037f),A(AccentColor,.28f),3);
    C.Text(1776,Y+2,Value,10,AccentColor,true);
    if(Interactive)Region(ActionId::ParameterValueBase+I,1770,Y,95,20);
    float T=Sat((GetParam(Values,I)-Params[I].Min)/(Params[I].Max-Params[I].Min));
    C.Cut(1468,Y+30,294,10,FLinearColor(.003f,.013f,.024f),A(AccentColor,.23f),3);
    C.Gradient(1470,Y+32,290*T,6,A(AccentColor,.52f),AccentColor,true);
    C.Halo(1468+290*T,Y+35,13,11,AccentColor,.31f);C.Cut(1466+290*T,Y+27,8,16,White,AccentColor,2);
    if(Interactive)Region(ActionId::ParameterSliderBase+I,1468,Y+21,294,29);
    C.Text(1468,Y+48,FString::Printf(TEXT("%.2f to %.2f  /  click value to type"),Params[I].Min,Params[I].Max),8,Muted);
   }
   if(!GlobalScope&&Interactive)Button(ActionId::ParameterResetBase+I,1773,Y+26,93,27,Overrides[I]?TEXT("Use preset"):TEXT("Linked"),Overrides[I]);
  }
  Position+=14;
 }
 C.Unclip();
 float Total=ParameterContentHeight(Category),View=Bottom-Top;
 if(Total>View){C.Box(1875,Top,2,View,Line);float Thumb=FMath::Max(30.f,View*View/Total);float MaxScroll=FMath::Max(1.f,Total-View);C.Box(1873,Top+(View-Thumb)*FMath::Clamp(Scroll/MaxScroll,0.f,1.f),6,Thumb,A(AccentColor,.85f));}
}
}
