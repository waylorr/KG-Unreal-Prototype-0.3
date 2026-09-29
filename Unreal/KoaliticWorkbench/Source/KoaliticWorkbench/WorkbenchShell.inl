// Top-level destinations share one Unreal executable and one application shell.
void DrawShell(Canvas& C)const{
 C.Box(0,0,1920,1080,Ink);C.Gradient(0,0,1920,1080,FLinearColor(.012f,.036f,.052f),Ink);
 if(AtmosphereMaterial.IsValid())C.Image(0,0,1920,1080,AtmosphereBrush,A(White,.65f));
 for(int I=0;I<65;I++)C.Seg(0,I*29,1920,I*29,A(Cyan,.018f));
 for(int I=0;I<65;I++)C.Seg(I*30,0,I*30,1080,A(Cyan,.018f));
 C.Gradient(0,0,1920,161,FLinearColor(.016f,.054f,.075f),FLinearColor(.004f,.017f,.030f));
 C.Poly({{82,77},{105,50},{129,50},{106,77}},Red);C.Halo(106,64,30,24,Red,.18f);
 C.Text(150,43,TEXT("KOALITIC"),42,White,true);
 C.Text(84,112,TEXT("CREATE IMMERSIVE HUD EXPERIENCES"),12,Cyan,true);
 C.Seg(82,156,1838,156,A(Cyan,.48f));C.Seg(82,157,575,157,A(Cyan,.18f),2);
 Button(C,ActionId::ToggleSound,1607,73,133,42,SoundEnabled?TEXT("Sound On"):TEXT("Sound Off"),SoundEnabled);
 Button(C,ActionId::Close,1751,73,87,42,TEXT("Exit"));
 if(AppPage!=0){
  Button(C,ActionId::GoHome,82,196,155,43,TEXT("<  Main menu"));
  C.Text(81,300,AppPage==2?TEXT("EPISODES"):TEXT("EDITOR"),49,White,true);
  C.Text(84,377,TEXT("WORKSPACE RESERVED"),17,Cyan,true);
  C.Seg(84,428,1320,428,A(Cyan,.5f));
  C.Text(84,461,AppPage==2?TEXT("Episode assets and rendered projects will live here."):TEXT("Video POV and component keyframes will live here."),18,Muted);
  C.Text(84,510,TEXT("This area has no editing or export controls yet."),13,Muted);
  DrawWorkbenchPanel(C,80,598,1760,278,Cyan,true);DrawWorkbenchFascia(C,80,598,1760,68,Cyan);
  DrawWorkbenchIcon(C,AppPage==2?0:2,1714,621,56,A(Cyan,.7f));
  C.Text(112,635,TEXT("ONE APPLICATION  /  CONNECTED WORKSPACES"),12,Cyan,true);
  C.Text(112,676,TEXT("UI Workbench creates component presentations and reusable resources."),20,White);
  C.Text(112,722,TEXT("Other workspaces will use those components when their workflows are defined."),15,Muted);
  Button(C,ActionId::GoWorkbench,112,792,264,48,TEXT("Open UI Workbench  >"),true);
 }else{
  C.Text(82,216,TEXT("YOUR CREATIVE SYSTEM"),14,Cyan,true);
  C.Text(79,254,TEXT("Choose a workspace."),45,White,true);
  C.Text(83,328,TEXT("Design components now. The other workspaces are reserved for the next stages."),17,Muted);
  struct FCard{int Id;float X;const TCHAR* Number;const TCHAR* Title;const TCHAR* Detail;const TCHAR* Foot;};
  const FCard Cards[]={
   {ActionId::GoWorkbench,82,TEXT("01"),TEXT("UI WORKBENCH"),TEXT("Design and test UI components"),TEXT("AVAILABLE  /  PLAYER PROFILE")},
   {ActionId::GoEpisodes,674,TEXT("02"),TEXT("EPISODES"),TEXT("Organize episodes and their assets"),TEXT("WORKSPACE RESERVED")},
   {ActionId::GoEditor,1266,TEXT("03"),TEXT("EDITOR"),TEXT("Video, component states and keyframes"),TEXT("WORKSPACE RESERVED")}};
  bool Unsaved=ComponentDirty;for(bool Dirty:KitDirty)Unsaved|=Dirty;
  for(const auto& Card:Cards){bool Ready=Card.Id==ActionId::GoWorkbench,Hot=Hover==Card.Id;
   FLinearColor CardAccent=Ready?Cyan:Card.Id==ActionId::GoEpisodes?FLinearColor(.64f,.43f,1.f):FLinearColor(.25f,1.f,.72f);
   DrawWorkbenchPanel(C,Card.X,414,564,443,CardAccent,Ready||Hot);DrawWorkbenchFascia(C,Card.X,414,564,116,CardAccent);
   C.Box(Card.X+24,444,4,66,CardAccent);C.Text(Card.X+43,439,Card.Number,43,CardAccent,true);
   DrawWorkbenchIcon(C,Card.Id==ActionId::GoWorkbench?0:Card.Id==ActionId::GoEpisodes?5:2,Card.X+470,449,47,CardAccent);
   C.Text(Card.X+30,552,Card.Title,24,White,true);C.Seg(Card.X+30,612,Card.X+532,612,Line);
   C.Text(Card.X+30,646,Card.Detail,14,Muted);C.Text(Card.X+30,772,Ready&&Unsaved?TEXT("UNSAVED CHANGES / OPEN WORKBENCH"):Card.Foot,10,Ready&&Unsaved?Red:Ready?Green:Muted,true);
   C.Text(Card.X+515,772,TEXT(">"),18,Ready?Cyan:Muted,true);
   Region(Card.Id,Card.X,414,564,443);
  }
 }
 C.Seg(82,1007,1838,1007,Line);C.Text(84,1024,TEXT("UNREAL ENGINE 5.8  /  KOALITIC WORKSPACE"),10,Muted,true);
 C.Text(1533,1024,TEXT("ONE PROJECT  /  MODULAR SPACES"),10,Cyan,true);
}

void DrawPresetPicker(Canvas& C)const{
 if(!PresetPicker)return;
 // Modal hit target sits below the list so clicking anywhere else dismisses it.
 C.Box(0,0,1920,1080,FLinearColor(0,0,0,.40f));Region(ActionId::DismissPresetPicker,0,0,1920,1080);
 float Top=Nav.KitScope?288.f:(Nav.Tab==0?325.f:378.f);int Visible=FMath::Min(6,PresetNames.Num()-PickerOffset);
 float Height=99+Visible*42;DrawWorkbenchPanel(C,1438,Top,438,Height,CategoryColor(Nav.Category),true);
 C.Text(1455,Top+17,TEXT("SELECT SAVED PRESET"),12,White,true);
 C.Text(1455,Top+39,CategoryName(Nav.Category),10,Cyan,true);
 for(int Row=0;Row<Visible;Row++){int Index=PickerOffset+Row;const FString& Name=PresetNames[Index];bool Selected=Nav.KitScope?Name==PresetName:Name==Linked[Nav.Category];Button(C,ActionId::PresetSlotBase+Index,1454,Top+60+Row*42,404,35,Name,Selected);}
 int Bottom=Top+63+Visible*42;
 Button(C,ActionId::PreviousPickerPage,1454,Bottom,94,30,TEXT("<  Up"));
 C.Text(1568,Bottom+8,FString::Printf(TEXT("%d-%d / %d"),PickerOffset+1,PickerOffset+Visible,PresetNames.Num()),10,Muted);
 Button(C,ActionId::NextPickerPage,1698,Bottom,76,30,TEXT("Down"));
 Button(C,ActionId::DismissPresetPicker,1782,Bottom,76,30,TEXT("Close"));
}
