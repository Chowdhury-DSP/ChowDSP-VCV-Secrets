# ChowDSP VCV Rack Secret Modules

This repository contains some "secret" VCV Rack modules
from Chowdhury DSP.

- ChowBell: an 808-style cowbell emulation
- More to come...

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
If you're feeling adventurous, you could try to build
the modules yourself
[from the source code](https://vcvrack.com/manual/Building#building-rack-plugins).
We might make some builds available to download sometime
in the future, but in the meantime, it's probably best
to be patient and wait for a bit more development to
take place.

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
