#pragma once
#include "WorkbenchCanvas.h"
namespace KW {
// Reusable presentation-only part. Its caller owns XP, levels and event semantics.
struct ProgressPartStyle {float Height=23,Inset=.16f,Glow=1,Sweep=1,Speed=1,Tip=1;};
inline TArray<FVector2D> BarShape(float X,float Y,float W,float H){float N=FMath::Min(H*.45f,W*.5f);return {{X+N,Y},{X+W-N,Y},{X+W,Y+H*.5f},{X+W-N,Y+H},{X+N,Y+H},{X,Y+H*.5f}};}
inline TArray<FVector2D> ClipBarX(const TArray<FVector2D>& Shape,float Edge){TArray<FVector2D> Out;for(int I=0;I<Shape.Num();I++){auto A=Shape[I],B=Shape[(I+1)%Shape.Num()];bool InA=A.X<=Edge,InB=B.X<=Edge;if(InA)Out.Add(A);if(InA!=InB){float T=(Edge-A.X)/(B.X-A.X);Out.Add(FMath::Lerp(A,B,T));}}return Out;}
inline void DrawProgressPart(Canvas& C,float X,float Y,float W,const ProgressPartStyle& P,float Value,float Time){
 float H=P.Height,Inset=H*P.Inset;auto Outer=BarShape(X,Y,W,H);C.Poly(Outer,FLinearColor(.004f,.01f,.015f));auto Outline=Outer;Outline.Add(Outer[0]);C.Path(Outline,A(Muted,.85f),1);
 auto Inner=BarShape(X+Inset,Y+Inset,W-Inset*2,H-Inset*2);float Cut=X+Inset+(W-2*Inset)*Sat(Value);float Tip=P.Tip*.65f;TArray<FVector2D> Tilted;for(auto V:Inner)Tilted.Add({V.X+(V.Y-Y-Inset)*Tip,V.Y});auto Fill=ClipBarX(Tilted,Cut+(Value>=.9999f?H*Tip:0));for(auto& V:Fill)V.X-=(V.Y-Y-Inset)*Tip;
 if(Fill.Num()<3)return;
 auto Border=Fill;Border.Add(Fill[0]);for(int I=5;I>0;I--)C.Path(Border,A(Green,P.Glow*.012f),I*3.f);
 C.PolyGradient(Fill,Y+Inset,H-2*Inset,FLinearColor(.38f,1,.69f),FLinearColor(.025f,.55f,.23f));
 float Sweep=FMath::Frac(Time*P.Speed*.17f);float BeamX=X+Sweep*W;auto Highlight=ClipBarX(Fill,BeamX);TArray<FVector2D> Slice;for(auto V:Highlight)Slice.Add({-V.X,V.Y});Slice=ClipBarX(Slice,-BeamX+W*.09f);for(auto& V:Slice)V.X=-V.X;if(Slice.Num()>2)C.PolyGradient(Slice,Y,H,A(White,.4f*P.Sweep),A(Green,0));
 C.Path(Border,A(Green,.45f),.8f);
}
}
