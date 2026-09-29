#pragma once
#include "WorkbenchCanvas.h"

namespace KW {
// Application chrome only. Component renderers deliberately do not use this skin.
inline void DrawWorkbenchPanel(Canvas& C,float X,float Y,float W,float H,FLinearColor Accent=Cyan,bool Emphasis=false){
 const float N=9;TArray<FVector2D> Shape={{X+N,Y},{X+W-N,Y},{X+W,Y+N},{X+W,Y+H-N},{X+W-N,Y+H},{X+N,Y+H},{X,Y+H-N},{X,Y+N}};
 C.Cut(X-2,Y+4,W+4,H+2,FLinearColor(.001f,.004f,.009f,.66f),A(Accent,.07f),N+1);
 C.PolyGradient(Shape,Y,H,Emphasis?FLinearColor(.006f,.023f,.040f):FLinearColor(.004f,.015f,.027f),FLinearColor(.001f,.004f,.010f));
 auto Border=Shape;Border.Add(Shape[0]);C.Path(Border,A(Accent,Emphasis?.72f:.43f),1.15f);
 C.Seg(X+N+2,Y+1,X+W-N-2,Y+1,A(White,.24f));
 C.Seg(X+N+2,Y+2,X+FMath::Min(W*.48f,280.f),Y+2,A(Accent,Emphasis?.81f:.35f),1.5f);
 C.Seg(X+N+5,Y+H-2,X+W-N-5,Y+H-2,A(Accent,.10f));
 C.Seg(X+3,Y+N+5,X+3,Y+FMath::Min(H*.31f,136.f),A(Accent,.25f));
 C.Seg(X+W-3,Y+H-FMath::Min(H*.24f,90.f),X+W-3,Y+H-N-4,A(Accent,.29f));
 C.Seg(X+16,Y+15,X+FMath::Min(W*.24f,128.f),Y+15,A(White,.055f));
 C.Box(X+10,Y+H-3,FMath::Min(51.f,W*.13f),1,A(Accent,.55f));
 C.Box(X+W-34,Y+H-3,24,1,A(White,.15f));
 C.Seg(X+N,Y+N+8,X+N,Y+N+27,A(Accent,.18f));
 C.Seg(X+W-N,Y+H-N-28,X+W-N,Y+H-N-9,A(Accent,.19f));
 if(Emphasis){C.Halo(X+W-14,Y+9,34,14,Accent,.22f);C.Halo(X+12,Y+H-8,29,10,Accent,.11f);C.Glow(X+8,Y+5,W-16,H-10,Accent,.036f);}
}
inline void DrawWorkbenchFascia(Canvas& C,float X,float Y,float W,float H,FLinearColor Accent){
 C.Gradient(X+8,Y+7,W-16,H,FLinearColor(.015f,.048f,.073f,.86f),FLinearColor(.001f,.007f,.019f,0));
 C.Gradient(X+8,Y+7,W-16,18,A(White,.060f),A(Ink,0));
 C.Seg(X+14,Y+H+6,X+W-14,Y+H+6,A(Accent,.35f));
 C.Seg(X+14,Y+H+7,X+FMath::Min(W*.42f,210.f),Y+H+7,A(Accent,.24f));
 C.Box(X+18,Y+19,3,15,Accent);C.Halo(X+19.5f,Y+26,11,18,Accent,.16f);
}
inline void DrawWorkbenchEye(Canvas& C,float X,float Y,FLinearColor Color){
 C.Path({{X,Y+6},{X+7,Y+1},{X+14,Y+6},{X+7,Y+11},{X,Y+6}},Color,1.35f);C.Halo(X+7,Y+6,2.5f,2.5f,Color,.75f);
}
inline void DrawWorkbenchGrip(Canvas& C,float X,float Y,FLinearColor Color){for(int Row=0;Row<3;Row++)for(int Col=0;Col<2;Col++)C.Box(X+Col*4,Y+Row*5,2,2,A(Color,.75f));}
inline void DrawWorkbenchWell(Canvas& C,float X,float Y,float W,float H,FLinearColor Accent){
 C.Cut(X,Y,W,H,FLinearColor(.001f,.007f,.016f,.94f),A(Accent,.30f),4);
 C.Gradient(X+4,Y+4,W-8,FMath::Min(22.f,H*.4f),A(Accent,.07f),A(Ink,0));
 C.Seg(X+7,Y+2,X+FMath::Min(W*.45f,130.f),Y+2,A(Accent,.24f));
 C.Box(X+2,Y+11,2,FMath::Min(16.f,H-18),A(Accent,.7f));
}
inline void DrawWorkbenchIcon(Canvas& C,int Kind,float X,float Y,float S,FLinearColor Color){
 const float W=FMath::Max(1.f,S*.075f);auto P=[&](float DX,float DY){return FVector2D(X+DX*S,Y+DY*S);};
 switch(Kind){
  case 0: C.Path({P(.5,.04),P(.87,.25),P(.87,.74),P(.5,.96),P(.13,.74),P(.13,.25),P(.5,.04)},Color,W);C.Path({P(.13,.25),P(.5,.48),P(.87,.25)},Color,W);C.Seg(X+.5f*S,Y+.48f*S,X+.5f*S,Y+.96f*S,Color,W);break;
  case 1: {TArray<FVector2D> Ring;for(int I=0;I<=16;I++){float T=I*2*PI/16;Ring.Add(P(.5f+.40f*FMath::Cos(T),.5f+.40f*FMath::Sin(T)));}C.Path(Ring,Color,W);C.Box(X+.28f*S,Y+.25f*S,S*.09f,S*.09f,Color);C.Box(X+.57f*S,Y+.20f*S,S*.09f,S*.09f,Color);C.Box(X+.73f*S,Y+.47f*S,S*.09f,S*.09f,Color);C.Halo(X+.42f*S,Y+.67f*S,S*.14f,S*.14f,Color,.35f);break;}
  case 2:C.Path({P(.04,.55),P(.22,.55),P(.34,.25),P(.45,.82),P(.58,.13),P(.70,.72),P(.80,.48),P(.96,.48)},Color,W);break;
  case 3:C.Path({P(.22,.52),P(.43,.18),P(.53,.60),P(.77,.22)},Color,W);C.Path({P(.22,.52),P(.46,.82),P(.77,.22)},Color,W);C.Seg(X+.16f*S,Y+.53f*S,X+.75f*S,Y+.83f*S,A(Color,.64f),W*.7f);break;
  case 4:for(int I=0;I<8;I++){float T=I*PI/4;C.Seg(X+S*(.5f+.25f*FMath::Cos(T)),Y+S*(.5f+.25f*FMath::Sin(T)),X+S*(.5f+.45f*FMath::Cos(T)),Y+S*(.5f+.45f*FMath::Sin(T)),Color,W);}C.Halo(X+.5f*S,Y+.5f*S,S*.17f,S*.17f,Color,.75f);break;
  default:C.Path({P(.5,.05),P(.88,.27),P(.88,.73),P(.5,.95),P(.12,.73),P(.12,.27),P(.5,.05)},Color,W);break;
 }
}
inline void DrawWorkbenchGrid(Canvas& C,float X,float Y,float W,float H){
 C.Gradient(X,Y,W,H,FLinearColor(.002f,.010f,.020f),FLinearColor(.0007f,.003f,.008f));
 for(float GX=X+13;GX<X+W;GX+=32)C.Seg(GX,Y+2,GX,Y+H-2,A(Cyan,.039f));
 for(float GY=Y+15;GY<Y+H;GY+=32)C.Seg(X+2,GY,X+W-2,GY,A(Cyan,.039f));
 C.Gradient(X,Y,W,H*.35f,FLinearColor(.013f,.045f,.064f,.45f),FLinearColor(.003f,.015f,.026f,0));
 for(int I=0;I<5;I++){float P=I*15.f;C.Seg(X+16+P,Y+99,X+92+P,Y+23,A(Cyan,.055f));C.Seg(X+W-170+P,Y+H-17,X+W-118+P,Y+H-69,A(Cyan,.043f));}
 C.Seg(X+W-102,Y+24,X+W-32,Y+24,A(Cyan,.10f));C.Seg(X+W-102,Y+28,X+W-55,Y+28,A(Cyan,.10f));
 C.Text(X+W-97,Y+37,TEXT("DESIGN"),7,A(Cyan,.38f),true);C.Text(X+W-97,Y+49,TEXT("ANIMATE"),7,A(Cyan,.27f),true);
 C.Seg(X+14,Y+14,X+38,Y+14,A(Cyan,.44f));C.Seg(X+14,Y+14,X+14,Y+38,A(Cyan,.44f));
 C.Seg(X+W-14,Y+H-14,X+W-38,Y+H-14,A(Cyan,.44f));C.Seg(X+W-14,Y+H-14,X+W-14,Y+H-38,A(Cyan,.44f));
}
}
