# Basic parameters; check that these match your project / environment
cmake_minimum_required(VERSION 3.8)

# we're using the copy of the import files from the 32Blit SDK here
# this example checks if PICO_SDK_PATH is set to allow compiling for other targets when it isn't set
if(PICO_SDK_PATH)
  include(${32BLIT_DIR}/32blit-pico/pico_sdk_import.cmake)
  include(${32BLIT_DIR}/32blit-pico/pico_extras_import.cmake)
endif()

project(32blox)

set(PROJECT_SOURCE 32blox.cpp AssetFactory.cpp Ball.cpp Level.cpp HighScore.cpp
                   OutputManager.cpp PowerUp.cpp MenuState.cpp
                   daft_freak_wav.cpp
                   SplashState.cpp GameState.cpp DeathState.cpp HiscoreState.cpp)
set(PROJECT_DISTRIBS README.md LICENSE)

# Build configuration; approach this with caution!
if(MSVC)
  add_compile_options("/W4" "/wd4244" "/wd4324")
else()
  add_compile_options("-Wall" "-Wextra" "-Wdouble-promotion" "-Wno-unused-parameter" "-Wno-switch")
endif()

find_package(32BLIT CONFIG REQUIRED PATHS ../32blit-sdk-v0.1.12)

install(FILES ${DISTRIBS} DESTINATION bin)
blit_executable (${PROJECT_NAME} ${PROJECT_SOURCE})
blit_assets_yaml (${PROJECT_NAME} assets.yml)
blit_metadata (${PROJECT_NAME} metadata.yml)
add_custom_target (flash DEPENDS ${PROJECT_NAME}.flash)

# setup release packages
install (FILES ${PROJECT_DISTRIBS} DESTINATION .)
set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set (CPACK_GENERATOR "ZIP" "TGZ")
include (CPack)
# End of CMakeList.txt
