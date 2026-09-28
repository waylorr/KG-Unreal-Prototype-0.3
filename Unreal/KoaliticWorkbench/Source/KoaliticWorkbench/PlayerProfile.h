#pragma once
#include "WorkbenchCanvas.h"
#include "ProgressPart.h"
#include "Framework/Application/SlateApplication.h"
#include "Rendering/SlateRenderer.h"
#include "Fonts/FontMeasure.h"
namespace KW {
// Presentation, source data, interaction and event history are deliberately separate.
struct ProfileData {FString Name=TEXT("KOALITIC");int Level=12,XP=3250,MaxXP=5000;};
struct Presentation {int Portrait=0,Accent=0,Entrance=0,Exit=0,Ambient=1,Direction=0;float Duration=1.5f,ExitDuration=.8f,Stagger=.16f,Glow=.75f,Glitch=.22f,Frequency=.65f,AmbientStrength=.78f,Speed=1.f,Sweep=1.f,Particles=.7f,TextNoise=.45f,Reaction=1.f,ReactionDuration=1.2f,Burst=.8f;int AmbientMode=0,Design=0,Ornaments=0;float EnterEase=3,ExitEase=3,BarTip=1;float Perimeter=1,Scan=0,GlitchBands=7,GlitchDuration=.12f,GlitchOffset=18,BarHeight=1,BarInset=.16f,BarGlow=1,BarSpeed=1,ParticleSize=1,ParticleDrift=1;bool Layers[8]={true,true,true,true,true,true,true,true};};
struct ProfileState {
 bool Compact=false,Hover=false,Disabled=false,Pinned=false,AutoCollapse=false;
 float LayoutFrom=0,LayoutAt=-10;
 float Layout(float Time)const{return FMath::Lerp(LayoutFrom,Compact?1.f:0.f,Ease((Time-LayoutAt)/.38f));}
 void SetCompact(bool Value,float Time,bool Snap=false){if(Compact==Value)return;LayoutFrom=Snap?(Value?1.f:0.f):Layout(Time);LayoutAt=Time;Compact=Value;}
};
struct Reward {float Time;int Amount;};
struct Evaluation {float Reveal=1,Portrait=1,Labels=1,Bar=1,Offset=0,Alpha=1,Fill=0;int Level=12,XP=3250,RewardAmount=0;float RewardAge=100;bool LevelUp=false;};
struct Playback {float Time=0,Transition=0,From=0,PulseAt=-100;bool Playing=true,Exiting=false;};
inline Evaluation Evaluate(const ProfileData& D,const Presentation& P,const Playback& K,const TArray<Reward>& Events){
 Evaluation V;float U=(K.Time-K.Transition)/(K.Exiting?P.ExitDuration:P.Duration);float Eased=1-FMath::Pow(1-Sat(U),K.Exiting?P.ExitEase:P.EnterEase);float R=K.Exiting?K.From*(1-Eased):K.From+(1-K.From)*Eased;if(K.Exiting&&U>1.2f)R=Ease((U-1.2f)/.22f);V.Reveal=Sat(R);V.Portrait=Sat((R-P.Stagger)/(1-P.Stagger));V.Labels=Sat((R-P.Stagger*2)/(1-P.Stagger*2));V.Bar=Sat((R-P.Stagger*2.5f)/(1-P.Stagger*2.5f));
 V.Offset=(P.Entrance==1&&!K.Exiting)?(1-R)*110*(P.Direction?-1:1):0;V.Alpha=K.Exiting&&P.Exit==1?Sat(R*2):1;
 int Total=D.XP;float Visual=D.XP;for(const auto& Ev:Events){if(Ev.Time>K.Time)break;float Age=K.Time-Ev.Time;float Before=Visual;Visual=FMath::Lerp(Before,float(Total+Ev.Amount),Ease(Age/P.ReactionDuration));Total+=Ev.Amount;V.RewardAmount=Ev.Amount;V.RewardAge=Age;V.LevelUp=((Total-Ev.Amount)/D.MaxXP!=Total/D.MaxXP);}
 V.Level=D.Level+Total/D.MaxXP;V.XP=Total%D.MaxXP;V.Fill=FMath::Fmod(Visual,float(D.MaxXP))/D.MaxXP;if(Visual>0&&FMath::IsNearlyEqual(FMath::Fmod(Visual,float(D.MaxXP)),0.f))V.Fill=0;return V;
}
// Pure component renderer: accepts data/time and can be used without the Workbench.
inline void DrawProfileLayers(Canvas& C,float X,float Y,float W,const ProfileData& D,const Presentation& P,const ProfileState& State,const Playback& K,const TArray<Reward>& Events,const FSlateBrush& Portrait,bool Still=false,const FSlateBrush* Glass=nullptr){
 Evaluation V=Evaluate(D,P,K,Events);if(Still){V.Reveal=V.Portrait=V.Labels=V.Bar=V.Alpha=1;V.Offset=0;}if(V.Reveal<.001f)return;
 float Blend=Still?(State.Compact?1.f:0.f):State.Layout(K.Time);bool Compact=Blend>.5f;auto Mix=[Blend](float Expanded,float Small){return FMath::Lerp(Expanded,Small,Blend);};float H=W*Mix(.253f,.135f);float S=W/1000.f;float Old=C.Alpha;C.Alpha*=V.Alpha*(State.Disabled?.36f:1);FLinearColor Accent=P.Accent==0?Red:P.Accent==1?Cyan:FLinearColor(.65f,.15f,1);float Reaction=FMath::Max((K.Time>=K.PulseAt?Sat(1-(K.Time-K.PulseAt)/P.ReactionDuration):0.f),Sat(1-V.RewardAge/P.ReactionDuration))*P.Reaction;float Amb=(P.Ambient&&!State.Disabled?(.5f+.5f*FMath::Sin(K.Time*P.Speed*1.25f))*P.AmbientStrength:0);
 X+=V.Offset;float ClipWidth=W*(K.Exiting&&P.Exit==0?V.Reveal:(!K.Exiting&&P.Entrance==0?V.Reveal:1));float ClipX=P.Direction?X+W-ClipWidth:X;float ClipH=K.Exiting&&P.Exit==1?H*V.Reveal:H;
 C.Clip(ClipX-22,Y+(H-ClipH)/2-22,ClipWidth+44,ClipH+44);
 TArray<FVector2D>Shape={{X+24*S,Y},{X+W*.81f,Y},{X+W*.842f,Y+40*S},{X+W-58*S,Y+40*S},{X+W,Y+H*.51f},{X+W,Y+H-25*S},{X+W-18*S,Y+H},{X+12*S,Y+H},{X,Y+H-14*S},{X,Y+27*S}}; if(P.Design==1){Shape={{X+12*S,Y},{X+W-12*S,Y},{X+W,Y+12*S},{X+W,Y+H-12*S},{X+W-12*S,Y+H},{X+12*S,Y+H},{X,Y+H-12*S},{X,Y+12*S}};}
 if(P.Layers[1]){C.PolyGradient(Shape,Y,H,FLinearColor(.016f,.026f,.04f),FLinearColor(.003f,.008f,.013f));C.Poly({{X+W*.70f,Y+1},{X+W*.81f,Y+1},{X+W-18*S,Y+H-1},{X+W*.87f,Y+H-1}},FLinearColor(.045f,.058f,.085f,.14f));}

 if(P.Layers[0]){auto Outline=Shape;const FVector2D First=Outline[0];Outline.Add(First);for(int I=3;I>0;I--)C.Path(Outline,A(State.Hover?Cyan:White,.022f*P.Glow),I*4.f*S);C.Path(Outline,A(State.Hover?Cyan:FLinearColor(.46f,.54f,.67f),.9f),1.3f*S);C.Seg(X+27*S,Y+1,X+W*.81f,Y+1,A(White,.55f+Amb),1.2f*S);C.Seg(X+W,Y+H*.52f,X+W,Y+H-25*S,White,1.4f*S);}
 if(Glass&&P.Layers[1]) C.Image(X,Y,W,H,*Glass);
 float PX=X+25*S,PY=Y+18*S,PW=Mix(225,92)*S,PH=H-36*S;
 if(P.Layers[2]){float Cut=PH*Ease(V.Portrait);C.Clip(PX,PY+PH-Cut,PW,Cut);C.Image(PX,PY,PW,PH,Portrait,State.Disabled?FLinearColor(.5f,.5f,.5f):FLinearColor::White);C.Unclip();C.Path({{PX,PY+20*S},{PX+20*S,PY},{PX+PW-10*S,PY},{PX+PW,PY+10*S},{PX+PW,PY+PH-18*S},{PX+PW-18*S,PY+PH},{PX+10*S,PY+PH},{PX,PY+PH-10*S},{PX,PY+20*S}},A(Muted,.7f),S);if(V.Portrait<.99f)C.Seg(PX,PY+PH-Cut,PX+PW,PY+PH-Cut,A(Cyan,.9f),2*S);}
 if(P.Layers[2]&&P.Ambient&&!State.Disabled){float SweepY=PY+FMath::Frac(K.Time*P.Speed*.16f)*PH;C.Clip(PX,PY,PW,PH);C.Gradient(PX,SweepY-18*S,PW,18*S,A(Cyan,0),A(Cyan,.15f*P.Sweep*P.AmbientStrength));C.Seg(PX,SweepY,PX+PW,SweepY,A(White,.16f*P.AmbientStrength),S);C.Unclip();C.Halo(PX+PW*.55f,PY+5*S,PW*.6f,26*S,Cyan,P.Glow*.15f);}
 float TX=X+Mix(290,142)*S,TY=Y+Mix(39,21)*S,TW=Mix(.58f,.48f)*W;float LabelA=Ease(V.Labels);C.Alpha*=LabelA;
 if(P.Layers[3]){FString Name=D.Name; if(V.Labels>.05f&&V.Labels<.8f&&P.Glitch>0){int Count=FMath::FloorToInt(Name.Len()*V.Labels/.8f);for(int I=Count;I<Name.Len();I++)Name[I]=TEXT("0123456789/|")[(I*7+int(K.Time*22))%12];}int NameSize=FMath::Max(8,int(Mix(40,26)*S));auto Measure=FSlateApplication::Get().GetRenderer()->GetFontMeasureService();while(NameSize>6&&Measure->Measure(Name,FSlateFontInfo(Canvas::Font(true),NameSize)).X>TW)--NameSize;float NoisePhase=FMath::Frac(K.Time*P.Speed*.41f);bool Noisy=!State.Disabled&&P.Ambient&&P.TextNoise*P.AmbientStrength>0&&NoisePhase<.08f;
 if(Noisy){int Index=(int(FMath::Abs(K.Time*P.Speed)*19)+3)%FMath::Max(1,Name.Len());if(NoisePhase<.025f)Name[Index]=TEXT("/0123456789")[(int(FMath::Abs(K.Time*P.Speed)*31))%11];C.Text(TX-3*S*P.TextNoise,TY,Name,NameSize,A(Cyan,.6f*P.TextNoise),true);C.Text(TX+3*S*P.TextNoise,TY+S,Name,NameSize,A(Red,.45f*P.TextNoise),true);}
 C.Text(TX,TY,Name,NameSize,White,true);}
 if(P.Layers[4]){float LX=FMath::Lerp(TX,X+W*.69f,Blend),LY=TY+Mix(50,0)*S;C.Text(LX,LY+7*S,Compact?TEXT("LVL"):TEXT("L E V E L"),FMath::Max(7,int(Mix(18,15)*S)),Muted,true);C.Text(LX+Mix(129,47)*S,LY-7*S,FString::FromInt(V.Level),FMath::Max(9,int(Mix(37,29)*S)),Accent,true);}
 C.Alpha/=FMath::Max(.0001f,LabelA);if(LabelA==0)C.Alpha=Old*V.Alpha*(State.Disabled?.36f:1);
 float BX=TX,BY=Y+Mix(153,80)*S,BW=W*Mix(.56f,.68f),BH=Mix(23,10)*S;
 if(P.Layers[5]){ProgressPartStyle BarStyle;BarStyle.Height=BH*P.BarHeight;BarStyle.Inset=P.BarInset;BarStyle.Tip=P.BarTip;BarStyle.Glow=P.Glow*P.BarGlow*(.9f+Reaction*2);BarStyle.Sweep=P.Ambient&&!State.Disabled?P.Sweep*P.AmbientStrength:0;BarStyle.Speed=P.Speed*P.BarSpeed;DrawProgressPart(C,BX,BY,BW,BarStyle,V.Fill*Ease(V.Bar),K.Time);C.Text(BX,BY+Mix(42,24)*S+FMath::Max(0.f,BarStyle.Height-BH),FString::Printf(TEXT("%s / %s XP"),*FText::AsNumber(V.XP).ToString(),*FText::AsNumber(D.MaxXP).ToString()),FMath::Max(6,int(Mix(22,11)*S)),Muted,true);}
 if(P.Layers[6]){float AT=Mix(58,32)*S,AB=H-Mix(60,28)*S;C.Glow(X-3*S,Y+AT,5*S,AB-AT,Accent,P.Glow*(.6f+Amb));C.Poly({{X-7*S,Y+AT+8*S},{X+1*S,Y+AT},{X+1*S,Y+AB},{X-7*S,Y+AB-8*S}},Accent);if(P.Ornaments){C.Seg(X+W-70*S,Y+53*S,X+W-47*S,Y+85*S,Accent,5*S);for(int I=0;I<3;I++)C.Seg(X+W-125*S+I*13*S,Y+69*S,X+W-112*S+I*13*S,Y+54*S,Muted,4*S);C.Seg(X+W-33*S,Y+H-39*S,X+W-14*S,Y+H-39*S,Muted,S);C.Seg(X+W-23*S,Y+H-49*S,X+W-23*S,Y+H-29*S,Muted,S);}}
 if(P.Layers[7]&&!State.Disabled){
 if(Reaction>.01f){auto Outline=Shape;FVector2D First=Outline[0];Outline.Add(First);for(int J=5;J>0;--J)C.Path(Outline,A(Green,Reaction*.022f),J*4*S);C.Path(Outline,A(Green,Reaction*.65f),1.5f*S);}
 if(V.LevelUp&&V.RewardAge<P.ReactionDuration+1.3f){float Life=V.RewardAge/(P.ReactionDuration+1.3f);float Light=FMath::Sin(Sat(Life)*PI)*P.Reaction;for(int I=0;I<3;I++){float CX=X+W-60*S,CY=Y+H*.42f-I*15*S-Life*16*S;C.Path({{CX-11*S,CY+9*S},{CX,CY},{CX+11*S,CY+9*S}},A(Green,Light),2*S);}C.Halo(X+W*.66f,Y+H*.5f,W*.35f,H*.7f,Green,Light*.11f);}
 float Age=FMath::Min(K.Time>=K.PulseAt?K.Time-K.PulseAt:100.f,V.RewardAge);
 if(Age>=0&&Age<P.ReactionDuration&&P.Burst>0){for(int I=0;I<int(24*P.Burst);I++){float Seed=FMath::Frac(FMath::Sin(I*127.1f+3.7f)*43758.5453f);float Life=Age/P.ReactionDuration;float SX=BX+BW*(.25f+.75f*Seed)+FMath::Sin(I*2.4f)*Life*50*S;float SY=BY-Life*(35+90*Seed)*S;float Fade=FMath::Sin(Life*PI)*P.Reaction;C.Halo(SX,SY,9*S,9*S,Green,Fade*.5f);C.Seg(SX,SY,SX-Life*5*S,SY+Life*9*S,A(Green,Fade),1.3f*S);}}
if(P.Ambient&&P.AmbientStrength>0&&P.Perimeter>0){
 float Perimeter=0;for(int I=0;I<Shape.Num();I++)Perimeter+=FVector2D::Distance(Shape[I],Shape[(I+1)%Shape.Num()]);
 float Head=FMath::Frac(K.Time*P.Speed*.12f)*Perimeter;float Distance=0;
 for(int I=0;I<Shape.Num();I++){FVector2D A0=Shape[I],B0=Shape[(I+1)%Shape.Num()];float Len=FVector2D::Distance(A0,B0);for(int Wrap=0;Wrap<2;Wrap++){float Start=FMath::Max(0.f,Head-Wrap*Perimeter-Distance),End=FMath::Min(Len,Head+105*S-Wrap*Perimeter-Distance);if(End>Start){FVector2D A1=FMath::Lerp(A0,B0,Start/Len),B1=FMath::Lerp(A0,B0,End/Len);float Light=P.AmbientStrength*P.Glow*P.Perimeter;C.Path({A1,B1},A(Cyan,Light*.035f),12*S);C.Path({A1,B1},A(Cyan,Light*.12f),5*S);C.Path({A1,B1},A(White,Light*.9f),1.3f*S);C.Halo(B1.X,B1.Y,24*S,12*S,Cyan,Light*.38f);}}Distance+=Len;}
 }
 if(!Still&&V.Reveal<.99f){float EdgeX=K.Exiting&&P.Exit==1?X+W*.5f:(P.Direction?X+W*(1-V.Reveal):X+W*V.Reveal);C.Halo(EdgeX,Y+H*.5f,34*S,H*.75f,Cyan,.25f*P.Glow);C.Seg(EdgeX,Y+15*S,EdgeX,Y+H-15*S,A(White,.65f),1.5f*S);}
}
 C.Unclip();C.Alpha=Old;
 if(V.RewardAge>=0&&V.RewardAge<P.ReactionDuration+1.3f&&!Still){float Fade=Sat(V.RewardAge/.12f)*Sat((P.ReactionDuration+1.3f-V.RewardAge)/.6f);C.Alpha=Fade;C.Cut(X+W-235*S,BY-39*S-V.RewardAge*12*S,235*S,38*S,A(Panel,.98f),A(Green,.5f));C.Text(X+W-222*S,BY-32*S-V.RewardAge*12*S,FString::Printf(TEXT("+%d XP  %s"),V.RewardAmount,V.LevelUp?TEXT(" / LEVEL UP"):TEXT(" / RECEIVED")),int(13*S),Green,true);C.Alpha=Old;}
}
inline void DrawProfile(Canvas& C,float X,float Y,float W,const ProfileData& D,const Presentation& P,const ProfileState& State,const Playback& K,const TArray<Reward>& Events,const FSlateBrush& Portrait,bool Still=false,const FSlateBrush* Glass=nullptr){
 if(Still){DrawProfileLayers(C,X,Y,W,D,P,State,K,Events,Portrait,true,Glass);return;}
 auto V=Evaluate(D,P,K,Events);float H=W*FMath::Lerp(.253f,.135f,State.Layout(K.Time));
 float Age=K.Time-K.Transition,Duration=K.Exiting?P.ExitDuration:P.Duration;
 bool Motion=Age>=0&&Age<Duration;int Mode=K.Exiting?P.Exit:P.Entrance;
 float Phase=FMath::Frac(FMath::Max(0.f,K.Time)*P.Frequency);
 float Reaction=FMath::Max(Sat(1-V.RewardAge/P.ReactionDuration),K.Time>=K.PulseAt?Sat(1-(K.Time-K.PulseAt)/P.ReactionDuration):0.f)*P.Reaction;
 bool Glitch=P.Layers[7]&&!State.Disabled&&P.Glitch>0&&((P.Ambient&&P.AmbientStrength>0&&Phase<P.GlitchDuration*P.Frequency)||(Reaction>.7f));
 if(!Glitch&&!(Motion&&Mode>=2)){DrawProfileLayers(C,X,Y,W,D,P,State,K,Events,Portrait,false,Glass);return;}
 const float S=W/1000.f;int Bands=Glitch?FMath::Clamp(FMath::RoundToInt(P.GlitchBands),2,16):8;
 float OldAlpha=C.Alpha;FLinearColor OriginalTint=C.Tint;
 for(int I=0;I<Bands;I++){
  float Seed=FMath::Frac(FMath::Sin(I*127.1f+FMath::FloorToFloat(K.Time*24)*7.3f)*43758.5453f);
  float Shift=Glitch?(Seed-.5f)*2*P.GlitchOffset*P.Glitch*S:0;
  float T=Sat(Age/Duration);float Reveal=K.Exiting?FMath::Pow(1-T,P.ExitEase):1-FMath::Pow(1-Sat((T-I*.035f)/.755f),P.EnterEase);
  if(Motion&&Mode==2)Shift+=(I%2?1:-1)*(1-Reveal)*W*.34f;
  if(Motion&&Mode==3)Shift+=(Seed-.5f)*W*(1-Reveal)*.28f;
  if(Motion&&Mode==4)Shift+=(1-Reveal)*W*.16f*(P.Direction?-1:1);
  float SliceY=Y-32*S+I*(H+64*S)/Bands,SliceH=(H+64*S)/Bands;
  C.Clip(X-70*S,SliceY,W+140*S,SliceH);
  if(Glitch){C.Alpha=OldAlpha*.35f*FMath::Min(P.Glitch,2.f);C.Tint=OriginalTint*FLinearColor(.08f,.8f,1);DrawProfileLayers(C,X+Shift+5*S*P.Glitch,Y,W,D,P,State,K,Events,Portrait,false,Glass);C.Tint=OriginalTint;}
  C.Alpha=OldAlpha*(Motion&&Mode>=2?Reveal:1);
  DrawProfileLayers(C,X+Shift,Y,W,D,P,State,K,Events,Portrait,false,Glass);
  C.Alpha=OldAlpha;C.Unclip();
 }
}

}



