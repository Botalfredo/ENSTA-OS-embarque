################################################################################
#
# qt5connectivity
#
################################################################################

QT5CONNECTIVITY_VERSION = c5f35f89e975c2ad58c83c6b606d40ba41057f15
QT5CONNECTIVITY_SITE = $(QT5_SITE)/qtconnectivity/-/archive/$(QT5CONNECTIVITY_VERSION)
QT5CONNECTIVITY_SOURCE = qtconnectivity-$(QT5CONNECTIVITY_VERSION).tar.bz2
QT5CONNECTIVITY_INSTALL_STAGING = YES
QT5CONNECTIVITY_SYNC_QT_HEADERS = YES

QT5CONNECTIVITY_LICENSE = GPL-2.0+ or LGPL-3.0, GPL-3.0 with exception(tools), GFDL-1.3 (docs)
QT5CONNECTIVITY_LICENSE_FILES = LICENSE.GPL2 LICENSE.GPL3 LICENSE.GPL3-EXCEPT LICENSE.LGPL3 LICENSE.FDL

ifeq ($(BR2_PACKAGE_QT5BASE_EXAMPLES),y)
QT5CONNECTIVITY_LICENSE += , BSD-3-Clause (examples)
endif

QT5CONNECTIVITY_DEPENDENCIES += $(if $(BR2_PACKAGE_QT5DECLARATIVE),qt5declarative)
QT5CONNECTIVITY_DEPENDENCIES += $(if $(BR2_PACKAGE_BLUEZ5_UTILS),bluez5_utils)
QT5CONNECTIVITY_DEPENDENCIES += $(if $(BR2_PACKAGE_NEARD),neard)

$(eval $(qmake-package))
