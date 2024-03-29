# A class to create fake package, based on the approach of the Debian package
# equivs (https://packages.debian.org/bookworm/equivs). It will create a Debian
# package without any "real" content, but instead a package that can satisfy 
# package dependencies.

FAKEDEBPKG_NAME ?= "fakedebpackage"
FAKEDEBPKG_VERS ?= "1.0"
FAKEDEBPKG_ARCH ?= "all"
FAKEDEBPKG_DEPENDS ?= ""
FAKEDEBPKG_PROVIDES ?= ""
FAKEDEBPKG_REPLACES ?= ""
FAKEDEBPKG_CONFLICTS ?= ""
FAKEDEBPKG_MAINTAINER ?= ""

FAKEDEBPKG_DESC_SHORT ?= "Dummy package to fulfill package dependencies"
FAKEDEBPKG_DESC_LONG ?= "\
 This is a dummy package that makes Debian's package management\n\
 system believe that equivalents to packages on which other\n\
 packages depend on are actually installed.  Deinstallation of\n\
 this package is only possible when all pending dependency issues\n\
 are properly resolved in some other way.\n\
 .\n\
 Please note that this is a crude hack and if thoughtlessly used\n\
 might possibly do damage to your packaging system. And please\n\
 note as well that using it is not the recommended way of dealing\n\
 with broken dependencies. It is better to file a bug report.\n\
"
FAKEDEBPKG_README ?= "\
${FAKEDEBPKG_NAME} for Debian\n\
\n\
\n\
This is a dummy package that makes Debian's package management\n\
system believe that equivalents to packages on which other\n\
packages do depend on are actually installed.\n\
\n\
The special dependencies used in this package are:\n\
Depends: ${FAKEDEBPKG_DEPENDS}\n\
Provides: ${FAKEDEBPKG_PROVIDES}\n\
Replaces: ${FAKEDEBPKG_REPLACES}\n\
Conflicts: ${FAKEDEBPKG_CONFLICTS}\n\
\n\
Please note that this is a crude hack and if thoughtlessly used\n\
might possibly do damage to your packaging system. And please\n\
note as well that using it is not the recommended way of dealing\n\
with broken dependencies. Better file a bug report instead.\n\
\n\
Deinstallation of this package is only possible when all pending\n\
dependency issues are properly resolved in some other way. A more\n\
brutal approach for it's deinstallation is to create and install\n\
the package configured using an empty control file.\n\
"
FAKEDEBPKG_CHANGELOG ?= "\
${FAKEDEBPKG_NAME} (${FAKEDEBPKG_VERS}) unstable; urgency=low\n\
\n\
  * First version\n\
\n\
 -- ${FAKEDEBPKG_MAINTAINER}  Tue, 26 Mar 2024 15:50:02 +0100\n\
"
FAKEDEBPKG_COPYRIGHT ?= "\
Format: https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/\n\
Source: /usr/share/equivs/template/debian/copyright\n\
\n\
Files: *\n\
Copyright: 2024 ${FAKEDEBPKG_MAINTAINER}\n\
License: GPL-2+\n\
\n\
License: GPL-2+\n\
 This program is free software; you can redistribute it and/or modify\n\
 it under the terms of the GNU General Public License as published by\n\
 the Free Software Foundation; either version 2 of the License, or (at\n\
 your option) any later version.\n\
 .\n\
 This program is distributed in the hope that it will be useful, but\n\
 WITHOUT ANY WARRANTY; without even the implied warranty of\n\
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n\
 General Public License for more details.\n\
 .\n\
 On Debian systems, the complete text of the GNU General Public\n\
 License, version 3 can be found in \"/usr/share/common-licenses/GPL-2\".\n\
"

DEPENDS += " dpkg-native"

python create_fakedebpackage() {
    import os;
    import subprocess;

    dpgk_name = d.getVar('FAKEDEBPKG_NAME', True)
    bb.debug(1, f"Creating fakedebpackage '{dpgk_name}'")

    workdir = d.getVar('FAKEDEBPKG_WORKDIR') or d.getVar('WORKDIR')

    controldir = os.path.join(workdir, 'packagedata/DEBIAN')
    os.makedirs(controldir, exist_ok=True)
    with open(f'{controldir}/control', 'w') as f:
        f.write("Package: %s\n" % (dpgk_name))
        dpkg_vers = d.getVar('FAKEDEBPKG_VERS', True)
        f.write("Version: %s\n" % (dpkg_vers))
        dpkg_maintainer = d.getVar('FAKEDEBPKG_MAINTAINER', True)
        f.write("Maintainer: %s\n" % (dpkg_maintainer))
        dpkg_arch = d.getVar('FAKEDEBPKG_ARCH', True)
        f.write("Architecture: %s\n" % (dpkg_arch))
        dep_fields = ['Depends', 'Provides', 'Replaces', 'Conflicts']
        for field in dep_fields:
            content = d.getVar(f'FAKEDEBPKG_{field.upper()}', True)
            if content:
                f.write(f"{field}: %s\n" % (content))
        dpkg_desc_short = d.getVar('FAKEDEBPKG_DESC_SHORT', True)
        dpkg_desc_long = d.getVar('FAKEDEBPKG_DESC_LONG', True)
        f.write("Description: %s\n%s\n" % (
            dpkg_desc_short.encode().decode('unicode_escape'),
            dpkg_desc_long.encode().decode('unicode_escape')))

    docdir = os.path.join(workdir, f'packagedata/usr/share/doc/{dpgk_name}')
    os.makedirs(docdir, exist_ok=True)
    pkgdata_files = ['README', 'copyright', 'changelog']
    for file in pkgdata_files:
        with open(f'{docdir}/{file}', 'w') as f:
            text = d.getVar(f'FAKEDEBPKG_{file.upper()}', True)
            f.write(text.encode().decode('unicode_escape'))

    subprocess.check_output("PATH=\"%s\" %s -b %s %s" % (d.getVar("PATH"), 'dpkg-deb', workdir + '/packagedata', workdir),
                            stderr=subprocess.STDOUT,
                            shell=True)
}
