DESCRIPTION = "Variscite wallpapers"
LICENSE="MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	file://wallpaper.png \
	file://wallpaper_hd.png \
"

do_install() {
	mkdir -p ${D}${datadir}/images/desktop-base/
	install -m 0644 ${WORKDIR}/wallpaper_hd.png ${D}${datadir}/images/desktop-base/default
}

FILES:${PN} = " \
	${datadir}/images/desktop-base/* \
"

COMPATIBLE_MACHINE ="(.*debian)"
