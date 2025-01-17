SUMMARY = "A prebuilt Desktop Base image as baseline for custom work"
require debian-license.inc
SECTION = "devel"

# Desktop 22.04 baseline
SRC_URI[md5sum] = "1b002df0ac3dd86636f813b802002ad1"
SRC_URI[sha256sum] = "81ff70a85aef8cd7c6a619a51ad781bff4a58ef6ebf9bad69b6360526162691a"

require debian-base.inc

# There are some basic differences between different Desktop versions.
# We try not to address them in the generic recipe
APTGET_EXTRA_PACKAGES += ""

# Desktop 20 unifies things and turns some things into symlinks. We
# solve this with Yocto "usrmerge" but that isn't quite enough.
# We still need to ship the symlinks.
# We also need to remove the udev reference as apparently bitbake.conf
# isn't quite adapted to usrmerge there.
FILES:${PN}:remove = "/lib/udev"
FILES:${PN} += "/bin"
FILES:${PN} += "/sbin"

# The downside of not having the symlink destination content is that we
# are missing a few basic files that are must have for dependencies.
RPROVIDES:${PN}:debian += " \
    /bin/sh \
    /bin/bash \
    /bin/dash \
"

APTGET_EXTRA_PACKAGES += " \
    libc++1-15 \
    libdebuginfod1 \
"

# Extra packages that Desktop will replace and thus enables the use of 
# Yocto packages such as gstreamer to be used in Desktop without package conflitcs
YOCTO-DEPENDS-LIST = " glib-2.0 libglib-2.0-0 libglib-2.0-utils python3-core python3-dev \
                      python3-distutils python3-pickle python3-xml \
                      update-alternatives-opkg \
                      libpam-runtime shadow-base shadow dbus polkit \
                      systemd systemd-dev iso-codes-dev file gptfdisk \
                      shared-mime-info-dev bluez5 dtc e2fsprogs kmod \
                      e2fsprogs-e2fsck e2fsprogs-tune2fs e2fsprogs-badblocks \
                      procps procps-sysctl pstree fuser killall elfutils \
                      fio i2c-tools ifenslave keyutils libkmod2 libelf1 \
                      libasm1 libpci3 libsysfs2 udev \
                      udev-hwdb iproute2-ip iproute2 glibc-utils \
                      memtester minicom mmc-utils pciutils psmisc ptpd \
                      coreutils openssl-conf libcap merge-files upower \
                      openssl libssl1.1 libcrypto1.1 libssl3 libcrypto3 \
                      python3-pyelftools slang zstd libdrm-dev pkgconfig \
                      python3-resource sysfsutils sysklogd sysstat lrzsz \
                      python3-stringold python3-numbers python3-numbers \
                      python3-shell python3-pprint python3-logging \
                      python3-datetime python3-difflib python3-typing \
                      python3-debugger python3-audio python3-codecs \
                      python3-mime python3-mmap python3-threading \
                      python3-ctypes python3-math python3-crypt \
                      python3-email python3-io python3-netclient \
                      python3-asyncio python3-unittest python3-pydoc \
                      python3-misc python3-doctest python3-multiprocessing \
                      python3-compression python3-html python3-netserver \
                      python3-compile python3-json python3-unixadmin \
                      python3-plistlib python3-xmlrpc python2 python3 bash \
                     attr avahi-daemon base-files base-passwd bash-completion consolekit \
                     cracklib cryptodev-linux curl flex-libfl gobject-introspection icu \
                     lame libatomic-ops libattr1 libavahi-client3 libavahi-common3 \
                     libavahi-core7 libavahi-glib1 libavahi-gobject0 libblkid1 \
                     libcairo-gobject2 libcairo-script-interpreter2 libcairo2 libcap-ng0 \
                     libcap2 libdaemon0 libdbus-1-3 libdbus-glib-1-2 \
                     libdrm-radeon libdrm-nouveau libdrm-omap \
                     libdrm-intel libdrm-exynos libdrm-freedreno libdrm-amdgpu libdrm-etnaviv \
                     libexpat1 libfdisk1 libffi7 libfontconfig1 \
                     libfreetype6 libfribidi0 libgcc1 libgdk-pixbuf-2.0-0 libgdk-pixbuf-xlib-2.0-0 \
                     libgmp10 libgmpxx4 libgnutls-openssl27 libgnutls30 libgnutlsxx28 libgudev-1.0-0 \
                     libharfbuzz0 libical libice6 libicudata64 libicui18n64 libicuio64 libicutu64 \
                     libicuuc64 libidn2-0 libltdl7 liblzma5 libmount1 libmp3lame0 \
                     libogg0 liborc-0.4-0 liborc-test-0.4-0 libpam libpciaccess0 \
                     libpcrecpp0 libpcreposix0 libpixman-1-0 libpng16-16 libpsl5 \
                     libreadline8 libsbc1 libsm6 libsmartcols1 libsoup-2.4 libspeex1 \
                     libspeexdsp1 libsqlite3-0 libstdc++6 libsystemd0 libtag-c0 libtag1 \
                     libtheora libticw5 libtool libturbojpeg0 libjpeg-turbo libjpeg62 libudev1 \
                     libunistring2 libuuid1 libvorbis libpulse0 \
                     libpulse-mainloop-glib0 libpulse-simple0 pulseaudio-dev \
                     libx11-6 libx11-xcb1 libxau6 libxcb-composite0 libxcb-damage0 \
                     libxcb-dpms0 libxcb-dri2-0 libxcb-dri3-0 libxcb-glx0 libxcb-present0 \
                     libxcb-randr0 libxcb-record0 libxcb-render0 libxcb-res0 libxcb-screensaver0 \
                     libxcb-shape0 libxcb-shm0 libxcb-sync1 libxcb-xf86dri0 libxcb-xfixes0 \
                     libxcb-xinerama0 libxcb-xinput0 libxcb-xkb1 libxcb-xtest0 libxcb-xv0 \
                     libxcb-xvmc0 libxcb1 libxdamage1 libxdmcp6 libxext6 libxfixes3 libxft2 \
                     libxi6 libxml2 libxrender1 libxtst6 libxv1 libz1 libacl1 libbsd0 libevdev \
                     libffi8 libflac12 libgomp1 libmd0 libpopt0 libmpfr6 libjson-c5 libtinfo5 \
                     libcurl4 libsodium23 libarchive libsoup libmpc3 libmpc libmpc-dev \
                     libncurses5 ncurses-libncurses \
                     mozjs nspr orc linux-libc-headers-dev pango pulseaudio \
                     python3-pycairo python3-pkgutil python3-dbus python3-pygobject \
                     valgrind attr-dev avahi-dev \
                     base-files-dev base-passwd-dev bash-completion-dev \
                     bluez5-dev bzip2-dev ca-certificates consolekit-dev \
                     cracklib-dev cryptodev-linux-dev curl-devflac-dev \
                     flex-dev glib-networking gobject-introspection-dev \
                      icu-dev lame-dev libatomic-dev libatomic-ops-dev \
                      libc6-dev libc6-dbg libcxx libcap-dbg libcap-dev libcap-ng-dev \
                      perl perl-dev libcrypt libcrypt-dbg libcrypt-dev \
                      libdaemon-dev libdbus-glib-1-dev libexpat-dbg libexpat-dev \
                      libffi-dbg libffi-dev libfontconfig-dbg libfontconfig-dev \
                      libfreetype-dbg libfreetype-dev libfribidi-dbg libfribidi-dev \
                      libgcc-s-dbg libgcc-s-dev cpp libgdk-pixbuf-2.0-dbg libgdk-pixbuf-2.0-dev \
                      glib-2.0-dbg glib-2.0-dev libgmp-dbg libgmp-dev libgnutls-dev \
                      libgudev-1.0-dbg libgudev-1.0-dev libical-dev libice-dev libidn2-dbg libidn2-dev \
                      libjpeg-dbg libjpeg-dev libmp3lame-dev libnss-mdns libogg-dbg libogg-dev libpam-dev \
                      libpciaccess-dev libpcre-dbg libpcre-dev libpixman-1-dbg libpixman-1-dev \
                      libpng16-dbg libpng16-dev libpsl-dbg libpsl-dev libpthread-stubs-dev libreadline-dev \
                      libsbc-dbg libsbc-dev libsm-dev libsoup-2.4-dbg libsoup-2.4-dev \
                      libspeex-dbg libspeex-dev libspeexdsp-dev libsqlite3-dev libstdc++-dev \
                      libtag-dbg libtag-dev libtheora-dbg libtheora-dev libtool-dev \
                      libunistring-dbg libunistring-dev libusb-1.0-dbg libusb-1.0-dev \
                      libvorbis-dbg libvorbis-dev libx11-dbg libx11-dev \
                      libxau-dbg libxau-dev libxcb-dbg libxcb-dev libxdamage-dbg libxdamage-dev \
                      libxdmcp-dbg libxdmcp-dev libxext-dbg libxext-dev libxfixes-dbg libxfixes-dev \
                      libxft-dbg libxft-dev libxi-dev libxml2-dbg libxml2-dev libxrender-dbg libxrender-dev \
                      libxtst-dev libxv-dbg libxv-dev libz-dbg libz-dev mozjs-dev \
                      ncurses-terminfo-base ncurses-dev nettle-dbg nettle-dev nspr-dev openssl-dev orc-dev polkit-dev \
                      python3-dbus-dev python3-pycairo-dev python3-pygobject-dev shadow-dev \
                      util-linux-dev util-macros-dev valgrind-dev \
                      wireless-tools xcb-proto-dev xorgproto-dev xtrans-dev \
                     rgb xkeyboard-config xf86-input-libinput libinput10 \
                      libxmu6 libxt6 setxkbmap xkbevd xkbutils xkbprint \
                     xkbcomp xserver-xf86-config mkfontdir mkfontscale font-util \
                     mkfontdir-dev mkfontscale-dev xkeyboard-config-dev encodings-dev \
                     font-util-dev libxkbfile-dev libfontenc-dev libxfont2-dev \
                     libxkbfile1 libcairo-dev alsa-utils-alsactl libasound libasound2 alsa-lib \
                     libx11-locale libclutter-1.0-0 libatk-1.0-0 \
                      alsa-topology-conf alsa-ucm-conf libatopology2 libepoxy0 \
                      libicudata67 libicui18n67 libicuuc67 librsvg-2-2 libsndfile1 \
                      libxshmfence1 python3-nose python3-numpy libcairo-dbg \
                      libharfbuzz-dev libharfbuzz-dbg libpciaccess-dbg pango-dbg pango-dev \
                      librsvg-2-dbg librsvg-2-dev libsndfile-dbg libsndfile-dev \
                      xrandr xrandr-dev libxrandr-dev libxrandr2 libxxf86vm1 \
                      libxxf86vm-dev ibxshmfence-dev mesa-dev \
                     libgdk-pixbuf-2.0-loader-xpm libgdk-pixbuf-2.0-loader-png libgdk-pixbuf-2.0-loader-jpeg libgdk-pixbuf-2.0-loader-gif libgtk-2.0 \
                     libclutter-1.0-dev libclutter-gtk-1.0-0 libclutter-gtk-1.0-dev libcogl-gles2-20 \
                     libcogl-pango20 libcogl-path20 libcanberra libcanberra-dev libcanberra-gtk2 \
                     libcanberra-gtk3 gtk+-dev libepoxy-dev \
                     gsettings-desktop-schemas-dev libxkbcommon \
                     libxcb-util-dev libxcb-util1 libxcomposite-dev libxcomposite1 \
                     libxcursor-dev libxcursor1 libxkbcommon-dev libxshmfence-dev \
                     libjson-glib-1.0-0 libjson-glib-1.0-dev \
                      gsettings-desktop-schemas adwaita-icon-theme adwaita-icon-theme-cursors adwaita-icon-theme-symbolic \
                      hicolor-icon-theme libatk-bridge-2.0-0 libxslt nss \
                      libatspi0 libgtk-3.0 gtk+3 gtk+3-dev gdk-pixbuf \
                      libxfont2-2 libfontenc1 desktop-file-utils \
                      libunwind libwebp libxcvt libzstd1 \
                      usbutils libusb-1.0-0 libdw1 \
                      libgcc libxkbcommon0 libdebuginfod1 \
                      libssl libcrypto wpa-supplicant \
                      libpython3.10 libpython3.11 libpython3 libpython3.11-1.0 bash-dev \
                      libgtk-4.0  util-linux-lsblk util-linux-blkid util-linux-fdisk util-linux \
                      update-rc.d \
                      init-system-helpers-service \
                      at-spi2-core apitrace \
                      fontconfig-utils fonts-liberation \
                      libtirpc3 sed python3-jsonschema python3-idna \
                      python3-2to3 python3-pip python3-sympy python3-attrs \
                      pm-utils kbd python3-certifi python3-cffi python3-chardet python3-cryptography python3-decorator \
                      python3-future python3-ndg-httpsclient python3-pillow python3-ply python3-psutil \
                      python3-pyasn1 python3-pycparser python3-requests python3-six python3-typing-extensions python3-urllib3 \
                      python3-lxml python3-coloredlogs python3-humanfriendly python3-protobuf python3-flatbuffers \
                      perf liberation-fonts nodejs expect bison m4 bc gawk flex numactl mpg123 \
                      gcc-plugins \
                    libgpiod libgpiod2 libgpiod-tools \
                      flatbuffers libflatbuffers2 \
                    libtinyxml2 libtinyxml2-9 libtinyxml2-dev \
                      tiff libtiff6 libassimp5 tbb \
"

RCONFLICTS:${PN} += " ${YOCTO-DEPENDS-LIST} "
RREPLACES:${PN} += " ${YOCTO-DEPENDS-LIST} "
RPROVIDES:${PN} += " ${YOCTO-DEPENDS-LIST} "

RPROVIDES:${PN}:debian += " libglib-2.0 "

do_install[network] = "1"

QA_EMPTY_DIRS = ""

python do_package:append() {
    os.environ['PSEUDO_DISABLED'] = '1'
    d.setVar('ERROR_QA', '')
}
