cmake_minimum_required(VERSION 3.14)

# Project setup
project(AudioApp VERSION 1.0 LANGUAGES C)

# Enable FetchContent module
include(FetchContent)

set(SOURCES
        main.c
        audio.c
        miniaudio.c  # Add this file for the implementation
)

# Download and build PortAudio
FetchContent_Declare(
        PortAudio
        GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(PortAudio)

# Download and build libsndfile
FetchContent_Declare(
        libsndfile
        GIT_REPOSITORY https://github.com/libsndfile/libsndfile.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(libsndfile)

# Download and make MiniAudio available (header-only)
FetchContent_Declare(
        miniaudio
        GIT_REPOSITORY https://github.com/mackron/miniaudio.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(miniaudio)

# Add the executable
add_executable(AudioApp main.c
        audio.c
)

# Include directories for dependencies
target_include_directories(AudioApp PRIVATE
        ${PortAudio_SOURCE_DIR}/include
        ${libsndfile_SOURCE_DIR}/include
        ${miniaudio_SOURCE_DIR}
)

# Add library paths for PortAudio and libsndfile (MiniAudio is header-only, no need to link to any libraries)
target_link_directories(AudioApp PRIVATE
        ${PortAudio_BINARY_DIR}/lib
        ${libsndfile_BINARY_DIR}/lib
)

# Link libraries
target_link_libraries(AudioApp PRIVATE portaudio sndfile)
