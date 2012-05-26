declare name      "cabsim";
declare author    "Daniel Thompson";
declare copyright "2012";
declare version   "1.0";
declare license   "GPLv3+";

import("maxmsp.lib");

// Starting point (based on a graph of the condor from runoffgroove)
//
// Five biquads
//     Partial notch filter at 400Hz (peaking EQ) (-16dB)
//     High boosting shelf filter at 400Hz (6dB)
//     High pass at 60Hz
//     2 x low pass at 4000Hz
//
// fiview 48000 -i \
//   PkBq/0.7/-16/400 x \
//   HsBq/0.7/6/400 x \
//   HpBq/0.7/60 x \
//   LpBq/0.7/4000 x LpBq/0.7/4000

// Tuneable values
notchCentre = hslider("[10] Notch centre [unit:Hz]", 400, 120, 1000, 10);
notchDepth = hslider("[11] Notch depth [unit:dB]", -16, -36, 0, 0.5);
notchQ = hslider("[12] Notch Q", 0.7, 0.1, 2, 0.1);
shelfCentre = hslider("[20] Shelf centre [unit:Hz]", 400, 120, 1000, 10);
shelfHeight = hslider("[21] Shelf height [unit:dB]", 6, -18, 18, 0.5);
shelfQ = hslider("[22] Shelf Q", 0.7, 0.1, 2, 0.1);
lowCutoff = hslider("[30] Low cutoff [unit:Hz]", 60, 20, 120, 10);
lowQ = hslider("[31] Low Q", 0.7, 0.1, 2, 0.1);
highCutoff = hslider("[40] High cutoff [unit:Hz]", 4000, 1000, 6000, 100);
highQ = hslider("[41] High Q", 0.7, 0.1, 2, 0.1);

process = notch : shelf : highpass : lowpass : lowpass
with {
	notch = peakingEQ(_, notchCentre, notchDepth, notchQ);
	shelf = highShelf(_, shelfCentre, shelfHeight, shelfQ);
	highpass = HPF(_, lowCutoff, 0, lowQ);
	lowpass = LPF(_, highCutoff, 0, highQ);
};
