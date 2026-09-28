int32 OnPaint(const FPaintArgs&,const FGeometry& Geo,const FSlateRect&,FSlateWindowElementList& E,int32 Layer,const FWidgetStyle&,bool)const override{
 float Scale=FMath::Min(Geo.GetLocalSize().X/1920.f,Geo.GetLocalSize().Y/1080.f);
 FGeometry G=Geo.MakeChild(FVector2D(1920,1080),FSlateLayoutTransform(Scale,(Geo.GetLocalSize()-FVector2D(1920,1080)*Scale)*.5f));Canvas C{G,E,Layer};Hits.Empty();
 C.Box(0,0,1920,1080,Ink);C.Gradient(0,0,1920,100,FLinearColor(.013f,.035f,.047f),Ink);
 C.Poly({{30,33},{44,17},{54,17},{40,33}},Red);C.Text(66,15,TEXT("KOALITIC"),25,White,true);C.Text(32,55,TEXT("C R E A T E  I M M E R S I V E  H U D  E X P E R I E N C E S"),7,Muted);
 C.Seg(267,20,267,66,Line);C.Text(293,17,TEXT("UI WORKBENCH"),21,White,true);C.Text(294,53,TEXT("BUILD   /   PREVIEW   /   REFINE"),9,Cyan,true);
 Button(C,60,718,21,178,40,TEXT("01  Components"),!KitScope);Button(C,61,906,21,178,40,TEXT("02  UI Kit"),KitScope);
 Button(C,78,1120,21,100,39,TEXT("Undo"));Button(C,79,1230,21,100,39,TEXT("Redo"));
 Button(C,2,1460,21,116,39,TEXT("Reload"));Button(C,1,1586,21,187,39,TEXT("Save component"),true);Button(C,3,1783,21,105,39,TEXT("Close"));
 C.Seg(30,89,1890,89,Line);C.Text(32,111,KitScope?TEXT("SHARED RESOURCES"):TEXT("COMPONENT LIBRARY"),12,Cyan,true);
 C.Text(285,111,KitScope?TEXT("UI KIT / LIVE MATERIAL LAB"):TEXT("PLAYER PROFILE / LIVE PREVIEW"),12,Cyan,true);C.Text(1430,111,KitScope?TEXT("GLOBAL PRESET EDITOR"):TEXT("COMPONENT INSPECTOR"),12,Cyan,true);
 C.Cut(30,150,225,869,Panel,Line,6);
 if(!KitScope){
 C.Text(48,172,TEXT("COMPONENTS"),9,Muted,true);C.Cut(44,203,197,67,FLinearColor(.012f,.09f,.135f),A(Cyan,.8f),5);C.Text(59,216,TEXT("Player Profile"),15,White,true);C.Text(60,246,TEXT("PLAYER  /  LEVEL  /  XP"),8,Cyan,true);C.Text(49,290,TEXT("1 component  /  8 layers"),10,Muted);C.Seg(48,327,238,327,Line);
 C.Text(48,348,TEXT("EDITABLE LAYERS"),10,White,true);C.Text(48,372,TEXT("Click to show / hide"),9,Muted);
 const TCHAR* Layers[]={TEXT("Angular frame"),TEXT("Glass surface"),TEXT("Portrait + mask"),TEXT("Player name"),TEXT("Level label"),TEXT("XP track + fill"),TEXT("Edge lighting"),TEXT("Motion overlays")};
 for(int I=0;I<8;I++){float Y=411+I*43;C.Box(49,Y+2,10,10,P.Layers[I]?A(Cyan,.8f):Line);C.Text(71,Y-2,Layers[I],11,P.Layers[I]?White:Muted);Region(200+I,44,Y-8,197,35);}
 C.Seg(48,780,238,780,Line);C.Text(49,802,TEXT("LINKED TO UI KIT"),9,Cyan,true);C.Text(49,834,TEXT("Shared motion + FX"),11,White);C.Text(49,863,TEXT("Local values stay yours."),9,Muted);Button(C,81,44,905,197,35,SoloBar?TEXT("Exit bar solo"):TEXT("Edit XP bar / Solo"),SoloBar);
 }else{
 C.Text(48,172,TEXT("RESOURCE CATEGORIES"),9,Muted,true);
 C.Text(49,204,TEXT("MOTION FX"),10,Cyan,true);Button(C,70,58,228,183,33,TEXT("Entrance"),Category==0);Button(C,71,58,270,183,33,TEXT("Exit"),Category==1);for(int Cat=2;Cat<6;Cat++)Button(C,70+Cat,44,321+(Cat-2)*39,197,32,CategoryName(Cat),Category==Cat);
 C.Seg(48,479,238,479,Line);C.Text(49,500,TEXT("SAVED PRESETS"),10,White,true);C.Text(49,526,TEXT("Select to audition"),9,Muted);
 for(int I=PresetPage*6;I<FMath::Min(PresetPage*6+6,PresetNames.Num());I++)Button(C,600+I,44,561+(I-PresetPage*6)*44,197,35,PresetNames[I],PresetName==PresetNames[I]);if(PresetNames.Num()>6){Button(C,67,44,834,91,32,TEXT("Previous"));Button(C,68,145,834,96,32,TEXT("Next"));}
 C.Text(49,909,TEXT("EDIT SHARED VALUES"),9,Cyan,true);C.Text(49,936,TEXT("Save resource. Then Apply."),9,Muted);C.Text(49,960,TEXT("Preview ignores overrides."),9,Muted);
 }
 C.Box(49,990,5,5,Green);C.Text(62,985,TEXT("UNREAL 5.8 / NATIVE UI"),8,Muted);
 C.Cut(277,150,1118,555,Panel,Line,6);C.Text(301,171,TEXT("PLAYER PROFILE"),17,White,true);C.Text(301,204,KitScope?FString(TEXT("Shared resource / "))+CategoryName(Category):TEXT("Obsidian glass / independent live layers"),10,Muted);
 Button(C,30,1089,171,131,33,TEXT("Expanded"),!State.Compact);Button(C,31,1230,171,139,33,TEXT("Compact"),State.Compact);
 C.Box(294,241,1084,441,FLinearColor(.005f,.013f,.021f));C.Gradient(294,241,1084,441,FLinearColor(.008f,.022f,.032f),Ink);
 for(int I=0;I<37;I++)C.Seg(296+I*30,244,296+I*30,678,A(Cyan,.022f));for(int I=0;I<15;I++)C.Seg(295,245+I*30,1376,245+I*30,A(Cyan,.022f));
 C.Seg(321,266,341,266,Muted);C.Seg(321,266,321,286,Muted);C.Seg(1351,657,1331,657,Muted);C.Seg(1351,657,1351,637,Muted);
 C.Text(325,627,TEXT("PROFILE_001  /  LAYERED GLASS + ENERGY"),8,Muted,true);C.Text(1165,627,K.Playing?TEXT("AMBIENT / LIVE"):TEXT("PREVIEW / PAUSED"),8,K.Playing?Cyan:Muted,true);
 Presentation Preview=PreviewValues();C.Clip(294,241,1084,441);if(SoloBar&&!KitScope){ProgressPartStyle BS;BS.Height=38*P.BarHeight;BS.Inset=P.BarInset;BS.Tip=P.BarTip;BS.Glow=P.Glow*P.BarGlow;BS.Speed=P.Speed*P.BarSpeed;BS.Sweep=P.Ambient&&!State.Disabled?P.Sweep*P.AmbientStrength:0;DrawProgressPart(C,377,420,900,BS,Evaluate(D,P,K,Events).Fill,K.Time);C.Text(378,495,TEXT("PROGRESS BAR / DATA-AGNOSTIC REUSABLE PART"),12,Cyan,true);}else DrawProfile(C,337,333+62*State.Layout(K.Time),998,D,Preview,State,K,Events,Portraits[P.Portrait],false,GlassMaterial.IsValid()?&GlassBrush:nullptr);C.Unclip();
 C.Cut(277,717,1118,81,Panel,Line,6);C.Text(296,729,TEXT("MOTION / ONE-SHOT PREVIEW"),8,Muted,true);
 Button(C,22,294,752,117,32,TEXT("Replay enter"));Button(C,23,421,752,117,32,TEXT("Preview exit"));Button(C,62,548,752,117,32,TEXT("Pulse"));
 C.Text(697,732,TEXT("Actions return to the selected layout."),10,White);C.Text(697,757,TEXT("Ambient continues. No sequence to manage."),10,Muted);
 Button(C,20,1132,740,123,37,K.Playing?TEXT("Pause preview"):TEXT("Resume"),!K.Playing);Button(C,24,1265,740,108,37,TEXT("Reset test"));
 C.Cut(277,810,389,209,Panel,Line,6);C.Text(297,829,TEXT("TEST SESSION / EVENTS"),13,Cyan,true);C.Text(298,858,TEXT("Synthetic XP / one event, one reaction"),9,Muted);
 Button(C,40,297,890,103,40,TEXT("+ 50 XP"));Button(C,41,409,890,107,40,TEXT("+ 100 XP"));Button(C,63,525,890,119,40,TEXT("+ 150 XP"),true);
 Button(C,42,297,942,164,35,TEXT("Near threshold"));Button(C,43,475,942,169,35,TEXT("Clear rewards"));C.Text(298,994,TEXT("Threshold crossing adds a level; overflow carries."),8,Muted);
 C.Cut(678,810,717,209,Panel,Line,6);C.Text(698,829,TEXT("LAYOUT STUDIES"),13,Cyan,true);
 Button(C,32,1010,824,115,29,State.Pinned?TEXT("Pinned: on"):TEXT("Pinned: off"),State.Pinned);Button(C,33,1137,824,119,29,State.Disabled?TEXT("Disabled: on"):TEXT("Disabled: off"),State.Disabled);Button(C,34,1268,824,105,29,State.AutoCollapse?TEXT("Hover: on"):TEXT("Hover: off"),State.AutoCollapse);
 ProfileState AState;Playback Static=K;DrawProfile(C,700,885,315,D,Preview,AState,Static,Events,Portraits[P.Portrait],true);AState.Compact=true;DrawProfile(C,1050,889,315,D,Preview,AState,Static,Events,Portraits[P.Portrait],true);
 C.Text(704,983,TEXT("01 / EXPANDED"),8,!State.Compact?Cyan:Muted,true);C.Text(1053,983,TEXT("02 / COMPACT"),8,State.Compact?Cyan:Muted,true);Region(30,697,871,330,139);Region(31,1044,871,334,139);
 C.Cut(1412,150,478,869,Panel,Line,6);
 C.Text(1434,170,KitScope?CategoryName(Category):TEXT("PLAYER PROFILE"),17,White,true);C.Text(1434,202,KitScope?TEXT("Audition draft / independent of local overrides"):TEXT("Content, assignments and local overrides"),10,Muted);
 if(KitScope){
 C.Text(1454,249,FString(TEXT("EDITING / "))+PresetName,12,Cyan,true);C.Text(1454,278,KitDirty[Category]?TEXT("Unsaved shared changes"):TEXT("Shared resource / ready"),10,KitDirty[Category]?Red:Muted);
 KitControls(C,326);Section(C,809,TEXT("SAVE SHARED PRESET"));Field(C,104,850,TEXT("Preset name"),PresetName);
 Button(C,64,1454,907,130,39,TEXT("Save preset"),true);Button(C,80,1594,907,130,39,TEXT("Save as..."));Button(C,69,1734,907,132,39,TEXT("Apply"));Button(C,65,1454,958,130,30,TEXT("Reload preset"));C.Text(1594,966,TEXT("Apply keeps local overrides."),9,Cyan);
 }else{
 Button(C,10,1433,239,141,36,TEXT("Content"),Tab==0);Button(C,11,1580,239,140,36,TEXT("Motion"),Tab==1);Button(C,12,1726,239,141,36,TEXT("Style / FX"),Tab==2);
 if(Tab==0){Section(C,300,TEXT("PROFILE DATA"));Field(C,100,348,TEXT("Player name"),D.Name);Field(C,101,396,TEXT("Level"),FString::FromInt(D.Level));Field(C,102,444,TEXT("Current XP"),FString::FromInt(D.XP));Field(C,103,492,TEXT("Next level XP"),FString::FromInt(D.MaxXP));C.Text(1454,551,TEXT("Portrait"),12,Muted);C.Image(1614,545,62,61,Portraits[P.Portrait]);Button(C,50,1690,552,176,38,P.Portrait?TEXT("02 / Blue operator"):TEXT("01 / Golden portrait"));
 Section(C,636,TEXT("SHARED RESOURCE LINKS"));for(int Cat=0;Cat<6;Cat++){float Y=683+Cat*43;C.Text(1454,Y,CategoryName(Cat),10,Muted);C.Text(1595,Y,Linked[Cat],11,Cyan);}
 Button(C,61,1454,923,412,39,TEXT("Open UI Kit / edit shared presets"));C.Text(1454,980,TEXT("Saved profile data excludes synthetic rewards."),9,Muted);
 }else{
 if(Tab==2){Button(C,74,1434,294,204,31,TEXT("Appearance"),Category==4);Button(C,75,1648,294,218,31,TEXT("Progress Bar"),Category==5);}
 if(Tab==1){for(int Cat=0;Cat<4;Cat++)Button(C,70+Cat,1434+Cat*110,294,104,31,CategoryName(Cat),Category==Cat);}
 C.Text(1454,344,FString(TEXT("LINK / "))+Linked[Category],11,Cyan,true);KitControls(C,382);
 Button(C,66,1454,864,412,37,TEXT("Reset this category to shared values"));Button(C,61,1454,914,412,37,TEXT("Edit shared preset in UI Kit"));C.Text(1454,974,TEXT("Moving a slider creates a local override."),10,Muted);C.Text(1454,997,TEXT("Reset restores that value's live link."),9,Cyan);
 }
 }
 C.Seg(30,1035,1890,1035,Line);C.Box(32,1052,5,5,Green);C.Text(46,1047,Notice,10,Muted);C.Text(1190,1047,TEXT("SPACE pause   E enter   X exit   F11 fullscreen"),9,Muted);C.Text(1700,1047,FString::Printf(TEXT("%dx%d / %.0f FPS"),int(Geo.GetLocalSize().X),int(Geo.GetLocalSize().Y),FrameSum>0?FrameCount/FrameSum:0),9,Cyan,true);
 return C.L;
}

