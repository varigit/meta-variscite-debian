# Fix torchvision gcc runtime dependencies by using the previous
# version. This should be fixed within the next releases.
PYTORCH_SRC:var-som ?= "git://github.com/nxp-imx/pytorch-release.git;protocol=https"
SRCBRANCH:var-som = "lf-6.1.22_2.0.0"
SRCREV:var-som = "6948822f1d3e6cdbbd067b9e21885b697a70f18a" 
