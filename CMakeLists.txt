# Basic parameters; check that these match your project / environment
cmake_minimum_required(VERSION 3.8)

project(32blox)
set(32BLIT_PATH "../" CACHE PATH "Path to 32blit.cmake")
set(PROJECT_SOURCE 32blox.cpp AssetFactory.cpp Ball.cpp Level.cpp HighScore.cpp
                   OutputManager.cpp PowerUp.cpp MenuState.cpp
                   daft_freak_wav.cpp
                   SplashState.cpp GameState.cpp DeathState.cpp HiscoreState.cpp)
set(DISTRIBS README.md LICENSE)

# Build configuration; approach this with caution!
if(MSVC)
  add_compile_options("/W4" "/wd4244" "/wd4324")
else()
  add_compile_options("-Wall" "-Wextra" "-Wdouble-promotion" "-Wno-unused-parameter" "-Wno-switch")
endif()
if(NOT EXISTS ${32BLIT_PATH}/32blit.cmake)
  message(FATAL_ERROR "Define location of 32Blit API with -D32BLIT_PATH=<path to 32blit.cmake>")
endif()
include (${32BLIT_PATH}/32blit.cmake)
install(FILES ${DISTRIBS} DESTINATION bin)
blit_executable (${PROJECT_NAME} ${PROJECT_SOURCE})
blit_assets_yaml (${PROJECT_NAME} assets.yml)
blit_metadata (${PROJECT_NAME} metadata.yml)
add_custom_target (flash DEPENDS ${PROJECT_NAME}.flash)
# End of CMakeList.txt
