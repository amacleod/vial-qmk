VIA_ENABLE = yes
VIAL_ENABLE = yes
#VIAL_INSECURE = yes
ifeq ($(strip $(ENCODER_ENABLE)), yes)
  ENCODER_MAP_ENABLE = yes
  VIAL_ENCODERS_ENABLE = yes
endif
ifeq ($(strip $(RGB_MATRIX_ENABLE)), yes)
   VIALRGB_ENABLE := yes  # not required, but enabling for mouse button keys
endif
LTO_ENABLE = yes
QMK_SETTINGS = no

# Board rules.mk sets AUDIO_ENABLE = no unconditionally, so the later
# `AUDIO_ENABLE ?= yes` there never takes effect. Override it here instead
# of touching Sadek's file -- piezo is populated on this board.
AUDIO_ENABLE = yes
