#include "WorkbenchGameMode.h"
#include "WorkbenchSkin.h"
#include "KitParameters.h"
#include "KitPresetStore.h"
#include "KitBindings.h"
#include "KitParameterEditor.h"
#include "WorkbenchActions.h"
#include "WorkbenchNavigation.h"
#include "WorkbenchSound.h"
#include "ProfileVariants.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Modules/ModuleManager.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Texture2D.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Widgets/SLeafWidget.h"
#include "Widgets/SWindow.h"
#include "Framework/Application/SlateApplication.h"
#include "UObject/StrongObjectPtr.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformApplicationMisc.h"
#include "UnrealClient.h"
IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl,KoaliticWorkbench,"KoaliticWorkbench");
namespace KW {
void SetInt(FConfigFile& C,const TCHAR* Section,const TCHAR* Key,int V){C.SetString(Section,Key,*FString::FromInt(V));}
struct Hit {int Id;FSlateRect Rect;};
class SWorkbench final:public SLeafWidget {
public:
 SLATE_BEGIN_ARGS(SWorkbench){} SLATE_ARGUMENT(UWorld*,World) SLATE_END_ARGS()
 void Construct(const FArguments& Args){World=Args._World;SetCanTick(true);ForceVolatile(true);SetCursor(EMouseCursor::Default);Portraits[0]=LoadImage(TEXT("PortraitSource.png"));Portraits[0].SetUVRegion(FBox2f({170.f/2048,151.f/726},{583.f/2048,546.f/726}));Portraits[1]=LoadImage(TEXT("PortraitAlternateSource.png"));Portraits[1].SetUVRegion(FBox2f({524.f/1672,257.f/941},{722.f/1672,444.f/941}));DataDir=FString(FPlatformProcess::UserSettingsDir())/TEXT("KOALITIC/WorkbenchV03");Verify=FParse::Param(FCommandLine::Get(),TEXT("WorkbenchVerify"));QA=FParse::Param(FCommandLine::Get(),TEXT("QAControl"));AppPage=(Verify||QA)?1:0;FParse::Value(FCommandLine::Get(),TEXT("Evidence="),Evidence);if(Evidence.IsEmpty())Evidence=FPaths::ProjectSavedDir()/TEXT("Verification");PersistenceRead=FParse::Param(FCommandLine::Get(),TEXT("PersistenceRead"));if(Verify||PersistenceRead||FParse::Param(FCommandLine::Get(),TEXT("IsolatedWorkspace")))DataDir/=TEXT("VerificationKit");IFileManager::Get().MakeDirectory(*DataDir,true);Load();LoadKit();SeedKit();LoadVariants();LoadSoundPreference();LoadUiSounds();if(auto* Material=LoadObject<UMaterialInterface>(nullptr,TEXT("/Game/InterfaceMaterials/M_ProfileGlass.M_ProfileGlass"))){GlassMaterial.Reset(UMaterialInstanceDynamic::Create(Material,nullptr));GlassBrush.SetResourceObject(GlassMaterial.Get());GlassBrush.DrawAs=ESlateBrushDrawType::Image;}if(auto* Material=LoadObject<UMaterialInterface>(nullptr,TEXT("/Game/InterfaceMaterials/M_WorkbenchAtmosphere.M_WorkbenchAtmosphere"))){AtmosphereMaterial.Reset(UMaterialInstanceDynamic::Create(Material,nullptr));AtmosphereBrush.SetResourceObject(AtmosphereMaterial.Get());AtmosphereBrush.DrawAs=ESlateBrushDrawType::Image;}if(Verify){D=ProfileData();P=Presentation();Shared=Presentation();State=ProfileState();SoundEnabled=false;for(int I=0;I<ParamCount;I++)Overrides[I]=false;}K.Time=0;K.From=0;Started=FPlatformTime::Seconds();UE_LOG(LogTemp,Display,TEXT("WORKBENCH_READY Unreal 5.8 layered runtime; portrait assets=%d/%d; storage=%s"),Portraits[0].GetResourceObject()!=nullptr,Portraits[1].GetResourceObject()!=nullptr,*DataDir);}
 FVector2D ComputeDesiredSize(float)const override{return {1920,1080};}bool SupportsKeyboardFocus()const override{return true;}
 FSlateBrush LoadImage(const FString& File){FSlateBrush B;UTexture2D* T=UKismetRenderingLibrary::ImportFileAsTexture2D(World.Get(),FPaths::ProjectContentDir()/TEXT("Interface")/File);if(T){T->Filter=TF_Bilinear;Textures.Emplace(T);B.SetResourceObject(T);B.ImageSize=FVector2D(T->GetSizeX(),T->GetSizeY());B.DrawAs=ESlateBrushDrawType::Image;}return B;}
 void LoadSoundPreference(){FConfigFile C;C.Read(DataDir/TEXT("Interface.ini"));C.GetBool(TEXT("Audio"),TEXT("Enabled"),SoundEnabled);}
 void SaveSoundPreference(){FConfigFile C;C.SetBool(TEXT("Audio"),TEXT("Enabled"),SoundEnabled);if(!C.Write(DataDir/TEXT("Interface.ini")))UE_LOG(LogTemp,Warning,TEXT("UI_SOUND preference could not be saved"));}
 void LoadUiSounds(){const TCHAR* Names[]={TEXT("Navigate"),TEXT("Select"),TEXT("Confirm"),TEXT("Enter"),TEXT("Exit"),TEXT("Reward")};for(int I=0;I<6;I++){FString Path=FString::Printf(TEXT("/Game/UISounds/UI_%s.UI_%s"),Names[I],Names[I]);UiSounds[I].Reset(LoadObject<USoundBase>(nullptr,*Path));UE_LOG(LogTemp,Display,TEXT("UI_SOUND_ASSET %s loaded=%d"),Names[I],UiSounds[I].Get()!=nullptr);}}
 void PlayUiCue(EUiCue Cue){int I=int(Cue);if(!SoundEnabled||I<0||I>=6||!UiSounds[I].Get()||!World.IsValid())return;UGameplayStatics::PlaySound2D(World.Get(),UiSounds[I].Get(),.44f);UE_LOG(LogTemp,Display,TEXT("UI_SOUND_PLAY %d"),I);}
 void Tick(const FGeometry& G,double Now,float Dt)override{ResolveKit();if(DeferredAction>=0&&--DeferredFrames<=0){int Action=DeferredAction;DeferredAction=-1;Activate(Action);}if(K.Playing)K.Time+=FMath::Min(Dt,.1f);SeekEnd=FMath::Max(SeekEnd,FMath::Max(12.f,FMath::CeilToFloat(K.Time/6)*6));RealTime=FPlatformTime::Seconds()-Started;if(!TitleSet&&RealTime>.5f&&GEngine&&GEngine->GameViewport){if(auto Window=GEngine->GameViewport->GetWindow()){Window->SetTitle(FText::FromString(TEXT("KOALITIC | UI Workbench 0.3")));TitleSet=true;}}if(RealTime>3){FrameSum+=Dt;FrameCount++;MaxFrame=FMath::Max(MaxFrame,Dt);}if(State.AutoCollapse&&!State.Pinned&&!State.Hover)State.SetCompact(true,K.Time,!K.Playing);if(Verify)VerifyTick();if(PersistenceRead&&RealTime>3){Check(D.Name==TEXT("KIT VERIFY")&&D.XP==4990&&P.Entrance==1&&P.Exit==1,TEXT("Shared assignments survive packaged restart"));Check(Overrides[7]&&FMath::IsNearlyEqual(P.AmbientStrength,.33f)&&FMath::IsNearlyEqual(Shared.AmbientStrength,.81f),TEXT("Per-value override and shared preset survive packaged restart"));IFileManager::Get().MakeDirectory(*Evidence,true);FFileHelper::SaveStringToFile(Checks,*(Evidence/TEXT("restart-results.txt")));FGenericPlatformMisc::RequestExit(false);PersistenceRead=false;}if(QA&&RealTime-LastQA>.2f){LastQA=RealTime;FString Cmd;FString Path=Evidence/TEXT("command.txt");if(FFileHelper::LoadFileToString(Cmd,*Path)&&!Cmd.IsEmpty()){FFileHelper::SaveStringToFile(TEXT(""),*Path);ExecuteQA(Cmd);}}UpdateMaterial();if(AtmosphereMaterial.IsValid())AtmosphereMaterial->SetScalarParameterValue(TEXT("Clock"),RealTime);Invalidate(EInvalidateWidgetReason::Paint);}
 void BeginTransition(bool Exit){auto V=Evaluate(D,PreviewValues(),K,Events);K.From=(!Exit&&V.Reveal>.99f)?0:V.Reveal;K.Transition=K.Time;K.Exiting=Exit;K.Playing=true;}
 ProfileData PreviewProfile()const{ProfileData Preview=D;if(FixtureXP>=0)Preview.XP=FixtureXP;return Preview;}
 void AddXP(int Amount){Events.RemoveAll([&](const Reward& R){return R.Time>K.Time;});Events.Add({K.Time,Amount});K.Playing=true;Notice=FString::Printf(TEXT("Preview event #%d  /  +%d XP"),Events.Num(),Amount);UE_LOG(LogTemp,Display,TEXT("REWARD event=%d amount=%d time=%.3f"),Events.Num(),Amount,K.Time);}
 void WriteConfig(FConfigFile& C){WriteKitLinks(C);C.SetString(TEXT("Profile"),TEXT("Name"),*D.Name);SetInt(C,TEXT("Profile"),TEXT("Level"),D.Level);SetInt(C,TEXT("Profile"),TEXT("XP"),D.XP);SetInt(C,TEXT("Profile"),TEXT("Threshold"),D.MaxXP);SetInt(C,TEXT("Style"),TEXT("Portrait"),P.Portrait);for(int I=0;I<8;I++)C.SetBool(TEXT("Layers"),*FString::FromInt(I),P.Layers[I]);C.SetBool(TEXT("State"),TEXT("Compact"),State.Compact);C.SetBool(TEXT("State"),TEXT("Pinned"),State.Pinned);C.SetBool(TEXT("State"),TEXT("AutoCollapse"),State.AutoCollapse);}
 void ReadMotion(FConfigFile& C,const TCHAR* Sec){C.GetInt(Sec,TEXT("Entrance"),P.Entrance);C.GetInt(Sec,TEXT("Exit"),P.Exit);C.GetInt(Sec,TEXT("Direction"),P.Direction);C.GetFloat(Sec,TEXT("Duration"),P.Duration);C.GetFloat(Sec,TEXT("ExitDuration"),P.ExitDuration);C.GetFloat(Sec,TEXT("Stagger"),P.Stagger);P.Entrance=FMath::Clamp(P.Entrance,0,7);P.Exit=FMath::Clamp(P.Exit,0,7);P.Duration=FMath::Clamp(P.Duration,.15f,6.f);P.ExitDuration=FMath::Clamp(P.ExitDuration,.15f,6.f);P.Stagger=FMath::Clamp(P.Stagger,0.f,.35f);}
 void Save(){CaptureVariant();FConfigFile C;WriteConfig(C);ProfileVariantStore::Write(C,Variants,ActiveVariant);bool Saved=C.Write(DataDir/TEXT("Presentation.ini"));ComponentDirty=!Saved;Notice=Saved?TEXT("Component and variants saved  /  synthetic rewards excluded"):TEXT("SAVE FAILED: cannot write local settings");}
 void Load(){FConfigFile C;C.Read(DataDir/TEXT("Presentation.ini"));if(!C.Contains(TEXT("Profile"))){Notice=TEXT("Fresh presentation  /  local workspace");return;}C.GetString(TEXT("Profile"),TEXT("Name"),D.Name);C.GetInt(TEXT("Profile"),TEXT("Level"),D.Level);C.GetInt(TEXT("Profile"),TEXT("XP"),D.XP);C.GetInt(TEXT("Profile"),TEXT("Threshold"),D.MaxXP);D.MaxXP=FMath::Clamp(D.MaxXP,100,999999);D.XP=FMath::Clamp(D.XP,0,D.MaxXP-1);D.Level=FMath::Clamp(D.Level,1,999);D.Name=D.Name.Left(18);C.GetInt(TEXT("Style"),TEXT("Portrait"),P.Portrait);P.Portrait=FMath::Clamp(P.Portrait,0,1);C.GetInt(TEXT("Style"),TEXT("Accent"),P.Accent);C.GetInt(TEXT("Style"),TEXT("Ambient"),P.Ambient);C.GetFloat(TEXT("Style"),TEXT("Glow"),P.Glow);C.GetFloat(TEXT("Style"),TEXT("Glitch"),P.Glitch);C.GetFloat(TEXT("Style"),TEXT("Frequency"),P.Frequency);C.GetFloat(TEXT("Style"),TEXT("AmbientStrength"),P.AmbientStrength);for(int I=0;I<8;I++)C.GetBool(TEXT("Layers"),*FString::FromInt(I),P.Layers[I]);C.GetBool(TEXT("State"),TEXT("Compact"),State.Compact);C.GetBool(TEXT("State"),TEXT("Pinned"),State.Pinned);C.GetBool(TEXT("State"),TEXT("AutoCollapse"),State.AutoCollapse);ReadMotion(C,TEXT("Motion"));Events.Empty();FixtureXP=-1;Notice=TEXT("Presentation loaded  /  preview rewards reset");}
 void Commit(){if(Edit<0)return;if(Edit>=900&&Edit<900+ParamCount){EditParameter(Edit-900,FCString::Atof(*Buffer));Edit=-1;return;}if(Edit==100){D.Name=Buffer.IsEmpty()?TEXT("OPERATOR"):Buffer.Left(18);ComponentDirty=true;}if(Edit==101){D.Level=FMath::Clamp(FCString::Atoi(*Buffer),1,999);ComponentDirty=true;}if(Edit==102){D.XP=FMath::Clamp(FCString::Atoi(*Buffer),0,D.MaxXP-1);Events.Empty();FixtureXP=-1;ComponentDirty=true;}if(Edit==103){D.MaxXP=FMath::Clamp(FCString::Atoi(*Buffer),100,999999);D.XP=FMath::Min(D.XP,D.MaxXP-1);Events.Empty();FixtureXP=-1;ComponentDirty=true;}if(Edit==104){StoreCurrentDraft();PresetName=Buffer.IsEmpty()?TEXT("My motion"):Buffer.Left(24);KitDirty[Nav.Category]=true;}if(Edit==105){Variants[ActiveVariant].Name=Buffer.IsEmpty()?FString::Printf(TEXT("Variant %02d"),ActiveVariant+1):Buffer.Left(24);ComponentDirty=true;}if(Edit==106)ComponentFilter=Buffer.Left(24);Edit=-1;}
 FVector2D Position(const FGeometry& G,const FPointerEvent& E)const{float S=FMath::Min(G.GetLocalSize().X/1920.f,G.GetLocalSize().Y/1080.f);return (G.AbsoluteToLocal(E.GetScreenSpacePosition())-(G.GetLocalSize()-FVector2D(1920,1080)*S)*.5f)/S;}
 int HitAt(FVector2D Pnt)const{for(int I=Hits.Num()-1;I>=0;I--)if(Hits[I].Rect.ContainsPoint(Pnt))return Hits[I].Id;return -1;}
 void Slide(int Id,FVector2D Pnt){for(auto H:Hits)if(H.Id==Id){float T=Sat((Pnt.X-H.Rect.Left)/(H.Rect.Right-H.Rect.Left));if(Id>=ActionId::ParameterSliderBase&&Id<ActionId::ParameterSliderBase+ParamCount){int I=Id-ActionId::ParameterSliderBase;EditParameter(I,FMath::Lerp(Params[I].Min,Params[I].Max,T));}if(Id==ActionId::SeekPreview){K.Time=T*SeekEnd;K.Playing=false;}}}
 FReply OnMouseMove(const FGeometry& G,const FPointerEvent& E)override{auto Pos=Position(G,E);Hover=HitAt(Pos);if(Dragging>=ActionId::SeekPreview&&Dragging<ActionId::ParameterResetBase)Slide(Dragging,Pos);State.Hover=!State.Disabled&&FSlateRect(Nav.DrawerOpen?445.f:195.f,280,1370,610).ContainsPoint(Pos);if(State.Hover&&State.AutoCollapse)State.SetCompact(false,K.Time,!K.Playing);return FReply::Handled();}
 void OnMouseLeave(const FPointerEvent& E)override{State.Hover=false;Hover=-1;SLeafWidget::OnMouseLeave(E);}
 FReply OnMouseButtonDown(const FGeometry& G,const FPointerEvent& E)override{if(E.GetEffectingButton()!=EKeys::LeftMouseButton)return FReply::Unhandled();auto Pos=Position(G,E);int Id=HitAt(Pos);if((Id>=ActionId::SeekPreview&&Id<ActionId::ParameterResetBase)){Commit();Remember();Dragging=Id;Slide(Id,Pos);PlayUiCue(EUiCue::Select);return FReply::Handled().CaptureMouse(SharedThis(this)).SetUserFocus(SharedThis(this));}Activate(Id);return FReply::Handled().SetUserFocus(SharedThis(this));}
 FReply OnMouseButtonUp(const FGeometry&,const FPointerEvent&)override{Dragging=-1;return FReply::Handled().ReleaseMouseCapture();}
 FReply OnMouseWheel(const FGeometry& G,const FPointerEvent& E)override{if(PresetPicker){PickerOffset=FMath::Clamp(PickerOffset-(E.GetWheelDelta()>0?1:-1),0,FMath::Max(0,PresetNames.Num()-6));return FReply::Handled();}if(AppPage!=1)return FReply::Unhandled();FVector2D Pos=Position(G,E);if(Nav.KitScope&&Pos.X>=1450&&Pos.X<=1875&&Pos.Y>=345&&Pos.Y<ResourceEditorTop()-12&&PresetNames.Num()>6){PickerOffset=FMath::Clamp(PickerOffset-(E.GetWheelDelta()>0?1:-1),0,FMath::Max(0,PresetNames.Num()-6));return FReply::Handled();}if(!Nav.KitScope&&Nav.Tab!=1&&!(Nav.SelectedLayer==5))return FReply::Unhandled();float Top=Nav.KitScope?ResourceEditorTop():Nav.Tab==1?406.f:345.f,Bottom=Nav.KitScope?978.f:847.f;if(Pos.X<1446||Pos.X>1882||Pos.Y<Top||Pos.Y>Bottom)return FReply::Unhandled();float& Scroll=Nav.KitScope?Nav.KitScroll[Nav.Category]:Nav.ComponentScroll[Nav.Category];float Max=FMath::Max(0.f,ParameterContentHeight(Nav.Category)-(Bottom-Top));Scroll=FMath::Clamp(Scroll-E.GetWheelDelta()*82.f,0.f,Max);return FReply::Handled();}
 FReply OnKeyChar(const FGeometry&,const FCharacterEvent& E)override{TCHAR Ch=E.GetCharacter();if(Edit>=0&&Ch>=32&&Ch!=127){if(Edit>=101&&Edit<=103&&!FChar::IsDigit(Ch))return FReply::Handled();if(Replace){Buffer.Empty();Replace=false;}if(Buffer.Len()<((Edit==100)?18:((Edit==104||Edit==105||Edit==106)?24:12)))Buffer.AppendChar(Ch);return FReply::Handled();}return FReply::Unhandled();}
 FReply OnKeyDown(const FGeometry&,const FKeyEvent& E)override{auto Key=E.GetKey();if(Edit>=0){if(Key==EKeys::V&&E.IsControlDown()){FString Paste;FPlatformApplicationMisc::ClipboardPaste(Paste);if(Replace)Buffer.Empty();for(TCHAR Ch:Paste){if(Ch>=32&&Ch!=127&&(!(Edit>=101&&Edit<=103)||FChar::IsDigit(Ch)))Buffer.AppendChar(Ch);}Buffer=Buffer.Left(Edit==100?18:(Edit==104||Edit==105||Edit==106)?24:12);Replace=false;}else if(Key==EKeys::Enter)Commit();else if(Key==EKeys::Escape)Edit=-1;else if(Key==EKeys::BackSpace){if(Replace)Buffer.Empty();else Buffer=Buffer.LeftChop(1);Replace=false;}else if(Key==EKeys::A&&E.IsControlDown())Replace=true;else if(Key==EKeys::Tab){Commit();}return FReply::Handled();}if(Key==EKeys::Escape){if(PresetPicker)PresetPicker=false;else if(AppPage!=0)Activate(ActionId::GoHome);return FReply::Handled();}if(AppPage!=1&&Key!=EKeys::F11)return FReply::Unhandled();if(Key==EKeys::Z&&E.IsControlDown())Activate(ActionId::Undo);else if(Key==EKeys::Y&&E.IsControlDown())Activate(ActionId::Redo);else if(Key==EKeys::SpaceBar)Activate(ActionId::PausePreview);else if(Key==EKeys::R)Activate(ActionId::ResetTest);else if(Key==EKeys::E)Activate(ActionId::ReplayEnter);else if(Key==EKeys::X)Activate(ActionId::PreviewExit);else if(Key==EKeys::F11){if(auto PC=World->GetFirstPlayerController())PC->ConsoleCommand(TEXT("togglefullscreen"));}else if(Key==EKeys::S&&E.IsControlDown())Activate(Nav.KitScope?ActionId::SavePreset:ActionId::SaveComponent);else if(Key==EKeys::F9)Shot(TEXT("Manual.png"));else return FReply::Unhandled();return FReply::Handled();}
 void Region(int Id,float X,float Y,float W,float H)const{Hits.Add({Id,FSlateRect(X,Y,X+W,Y+H)});}
 void Button(Canvas& C,int Id,float X,float Y,float W,float H,const FString& Label,bool Active=false,FLinearColor AccentColor=Cyan,bool ColorLabel=false)const{
  bool Hot=Hover==Id;FLinearColor Edge=Active?A(AccentColor,.9f):Hot?A(AccentColor,.55f):A(Line,.95f);
  C.Cut(X,Y,W,H,Active?FMath::Lerp(FLinearColor(.008f,.025f,.041f),AccentColor,.12f):FLinearColor(.008f,.025f,.041f),Edge,4);
  C.Gradient(X+4,Y+4,W-8,FMath::Min(H*.55f,19.f),Active?A(AccentColor,.20f):A(White,.047f),A(Ink,0));
  C.Seg(X+6,Y+1,X+W-6,Y+1,Active?A(AccentColor,.48f):A(White,.095f));
  C.Seg(X+W-20,Y+H-3,X+W-8,Y+H-3,A(AccentColor,Active?.48f:.20f));
  if(Active){C.Box(X+1,Y+5,3,H-10,AccentColor);C.Halo(X+3,Y+H*.5f,11,H*.42f,AccentColor,.20f);C.Glow(X+5,Y+4,W-10,H-8,AccentColor,.11f);}
  C.Text(X+12,Y+(H-18)/2,Label,11,Active||Hot?White:ColorLabel?A(AccentColor,.88f):Muted);Region(Id,X,Y,W,H);
 }
 void RailButton(Canvas& C,int Id,int Icon,float Y,const FString& Label,bool Active,FLinearColor AccentColor)const{
  bool Hot=Hover==Id;float X=27,W=126,H=78;DrawWorkbenchPanel(C,X,Y,W,H,AccentColor,Active||Hot);
  if(Active){C.Box(X,Y+8,4,H-16,AccentColor);C.Gradient(X+5,Y+6,W-10,H-12,A(AccentColor,.16f),A(Ink,0),true);C.Glow(X+17,Y+13,W-34,H-26,AccentColor,.20f);}
  DrawWorkbenchIcon(C,Icon,X+49,Y+10,28,Active||Hot?AccentColor:A(AccentColor,.82f));
  C.Seg(X+33,Y+47,X+93,Y+47,A(AccentColor,Active?.32f:.13f));C.Text(X+13,Y+52,Label,11,Active?White:A(AccentColor,.87f));Region(Id,X,Y,W,H);
 }
 void PresetTile(Canvas& C,int Id,float X,float Y,float W,float H,const FString& Name,bool Active,int Category)const{
  FLinearColor AccentColor=CategoryColor(Category);bool Hot=Hover==Id;C.Cut(X,Y,W,H,Active?FLinearColor(.013f,.078f,.101f):FLinearColor(.008f,.029f,.045f),Active?AccentColor:Hot?A(AccentColor,.65f):Line,5);
  C.Gradient(X+4,Y+3,W-8,FMath::Min(22.f,H*.40f),A(AccentColor,Active?.20f:.08f),A(Ink,0));
  C.Cut(X+7,Y+8,35,H-16,FLinearColor(.004f,.018f,.031f),A(AccentColor,.5f),4);
  int Icon=Category==4?1:Category<2?2:Category==2?3:Category==3?4:5;DrawWorkbenchIcon(C,Icon,X+15,Y+H*.5f-10,20,AccentColor);
  FString Label=Name.Len()>21?Name.Left(19)+TEXT("..."):Name;C.Text(X+50,Y+(H-17)*.5f,Label,10,Active?White:A(White,.8f),Active);
  Region(Id,X,Y,W,H);
 }
 float ResourceEditorTop()const{int Rows=(FMath::Min(6,PresetNames.Num())+1)/2;return FMath::Max(484.f,369.f+Rows*51.f+(PresetNames.Num()>6?43.f:12.f));}
 void Field(Canvas& C,int Id,float Y,const FString& Label,const FString& Value)const{C.Text(1454,Y+10,Label,12,Muted);C.Cut(1613,Y,253,38,FLinearColor(.005f,.019f,.032f),Edit==Id?Cyan:A(Cyan,.28f),3);C.Gradient(1617,Y+3,245,12,A(Cyan,.055f),A(Ink,0));FString Display=Edit==Id?Buffer+TEXT("|"):Value;int Size=12;auto Measure=FSlateApplication::Get().GetRenderer()->GetFontMeasureService();while(Size>8&&Measure->Measure(Display,FSlateFontInfo(Canvas::Font(false),Size)).X>229)--Size;C.Clip(1622,Y+3,236,33);C.Text(1625,Y+9,Display,Size,White);C.Unclip();Region(Id,1613,Y,253,38);}
 void Section(Canvas& C,float Y,const FString& Label)const{C.Box(1454,Y+2,3,18,Cyan);C.Text(1467,Y,Label,12,White,true);C.Seg(1454,Y+29,1866,Y+29,A(Cyan,.35f));}

#include "WorkbenchView.inl"
#include "WorkbenchShell.inl"
 void Shot(const FString& Name){IFileManager::Get().MakeDirectory(*Evidence,true);FScreenshotRequest::RequestScreenshot(Evidence/Name,true,false);}
 void ExecuteQA(FString Cmd){Cmd.TrimStartAndEndInline();TArray<FString>A;Cmd.ParseIntoArrayWS(A);if(A.Num()==0)return;if(A[0]==TEXT("action")&&A.Num()>1)Activate(FCString::Atoi(*A[1]));if(A[0]==TEXT("seek")&&A.Num()>1){K.Time=FCString::Atof(*A[1]);K.Playing=false;}if(A[0]==TEXT("shot")&&A.Num()>1)Shot(A[1]);if(A[0]==TEXT("field")&&A.Num()>2){Activate(FCString::Atoi(*A[1]));Buffer=Cmd.Mid(Cmd.Find(A[2]));Commit();}if(A[0]==TEXT("slider")&&A.Num()>2){int Id=FCString::Atoi(*A[1]);for(auto H:Hits)if(H.Id==Id)Slide(Id,{FMath::Lerp(H.Rect.Left,H.Rect.Right,Sat(FCString::Atof(*A[2]))),H.Rect.Top});}if(A[0]==TEXT("param")&&A.Num()>2){EditParameter(FCString::Atoi(*A[1]),FCString::Atof(*A[2]));}if(A[0]==TEXT("browse")&&A.Num()>1)BrowseKitPreset(Cmd.Mid(7));if(A[0]==TEXT("preset")&&A.Num()>1)ApplyKitPreset(Cmd.Mid(7));if(A[0]==TEXT("kit-status")){FString Out;for(int I=0;I<ParamCount;I++)Out+=FString::Printf(TEXT("%s effective=%.4f shared=%.4f override=%d\n"),Params[I].Key,GetParam(P,I),GetParam(Shared,I),Overrides[I]);FFileHelper::SaveStringToFile(Out,*(Evidence/TEXT("kit-status.txt")));}if(A[0]==TEXT("metrics-reset")){FrameSum=0;FrameCount=0;MaxFrame=0;}if(A[0]==TEXT("exit"))FGenericPlatformMisc::RequestExit(false);if(A[0]==TEXT("status")){auto V=Evaluate(D,P,K,Events);FString Status=FString::Printf(TEXT("time=%.4f playing=%d events=%d level=%d xp=%d fill=%.6f compact=%d disabled=%d pinned=%d entry=%d exit=%d FPS=%.2f maxFrameMs=%.2f"),K.Time,K.Playing,Events.Num(),V.Level,V.XP,V.Fill,State.Compact,State.Disabled,State.Pinned,P.Entrance,P.Exit,FrameCount/FMath::Max(.001f,FrameSum),MaxFrame*1000);FFileHelper::SaveStringToFile(Status,*(Evidence/TEXT("status.txt")));}}

#include "KitVerification.inl"
#include "WorkbenchKit.inl"
#include "WorkbenchActions.inl"
 void Check(bool Pass,const FString& Label){Checks+=(Pass?TEXT("PASS "):TEXT("FAIL "))+Label+TEXT("\n");UE_LOG(LogTemp,Display,TEXT("WORKBENCH_CHECK %s %s"),Pass?TEXT("PASS"):TEXT("FAIL"),*Label);}
private:
 TWeakObjectPtr<UWorld>World;TArray<TStrongObjectPtr<UTexture2D>>Textures;FSlateBrush Portraits[2];ProfileData D;Presentation P;ProfileState State;Playback K;TArray<Reward>Events;mutable TArray<Hit>Hits;
 TStrongObjectPtr<UMaterialInstanceDynamic> GlassMaterial,AtmosphereMaterial;FSlateBrush GlassBrush,AtmosphereBrush;TStrongObjectPtr<USoundBase> UiSounds[6];bool SoundEnabled=true;Presentation Shared,Draft,Drafts[7];FString DraftNames[7];bool Overrides[ParamCount]={};float ModuleRecall[ParamCount]={};bool KitDirty[7]={};TMap<FString,ResourceDraft> ResourceDrafts;WorkbenchNavigation Nav;FString Linked[7];TArray<FString> PresetNames;TArray<ProfileVariant> Variants;int ActiveVariant=0;
 FString DataDir,Notice=TEXT("Ready"),Buffer,PresetName=TEXT("My motion"),ComponentFilter,Evidence,Checks;int DeferredAction=-1,DeferredFrames=0;int Hover=-1,Edit=-1,Dragging=-1,VerifyStep=0,FrameCount=0,AppPage=0,PickerOffset=0,FixtureXP=-1;bool SoloBar=false,Replace=false,Verify=false,QA=false,PersistenceRead=false,TitleSet=false,ComponentDirty=false,PresetPicker=false,DeleteArmed=false;double Started=0;float SeekEnd=12,RealTime=0,FrameSum=0,MaxFrame=0,Frozen=0,FrozenFill=0,LastQA=0;
};
}
AWorkbenchGameMode::AWorkbenchGameMode(){DefaultPawnClass=nullptr;}
void AWorkbenchGameMode::BeginPlay(){Super::BeginPlay();if(GEngine&&GEngine->GameViewport){auto UI=SNew(KW::SWorkbench).World(GetWorld());GEngine->GameViewport->AddViewportWidgetContent(UI,100);if(auto PC=GetWorld()->GetFirstPlayerController()){PC->bShowMouseCursor=true;FInputModeUIOnly Mode;Mode.SetWidgetToFocus(UI);Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);PC->SetInputMode(Mode);}if(auto Window=GEngine->GameViewport->GetWindow())Window->SetTitle(FText::FromString(TEXT("KOALITIC | UI Workbench 0.3")));FSlateApplication::Get().SetKeyboardFocus(UI);}}











