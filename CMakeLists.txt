# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

#Les lignes 29-39 proviennent de https://gitea.planet-casio.com/Slyvtt/Collab_RPG

cmake_minimum_required(VERSION 3.15)
project(ShooterGame)

include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.9 REQUIRED)
find_package(LibProf 2.1 REQUIRED)

set(SOURCES
  src/main.c
  src/moteur.c
  src/sprites.c
  src/map.c
  src/utils.c
)

set(ASSETS
  assets-cg/textures/briques0.png
  assets-cg/textures/buisson1.png
  assets-cg/textures/herbe0.png
  assets-cg/sprites/zombard1.png
  #assets-cg/skybox0.png
)

add_custom_command(
  COMMENT "Convert Tiled TMX map to usable JSON file"
	COMMAND tiled --export-tileset json tilesetnpp.tsx tilesetnpp.json
	COMMAND find | grep .*.tmx | sed 's/.tmx//g' | xargs -l bash -c 'tiled --export-map json $$0.tmx $$0.json'
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets/  

  OUTPUT "${CMAKE_CURRENT_LIST_DIR}/assets/Shooterlevel0.json"
# if several levels/maps are created, just copy the previous line and change the .json name with the new level/map
  DEPENDS 	assets/converters.py
		assets-cg/ShooterLevel0.tmx
)

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(myaddin ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(myaddin PRIVATE -Wall -Wextra -Os)
target_link_libraries(myaddin Gint::Gint )
target_link_libraries(myaddin LibProf::LibProf)
target_link_options(myaddin PRIVATE -Wl,--print-memory-usage)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET myaddin OUTPUT "ShooterGame.g3a"
    NAME "ShGame" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
