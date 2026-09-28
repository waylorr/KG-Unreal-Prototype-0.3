#pragma once
#include "KitPresetStore.h"

namespace KW {
// Component-local values inherit each unoverridden parameter from the selected UI Kit resources.
struct KitBindings {
 static void Resolve(Presentation& Local,const Presentation& Shared,const bool (&Overrides)[ParamCount]){for(int I=0;I<ParamCount;I++)if(!Overrides[I])SetParam(Local,I,GetParam(Shared,I));}
 static void SetLocal(Presentation& Local,bool (&Overrides)[ParamCount],int Parameter,float Value){SetParam(Local,Parameter,Value);Overrides[Parameter]=true;}
 static void Write(FConfigFile& C,const Presentation& Local,const bool (&Overrides)[ParamCount],const FString (&Links)[6]){for(int Cat=0;Cat<6;Cat++)C.SetString(TEXT("KitLinks"),*FString::FromInt(Cat),*Links[Cat]);for(int I=0;I<ParamCount;I++){C.SetBool(TEXT("Overrides"),Params[I].Key,Overrides[I]);C.SetFloat(TEXT("LocalValues"),Params[I].Key,GetParam(Local,I));}}
 static void Read(const FConfigFile& C,const KitPresetStore& Store,Presentation& Local,Presentation& Shared,bool (&Overrides)[ParamCount],FString (&Links)[6]){
  static const TCHAR* Defaults[]={TEXT("Frame construct"),TEXT("Shutter retract"),TEXT("Obsidian drift"),TEXT("Energy response"),TEXT("Signal red"),TEXT("Angular energy")};
  Shared=Presentation();for(int Cat=0;Cat<6;Cat++){Links[Cat]=Defaults[Cat];C.GetString(TEXT("KitLinks"),*FString::FromInt(Cat),Links[Cat]);Store.Load(Cat,Links[Cat],Shared);}
  for(int I=0;I<ParamCount;I++){Overrides[I]=false;C.GetBool(TEXT("Overrides"),Params[I].Key,Overrides[I]);float V=GetParam(Local,I);C.GetFloat(TEXT("LocalValues"),Params[I].Key,V);SetParam(Local,I,V);}Resolve(Local,Shared,Overrides);
 }
};
}
