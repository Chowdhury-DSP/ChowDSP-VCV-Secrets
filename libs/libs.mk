# Define the path of the built static library
libchowdsp := libs/build-libs/libchowdsp.a

# Build the static library into your plugin.dll/dylib/so
OBJECTS += $(libchowdsp)

# Trigger the static library to be built when running `make dep`
DEPS += $(libchowdsp)

# Add chowdsp_utils flags
FLAGS += -Ilibs/chowdsp_utils/modules/common -Ilibs/chowdsp_utils/modules/dsp

# Add chowdsp_wdf flags
FLAGS += -Ilibs/chowdsp_wdf/include

# add RTNeural flags
FLAGS += -Ilibs/RTNeural -DRTNEURAL_DEFAULT_ALIGNMENT=16 -DRTNEURAL_USE_XSIMD=1 -Ilibs/RTNeural/modules/xsimd/include

$(libchowdsp):
	echo $(PWD)
	cd libs && $(CMAKE) -Bbuild-libs
	cd libs/build-libs && $(MAKE) all
