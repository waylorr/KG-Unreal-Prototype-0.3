// UI-domain shader. All animation uses a supplied clock, never engine wall time.
// Angular coverage follows the editable native frame. Alpha blends in Slate.
float2 p = UV;
float aspect = lerp(3.95257, 7.4074, Compact);
float t = Clock * Speed;
float shoulder = 40.0/lerp(253.0,135.0,Compact);
float top = p.x < .81 ? 0 : (p.x < .842 ? (p.x-.81)/.032 * shoulder : shoulder);
float right = p.y < .51 ? .942 + .058*saturate((p.y-shoulder)/(.51-shoulder)) : 1;
float mask = step(top,p.y)*step(p.x,right)*step(.024-p.y*.225,p.x)*step(p.x,1-max(0,p.y-(1-25.0/lerp(253.0,135.0,Compact)))*(.018/(25.0/lerp(253.0,135.0,Compact))));
mask *= step(.012-(1-p.y)*.2,p.x);
if(Design>.5){float2 q=min(p,1-p);mask=step(.012-q.y*.10,q.x);}
float sweepPos = .5 + .7*sin(t*.58);
float axis = p.x + p.y*.13;
float beam = exp(-pow((axis-sweepPos)*10,2));
float sharp = exp(-pow((axis-sweepPos+.085)*130,2));
float haze = exp(-length((p-float2(.19,.5))*float2(4,1.1))*3);
float energy = pow(.5+.5*sin(p.x*15-t*1.4),5);
float fine = pow(.5+.5*sin(p.y*820),18)*.014;
float3 col = float3(.11,.25,.35) * (beam*.48 + haze*.16)*Sweep;
col += float3(.40,.74,.91)*sharp*.38*Sweep;
col += float3(.15,.4,.6)*fine;
float spark = 0;
// Deterministic, antialiased drifting particles, clipped to the card itself.
for(int i=0;i<126;i++) {
    if(float(i) >= Particles*42) continue;
    float seed = frac(sin(float(i)*127.1+3.7)*43758.5453);
    float life = frac(t*ParticleDrift*(.08+seed*.12)+seed*17);
    float2 pos = float2(frac(seed*13.17+.04*sin(t*.27+i)),1-life);
    float2 d = (p-pos)*float2(aspect,1);
    float r = (.002 + seed*.002)*ParticleSize;
    float fade = sin(life*3.14159);
    spark += exp(-dot(d,d)/(r*r))*fade;
    spark += exp(-dot(d,d)/(r*r*32))*.065*fade*Glow;
}
col += float3(.3,.72,1)*spark*1.8;
float border = exp(-pow((p.y-top)*150,2)) + exp(-pow((1-p.y)*150,2));
col += float3(.12,.55,.85)*border*energy*.3*Glow*Perimeter;
float scan = exp(-pow((p.y-frac(t*.18))*150,2));
col += float3(.12,.62,.76)*scan*.22*Scan;
if(Mode>.5) col *= float3(.55,1.1,1.2);
col *= Strength;
float ring = exp(-pow((length((p-float2(.58,.57))*float2(aspect,1))-(1-saturate(Reaction))*2.8)*30,2));
col += float3(.15,1,.55)*ring*Reaction*.65;
float opacity = saturate(max(col.r,max(col.g,col.b)));
return float4(col/max(.001,opacity),opacity*mask);
