#pragma once
#include "KitBindings.h"

namespace KW {
// Variants share ProfileData. Each stores a visual composition and preset bindings.
struct ProfileVariant {
 FString Name=TEXT("Original");
 Presentation Local;
 bool Overrides[ParamCount]={};
 FString Links[7];
};

struct ProfileVariantStore {
 static FString Prefix(int Index){return FString::Printf(TEXT("Variant%d/"),Index);}
 static void Write(FConfigFile& C,const TArray<ProfileVariant>& Variants,int Active){
  C.SetString(TEXT("Variants"),TEXT("Count"),*FString::FromInt(Variants.Num()));C.SetString(TEXT("Variants"),TEXT("Active"),*FString::FromInt(Active));
  for(int Index=0;Index<Variants.Num();Index++){
   const auto& V=Variants[Index];FString Base=Prefix(Index),Visual=Base+TEXT("Visual");
   C.SetString(*Visual,TEXT("Name"),*V.Name);C.SetString(*Visual,TEXT("Portrait"),*FString::FromInt(V.Local.Portrait));
   for(int Layer=0;Layer<8;Layer++)C.SetBool(*Visual,*FString::FromInt(Layer),V.Local.Layers[Layer]);
   KitBindings::Write(C,V.Local,V.Overrides,V.Links,Base);
  }
 }
 static bool Read(const FConfigFile& C,const KitPresetStore& Store,TArray<ProfileVariant>& Variants,int& Active){
  int Count=0;if(!C.GetInt(TEXT("Variants"),TEXT("Count"),Count)||Count<1)return false;
  Count=FMath::Clamp(Count,1,12);Variants.Reset();
  for(int Index=0;Index<Count;Index++){
   ProfileVariant V;FString Base=Prefix(Index),Visual=Base+TEXT("Visual");Presentation Shared;
   C.GetString(*Visual,TEXT("Name"),V.Name);C.GetInt(*Visual,TEXT("Portrait"),V.Local.Portrait);
   V.Local.Portrait=FMath::Clamp(V.Local.Portrait,0,1);
   for(int Layer=0;Layer<8;Layer++)C.GetBool(*Visual,*FString::FromInt(Layer),V.Local.Layers[Layer]);
   KitBindings::Read(C,Store,V.Local,Shared,V.Overrides,V.Links,Base);Variants.Add(V);
  }
  C.GetInt(TEXT("Variants"),TEXT("Active"),Active);Active=FMath::Clamp(Active,0,Variants.Num()-1);return true;
 }
};
}
