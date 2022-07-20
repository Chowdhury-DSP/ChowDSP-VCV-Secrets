cmake_minimum_required(VERSION 3.15)
project(chowdsplib)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(chowdsp_utils)
setup_chowdsp_lib(chowdsp
    MODULES chowdsp_dsp_utils chowdsp_waveshapers
)

install(TARGETS chowdsp
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
