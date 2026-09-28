#pragma once
#include "KitParameters.h"
#include "Templates/Function.h"

namespace KW {
// Both the component inspector and UI Kit use exactly this control renderer.
inline void DrawKitParameterEditor(Canvas& C,float Top,const Presentation& Values,int Category,int ControlPage,bool GlobalScope,const bool* Overrides,int Edit,const FString& Buffer,
 TFunctionRef<void(int,float,float,float,float,const FString&,bool)> Button,
 TFunctionRef<void(int,float,float,float,float)> Region){
 int Count=0;for(int I=0;I<ParamCount;I++)if(Params[I].Category==Category)Count++;
 int Page=FMath::Min(ControlPage,FMath::Max(0,(Count-1)/6));int Row=0;
 for(int I=0;I<ParamCount;I++)if(Params[I].Category==Category){int Index=Row++;if(Index/6!=Page)continue;float Y=Top+(Index%6)*63;
 C.Text(1454,Y,Params[I].Label,11,White);
 if(I==9||I==10||I==11||I==17||I==20||I==21)Button(1100+I,1679,Y-4,53,21,GetParam(Values,I)>0?TEXT("On"):TEXT("Off"),GetParam(Values,I)>0);
 if(Params[I].Choice)Button(800+I,1454,Y+19,337,31,ParamValue(Values,I),false);
 else{FString Value=Edit==900+I?Buffer+TEXT("|"):ParamValue(Values,I);C.Text(1745,Y,Value,9,Cyan,true);Region(900+I,1738,Y-3,120,22);
 float T=Sat((GetParam(Values,I)-Params[I].Min)/(Params[I].Max-Params[I].Min));C.Box(1454,Y+30,337,3,Line);C.Box(1454,Y+30,337*T,3,Cyan);C.Cut(1450+337*T,Y+25,8,13,White,Cyan,2);Region(400+I,1454,Y+18,337,31);
 C.Text(1454,Y+39,FString::Printf(TEXT("%.2f to %.2f / click value to type"),Params[I].Min,Params[I].Max),7,Muted);}
 if(!GlobalScope)Button(500+I,1801,Y+21,65,25,Overrides[I]?TEXT("Reset"):TEXT("Linked"),Overrides[I]);
 }
 if(Count>6){Button(76,1454,Top+382,130,30,TEXT("Previous"),false);Button(77,1594,Top+382,130,30,TEXT("More controls"),false);C.Text(1740,Top+390,FString::Printf(TEXT("%d / %d"),Page+1,(Count+5)/6),10,Cyan);}
}
}
