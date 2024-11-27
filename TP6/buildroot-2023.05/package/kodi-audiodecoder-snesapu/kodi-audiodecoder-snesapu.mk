################################################################################
#
# kodi-audiodecoder-snesapu
#
################################################################################

KODI_AUDIODECODER_SNESAPU_VERSION = 20.2.0-Nexus
KODI_AUDIODECODER_SNESAPU_SITE = $(call github,xbmc,audiodecoder.snesapu,$(KODI_AUDIODECODER_SNESAPU_VERSION))
KODI_AUDIODECODER_SNESAPU_LICENSE = GPL-2.0+
KODI_AUDIODECODER_SNESAPU_LICENSE_FILES = LICENSE.md
KODI_AUDIODECODER_SNESAPU_DEPENDENCIES = kodi

$(eval $(cmake-package))
