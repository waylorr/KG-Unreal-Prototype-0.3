"""Generate the original, deterministic KOALITIC interface cue source WAVs."""
from pathlib import Path
import math
import random
import struct
import wave

RATE = 48000
OUT = Path(__file__).resolve().parents[1] / "AudioSource"
OUT.mkdir(exist_ok=True)


def tone(t, start, length, f0, f1, level, decay=2.0):
    u = (t - start) / length
    if not 0 <= u < 1:
        return 0.0
    attack = min(1.0, u * 35)
    envelope = attack * (1 - u) ** decay
    phase = 2 * math.pi * length * (f0 * u + (f1 - f0) * u * u / 2)
    return level * envelope * math.sin(phase)


def noise(t, start, length, level, rng):
    u = (t - start) / length
    if not 0 <= u < 1:
        return 0.0
    return level * min(1, u * 50) * (1 - u) ** 3 * rng.uniform(-1, 1)


def render(name, duration, voice):
    rng = random.Random(7219)
    samples = []
    for i in range(round(duration * RATE)):
        t = i / RATE
        sample = voice(t, rng)
        # A short release avoids clicks at the file boundary.
        sample *= min(1.0, (duration - t) * 250)
        samples.append(sample)
    peak = max(abs(x) for x in samples)
    gain = 0.62 / peak
    with wave.open(str(OUT / f"UI_{name}.wav"), "wb") as wav:
        wav.setparams((1, 2, RATE, 0, "NONE", "not compressed"))
        wav.writeframes(b"".join(struct.pack("<h", round(max(-1, min(1, x * gain)) * 32767)) for x in samples))
    print(f"UI_{name}.wav: {duration:.2f}s, peak {peak * gain:.3f}")


render("Navigate", .12, lambda t, r: tone(t, 0, .09, 720, 940, .32) + tone(t, .012, .09, 1440, 1660, .12) + noise(t, 0, .028, .025, r))
render("Select", .16, lambda t, r: tone(t, 0, .12, 1080, 900, .28) + tone(t, .009, .12, 2160, 1750, .10) + noise(t, 0, .025, .024, r))
render("Confirm", .32, lambda t, r: tone(t, 0, .20, 740, 1000, .29) + tone(t, .08, .22, 1110, 1490, .24) + tone(t, .11, .18, 2220, 2980, .065) + noise(t, 0, .025, .02, r))
render("Enter", .48, lambda t, r: tone(t, 0, .40, 150, 620, .25, 1.2) + tone(t, .085, .34, 540, 1280, .21, 1.5) + tone(t, .21, .23, 1400, 1920, .08) + noise(t, .03, .34, .12, r))
render("Exit", .42, lambda t, r: tone(t, 0, .36, 810, 180, .27, 1.2) + tone(t, .04, .28, 1640, 380, .10) + noise(t, .02, .26, .10, r))
render("Reward", .52, lambda t, r: tone(t, 0, .34, 880, 1320, .24, 1.3) + tone(t, .055, .35, 1320, 1980, .18) + tone(t, .12, .35, 1760, 2640, .13) + tone(t, .17, .30, 3520, 3960, .045) + noise(t, .01, .05, .035, r))
