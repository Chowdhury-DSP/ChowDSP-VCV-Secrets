# Define the path of the built static library
libchowdsp := libs/build-libs/libchowdsp.a

# Build the static library into your plugin.dll/dylib/so
OBJECTS += $(libchowdsp)

# Trigger the static library to be built when running `make dep`
DEPS += $(libchowdsp)

FLAGS += -Ilibs/chowdsp_utils/modules/common -Ilibs/chowdsp_utils/modules/dsp -Ilibs/chowdsp_wdf/include

$(libchowdsp):
	echo $(PWD)
	cd libs && $(CMAKE) -Bbuild-libs
	cd libs/build-libs && $(MAKE) all
