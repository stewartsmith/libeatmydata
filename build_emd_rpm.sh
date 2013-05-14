#!/bin/bash
#
export PROJECT=eatmydata
#
show_help(){
        echo -e "Wrong ARG: \033[00;31m$1\033[00m"
cat << EOF
        --emd-version  - set VERSION for RPM
        --emd-release  - set RELEASE for RPM
        --rpm-builddir - set RPMBUILD directory
        --sign         - sign built RPM
EOF
}
#
if [ -z "${WORKSPACE}" ]; then
        export WORKSPACE=$(readlink -f $(dirname $0))
fi
#
cd "${WORKSPACE}"
#
while [ $# -gt 0 ]; do
        case $1 in
                --emd-version)
                export EMD_VERSION=$2
                shift 2
                ;;
                --emd-release)
                export EMD_RELEASE=$2
                shift 2
                ;;
                --rpm-builddir)
                export RPMBUILD=$(readlink -f $2)
                shift 2
                ;;
                --sign)
                export SIGN="--sign"
                ;;
                *)
                show_help $1
                exit 1
                ;;
        esac
done        
#
if [ -z "${RPMBUILD}" ]; then
        export RPMBUILD=${HOME}/rpmbuild
fi
#
rm -fr "${RPMBUILD}"
mkdir -p ${RPMBUILD}/{BUILD,RPMS,S{OURCE,PEC,RPM}S}
#
if [ -z "${EMD_VERSION}" ]; then
        export EMD_VERSION=0.1
fi
#
#release variable handling
#
if [ -n "$(which bzr 2>/dev/null)" ]; then
        BZR_REVNO=$(bzr revno)
        if [ $? -eq 0 ]; then
                export BZR_REVNO #bzr tree is valid
        fi
fi
#
[ -z "${BZR_REVNO}" ] && export BZR_REVNO=00 #not a bzr tree or bzr binary not found
#
if [ -z "${BUILD_NUMBER}" ]; then
        export BUILD_NUMBER=00
fi
#
if [ -z "${EMD_RELEASE}" ]; then
        export EMD_RELEASE="${BZR_REVNO}.${BUILD_NUMBER}"
fi
#
#tarball creation
mTMPDIR=/var/tmp/${PROJECT}-${EMD_VERSION}
rm -fr ${mTMPDIR}
mkdir -p ${mTMPDIR}
cp -a ${WORKSPACE}/* ${mTMPDIR}
cd /var/tmp && tar --exclude=.bzr -czf ${RPMBUILD}/SOURCES/${PROJECT}-${EMD_VERSION}.tar.gz ${PROJECT}-${EMD_VERSION}
rm -fr ${mTMPDIR}
#
cd ${WORKSPACE}
#
sed -e "s:@VERSION@:${EMD_VERSION}:g" \
       -e "s:@RELEASE@:${EMD_RELEASE}:" \
       ${WORKSPACE}/${PROJECT}.spec > ${RPMBUILD}/SPECS/${PROJECT}.spec
#
rpmbuild -ba --define "_topdir ${RPMBUILD}" ${RPMBUILD}/SPECS/${PROJECT}.spec











































