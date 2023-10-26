SUMMARY = "A prebuilt base image as baseline for custom work"
require debian-base-image.inc
SECTION = "devel"

APTGET_EXTRA_PACKAGES_REMOVE += " \
    xwayland \
    libgpod-common \
    whoopsie \
    libgles-dev \
    libegl-dev \
    libgl-dev \
    wayland-protocols \
"

APTGET_EXTRA_PACKAGES += " \
    libtinfo5 \
    udhcpc \
    python3.10 \
    libglu1-mesa \
    python3.11 \
    alsa-utils \
    libasound2 \
    libcairo2 \
    libpango1.0-dev \
    libinput-dev \
    libxcursor-dev \
    libxkbcommon-dev \
    libxcb-xfixes0-dev \
    libxcb-composite0-dev \
    libxshmfence-dev \
    libxfont2 \
    libxcvt0 \
    libepoxy0 \
    libtirpc3 \
    libc6-dev \
    fontconfig \
    libogg0 \
    libxv1 \
    libxdamage1 \
"

# GSTREAMER
APTGET_EXTRA_PACKAGES += "\
    liborc-0.4-0 \
    libcairo2-dev \
    libspeex-dev \
    libpng-dev \
    libshout3-dev \
    libjpeg-dev \
    libaa1-dev \
    libflac-dev \
    libdv4-dev \
    libxdamage-dev \
    libxext-dev \
    libxfixes-dev \
    libxml2-dev \
    libxv-dev \
    libx11-xcb-dev \
    libgtk-3-dev \
    libtag1-dev \
    libwavpack-dev \
    libpulse-dev \
    libbz2-dev \
    libjack-jackd2-dev \
    libvpx-dev \
    libmp3lame-dev \
    libmpg123-dev \
    libtwolame-dev \
    libusb-1.0-0 \
    libsbc1 \
"

APTGET_EXTRA_PACKAGES_LAST += " \
    systemd-resolved \
"

custom_install_tasks() {
    # Remove /etc/.pwd.lock to avoid conflicts with perform_groupadd in useradd.bbclass
    if [ -e ${D}${sysconfdir}/.pwd.lock ]; then
        rm -f ${D}${sysconfdir}/.pwd.lock
    fi

    # Add symbolic link to /usr/bin/fc-cache installed by debian to avoid errors
    # in update_font_cache called by fontcache.bbclass which is hardcoded to /libexec/bin/fc-cache

    if [ -e ${D}${bindir}/fc-cache ]; then
        install -d ${D}${libexecdir}/${binprefix}
        cd ${D}${libexecdir}/${binprefix}
        ln -s ../../${bindir#*/}/fc-cache fc-cache
    fi
}

do_install:append() {
    bb.build.exec_func('custom_install_tasks', d)
}
