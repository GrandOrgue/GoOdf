PORTNAME=		goodf
# git clone https://github.com/GrandOrgue/GoOdf; cd GoOdf; git describe --tags `git reflog | sed -e 's/ .*//'`
#	v0.15.0-3-g1353c46
DISTVERSIONPREFIX=	v
DISTVERSION=		0.15.0
DISTVERSIONSUFFIX=	-3-g1353c46
PORTREVISION=		16
CATEGORIES=		audio

MAINTAINER=		kwhite@uottawa.ca
COMMENT=		Create and modify GrandOrgue organ definition files
WWW=			https://github.com/GrandOrgue/GoOdf

LICENSE=		GPLv3
LICENSE_FILE=		${WRKSRC}/LICENSE.txt

BUILD_DEPENDS=		ImageMagick7>0:graphics/ImageMagick7 \
			zip>0:archivers/zip

USES=			cmake
USE_WX=			3.2+

USE_GITHUB=		yes
GH_PROJECT=		GoOdf
GH_ACCOUNT=		GrandOrgue
GH_ACCOUNT=		kwhite-uottawa
GH_TAGNAME=		28e51e0

.include <bsd.port.mk>
