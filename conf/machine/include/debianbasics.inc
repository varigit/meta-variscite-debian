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

# Turns out that we want the Yocto recipes as default provider, but
# then we must ensure they do not get deployed to avoid pollution of
# the target rootfs.
# Rationale is that Yocto builds need to be internally consistent,
# and we need the sysroots info for Yocto recipes, but the Debian rootfs
# provides things, too. We can't use that for building though, so Yocto
# packages conflicting must not reach the package manager.
# Whenever a Yocto package is installed, all dependencies which are
# to be provided by Debian must be added to the list. Check
# recipe-depends.dot from bitbake -g.
# The respective Debian image must then install the Debian version of
# the recipe.
require conf/machine/include/nodistropackage.inc
DISTRO_NEVER_WRITE_PACKAGE_class-target += "\
	alsa-conf \
	alsa-lib \
	update-alternatives \
	bash \
	base-files \
	bc \
	bison \
	bluez5 \
	bzip2 \
	ca-certificates \
	coreutils \
	db \
	diffutils \
	dpkg \
	dtc \
	flex \
	glibc \
	glibc-locale \
	gcc-runtime \
	gdbm \
	initscripts \
	libc6 \
	libc6-utils \
	libffi \
	libgcc \
	liblzma \
	libpam \
	libsqlite3 \
	libxml2 \
	m4 \
	ncurses \
	netbase \
	opkg-utils \
	perl \
	python3 \
	qemuwrapper-cross \
	shared-mime-info \
	sqlite3 \
	readline \
	util-linux \
	xz \
	zlib \
"

# We have some packages which we do not want to have in a target
# image because they are available as Debian-native packages.
# Given that we can't set a PREFERRED_RPROVIDER in an image recipe
# we need to isolate those packages globally.
DISTRO_NEVER_WRITE_PACKAGE_class-target += "\
	openssl \
	openssl-bin \
	openssl-conf \
	openssl-dev \
"

# All the runtime above should come from debian-base or a derived image
# but only for the target, not for native elements.
# The trick is that we need to retarget everything to one recipe because
# at configuration time we cannot predict algorithmically which derived
# images would add the final apt packages. Any "missing" dependencies
# would only show up then at image build time.
DISTRO_DEFAULT_RPROVIDER_class-target = "debian-base (= ${DEBIAN_TARGET_VERSION})"
