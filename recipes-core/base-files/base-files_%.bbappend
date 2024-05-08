FILES:${PN}-issue = "${sysconfdir}/issue \
                     ${sysconfdir}/issue.net"

PROVIDES += "${PN}-issue"
PACKAGES =+ "${PN}-issue"

RDEPENDS:${PN}-issue = ""
