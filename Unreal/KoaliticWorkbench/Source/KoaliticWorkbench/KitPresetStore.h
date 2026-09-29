#pragma once
#include "KitParameters.h"
#include "Misc/ConfigCacheIni.h"

namespace KW {
// UI Kit resources live independently of the component presentation file.
struct KitPresetStore {
 FString Path;
 explicit KitPresetStore(const FString& DataDir):Path(DataDir/TEXT("UIKit.ini")){}
 static FString Section(int Category,const FString& Name){return FString::Printf(TEXT("Category%d/%s"),Category,*Name);}
 static FString SafeName(FString Name){Name.TrimStartAndEndInline();Name.ReplaceInline(TEXT("["),TEXT(""));Name.ReplaceInline(TEXT("]"),TEXT(""));Name.ReplaceInline(TEXT("/"),TEXT("-"));Name.ReplaceInline(TEXT("\\"),TEXT("-"));return Name.IsEmpty()?TEXT("Untitled"):Name.Left(24);}
 TArray<FString> Names(int Category)const{FConfigFile C;C.Read(Path);TArray<FString> Result;FString Prefix=FString::Printf(TEXT("Category%d/"),Category);for(const auto& Sec:C)if(Sec.Key.StartsWith(Prefix))Result.Add(Sec.Key.Mid(Prefix.Len()));Result.Sort();return Result;}
 bool Load(int Category,const FString& Name,Presentation& Values)const{FConfigFile C;C.Read(Path);FString Sec=Section(Category,Name);if(!C.Contains(Sec))return false;for(int I=0;I<ParamCount;I++)if(Params[I].Category==Category){float V=GetParam(Values,I);C.GetFloat(*Sec,Params[I].Key,V);SetParam(Values,I,V);}return true;}
 bool Save(int Category,const FString& Name,const Presentation& Values)const{FConfigFile C;C.Read(Path);FString Sec=Section(Category,Name);for(int I=0;I<ParamCount;I++)if(Params[I].Category==Category)C.SetFloat(*Sec,Params[I].Key,GetParam(Values,I));return C.Write(Path);}
 bool Delete(int Category,const FString& Name)const{FConfigFile C;C.Read(Path);if(C.Remove(Section(Category,Name))==0)return false;return C.Write(Path);}
 bool Seed(const FString (&Linked)[7])const{
  FConfigFile C;C.Read(Path);int SeedVersion=0;C.GetInt(TEXT("Meta"),TEXT("SeedVersion"),SeedVersion);if(SeedVersion>=2)return true;
  for(int Cat=0;Cat<7;Cat++){if(Cat==5)continue;
   FString Prefix=FString::Printf(TEXT("Category%d/"),Cat);bool Existing=false;for(const auto& Sec:C)if(Sec.Key.StartsWith(Prefix)){Existing=true;break;}
   int Count=Cat<2?8:2;
   for(int Variant=0;Variant<Count;Variant++){
    if(Existing&&(Cat>=2||Variant<5))continue;
    Presentation V;FString Name=Linked[Cat];
    if(Cat==0){V.Entrance=Variant;Name=ParamValue(V,0);V.Extra[EntryParticles]=Variant>=4?1.3f:.4f;V.Extra[EntryGlitch]=Variant==3||Variant==6?.8f:.15f;}
    if(Cat==1){V.Exit=Variant;Name=ParamValue(V,4);V.Extra[ExitParticles]=Variant>=4?1.1f:.3f;V.Extra[ExitGlitch]=Variant==3||Variant==6?.9f:.2f;}
    if(Cat==2&&Variant){V.AmbientMode=1;V.Scan=1;V.Speed=1.4f;V.Sweep=1.4f;V.Particles=1.6f;V.Glitch=.65f;Name=TEXT("Ion surveillance");}
    if(Cat==3&&Variant){V.Reaction=2;V.Burst=2;Name=TEXT("Overdrive");}
    if(Cat==4&&Variant){V.Design=1;V.Accent=1;V.Glow=1.1f;V.Ornaments=0;Name=TEXT("Precision V2");}
    if(Cat==6&&Variant){V.Extra[EventIntensity]=2;V.Extra[EventBurst]=2.2f;V.Extra[LevelFlash]=2;Name=TEXT("Kinetic reward");}
    FString Sec=Section(Cat,Name);if(C.Contains(Sec))continue;
    for(int I=0;I<ParamCount;I++)if(Params[I].Category==Cat)C.SetFloat(*Sec,Params[I].Key,GetParam(V,I));
   }
  }
  C.SetString(TEXT("Meta"),TEXT("SeedVersion"),TEXT("2"));return C.Write(Path);
 }
};
}
