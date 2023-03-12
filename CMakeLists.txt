cmake_minimum_required(VERSION 3.24)
project(ps1_dev C)

set(CMAKE_C_STANDARD 99)


include_directories(third_party/psyq-iwyu/include)

add_executable(
        ps1_dev
        main.c

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
        lib/header/Controller.h
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
        lib/source/Controller.c
        lib/source/CollisionHandler.c

        # Projects
        projects/from-scratch/Main.c
        projects/from-scratch/Map.c
        projects/use-tilemaps/Main.c
        projects/use-tilemaps/Map.c
        projects/play-sounds-cdda/Main.c
        projects/play-sounds-libsnd/Main.c
        projects/play-sounds-spu/Main.c
        projects/use-fonts/Main.c
        projects/dialog-boxes/Main.c
        projects/tim-loading/Main.c
        projects/collision/Main.c
        projects/camera/Main.c
        projects/multiple-ots/Main.c
)
