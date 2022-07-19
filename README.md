# ChowDSP VCV Rack Secret Modules

[![Build VCV Rack Plugin](https://github.com/Chowdhury-DSP/ChowDSP-VCV-Secrets/actions/workflows/build-nightly.yml/badge.svg)](https://github.com/Chowdhury-DSP/ChowDSP-VCV-Secrets/actions/workflows/build-nightly.yml)

This repository contains some "secret" VCV Rack modules
from Chowdhury DSP.

- ChowBell: an 808-style cowbell emulation
- MetalFace: the most expensive guitar distortion in VCV Rack?

## Building

```bash
git clone https://github.com/Chowdhury-DSP/ChowDSP-VCV-Secrets
cd ChowDSP-VCV-Secrets
git submodule update --init --recursive
export RACK_DIR=location-of-rack-SDK-or-source
make -j dist
```

## About

### Why are these modules "secret"?

Chowdhury DSP has a set of
["non-secret" modules](https://github.com/jatinchowdhury18/ChowDSP-VCV)
which can be downloaded from the VCV plugin library.

These modules were developed as a separate plugin originally
to avoid a conflict with VCV Rack's C++11 requirement. However,
as C++11 is no longer a requirement for VCV Rack plugins, we're
trying to figure out what to do with these modules next...

### How can I try out the modules?

At the moment this code is in a _very_ early state.
If you're feeling adventurous, you can try downloading
the build artifacts from the
[CI pipeline](https://github.com/Chowdhury-DSP/ChowDSP-VCV-Secrets/actions/workflows/build-nightly.yml), or try to build the modules yourself. We might do some
"tagged" releases in the future, but in the meantime,
it's probably best to be patient and wait for a bit more
development to take place.

### Will these modules ever be available in the official plugin library?

Possibly. These modules might be integrated into the rest
of the ChowDSP modules, or they might be released as their
own plugin.

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

## License

ChowDSP Secret VCV Modules (both artwork and code) are licensed under the GPLv3 license. Enjoy!
