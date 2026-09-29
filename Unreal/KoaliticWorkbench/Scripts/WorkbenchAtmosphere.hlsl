// App chrome atmosphere only; never used by the Player Profile presentation.
float2 p = UV;
float2 cells = p * float2(48.0, 27.0);
float2 g = abs(frac(cells) - 0.5);
float grid = (1.0 - smoothstep(0.475, 0.495, g.x)) * 0.08
           + (1.0 - smoothstep(0.475, 0.495, g.y)) * 0.08;
float bloomA = exp(-dot((p - float2(0.19, 0.14)) * float2(1.8, 2.7),
                         (p - float2(0.19, 0.14)) * float2(1.8, 2.7)) * 5.0);
float bloomB = exp(-dot((p - float2(0.76, 0.61)) * float2(1.5, 2.5),
                         (p - float2(0.76, 0.61)) * float2(1.5, 2.5)) * 7.0);
float sweepY = frac(Clock * 0.017) * 1.32 - 0.15;
float sweep = exp(-abs(p.y - sweepY) * 170.0) * 0.25;
float hatch = 0.0;
float2 corner = p - float2(0.05, 0.12);
if (corner.x > 0.0 && corner.y > 0.0 && corner.x < 0.17 && corner.y < 0.23)
    hatch = (1.0 - smoothstep(0.01, 0.025, abs(frac((corner.x + corner.y) * 78.0) - 0.5))) * 0.14;
float2 starCell = p * float2(170.0, 96.0);
float speck = frac(sin(dot(floor(starCell), float2(127.1, 311.7))) * 43758.5453);
float point = 1.0 - smoothstep(0.025, 0.15, length(frac(starCell) - 0.5));
speck = step(0.996, speck) * point * (0.42 + 0.16 * sin(Clock * 1.3 + speck * 21.0));
float power = grid + bloomA * 0.34 + bloomB * 0.22 + sweep + hatch + speck;
float3 cyan = float3(0.025, 0.49, 0.75);
float3 violet = float3(0.12, 0.06, 0.24);
float3 color = lerp(cyan, violet, saturate(p.x * 0.37 + p.y * 0.18)) * power;
return float4(color, saturate(power * 0.58));
