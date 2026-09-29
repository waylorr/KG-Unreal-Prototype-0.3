#pragma once
#include "KitPresetStore.h"

namespace KW {
// Component-local values inherit each unoverridden parameter from the selected UI Kit resources.
struct KitBindings {
 static void Resolve(Presentation& Local,const Presentation& Shared,const bool (&Overrides)[ParamCount]){for(int I=0;I<ParamCount;I++)if(!Overrides[I])SetParam(Local,I,GetParam(Shared,I));}
 static void SetLocal(Presentation& Local,bool (&Overrides)[ParamCount],int Parameter,float Value){SetParam(Local,Parameter,Value);Overrides[Parameter]=true;}
 static void Write(FConfigFile& C,const Presentation& Local,const bool (&Overrides)[ParamCount],const FString (&Links)[7],const FString& Prefix=TEXT("")){FString LinkSec=Prefix+TEXT("KitLinks"),OverrideSec=Prefix+TEXT("Overrides"),ValueSec=Prefix+TEXT("LocalValues");for(int Cat=0;Cat<7;Cat++)C.SetString(*LinkSec,*FString::FromInt(Cat),*Links[Cat]);for(int I=0;I<ParamCount;I++){C.SetBool(*OverrideSec,Params[I].Key,Overrides[I]);C.SetFloat(*ValueSec,Params[I].Key,GetParam(Local,I));}}
 static void Read(const FConfigFile& C,const KitPresetStore& Store,Presentation& Local,Presentation& Shared,bool (&Overrides)[ParamCount],FString (&Links)[7],const FString& Prefix=TEXT("")){
  FString LinkSec=Prefix+TEXT("KitLinks"),OverrideSec=Prefix+TEXT("Overrides"),ValueSec=Prefix+TEXT("LocalValues");
  static const TCHAR* Defaults[]={TEXT("Frame construct"),TEXT("Shutter retract"),TEXT("Obsidian drift"),TEXT("Energy response"),TEXT("Signal red"),TEXT("Angular energy"),TEXT("Impact signal")};
  Shared=Presentation();for(int Cat=0;Cat<7;Cat++){Links[Cat]=Defaults[Cat];C.GetString(*LinkSec,*FString::FromInt(Cat),Links[Cat]);Store.Load(Cat,Links[Cat],Shared);}
  for(int I=0;I<ParamCount;I++){Overrides[I]=false;C.GetBool(*OverrideSec,Params[I].Key,Overrides[I]);float V=GetParam(Local,I);C.GetFloat(*ValueSec,Params[I].Key,V);SetParam(Local,I,V);}Resolve(Local,Shared,Overrides);
 }
};
}
