# Basic parameters; check that these match your project / environment
cmake_minimum_required(VERSION 3.8)

project(32blox)
set(32BLIT_PATH "../" CACHE PATH "Path to 32blit.cmake")
set(PROJECT_SOURCE 32blox.cpp AssetFactory.cpp Ball.cpp Level.cpp HighScore.cpp
                   SplashState.cpp GameState.cpp DeathState.cpp HiscoreState.cpp)
set(DISTRIBS README.md LICENSE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") 

# Build configuration; approach this with caution!
if(NOT EXISTS ${32BLIT_PATH}/32blit.cmake)
  message(FATAL_ERROR "Define location of 32Blit API with -D32BLIT_PATH=<path to 32blit.cmake>")
endif()
include (${32BLIT_PATH}/32blit.cmake)
install(FILES ${DISTRIBS} DESTINATION bin)
blit_executable (${PROJECT_NAME} ${PROJECT_SOURCE})
blit_assets_yaml (${PROJECT_NAME} assets.yml)
blit_metadata (${PROJECT_NAME} metadata.yml)
# End of CMakeList.txt
