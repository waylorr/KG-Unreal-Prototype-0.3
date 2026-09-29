void VerifyTick(){if(RealTime<3+VerifyStep*1.5f)return;switch(VerifyStep++){
case 0:Check(GlassMaterial.IsValid(),TEXT("Cooked UI material loads"));Shot(TEXT("01-components.png"));break;
case 1:Activate(31);break;
case 2:Check(State.Compact&&State.Layout(K.Time)>.99f,TEXT("Compact is persistent"));Shot(TEXT("02-compact.png"));DeferredAction=23;DeferredFrames=2;break;
case 3:Check(State.Compact&&Evaluate(D,P,K,Events).Reveal>.99f,TEXT("Exit returns to compact"));Activate(22);break;
case 4:Check(State.Compact&&Evaluate(D,P,K,Events).Reveal>.95f,TEXT("Entrance preserves compact"));Activate(30);D.XP=4990;Events.Empty();Activate(63);break;
case 5:{auto V=Evaluate(D,P,K,Events);Check(V.Level==13&&V.XP==140&&Events.Num()==1,TEXT("Single reward crosses threshold with carry"));Activate(22);Activate(62);break;}
case 6:Check(Events.Num()==1,TEXT("Motion replay does not duplicate rewards"));K.Playing=false;Frozen=K.Time;FrozenFill=Evaluate(D,P,K,Events).Fill;break;
case 7:Check(K.Time==Frozen&&Evaluate(D,P,K,Events).Fill==FrozenFill,TEXT("Pause freezes effects and progress"));Nav.KitScope=false;SelectCategory(2);EditParameter(7,.33f);Nav.KitScope=true;EditParameter(7,.81f);Check(FMath::IsNearlyEqual(P.AmbientStrength,.33f)&&FMath::IsNearlyEqual(Shared.AmbientStrength,.78f),TEXT("Draft editing leaves component and bound preset unchanged"));PresetName=TEXT("Verification ambient");SaveKitPreset();ApplyDraft();Check(FMath::IsNearlyEqual(P.AmbientStrength,.33f),TEXT("Apply preserves per-value local override"));Activate(507);Check(FMath::IsNearlyEqual(P.AmbientStrength,.81f),TEXT("Reset restores inherited value"));break;
case 8:BrowseKitPreset(TEXT("Ion surveillance"));Check(FMath::IsNearlyEqual(P.AmbientStrength,.81f)&&Linked[2]==TEXT("Verification ambient"),TEXT("Browsing another preset leaves assignment unchanged"));BrowseKitPreset(TEXT("Verification ambient"));Nav.KitScope=false;EditParameter(7,.33f);D.Name=TEXT("KIT VERIFY");Save();Load();LoadKit();Check(Overrides[7]&&FMath::IsNearlyEqual(P.AmbientStrength,.33f)&&FMath::IsNearlyEqual(Shared.AmbientStrength,.81f),TEXT("Save/reload preserves overrides and shared links"));Check(Events.Num()==0&&D.XP==4990,TEXT("Synthetic rewards excluded from saved base data"));break;
case 9:Nav.KitScope=true;SelectCategory(0);PresetName=TEXT("Verification entrance");EditParameter(0,1);EditParameter(1,2.1f);SaveKitPreset();ApplyDraft();SelectCategory(1);PresetName=TEXT("Verification exit");EditParameter(4,1);EditParameter(5,.65f);SaveKitPreset();ApplyDraft();Save();Load();LoadKit();Check(P.Entrance==1&&P.Exit==1&&FMath::IsNearlyEqual(P.Duration,2.1f)&&FMath::IsNearlyEqual(P.ExitDuration,.65f),TEXT("Entrance and exit persist independently"));break;
case 10:{float Entry=P.Duration;int Type=P.Entrance;Nav.KitScope=true;SelectCategory(4);ApplyKitPreset(TEXT("Precision V2"));Check(P.Design==1&&P.Entrance==Type&&P.Duration==Entry,TEXT("Visual V2 changes appearance without changing motion"));SelectCategory(2);EditParameter(8,-2);SelectCategory(3);SelectCategory(2);Check(Draft.Speed==-2,TEXT("Unsaved drafts survive category navigation"));Check(PreviewValues().Speed==-2&&P.Speed!= -2,TEXT("Signed draft speed is isolated from component"));Activate(ActionId::OpenKit);Check(Nav.Category==2&&Draft.Speed==-2,TEXT("Reopening active UI Kit preserves unsaved draft"));Activate(ActionId::OpenComponents);Activate(ActionId::OpenKit);Check(Nav.Category==2&&Draft.Speed==-2,TEXT("Switching scopes preserves UI Kit category and draft"));break;}
case 11:{Nav.KitScope=false;Remember();float Before=P.Speed;EditParameter(8,4);Undo(false);Check(P.Speed==Before,TEXT("Undo restores parameters and inheritance"));Undo(true);Check(P.Speed==4&&Overrides[8],TEXT("Redo restores local edit"));break;}
case 12:{auto Outer=BarShape(0,0,100,20);auto Zero=ClipBarX(Outer,0);auto Full=ClipBarX(Outer,100);Check(Full.Num()==Outer.Num(),TEXT("Full progress matches canonical track silhouette"));bool Within=true;for(float Value:{0.f,.01f,.5f,.99f,1.f})for(auto Vertex:ClipBarX(Outer,Value*100))Within&=Vertex.X<=Value*100+.001f;Check(Within,TEXT("Progress clipping holds at 0/1/50/99/100 percent"));break;}
case 13:D=ProfileData();P=Presentation();Shared=P;Draft=P;for(int I=0;I<ParamCount;I++)Overrides[I]=false;State=ProfileState();Events.Empty();K=Playback();K.Time=3;Nav.KitScope=true;Nav.Category=2;Nav.ControlPage=0;PresetName=Linked[2];RefreshNames();Shot(TEXT("03-ui-kit.png"));break;
case 14:Nav.KitScope=false;SelectCategory(2);EditParameter(17,1.5f);EditParameter(24,35);K.Time=4.63f;K.Playing=false;Shot(TEXT("04-whole-glitch.png"));break;
case 15:EditParameter(17,0);D.XP=4900;Events.Empty();AddXP(150);K.Time+=.4f;K.Playing=false;Shot(TEXT("05-xp-level-up.png"));break;
case 16:Activate(81);Shot(TEXT("06-progress-part.png"));break;
case 17:Activate(81);Activate(24);K.Time=3.25f;K.Playing=false;Nav.KitScope=false;Nav.Tab=0;Shot(TEXT("07-pause-a.png"));break;
case 18:Shot(TEXT("08-pause-b.png"));break;
case 19:K.Time=8;K.Time=3.25f;Shot(TEXT("09-seek-repeat.png"));break;
case 20:Nav.KitScope=false;EditParameter(6,0);EditParameter(13,0);K.Time=10;K.Playing=false;Shot(TEXT("10-effects-off.png"));break;
case 21:Activate(62);K.Time+=.4f;K.Playing=false;Shot(TEXT("11-zero-reaction.png"));break;
case 22:Activate(81);K.Time=20;K.Playing=false;Shot(TEXT("12-solo-ambient-off.png"));break;
case 23:K.Time=21;Shot(TEXT("13-solo-ambient-off-later.png"));break;
case 24:{FString Result=Checks+FString::Printf(TEXT("\nMeasured %.2f FPS; max frame %.2f ms; %d frames\n"),FrameCount/FMath::Max(.001f,FrameSum),MaxFrame*1000,FrameCount);FFileHelper::SaveStringToFile(Result,*(Evidence/TEXT("automated-results.txt")));FGenericPlatformMisc::RequestExit(false);break;}
}}


