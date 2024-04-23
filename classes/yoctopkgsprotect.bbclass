# This class provides a way to protect ressources/packages provided by Yocto
# to avoid being overriden by the Debian package management system. Therefor
# a fake package is generated which pretend to be the provider for requested
# Debian packages requirements/dependencies.

inherit fakedebpackage

FAKEDEBPKG_NAME = "yoctomegapackage"
FAKEDEBPKG_VERS = "${DEBIAN_TARGET_VERSION}"

FAKEDEBPKG_MAINTAINER = "Variscite Ltd."
FAKEDEBPKG_DESC_SHORT = "Fake package as a placeholder for ressources provided by Yocto"

python create_yoctopkgsprotect_debpackage()  {
    import re
    from oe.rootfs import image_list_installed_packages

    # Yocto, Debian equivalent packages list map. Each Yocto package should have a
    # Debian package equivalent - if it is a standard package. If not, use an empty
    # entry. Beside of the name of the package equivalent the versions needs to be
    # specified to ensure it is considered by the package management resolving the
    # dependencies.
    # 
    # The approach is to review the content of the Yocto package and find the Debian
    # package equivalents using, e.g., apt-file within the target Debian distributon.
    # In some cases a Yocto package can cover multiple Debian packages, this can be
    # considered by adding multiple packages for a Yocto package entry. 
    #
    # In some cases, e.g., for pure library packages there is no need to add a Debian
    # equivalent as they are not going to conflict at runtime. An empty entry can be
    # used instead.
    debpkgs_equivs = {
        'alsa-state': '',
        'alsa-states': '',
        'arm-compute-library': [('libarm-compute-dev','20.08')],
        'bcm43xx-utils': '',
        'bluealsa-aplay': [('bluez-alsa-utils','4.0.0')],
        'bluealsa': [('bluez-alsa-utils','4.0.0')],
        'brcm-patchram-plus': '',
        'debian-base': '',
        'debian-base-dbg': '',
        'debian-base-dev': '',
        'debian-base-doc': '',
        'chromium-ozone-wayland': [('chromium','123.0.6312.122')],
        'eiq-examples': '',
        'ethos-u-driver-stack': '',
        'ethos-u-vela': '',
        'firmware-imx-.*': '',
        'firmware-nxp-wifi-nxp.*': '',
        'freertos-variscite.*': '',
        'gli': '', # empty package
        'glm': '', # empty package
        'gputop': '',
        'gstreamer1.0': [('gstreamer1.0-tools','1.22.0'), ('gir1.2-gstreamer-1.0','1.22.0'), ('libgstreamer1.0-0','1.22.0')],
        'gstreamer1.0-plugins-bad.*': [('gstreamer1.0-plugins-bad','1.22.0')],
        'gstreamer1.0-plugins-base.*': [('gstreamer1.0-plugins-base','1.22.0')],
        'gstreamer1.0-plugins-good.*': [('gstreamer1.0-plugins-good','1.22.0')],
        'gstreamer1.0-rtsp-server.*': [('gstreamer1.0-rtsp','1.22.0')],
        'gtk+3-gles': '', # [('libgtk-3','3.24.38'),('libgtk-3-dev','3.24.38')], Special case: Yocto only provides libgdk-3 and libgtk-3 libs, rest comes from Debian
        'imx-alsa-plugins': '',
        'imx-dsp': '',
        'imx-dsp-codec-ext': '',
        'imx-g2d-samples': '',
        'imx-gpu-sdk': '',
        'imx-gpu-viv': '',
        'imx-gpu-viv-tools': '',
        'imx-gst1.0-plugin-gplay': '',
        'imx-gst1.0-plugin-grecorder': '',
        'imx-gst1.0-plugin': '',
        'imx-lib': '',
        'imx-parser': '',
        'imx-vpu-hantro.*': '',
        'iw612-utils': '',
        'isp-imx.*': '',
        'kernel-6.1.36.*': '',
        'kernel-devicetree': '',
        'kernel-dev': '',
        'kernel-devsrc': [('linux-headers','6.1.36')],
        'kernel-image.*': '',
        'kernel-module.*': '',
        'keyctl-caam': '',
        'lib-aac-dec-arm-elinux3': '',
        'libbz2-1': '', # Yocto: 1.0.4, Debian: 1.0.8, allow co-existence
        'libconfig11': '', # Yocto: libconfig11, Debian libconfig9 (no equiv. upstream package)
        'libdrm2': [('libdrm2','2.4.114')],
        'libegl-mesa': [('libegl-mesa0','22.3.6')],
        'libgbm1': [('libgbm1','22.3.6')],
        '^libg2d\d*(?:-dev)?$': '', # libg2d2, libg2d-dev
        'libglapi0': [('libglapi-mesa','22.3.6-1+deb12u1')],
        'libgles2-mesa': [('libgles2-mesa','22.3.6')],
        'libgst.*': [('libgstreamer-plugins-base1.0-0','1.22.0')], 
        '^libopencv-.*407$': [('libopencv-core406','4.6.0'), ('libopencv-highgui406','4.6.0'), ('libopencv-imgcodecs406','4.6.0'), ('libopencv-imgproc406','4.6.0')], # libopencv-core407, libopencv-highgui407, libopencv-imgcodecs407, libopencv-imgproc407
        'lib-mp3-dec-arm-elinux2': '',
        'lib-oggvorbis-dec-arm-elinux2': '',
        'libgpuperfcnt1.4.0': '',
        'libhantro-vc8000e1': '',
        'librecorder-engine-1.0-0': '',
        'libtensorflow-lite2.11.1': '',
        'libubootenv0': [('libubootenv0.1','0.3.2')],
        'libubootenv-bin': [('libubootenv-tool','0.3.2')],
        'libweston-11-0': [('libweston-10-0','10.0.1')],
        '^lib.*-imx(?:-dev)?$': '', # e.g. libclc-imx, libegl-imx, libgal-imx, libgbm-imx, libgbm-imx-dev ...
        'linux-firmware-ath10k.*': '',
        'linux-firmware-bcm43.*': [('firmware-brcm80211','20230210')],
        'linux-firmware-broadcom-license': '',
        'linux-firmware-cypress-license': '',
        'linux-imx-headers-dev': '',
        'linux-imx-headers': '',
        'mesa-megadriver': [('libgl1-mesa-dri','22.3.6')],
        'nlohmann-json': '',
        'nn-imx': '',
        'nxp-wlan-sdk': '',
        'onnxruntime': [('python3-onnx','1.12.0'), ('libonnx','1.12.0')],
        'onnxruntime-tests': [('libonnx-testdata','1.12.0')],
        'ot-daemon': '',
        'optee-.*': '', # optee-os, optee-test
        'optee-client': [('libckteec0','3.19.0'), ('tee-supplicant','3.19.0')],
        'packagegroup.*': '', # Ignore any packagegroup packages as those are usually empty
        'pm-utils-variscite': '',
        'pugixml': '',
        'python3-fcntl': '',
        'python3-flatbuffers': [('python3-flatbuffers','2.0.8')],
        'python3-protobuf': [('python3-protobuf','3.21.12')],
        'pytorch': [('python3-torch','1.13.1')],
        'rapid.*': '', # rapidjson, rapidopencl, rapidopenvx, rapidvulkan
        'sof-imx': '',
        'sof-zephyr': '',
        'stb': '',
        'spidev-test': '',
        'swupdate': [('swupdate','2022.12'), ('libswupdate0.1','2022.12')],
        'swupdate-www': [('swupdate-www','2022.12')],
        'systemd-gpuconfig': '',
        'tensorflow-lite-ethosu-delegate': '',
        'tensorflow-lite-vx-delegate': '',
        'tim-vx': '',
        'tinycompress': '',
        'torchvision': [('python3-torchvision','0.14.1')],
        'tvm': '',
        'u-boot-splash': '',
        'u-boot-variscite-env': '',
        'udev-extraconf': '',
        'udev-rules-imx': '',
        'usleep': '',
        'var-.*': '',
        'wayland-dev': [('libwayland-dev','1.21.0')],
        'wayland-tools': [('libwayland-bin','1.21.0')],
        'wayland-protocols': [('wayland-protocols','1.31')],
        'wayland': [('libwayland-client0','1.21.0'), ('libwayland-cursor0','1.21.0'), ('libwayland-egl1','1.21.0'), ('libwayland-server0','1.21.0')],
        'weston-examples': [('weston','10.0.1')],
        'weston-init': '',
        'weston': [('weston','10.0.1'), ('libweston-10-0','10.0.1')],
        'weston-xwayland': [('libweston-10-0','10.0.1')],
        'xwayland': [('xwayland','22.1.9')],
        'zephyr-demo-imx': '',
        'zeromq': [('libzmq5','4.3.4')],
    }

    def lookup_debpkg_equiv(yoctopkg_name):
        pkg = yoctopkg_name
        dictionary = debpkgs_equivs

        if pkg in dictionary:
            bb.debug(1, f"Found Debian package equivalent entry for package {pkg}")
            return dictionary[pkg]

        for key, value in dictionary.items():
            regex = re.compile(key)
            if regex.match(pkg):
                bb.debug(1, f"Found Debian package equivalent with {key} for package {pkg}")
                return value

        bb.warn(f"Could not find Debian package equivalent(s) for Yocto package '{pkg}'")
        return None

    workdir = os.path.join(d.expand('${WORKDIR}'), 'yoctopkgsprotect')
    os.makedirs(workdir, exist_ok=True)
    
    yocto_pkg_list = image_list_installed_packages(d)
    debian_pkg_list = []
    for ypkg in yocto_pkg_list:
        dpkgs = lookup_debpkg_equiv(ypkg) or []
        for dpkg in dpkgs:
            if not any(dpkg[0] == pkg[0] for pkg in debian_pkg_list):
                debian_pkg_list.append(dpkg)
    debian_pkg_list = sorted(debian_pkg_list, key=lambda x: x[0])

    if len(debian_pkg_list) <= 0:
        bb.debug(1, f"Skip yoctopackageprotect as there is nothing to protect.")
        return

    # TODO: create a manifest like <Yocto package> -> [<Debian Pacakge equiv 1>, ...]
    with open(f'{workdir}/debian-equiv-list.txt', 'w') as f:
        for pkg_name, version in debian_pkg_list:
            f.write('%s (%s)\n' % (pkg_name, version))

    d.setVar('FAKEDEBPKG_WORKDIR', f'{workdir}')
    d.setVar('FAKEDEBPKG_PROVIDES', f'{", ".join([f"{dpkg} (= {version})" for dpkg, version in debian_pkg_list])}')
    bb.build.exec_func('create_fakedebpackage',d)

    with open(f'{workdir}/aptpreferences', 'w') as f:
        for pkg_name, version in debian_pkg_list:
            f.write('Package: %s\n' % (pkg_name))
            f.write('Pin: release *\n')
            f.write('Pin-Priority: -1\n\n')
}

install_yoctopgsprotect_debpackage() {
	# Install fake package
	install -d ${IMAGE_ROOTFS}${servicedir}/local-apt-repository
	install -m 0644 ${WORKDIR}/yoctopkgsprotect/*.deb ${IMAGE_ROOTFS}${servicedir}/local-apt-repository
	# and configure apt preferences to make sure those package(s) is considered
	install -m 0644 ${WORKDIR}/yoctopkgsprotect/aptpreferences ${IMAGE_ROOTFS}${sysconfdir}/apt/preferences.d/${YOCTO_PKGS_ALIASES_DEBPKG_NAME}
}

python do_yoctopkgs_protect() {
    bb.build.exec_func('create_yoctopkgsprotect_debpackage', d)
    bb.build.exec_func('install_yoctopgsprotect_debpackage', d)
}

create_fake_debpackage[dirs] = "${WORKDIR}/yoctopkgsprotect"
create_fake_debpackage[cleandirs] = "${WORKDIR}/yoctopkgsprotect"
