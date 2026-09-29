int32 OnPaint(const FPaintArgs&,const FGeometry& Geo,const FSlateRect&,FSlateWindowElementList& E,int32 Layer,const FWidgetStyle&,bool)const override{
 float Scale=FMath::Min(Geo.GetLocalSize().X/1920.f,Geo.GetLocalSize().Y/1080.f);
 FGeometry G=Geo.MakeChild(FVector2D(1920,1080),FSlateLayoutTransform(Scale,(Geo.GetLocalSize()-FVector2D(1920,1080)*Scale)*.5f));Canvas C{G,E,Layer};Hits.Empty();
 if(AppPage!=1){DrawShell(C);return C.L;}
 C.Box(0,0,1920,1080,Ink);C.Gradient(0,0,1920,1080,FLinearColor(.002f,.009f,.019f),FLinearColor(.0005f,.003f,.009f));
 if(AtmosphereMaterial.IsValid())C.Image(0,92,1920,942,AtmosphereBrush);
 for(int I=0;I<48;I++)C.Seg(I*44,92,I*44+210,1032,A(Cyan,.005f));
 DrawWorkbenchPanel(C,9,8,1902,75,Cyan,true);C.Cut(22,17,67,57,FLinearColor(.006f,.026f,.044f),A(Cyan,.45f),8);DrawWorkbenchIcon(C,0,40,28,31,Cyan);
 C.Text(107,13,TEXT("KOALITIC"),27,White,true);C.Text(111,54,TEXT("U I   W O R K B E N C H"),8,A(Cyan,.78f),true);
 C.Seg(353,20,353,69,A(Cyan,.34f));C.Text(371,24,TEXT("UNREAL 5.8 / UI SYSTEM"),9,Muted,true);
 Button(C,ActionId::GoHome,591,21,117,40,TEXT("<  Home"));C.Text(731,28,TEXT("BUILD   >   CUSTOMIZE   >   PREVIEW"),9,Cyan,true);
 C.Box(1001,39,5,5,ComponentDirty?Red:Green);C.Halo(1003,41,14,14,ComponentDirty?Red:Green,.50f);C.Text(1015,30,ComponentDirty?TEXT("UNSAVED"):TEXT("READY"),9,ComponentDirty?Red:Green,true);
 Button(C,ActionId::Undo,1120,21,100,39,TEXT("Undo"));Button(C,ActionId::Redo,1230,21,100,39,TEXT("Redo"));
 Button(C,ActionId::ToggleSound,1340,21,110,39,SoundEnabled?TEXT("Sound On"):TEXT("Sound Off"),SoundEnabled);
 if(!Nav.KitScope){Button(C,ActionId::ReloadComponent,1460,21,116,39,TEXT("Reload"));Button(C,ActionId::SaveComponent,1586,21,187,39,ComponentDirty?TEXT("Save component *"):TEXT("Save component"),true);}else C.Text(1480,33,TEXT("GLOBAL PRESET EDITING"),10,Cyan,true);Button(C,ActionId::Close,1783,21,105,39,TEXT("Close"));
 C.Seg(21,89,1890,89,A(Cyan,.48f));C.Seg(21,90,740,90,A(White,.13f));
 float MX=Nav.DrawerOpen?427.f:174.f, MW=1395.f-MX, PX=MX+17.f, PW=MW-34.f;
 C.Text(28,111,TEXT("WORKSPACES"),11,Cyan,true);
 C.Text(MX+8,111,TEXT("PLAYER PROFILE / LIVE PREVIEW"),11,Cyan,true);
 C.Text(1430,111,Nav.KitScope?TEXT("GLOBAL RESOURCE EDITOR"):TEXT("COMPONENT INSPECTOR"),11,Nav.KitScope?CategoryColor(Nav.Category):Cyan,true);
 DrawWorkbenchPanel(C,20,149,140,870,Cyan,false);C.Text(36,168,TEXT("BUILD"),9,A(Cyan,.68f),true);
 RailButton(C,ActionId::SectionBase,0,197,TEXT("Library"),Nav.Section==0,Cyan);
 C.Seg(31,290,149,290,A(Cyan,.28f));C.Text(34,298,TEXT("GLOBAL FX"),8,Muted,true);
 RailButton(C,ActionId::SectionBase+3,1,323,TEXT("Themes"),Nav.Section==3,CategoryColor(4));
 RailButton(C,ActionId::SectionBase+4,2,411,TEXT("Motion FX"),Nav.Section==4,CategoryColor(0));
 RailButton(C,ActionId::SectionBase+5,3,499,TEXT("Ambient FX"),Nav.Section==5,CategoryColor(2));
 RailButton(C,ActionId::SectionBase+6,4,587,TEXT("Reaction FX"),Nav.Section==6,CategoryColor(3));
 RailButton(C,ActionId::EventSection,5,675,TEXT("Event FX"),Nav.Section==7,CategoryColor(6));
 C.Seg(31,927,149,927,A(Cyan,.22f));C.Text(35,949,Nav.DrawerOpen?TEXT("LIBRARY OPEN"):TEXT("PREVIEW WIDE"),8,Muted,true);
 C.Box(33,987,5,5,Green);C.Text(45,981,TEXT("NATIVE / UE 5.8"),8,Muted,true);
 if(Nav.DrawerOpen&&!Nav.KitScope){
  DrawWorkbenchPanel(C,170,149,244,870,Cyan,true);DrawWorkbenchFascia(C,170,149,244,51,Cyan);
  C.Text(211,169,TEXT("LIBRARY"),17,White,true);C.Box(211,195,46,2,Cyan);
  Button(C,ActionId::ToggleDrawer,374,164,28,28,TEXT("<"));C.Seg(183,208,401,208,A(Cyan,.35f));
  if(Nav.Section==0){
   C.Cut(183,224,218,37,FLinearColor(.002f,.014f,.025f),Edit==ActionId::ComponentSearch?Cyan:A(Line,1.5f),4);
   C.Path({{195,242},{196,236},{201,233},{206,236},{207,241},{204,245},{198,245},{195,242}},A(Cyan,.65f));C.Seg(206,245,211,250,A(Cyan,.65f));
   C.Text(218,235,Edit==ActionId::ComponentSearch?Buffer+TEXT("|"):ComponentFilter.IsEmpty()?TEXT("Search components..."):ComponentFilter,10,ComponentFilter.IsEmpty()&&Edit!=ActionId::ComponentSearch?Muted:White);Region(ActionId::ComponentSearch,183,224,218,37);
   C.Text(185,269,TEXT("COMPONENTS"),9,Cyan,true);
   FString Filter=Edit==ActionId::ComponentSearch?Buffer:ComponentFilter;
   if(Filter.IsEmpty()||FString(TEXT("Player Profile")).Contains(Filter,ESearchCase::IgnoreCase)){
    C.Cut(183,286,218,75,FLinearColor(.010f,.057f,.081f),A(Cyan,.84f),5);C.Gradient(187,289,210,28,A(Cyan,.10f),A(Ink,0));C.Image(190,292,51,62,Portraits[P.Portrait]);C.Seg(245,294,245,352,A(Cyan,.35f));C.Text(254,301,TEXT("Player Profile"),11,White,true);C.Text(254,327,TEXT("PLAYER / LEVEL / XP"),8,Cyan,true);C.Box(185,297,3,50,Cyan);Region(ActionId::SelectProfile,183,286,218,75);
   }else C.Text(185,292,TEXT("No matching components"),9,Muted);
   C.Seg(183,377,401,377,A(Cyan,.26f));Button(C,ActionId::OpenDesign,183,387,218,34,Nav.DesignOpen?TEXT("DESIGN VARIANTS  /  -"):TEXT("DESIGN VARIANTS  /  +"),Nav.DesignOpen);
   if(Nav.DesignOpen){Button(C,ActionId::PreviousVariant,183,429,33,31,TEXT("<"));Button(C,ActionId::VariantName,221,429,142,31,Edit==ActionId::VariantName?Buffer+TEXT("|"):Variants[ActiveVariant].Name,true);Button(C,ActionId::NextVariant,368,429,33,31,TEXT(">"));
   C.Cut(183,468,218,71,FLinearColor(.004f,.020f,.032f),A(Cyan,.36f),5);C.Clip(189,472,206,62);DrawProfile(C,196,482,192,PreviewProfile(),P,State,K,Events,Portraits[P.Portrait],true,nullptr);C.Unclip();
   Button(C,ActionId::CreateVariant,183,547,218,31,TEXT("+ Duplicate design"));C.Text(184,595,TEXT("LAYER STACK"),10,Cyan,true);C.Seg(183,613,401,613,A(Cyan,.3f));
   static const TCHAR* LayerNames[]={TEXT("Angular frame"),TEXT("Glass surface"),TEXT("Portrait + mask"),TEXT("Player name"),TEXT("Level label"),TEXT("XP Bar / track + fill"),TEXT("Edge lighting"),TEXT("Motion overlays")};
   for(int I=0;I<8;I++){float Y=620+I*47;C.Cut(183,Y,218,42,Nav.SelectedLayer==I?FLinearColor(.012f,.069f,.093f):FLinearColor(.004f,.018f,.033f),Nav.SelectedLayer==I?Cyan:A(Line,.7f),3);DrawWorkbenchEye(C,190,Y+15,A(Cyan,P.Layers[I]?.88f:.3f));C.Text(216,Y+13,LayerNames[I],9,P.Layers[I]?White:Muted);DrawWorkbenchGrip(C,334,Y+14,A(Cyan,.38f));Region(ActionId::SelectLayerBase+I,183,Y,158,42);Button(C,ActionId::LayerBase+I,347,Y+7,46,27,P.Layers[I]?TEXT("On"):TEXT("Off"),P.Layers[I]);}}
  }
 }
 DrawWorkbenchPanel(C,MX,149,MW,556,Cyan,true);DrawWorkbenchFascia(C,MX,149,MW,70,Cyan);C.Box(MX+18,174,8,8,Cyan);C.Halo(MX+22,178,18,18,Cyan,.3f);
 C.Text(MX+35,167,TEXT("LIVE PREVIEW"),18,White,true);C.Text(MX+20,204,Nav.KitScope?FString(TEXT("SHARED RESOURCE  /  "))+CategoryName(Nav.Category):FString(TEXT("PLAYER PROFILE  /  "))+Variants[ActiveVariant].Name,9,Muted,true);
 C.Box(1119,180,5,5,Green);C.Text(1131,172,K.Playing?TEXT("AMBIENT ONLINE"):TEXT("PREVIEW PAUSED"),9,K.Playing?Green:Muted,true);C.Text(1287,172,TEXT("16:9"),9,Muted,true);C.Seg(MX+15,227,MX+MW-15,227,A(Cyan,.33f));
 DrawWorkbenchGrid(C,PX,241,PW,441);if(AtmosphereMaterial.IsValid())C.Image(PX+2,243,PW-4,437,AtmosphereBrush,A(White,.48f));C.Cut(PX,241,PW,441,FLinearColor(0,0,0,0),A(Cyan,.35f),5);
 C.Text(PX+29,627,TEXT("PROFILE_001  /  INDEPENDENT PRESENTATION"),8,Muted,true);C.Text(PX+PW-168,627,K.Playing?TEXT("AMBIENT / LIVE"):TEXT("PREVIEW / PAUSED"),8,K.Playing?CategoryColor(2):Muted,true);
 Presentation Preview=PreviewValues();float ProfileW=FMath::Min((PW-70.f)*.80f,950.f),ProfileX=PX+(PW-ProfileW)*.5f;float ProfileH=ProfileW*FMath::Lerp(.253f,.135f,State.Layout(K.Time)),ProfileY=241+(441-ProfileH)*.5f;
 C.Clip(PX,241,PW,441);if(SoloBar&&!Nav.KitScope){ProgressPartStyle BS;BS.Height=38*P.BarHeight;BS.Inset=P.BarInset;BS.Tip=P.BarTip;BS.Glow=P.Glow*P.BarGlow;BS.Speed=P.Speed*P.BarSpeed;BS.Sweep=P.Ambient&&!State.Disabled?P.Sweep*P.AmbientStrength:0;BS.Color=ThemeColor(P,4,Green);DrawProgressPart(C,PX+60,420,PW-120,BS,Evaluate(PreviewProfile(),P,K,Events).Fill,K.Time);C.Text(PX+60,495,TEXT("PROGRESS BAR / DATA-AGNOSTIC REUSABLE PART"),12,Cyan,true);}else DrawProfile(C,ProfileX,ProfileY,ProfileW,PreviewProfile(),Preview,State,K,Events,Portraits[P.Portrait],false,GlassMaterial.IsValid()?&GlassBrush:nullptr);C.Unclip();
 DrawWorkbenchPanel(C,MX,717,MW,302,Cyan,true);DrawWorkbenchFascia(C,MX,717,MW,39,Cyan);float TX=MX+20,DeckW=MW-40;
 C.Box(TX,739,9,9,Cyan);C.Text(TX+21,729,TEXT("PREVIEW & TEST"),16,White,true);C.Box(MX+MW-208,740,5,5,K.Playing?Green:Muted);C.Text(MX+MW-196,733,K.Playing?TEXT("AMBIENT RUNNING"):TEXT("PREVIEW PAUSED"),9,K.Playing?Green:Muted,true);
 C.Seg(TX,763,MX+MW-20,763,A(Cyan,.33f));
 float LayoutW=DeckW*.22f,MotionX=TX+LayoutW+14,MotionW=DeckW*.29f,EventX=MotionX+MotionW+14,EventW=DeckW*.29f,PauseX=EventX+EventW+14,PauseW=DeckW-(PauseX-TX);
 DrawWorkbenchWell(C,TX-5,773,LayoutW+5,75,Cyan);DrawWorkbenchWell(C,MotionX-5,773,MotionW+5,75,CategoryColor(0));DrawWorkbenchWell(C,EventX-5,773,EventW+5,75,CategoryColor(6));DrawWorkbenchWell(C,PauseX-5,773,PauseW+5,75,Green);
 DrawWorkbenchWell(C,TX-5,861,DeckW+10,70,Cyan);DrawWorkbenchWell(C,TX-5,945,DeckW+10,50,Cyan);
 C.Text(TX,777,TEXT("LAYOUT"),9,Cyan,true);C.Text(MotionX,777,TEXT("MOTION / ONE SHOT"),9,CategoryColor(0),true);C.Text(EventX,777,TEXT("EVENT SIMULATION"),9,CategoryColor(6),true);
 C.Seg(MotionX-8,776,MotionX-8,843,A(Cyan,.22f));C.Seg(EventX-8,776,EventX-8,843,A(Cyan,.22f));C.Seg(PauseX-8,776,PauseX-8,843,A(Cyan,.22f));
 float LW=(LayoutW-7)*.5f;Button(C,ActionId::Expanded,TX,801,LW,39,TEXT("Expanded"),!State.Compact);Button(C,ActionId::Compact,TX+LW+7,801,LW,39,TEXT("Compact"),State.Compact);
 float MotB=(MotionW-12)/3;Button(C,ActionId::ReplayEnter,MotionX,801,MotB,39,TEXT("Enter"),false,CategoryColor(0));Button(C,ActionId::PreviewExit,MotionX+MotB+6,801,MotB,39,TEXT("Exit"),false,CategoryColor(0));Button(C,ActionId::Pulse,MotionX+(MotB+6)*2,801,MotB,39,TEXT("Pulse"),false,CategoryColor(3));
 float EvB=(EventW-6)*.5f;Button(C,ActionId::Reward100,EventX,801,EvB,39,TEXT("+ 100 XP"),false,CategoryColor(6));Button(C,ActionId::PreviewLevelUp,EventX+EvB+6,801,EvB,39,TEXT("Level up"),false,CategoryColor(6));
 Button(C,ActionId::PausePreview,PauseX,801,PauseW,39,K.Playing?TEXT("Pause"):TEXT("Resume"),!K.Playing);
 C.Seg(TX,853,MX+MW-20,853,A(Cyan,.22f));C.Text(TX,866,TEXT("QUICK FIXTURES"),9,Muted,true);
 Button(C,ActionId::Reward50,TX,890,92,35,TEXT("+ 50 XP"),false,CategoryColor(6));Button(C,ActionId::Reward150,TX+98,890,99,35,TEXT("+ 150 XP"),false,CategoryColor(6));Button(C,ActionId::NearThreshold,TX+203,890,144,35,TEXT("Near threshold"));Button(C,ActionId::ClearRewards,TX+353,890,139,35,TEXT("Clear rewards"));
 Button(C,ActionId::ResetTest,MX+MW-164,890,144,35,TEXT("Reset test"));
 C.Seg(TX,939,MX+MW-20,939,A(Cyan,.22f));C.Text(TX,951,TEXT("PREVIEW CONDITIONS"),8,Cyan,true);
 Button(C,ActionId::Pin,TX+160,950,118,34,State.Pinned?TEXT("Pinned: on"):TEXT("Pinned: off"),State.Pinned);Button(C,ActionId::Disabled,TX+285,950,132,34,State.Disabled?TEXT("Disabled: on"):TEXT("Disabled: off"),State.Disabled);Button(C,ActionId::AutoCollapse,TX+424,950,133,34,State.AutoCollapse?TEXT("Hover: on"):TEXT("Hover: off"),State.AutoCollapse);
 C.Text(MX+MW-270,959,TEXT("TRANSIENT / SELECTED LAYOUT"),8,Muted,true);
 DrawWorkbenchPanel(C,1412,149,478,870,Nav.KitScope?CategoryColor(Nav.Category):Cyan,true);DrawWorkbenchFascia(C,1412,149,478,70,Nav.KitScope?CategoryColor(Nav.Category):Cyan);
 if(Nav.KitScope){
  FLinearColor ResourceColor=CategoryColor(Nav.Category);C.Gradient(1419,156,463,84,A(ResourceColor,.09f),A(Ink,0));int ResourceIcon=Nav.Category==4?1:Nav.Category<2?2:Nav.Category==2?3:Nav.Category==3?4:5;
  DrawWorkbenchIcon(C,ResourceIcon,1436,169,26,ResourceColor);C.Text(1473,165,CategoryName(Nav.Category),18,White,true);C.Text(1473,192,TEXT("GLOBAL RESOURCE / LIVE EDITOR"),8,ResourceColor,true);C.Seg(1433,210,1869,210,A(ResourceColor,.32f));
  C.Text(1434,225,TEXT("PRESET"),9,ResourceColor,true);
  C.Cut(1492,213,194,36,Ink,Edit==ActionId::PresetName?Cyan:Line,3);
  C.Clip(1500,216,178,30);C.Text(1503,223,Edit==ActionId::PresetName?Buffer+TEXT("|"):PresetName,11,White);C.Unclip();Region(ActionId::PresetName,1492,213,194,36);
  Button(C,ActionId::SavePreset,1693,213,77,36,KitDirty[Nav.Category]?TEXT("Save *"):TEXT("Save"),true,ResourceColor);
  Button(C,ActionId::DeletePreset,1777,213,89,36,DeleteArmed?TEXT("Confirm"):TEXT("Delete"),DeleteArmed);
  Button(C,ActionId::SaveAs,1454,262,117,30,TEXT("Save as..."));Button(C,ActionId::ReloadPreset,1581,262,132,30,TEXT("Discard draft"));
  C.Text(1723,271,KitDirty[Nav.Category]?TEXT("UNSAVED DRAFT"):TEXT("SAVED PRESET"),8,KitDirty[Nav.Category]?Red:Cyan,true);
  if(Nav.Section==4){Button(C,ActionId::CategoryBase,1454,304,196,32,TEXT("Entrance"),Nav.Category==0,ResourceColor);Button(C,ActionId::CategoryBase+1,1660,304,206,32,TEXT("Exit"),Nav.Category==1,ResourceColor);}
  else if(Nav.Section==7){Button(C,ActionId::Reward100,1454,304,196,32,TEXT("Preview +100 XP"),false,ResourceColor);Button(C,ActionId::NearThreshold,1660,304,98,32,TEXT("Threshold"),false,ResourceColor);Button(C,ActionId::PreviewLevelUp,1766,304,100,32,TEXT("Level up"),false,ResourceColor);}
  else if(Nav.Section==6){Button(C,ActionId::Pulse,1454,304,196,32,TEXT("Preview pulse"),false,ResourceColor);}
  else C.Text(1454,314,TEXT("Select any preset to preview it immediately."),9,Muted);
  C.Text(1454,347,TEXT("PRESETS / CLICK TO PREVIEW"),10,ResourceColor,true);
  for(int I=PickerOffset;I<PresetNames.Num()&&I<PickerOffset+6;I++){int Cell=I-PickerOffset;float X=1454+(Cell%2)*206.f,Y=369+(Cell/2)*51.f;PresetTile(C,ActionId::PresetSlotBase+I,X,Y,196,45,PresetNames[I],PresetNames[I]==PresetName,Nav.Category);}
  float EditorTop=ResourceEditorTop();if(PresetNames.Num()>6){float PageY=EditorTop-43;Button(C,ActionId::PreviousPickerPage,1454,PageY,97,27,TEXT("< Previous"));Button(C,ActionId::NextPickerPage,1769,PageY,97,27,TEXT("Next >"));C.Text(1593,PageY+8,FString::Printf(TEXT("%d-%d / %d"),PickerOffset+1,FMath::Min(PickerOffset+6,PresetNames.Num()),PresetNames.Num()),9,Muted);}
  C.Seg(1454,EditorTop-11,1866,EditorTop-11,A(ResourceColor,.36f));KitControls(C,EditorTop,978);
  C.Text(1454,992,TEXT("Preview only / assign to a design under Data & links."),8,Cyan);
 }else if(Nav.SelectedLayer>=0){
  C.Gradient(1419,156,463,83,A(Cyan,.09f),A(Ink,0));DrawWorkbenchIcon(C,0,1435,169,25,Cyan);C.Text(1472,167,TEXT("DESIGN / LAYER"),17,White,true);C.Text(1472,199,FString(TEXT("Player Profile  /  "))+Variants[ActiveVariant].Name,9,Cyan,true);Button(C,ActionId::BackToProfile,1678,167,188,34,TEXT("< Profile inspector"));C.Seg(1433,227,1869,227,A(Cyan,.31f));
  static const TCHAR* PieceNames[]={TEXT("Angular frame"),TEXT("Glass surface"),TEXT("Portrait + mask"),TEXT("Player name"),TEXT("Level label"),TEXT("XP Bar / track + fill"),TEXT("Edge lighting"),TEXT("Motion overlays")};
  if(Nav.SelectedLayer==5){C.Cut(1454,282,412,38,Panel,Line,4);C.Text(1466,293,TEXT("XP BAR / LOCAL MINI COMPONENT"),11,Cyan,true);KitControls(C,345,847);Button(C,ActionId::ResetPartDefaults,1454,864,202,37,TEXT("Reset part defaults"));Button(C,ActionId::BarSolo,1666,864,200,37,SoloBar?TEXT("Show full profile"):TEXT("Solo preview"),SoloBar);C.Text(1454,925,TEXT("Reusable progress drawing for this design."),9,Muted);C.Text(1454,950,TEXT("Save component to keep local bar changes."),9,Cyan);}
  else{C.Text(1454,276,TEXT("SELECTED LAYER"),10,Cyan,true);C.Cut(1454,309,412,66,Panel,Line,4);C.Text(1470,328,PieceNames[Nav.SelectedLayer],14,White,true);Button(C,ActionId::LayerBase+Nav.SelectedLayer,1454,404,150,38,P.Layers[Nav.SelectedLayer]?TEXT("Visible: on"):TEXT("Visible: off"),P.Layers[Nav.SelectedLayer]);C.Text(1454,477,TEXT("Visibility belongs to this design variant."),10,Muted);C.Text(1454,506,TEXT("Visual recipes are edited under Global FX."),10,Muted);}
 }else{
  C.Gradient(1419,156,463,83,A(Cyan,.09f),A(Ink,0));DrawWorkbenchIcon(C,0,1435,168,27,Cyan);C.Text(1473,166,TEXT("PLAYER PROFILE"),18,White,true);C.Text(1473,201,ComponentDirty?TEXT("UNSAVED DESIGN / SAVE COMPONENT"):TEXT("DATA / LINKS / LOCAL DESIGN"),9,ComponentDirty?Red:Cyan,true);C.Seg(1433,227,1869,227,A(Cyan,.31f));
  Button(C,ActionId::InspectorContent,1434,239,207,36,TEXT("Data & links"),Nav.Tab==0);Button(C,ActionId::InspectorMotion,1651,239,215,36,TEXT("Design overrides"),Nav.Tab==1);
  if(Nav.Tab==0){DrawWorkbenchWell(C,1435,291,444,322,Cyan);DrawWorkbenchWell(C,1435,615,444,389,Cyan);Section(C,300,TEXT("PROFILE DATA"));Field(C,ActionId::ProfileName,348,TEXT("Player name"),D.Name);Field(C,ActionId::Level,396,TEXT("Level"),FString::FromInt(D.Level));Field(C,ActionId::CurrentXP,444,TEXT("Current XP"),FString::FromInt(D.XP));Field(C,ActionId::NextLevelXP,492,TEXT("Next level XP"),FString::FromInt(D.MaxXP));C.Text(1454,551,TEXT("Portrait"),12,Muted);C.Image(1614,545,62,61,Portraits[P.Portrait]);Button(C,ActionId::Portrait,1690,552,176,38,P.Portrait?TEXT("02 / Blue operator"):TEXT("01 / Golden portrait"));
   Section(C,620,TEXT("SHARED PRESETS / THIS DESIGN"));for(int Row=0;Row<6;Row++){int Cat=Row==5?6:Row;float Y=657+Row*47;C.Text(1454,Y,CategoryName(Cat),10,CategoryColor(Cat),true);Button(C,ActionId::LinkCategoryBase+Cat,1595,Y-6,271,32,Linked[Cat]+TEXT("  v"),false,CategoryColor(Cat),true);}
   C.Text(1454,954,TEXT("Choose a link; local overrides stay with this design."),9,Cyan);C.Text(1454,979,TEXT("Synthetic rewards never change saved profile data."),9,Muted);
  }else{
   C.Text(1454,295,TEXT("ONLY THIS DESIGN / SHARED PRESET UNCHANGED"),10,Cyan,true);
   Button(C,ActionId::CategoryBase,1454,324,128,31,TEXT("Entrance"),Nav.Category==0,CategoryColor(0));Button(C,ActionId::CategoryBase+1,1590,324,128,31,TEXT("Exit"),Nav.Category==1,CategoryColor(1));Button(C,ActionId::CategoryBase+2,1726,324,140,31,TEXT("Ambient"),Nav.Category==2,CategoryColor(2));
   Button(C,ActionId::CategoryBase+3,1454,361,128,31,TEXT("Reaction"),Nav.Category==3,CategoryColor(3));Button(C,ActionId::CategoryBase+4,1590,361,128,31,TEXT("Theme"),Nav.Category==4,CategoryColor(4));Button(C,ActionId::CategoryBase+6,1726,361,140,31,TEXT("Event"),Nav.Category==6,CategoryColor(6));
   KitControls(C,406,847);Button(C,ActionId::ResetCategory,1454,864,412,37,TEXT("Restore assigned preset values"));Button(C,ActionId::OpenKit,1454,914,412,37,TEXT("Edit global preset instead"));C.Text(1454,974,TEXT("Changes here override the assigned preset for this design."),9,Muted);C.Text(1454,997,TEXT("Save component to keep these overrides."),9,Cyan);
  }
 }
 C.Seg(30,1035,1890,1035,Line);C.Box(32,1052,5,5,Green);C.Text(46,1047,Notice,10,Muted);C.Text(1190,1047,TEXT("SPACE pause   E enter   X exit   F11 fullscreen"),9,Muted);C.Text(1700,1047,FString::Printf(TEXT("%dx%d / %.0f FPS"),int(Geo.GetLocalSize().X),int(Geo.GetLocalSize().Y),FrameSum>0?FrameCount/FrameSum:0),9,Cyan,true);
 DrawPresetPicker(C);
 return C.L;
}

