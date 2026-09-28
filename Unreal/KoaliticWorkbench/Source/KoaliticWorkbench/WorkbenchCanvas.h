#pragma once
#include "CoreMinimal.h"
#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"
#include "Fonts/CompositeFont.h"
#include "Misc/Paths.h"
namespace KW {
inline const FLinearColor Ink(.004f,.009f,.015f), Panel(.008f,.019f,.029f), Line(.045f,.13f,.18f), Cyan(.05f,.75f,1.f), White(.80f,.90f,.98f), Muted(.26f,.42f,.53f), Red(1.f,.025f,.10f), Green(.12f,1.f,.46f);
inline FLinearColor A(FLinearColor C,float V){C.A*=V;return C;}
inline float Sat(float V){return FMath::Clamp(V,0.f,1.f);}
inline float Ease(float V){V=Sat(V);return 1-FMath::Pow(1-V,3);}
struct Canvas {
 const FGeometry& G; FSlateWindowElementList& E; int L; float Alpha=1;FLinearColor Tint=FLinearColor::White;
 static TSharedPtr<const FCompositeFont> Font(bool Display){static auto D=MakeShared<FCompositeFont>(FName("Regular"),FPaths::ProjectContentDir()/TEXT("Interface/Display.ttf"),EFontHinting::Default,EFontLoadingPolicy::LazyLoad);static auto B=MakeShared<FCompositeFont>(FName("Regular"),FPaths::ProjectContentDir()/TEXT("Interface/Body.ttf"),EFontHinting::Default,EFontLoadingPolicy::LazyLoad);return Display?D:B;}
 FPaintGeometry PG(float X,float Y,float W=1,float H=1)const{return G.ToPaintGeometry(FVector2D(W,H),FSlateLayoutTransform(FVector2D(X,Y)));}
 void Box(float X,float Y,float W,float H,FLinearColor C){FSlateDrawElement::MakeBox(E,L++,PG(X,Y,W,H),FCoreStyle::Get().GetBrush("WhiteBrush"),ESlateDrawEffect::None,A(C*Tint,Alpha));}
 void Text(float X,float Y,const FString& S,int Size,FLinearColor C=White,bool Display=false){FSlateDrawElement::MakeText(E,L++,PG(X,Y),S,FSlateFontInfo(Font(Display),Size),ESlateDrawEffect::None,A(C*Tint,Alpha));}
 void Path(TArray<FVector2D>P,FLinearColor C,float W=1){FSlateDrawElement::MakeLines(E,L++,G.ToPaintGeometry(),P,ESlateDrawEffect::None,A(C*Tint,Alpha),true,W);}
 void Seg(float X,float Y,float U,float V,FLinearColor C,float W=1){Path({{X,Y},{U,V}},C,W);}
 void Poly(const TArray<FVector2D>& P,FLinearColor C){TArray<FSlateVertex>V;TArray<SlateIndex>I;for(auto Q:P)V.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(G.GetAccumulatedRenderTransform(),FVector2f(Q),FVector2f(0,0),A(C*Tint,Alpha).ToFColor(true)));for(int N=1;N<P.Num()-1;N++){I.Add(0);I.Add(N);I.Add(N+1);}FSlateDrawElement::MakeCustomVerts(E,L++,FSlateResourceHandle(),V,I,nullptr,0,0);}
 void PolyGradient(const TArray<FVector2D>& P,float Y,float H,FLinearColor Top,FLinearColor Bottom){TArray<FSlateVertex>V;TArray<SlateIndex>I;for(auto Q:P){FLinearColor Col=FMath::Lerp(Top,Bottom,Sat((Q.Y-Y)/H));V.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(G.GetAccumulatedRenderTransform(),FVector2f(Q),FVector2f(0,0),A(Col*Tint,Alpha).ToFColor(true)));}for(int N=1;N<P.Num()-1;N++){I.Add(0);I.Add(N);I.Add(N+1);}FSlateDrawElement::MakeCustomVerts(E,L++,FSlateResourceHandle(),V,I,nullptr,0,0);}
 void Gradient(float X,float Y,float W,float H,FLinearColor C,FLinearColor D,bool Horizontal=false){TArray<FSlateGradientStop>S;S.Emplace(FVector2D(0,0),A(C*Tint,Alpha));S.Emplace(FVector2D(Horizontal?W:0,Horizontal?0:H),A(D*Tint,Alpha));FSlateDrawElement::MakeGradient(E,L++,PG(X,Y,W,H),S,Horizontal?Orient_Vertical:Orient_Horizontal);}
 void Cut(float X,float Y,float W,float H,FLinearColor C,FLinearColor Edge,float N=8){TArray<FVector2D>P={{X+N,Y},{X+W-N,Y},{X+W,Y+N},{X+W,Y+H-N},{X+W-N,Y+H},{X+N,Y+H},{X,Y+H-N},{X,Y+N}};Poly(P,C);const FVector2D First=P[0];P.Add(First);Path(P,Edge);}
 void Image(float X,float Y,float W,float H,const FSlateBrush& B,FLinearColor ImageTint=FLinearColor::White){FSlateDrawElement::MakeBox(E,L++,PG(X,Y,W,H),&B,ESlateDrawEffect::None,A(ImageTint*Tint,Alpha));}
 void Halo(float X,float Y,float RX,float RY,FLinearColor Col,float Strength){
  TArray<FSlateVertex> V;TArray<SlateIndex> Ind;V.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(G.GetAccumulatedRenderTransform(),FVector2f(X,Y),FVector2f(0,0),A(Col*Tint,Alpha*Strength).ToFColor(true)));
  for(int I=0;I<=24;I++){float Angle=I*2*PI/24;V.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(G.GetAccumulatedRenderTransform(),FVector2f(X+FMath::Cos(Angle)*RX,Y+FMath::Sin(Angle)*RY),FVector2f(0,0),A(Col,0).ToFColor(true)));if(I>0){Ind.Add(0);Ind.Add(I);Ind.Add(I+1);}}
  FSlateDrawElement::MakeCustomVerts(E,L++,FSlateResourceHandle(),V,Ind,nullptr,0,0);
 }
 void Glow(float X,float Y,float W,float H,FLinearColor Col,float Strength){
  // Smooth rounded rectangular radiance, not stacked opaque rectangles.
  TArray<FSlateVertex> V;TArray<SlateIndex> Ind;const int N=32,Rings=6;
  V.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(G.GetAccumulatedRenderTransform(),FVector2f(X+W/2,Y+H/2),FVector2f(0,0),A(Col*Tint,Alpha*Strength*.18f).ToFColor(true)));
  for(int R=0;R<Rings;R++){float Base=FMath::Min(2.f,FMath::Min(W,H)*.5f);float Pad=R*4.f,Radius=Base+Pad;float Op=R==Rings-1?0:.18f*FMath::Exp(-R*R*.19f)*Strength;
   for(int I=0;I<N;I++){int Corner=I/8;float Angle=(-PI/2)+Corner*PI/2+(I%8)*PI/14;float CX=(Corner==0||Corner==1)?X+W-Base:X+Base;float CY=(Corner==1||Corner==2)?Y+H-Base:Y+Base;
    V.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(G.GetAccumulatedRenderTransform(),FVector2f(CX+FMath::Cos(Angle)*Radius,CY+FMath::Sin(Angle)*Radius),FVector2f(0,0),A(Col*Tint,Alpha*Op).ToFColor(true)));
   }
  }
  for(int I=0;I<N;I++){Ind.Add(0);Ind.Add(1+I);Ind.Add(1+(I+1)%N);}
  for(int R=0;R<Rings-1;R++)for(int I=0;I<N;I++){int A0=1+R*N+I,B0=1+R*N+(I+1)%N,C0=A0+N,D0=B0+N;Ind.Append({(SlateIndex)A0,(SlateIndex)C0,(SlateIndex)B0,(SlateIndex)B0,(SlateIndex)C0,(SlateIndex)D0});}
  FSlateDrawElement::MakeCustomVerts(E,L++,FSlateResourceHandle(),V,Ind,nullptr,0,0);
 }
 void Clip(float X,float Y,float W,float H){E.PushClip(FSlateClippingZone(G.MakeChild(FVector2D(W,H),FSlateLayoutTransform(FVector2D(X,Y)))));}
 void Unclip(){E.PopClip();}
};
}



