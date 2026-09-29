// One route for clicks, shortcuts and future feedback cues. Visual controls only emit IDs.
void Activate(int Id){
 if(Id<0)return;
 if(Id!=ActionId::DeletePreset)DeleteArmed=false;
 if(Id==ActionId::ToggleSound){SoundEnabled=!SoundEnabled;SaveSoundPreference();if(SoundEnabled)PlayUiCue(EUiCue::Select);UE_LOG(LogTemp,Display,TEXT("UI_SOUND_ENABLED %d"),SoundEnabled);return;}
 PlayUiCue(CueForAction(Id));
 if(Id>=ActionId::ParameterToggleBase&&Id<ActionId::ParameterToggleBase+ParamCount){
  Remember();int I=Id-ActionId::ParameterToggleBase;float V=GetParam(Nav.KitScope?Draft:P,I);
  if(V>0){ModuleRecall[I]=V;EditParameter(I,0);}else EditParameter(I,ModuleRecall[I]>0?ModuleRecall[I]:1);return;
 }
 if(Id>=ActionId::ParameterValueBase&&Id<ActionId::ParameterValueBase+ParamCount){
  Commit();Remember();Edit=Id;Buffer=FString::SanitizeFloat(GetParam(Nav.KitScope?Draft:P,Id-ActionId::ParameterValueBase));Replace=true;return;
 }
 if(Id>=ActionId::ParameterChoiceBase&&Id<ActionId::ParameterChoiceBase+ParamCount){
  Remember();int I=Id-ActionId::ParameterChoiceBase;float V=GetParam(Nav.KitScope?Draft:P,I)+1;
  if(V>Params[I].Max)V=Params[I].Min;EditParameter(I,V);return;
 }
 if(Id==ActionId::Undo){Undo(false);return;}
 if(Id==ActionId::Redo){Undo(true);return;}
 if(Id>=ActionId::ProfileName&&Id<=ActionId::PresetName){
  Commit();Edit=Id;Buffer=Id==ActionId::ProfileName?D.Name:Id==ActionId::Level?FString::FromInt(D.Level):Id==ActionId::CurrentXP?FString::FromInt(D.XP):Id==ActionId::NextLevelXP?FString::FromInt(D.MaxXP):PresetName;Replace=true;return;
 }
 if(Id==ActionId::VariantName){Commit();Edit=Id;Buffer=Variants[ActiveVariant].Name;Replace=true;return;}
 if(Id==ActionId::ComponentSearch){Commit();Edit=Id;Buffer=ComponentFilter;Replace=true;return;}
 Commit();
 if(Id==ActionId::EventSection){NavigateSection(7);return;}
 if(Id>=ActionId::SectionBase&&Id<ActionId::SectionBase+7){NavigateSection(Id-ActionId::SectionBase);return;}
 if(Id>=ActionId::LinkCategoryBase&&Id<ActionId::LinkCategoryBase+7){SelectCategory(Id-ActionId::LinkCategoryBase);PresetPicker=true;return;}
 if(Id>=ActionId::CategoryBase&&Id<ActionId::CategoryBase+7){SelectCategory(Id-ActionId::CategoryBase);if(Nav.KitScope&&Nav.Section==4){if(Nav.Category==0)BeginTransition(false);else if(Nav.Category==1)BeginTransition(true);}return;}
 if(Id>=ActionId::ParameterResetBase&&Id<ActionId::ParameterResetBase+ParamCount){Remember();Overrides[Id-ActionId::ParameterResetBase]=false;ResolveKit();ComponentDirty=true;return;}
 if(Id>=ActionId::PresetSlotBase&&Id<ActionId::PresetSlotBase+PresetNames.Num()){FString Name=PresetNames[Id-ActionId::PresetSlotBase];Remember();if(Nav.KitScope){BrowseKitPreset(Name);K.Playing=true;if(Nav.Category==0)BeginTransition(false);else if(Nav.Category==1)BeginTransition(true);else if(Nav.Category==3)K.PulseAt=K.Time;else if(Nav.Category==6){Events.Empty();AddXP(100);}}else LinkComponentPreset(Name);PresetPicker=false;return;}
 if(Id>=ActionId::SelectLayerBase&&Id<ActionId::SelectLayerBase+8){Nav.SelectedLayer=Id-ActionId::SelectLayerBase;Nav.SelectInspector(2);if(Nav.SelectedLayer==5)Nav.OpenProgressPart();else Nav.LayersInspector=true;SoloBar=false;return;}
 if(Id>=ActionId::LayerBase&&Id<ActionId::LayerBase+8){Remember();P.Layers[Id-ActionId::LayerBase]=!P.Layers[Id-ActionId::LayerBase];ComponentDirty=true;return;}
 switch(Id){
 case ActionId::PreviewLevelUp:FixtureXP=D.MaxXP-50;Events.Empty();AddXP(100);Notice=TEXT("Level-up event preview / +100 XP");break;
 case ActionId::OpenDesign:Nav.DesignOpen=!Nav.DesignOpen;break;
 case ActionId::BackToProfile:Nav.SelectedLayer=-1;Nav.SelectInspector(0);SoloBar=false;break;
 case ActionId::ToggleDrawer:Nav.DrawerOpen=!Nav.DrawerOpen;break;
 case ActionId::TogglePresetPicker:PresetPicker=!PresetPicker;PickerOffset=0;RefreshNames();break;
 case ActionId::DismissPresetPicker:PresetPicker=false;break;
 case ActionId::PreviousPickerPage:PickerOffset=FMath::Max(0,PickerOffset-(Nav.DrawerOpen&&Nav.KitScope?12:6));break;
 case ActionId::NextPickerPage:PickerOffset=FMath::Min(FMath::Max(0,PresetNames.Num()-(Nav.DrawerOpen&&Nav.KitScope?12:6)),PickerOffset+(Nav.DrawerOpen&&Nav.KitScope?12:6));break;
 case ActionId::CreateVariant:CreateVariant();break;
 case ActionId::SelectProfile:if(Nav.KitScope){StoreCurrentDraft();Drafts[Nav.Category]=Draft;DraftNames[Nav.Category]=PresetName;Nav.OpenComponents();}Nav.Section=0;Nav.DrawerOpen=true;Nav.SelectedLayer=-1;Nav.SelectInspector(0);PresetPicker=false;Notice=TEXT("Player Profile selected / design layers are below the component");break;
 case ActionId::ShowLayers:Nav.Section=0;Nav.DrawerOpen=true;Nav.DesignOpen=true;Nav.SelectedLayer=0;PresetPicker=false;break;
 case ActionId::OpenProgressPart:if(Nav.KitScope)Nav.OpenComponents();Nav.Section=0;Nav.DrawerOpen=true;Nav.DesignOpen=true;Nav.OpenProgressPart();Nav.SelectedLayer=5;Nav.LayersInspector=false;SoloBar=false;PresetPicker=false;break;
 case ActionId::ResetPartDefaults:{Remember();Presentation Defaults;for(int I=0;I<ParamCount;I++)if(Params[I].Category==5){SetParam(P,I,GetParam(Defaults,I));Overrides[I]=true;}ComponentDirty=true;Notice=TEXT("XP Bar styling restored to component defaults");break;}
 case ActionId::PreviousVariant:SwitchVariant((ActiveVariant+Variants.Num()-1)%FMath::Max(1,Variants.Num()));break;
 case ActionId::NextVariant:SwitchVariant((ActiveVariant+1)%FMath::Max(1,Variants.Num()));break;
 case ActionId::GoHome:AppPage=0;Notice=TEXT("Main menu");break;
 case ActionId::GoWorkbench:AppPage=1;Notice=TEXT("UI Workbench / Player Profile");break;
 case ActionId::DeletePreset:DeleteCurrentPreset();break;
 case ActionId::GoEpisodes:AppPage=2;break;
 case ActionId::GoEditor:AppPage=3;break;
 case ActionId::OpenComponents:PresetPicker=false;
  if(Nav.KitScope){Drafts[Nav.Category]=Draft;DraftNames[Nav.Category]=PresetName;Nav.OpenComponents();Draft=Drafts[Nav.Category];PresetName=DraftNames[Nav.Category];RefreshNames();}
  Nav.SelectSection(0);Nav.DrawerOpen=true;
  Notice=TEXT("Components / edit the profile and test its behavior");break;
 case ActionId::OpenKit:PresetPicker=false;
  if(!Nav.KitScope){Nav.OpenKit();Draft=Drafts[Nav.Category];PresetName=DraftNames[Nav.Category];}
  Nav.SelectSection(Nav.Category==4?3:Nav.Category<2?4:Nav.Category==2?5:Nav.Category==3?6:7);
  Nav.DrawerOpen=false;
  RefreshNames();
  Notice=TEXT("UI Kit / shared resources with per-value local overrides");break;
 case ActionId::InspectorStyle:NavigateSection(1);break;
 case ActionId::InspectorContent:case ActionId::InspectorMotion:
  Nav.SelectInspector(Id-ActionId::InspectorContent);
  if(Nav.Tab==1){if(Nav.Category==5)SelectCategory(0);}else if(Nav.Tab==2){Nav.LayersInspector=true;}else RefreshNames();break;
 case ActionId::PreviousControls:Nav.ControlPage=FMath::Max(0,Nav.ControlPage-1);break;
 case ActionId::NextControls:{int N=0;for(const auto& Param:Params)if(Param.Category==Nav.Category)N++;Nav.ControlPage=FMath::Min((N-1)/6,Nav.ControlPage+1);break;}
 case ActionId::PreviousPresetPage:Nav.PresetPage=FMath::Max(0,Nav.PresetPage-1);break;
 case ActionId::NextPresetPage:Nav.PresetPage=FMath::Min(FMath::Max(0,(PresetNames.Num()-1)/6),Nav.PresetPage+1);break;
 case ActionId::ApplyPreset:Remember();ApplyDraft();break;
 case ActionId::SaveAs:StoreCurrentDraft();PresetName=SafePresetName(PresetName.Left(19)+TEXT(" copy"));KitDirty[Nav.Category]=true;Notice=TEXT("New preset draft / edit name then Save preset");break;
 case ActionId::BarSolo:SoloBar=!SoloBar;Nav.OpenProgressPart();break;
 case ActionId::Pulse:K.PulseAt=K.Time;K.Playing=true;break;
 case ActionId::Reward150:AddXP(150);break;
 case ActionId::SavePreset:SaveKitPreset();break;
 case ActionId::ReloadPreset:BrowseKitPreset(PresetName,true);break;
 case ActionId::ResetCategory:Remember();for(int I=0;I<ParamCount;I++)if(Params[I].Category==Nav.Category)Overrides[I]=false;ResolveKit();ComponentDirty=true;Notice=TEXT("Category restored to shared values");break;
 case ActionId::SaveComponent:Save();break;
 case ActionId::ReloadComponent:Load();LoadKit();LoadVariants();break;
 case ActionId::Close:FGenericPlatformMisc::RequestExit(false);break;
 case ActionId::PausePreview:K.Playing=!K.Playing;break;
 case ActionId::StopPreview:SeekEnd=12;State.LayoutFrom=State.Compact?1.f:0.f;State.LayoutAt=-10;K.Time=0;K.Transition=0;K.From=0;K.Exiting=false;K.Playing=false;break;
 case ActionId::ReplayEnter:BeginTransition(false);break;
 case ActionId::PreviewExit:BeginTransition(true);break;
 case ActionId::ResetTest:K.PulseAt=-100;SeekEnd=12;State.LayoutFrom=State.Compact?1.f:0.f;State.LayoutAt=-10;Notice=TEXT("Preview restarted / synthetic rewards cleared");Events.Empty();FixtureXP=-1;K.Time=0;K.Transition=0;K.From=0;K.Exiting=false;K.Playing=true;break;
 case ActionId::Expanded:State.SetCompact(false,K.Time,!K.Playing);ComponentDirty=true;break;
 case ActionId::Compact:State.SetCompact(true,K.Time,!K.Playing);ComponentDirty=true;break;
 case ActionId::Pin:State.Pinned=!State.Pinned;if(State.Pinned)State.SetCompact(false,K.Time,!K.Playing);break;
 case ActionId::Disabled:State.Disabled=!State.Disabled;break;
 case ActionId::AutoCollapse:State.AutoCollapse=!State.AutoCollapse;ComponentDirty=true;break;
 case ActionId::Reward50:AddXP(50);break;
 case ActionId::Reward100:AddXP(100);break;
 case ActionId::NearThreshold:FixtureXP=D.MaxXP-50;Events.Empty();Notice=TEXT("Threshold fixture: next +100 XP crosses a level");break;
 case ActionId::ClearRewards:Events.Empty();FixtureXP=-1;Notice=TEXT("Synthetic events cleared; base profile restored");break;
 case ActionId::Portrait:P.Portrait=(P.Portrait+1)%2;ComponentDirty=true;break;
 default:return;
 }
 UE_LOG(LogTemp,Display,TEXT("UI_ACTION %d"),Id);
}
