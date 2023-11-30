# We currently support recipies for:
# - Debian Bookworm 12
DEBIAN_TARGET_VERSION ?= "12"

PREFERRED_VERSION_debian-base = "${DEBIAN_TARGET_VERSION}"

# Downconfigure Yocto environment to not pull in local things
# We do not want any default post intercepts or special default
# packages added.
IMAGE_LINGUAS = ""
POKY_DEFAULT_EXTRA_RDEPENDS:remove = "packagegroup-core-boot"
INITRAMFS_IMAGE = ""
#POSTINST_INTERCEPTS_DIR = "<--InVaLiD-->"

# Debian 12: 12.2.0
GCCVERSION = '${@oe.utils.conditional( \
	"DEBIAN_TARGET_VERSION", "12", "12.2.0", \
	"unsupporteddebianversion", \
	d \
)}'

# The Yocto glibc version must not exceed the Desktop glibc version
# or Yocto-created executables will fail on the Desktop image.
# We want to be as close as possible to the respective Desktop glibc
# version to avoid compatibility issues.
# The glibc version for a given Desktop release is a-priori knowledge,
# so the suitable Yocto version can be selected here.
# Note that the glibc is not compiled in Desktop identical fashion.
# It remains a glibc that works for Yocto, which is good enough for
# our purposes.
# Debian 12: max glibc 2.36...
GLIBCVERSION = '${@ \
	oe.utils.conditional("DEBIAN_TARGET_VERSION", "12", "2.35", \
	"unsupporteddebianversion", \
	d \
)}'

PREFERRED_VERSION_cross-localedef-native = "${GLIBCVERSION}"
BINUVERSION="2.40"

# More recent Desktop versions work with symlinks for the base
# directories. We can't refer to symlink based files
DISTRO_FEATURES:append = ' usrmerge'

PREFERRED_PROVIDER_bison-native = "bison"

# This is to be able to pull in the prebuilt Desktop rootfs
MACHINEOVERRIDES =. "debian:"

# When creating images, we would like to preserve any extended
# attributes that are part of our rootfs
IMAGE_CMD_TAR = "tar --xattrs --xattrs-include=*"
do_image_tar[depends] += "tar-replacement-native:do_populate_sysroot"
EXTRANATIVEPATH += "tar-native"
