TYPE = ps-exe

THIS_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

SRCS += $(THIS_DIR)third_party/nugget/common/crt0/crt0.s \
$(THIS_DIR)lib/source/AssetManager.c \
$(THIS_DIR)lib/source/CdReader.c \
$(THIS_DIR)lib/source/CollisionHandler.c \
$(THIS_DIR)lib/source/Controller.c \
$(THIS_DIR)lib/source/Frames.c \
$(THIS_DIR)lib/source/GameObject.c \
$(THIS_DIR)lib/source/GPUBase.c \
$(THIS_DIR)lib/source/JSONParser.c \
$(THIS_DIR)lib/source/Logger.c \
$(THIS_DIR)lib/source/TextBox.c \
$(THIS_DIR)lib/source/Tiled.c \
$(THIS_DIR)lib/source/TileFetcher.c \
$(THIS_DIR)lib/source/TxtHandler.c \

CPPFLAGS += -I$(THIS_DIR)third_party/psyq-iwyu/include -I$(THIS_DIR)lib/header
LDFLAGS += -L$(THIS_DIR)third_party/psyq/lib
LDFLAGS += -Wl,--start-group
LDFLAGS += -lapi
LDFLAGS += -lc
LDFLAGS += -lc2
LDFLAGS += -lcard
LDFLAGS += -lcd
LDFLAGS += -lcomb
LDFLAGS += -lds
LDFLAGS += -letc
LDFLAGS += -lgpu
LDFLAGS += -lgs
LDFLAGS += -lgte
LDFLAGS += -lgun
LDFLAGS += -lhmd
LDFLAGS += -lmath
LDFLAGS += -lmcrd
LDFLAGS += -lmcx
LDFLAGS += -lpad
LDFLAGS += -lpress
LDFLAGS += -lsio
LDFLAGS += -lsnd
LDFLAGS += -lspu
LDFLAGS += -ltap
LDFLAGS += -Wl,--end-group

include $(THIS_DIR)third_party/nugget/common.mk