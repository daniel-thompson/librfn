declare name      "tonestack";
declare author    "Daniel Thompson";
declare copyright "2012";
declare version   "1.0";
declare license   "GPLv3+";

import("maxmsp.lib");

import("util.lib");

// Tuneable values
bassCutoff = hslider("[1] Bass cutoff [unit:Hz]", 120, 40, 400, 10);
bassQ = hslider("[2] Bass Q", 0.7, 0.1, 1, 0.1);
midCentre = hslider("[3] Mid centre [unit:Hz]", 400, 120, 2000, 10);
midQ = hslider("[4] Mid Q", 0.7, 0.1, 1, 0.1);
trebleCutoff = hslider("[5] Treble cutoff [unit:Hz]", 2000, 400, 4000, 10);
trebleQ = hslider("[6] Treble Q", 0.7, 0.1, 1, 0.1);

tonestack(bass, mid, treble) = bassshelf : mideq : trebleshelf
with {
	bassshelf = lowShelf(_, bassCutoff, bass, bassQ);
	mideq = peakingEQ(_, midCentre, mid, midQ);
	trebleshelf = highShelf(_, trebleCutoff, treble, trebleQ);
};

tonestackapp = (gui, _) : tonestack
with {
	bass = hslider("[0] Bass [style: knob]", 5, 0, 10, 0.1) :
		rerange(0, 10, -36, 0) : smoothctl;
	mid = hslider("[0] Mid [style: knob]", 5, 0, 10, 0.1) :
		rerange(0, 10, -36, 0) : smoothctl;
	treble = hslider("[0] Treble [style: knob]", 5, 0, 10, 0.1) :
		rerange(0, 10, -36, 0) : smoothctl;
	
	gui = hgroup("[0] Tonestack", bass, mid, treble);
};

process = tonestackapp;
