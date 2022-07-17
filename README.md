# ChowDSP VCV Rack Secret Modules

[![Build VCV Rack Plugin](https://github.com/Chowdhury-DSP/ChowDSP-VCV-Secrets/actions/workflows/build-nightly.yml/badge.svg)](https://github.com/Chowdhury-DSP/ChowDSP-VCV-Secrets/actions/workflows/build-nightly.yml)

This repository contains some "secret" VCV Rack modules
from Chowdhury DSP.

- ChowBell: an 808-style cowbell emulation
- MetalFace: the most expensive guitar distortion in VCV Rack?

### Why are these modules "secret"?

Chowdhury DSP has a set of
["non-secret" modules](https://github.com/jatinchowdhury18/ChowDSP-VCV)
which can be downloaded from the VCV plugin library.

Currently, VCV requires plugins distributed through the official
VCV plugin library to be compiled with C++11. However, our
[DSP library](https://github.com/Chowdhury-DSP/chowdsp_utils#dsp-modules)
currently requires C++17 (or later). So we decided to make
a set of "secret" modules, which will not be distributed in
the VCV plugin library (for now).

### How can I try out the modules?

At the moment this code is in a _very_ early state.
If you're feeling adventurous, you can try downloading
the build artifacts from the
[CI pipeline](https://github.com/Chowdhury-DSP/ChowDSP-VCV-Secrets/actions/workflows/build-nightly.yml), or try to build the modules yourself
[from the source code](https://vcvrack.com/manual/Building#building-rack-plugins).
We might do some "tagged" releases in the future,
but in the meantime, it's probably best to be patient
and wait for a bit more development to take place.

### Will these modules ever be available in the official plugin library?

Possibly. VCV has mentioned that a future version of Rack
might change the C++ version requirement to C++14 or C++17.
We also may work on getting our DSP library to be compatible
with C++14 (getting it down to C++11 would be quite tricky).
If the C++ version compatibility works out such that we
would be able to distribute these modules to the VCV library,
then we'll make an effort to do that, either by adding them
to the existing ChowDSP VCV Rack plugin, or by submitting
this plugin to the library.

### What's up with the panel design?

We haven't decided yet whether to continue using the same design
pattern used by the official [ChowDSP modules](https://github.com/jatinchowdhury18/ChowDSP-VCV),
or if we should do something new/different. For now, the panel designs
are just "blank" with the default VCV components for the knobs and ports.
If you have opinions on what these modules should look like, feel free to
chime in with a [GitHub Issue](https://github.com/Chowdhury-DSP/ChowDSP-VCV-Secrets/issues).

### But... Why?

So Chowdhury DSP spends most of our time developing audio plugins (VST/AU/CLAP/...)
often with the [JUCE framework](https://github.com/juce-framework/JUCE).
We also really enjoy creating VCV Rack modules, but have found it a bit
trickier to work on recently. Often we have some DSP code from our existing
library that we wanted to re-use, but couldn't because it was dependent on
JUCE. We recently made it possible to compile a good amount of the DSP code
from our library outside of a JUCE environment which opens up some exciting
possibilities, including using the library to write DSP code which can run
on Arduino devices or in VCV modules!

We still have the C++ version issue, which is blocking us from releasing
these modules to the VCV plugin library, but we didn't feel that should 
stop us from having fun creating some new modules. And if we can make
it possible for others out there to enjoy using these modules in the
meantime, then so much the better!
