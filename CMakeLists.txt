cmake_minimum_required(VERSION 3.16)
project(ps1_dev C)

set(CMAKE_C_STANDARD 99)


include_directories(third_party/psyq-iwyu/include)

add_executable(
        ps1_dev

        lib/header/AssetManager.h
        lib/header/CdReader.h
        lib/header/Frames.h
        lib/header/GameObject.h
        lib/header/GPUBase.h
        lib/header/JSONParser.h
        lib/header/Logger.h
        lib/header/Map.h
        lib/header/MemUtils.h
        lib/header/StrUtils.h
        lib/header/Tiled.h
        lib/header/TileFetcher.h
        lib/header/SoundEffect.h
        lib/header/FntLogger.h
        lib/header/TextBox.h
        lib/header/TxtHandler.h
        lib/header/DPad.h
        lib/header/CollisionHandler.h

        lib/source/AssetManager.c
        lib/source/CdReader.c
        lib/source/Frames.c
        lib/source/GameObject.c
        lib/source/GPUBase.c
        lib/source/JSONParser.c
        lib/source/Logger.c
        lib/source/Tiled.c
        lib/source/TileFetcher.c
        lib/source/TextBox.c
        lib/source/TxtHandler.c
        lib/source/CollisionHandler.c

        # LIBGS Projects
        gs_projects/from-scratch/Main.c
        gs_projects/from-scratch/Map.c
        gs_projects/use-tilemaps/Main.c
        gs_projects/use-tilemaps/Map.c
        gs_projects/draw-primes-in-ot/Main.c
        gs_projects/play-sounds-cdda/Main.c
        gs_projects/play-sounds-libsnd/Main.c
        gs_projects/play-sounds-spu/Main.c
        gs_projects/use-fonts/Main.c
        gs_projects/dialog-boxes/Main.c
        gs_projects/tim-loading/Main.c
        gs_projects/collision/Main.c
        gs_projects/camera/Main.c
        gs_projects/multiple-ots/Main.c

        # NON LIBGS Projects
        projects/hello-debug-font/Main.c
        projects/hello-flat-shaded-polys/Main.c
        projects/hello-textured-sprites/Main.c
        projects/hello-controllers/Main.c
        projects/hello-fixed-point-math/Main.c
        lib/header/Controller.h
        lib/source/Controller.c
)
