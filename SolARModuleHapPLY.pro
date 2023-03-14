QT       -= core gui
CONFIG -= app_bundle qt

QMAKE_PROJECT_DEPTH = 0

TARGET = SolARModuleHapPLY
FRAMEWORK = $${TARGET}
INSTALLSUBDIR =  SolARBuild
VERSION=1.0.0
DEFINES +=  $${TARGET}VERSION=\"$${VERSION}\"

# Uncomment following line to add more verbose information from builddefs-qmake rules
# CONFIG += verbose
# Uncomment following line to prepare remaken package
# CONFIG += package_remaken

CONFIG += with_qtvs

with_qtvs {
    PROJECTCONFIG = QTVS
}

CONFIG += c++1z
CONFIG += shared

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG=1
    DEFINES += DEBUG=1
}

CONFIG(release,debug|release) {
    DEFINES += _NDEBUG=1
    DEFINES += NDEBUG=1
}

include(findremakenrules.pri)

DEPENDENCIESCONFIG = static install_recurse

## Configuration for Visual Studio to install binaries and dependencies. Work also for QT Creator by replacing QMAKE_INSTALL
PROJECTCONFIG = QTVS

#NOTE : CONFIG as staticlib or sharedlib, DEPENDENCIESCONFIG as staticlib or sharedlib and PROJECTDEPLOYDIR MUST BE DEFINED BEFORE templatelibbundle.pri inclusion
include ($${QMAKE_REMAKEN_RULES_ROOT}/templatelibconfig.pri)

INCLUDEPATH += interfaces

include (SolARModuleHapPLY.pri)

unix {
    # Avoids adding install steps manually. To be commented to have a better control over them.
    QMAKE_POST_LINK += "make install install_deps"
}

macx {
    DEFINES += _MACOS_TARGET_
    QMAKE_MAC_SDK= macosx
    QMAKE_CFLAGS += -mmacosx-version-min=10.7 #-x objective-c++
    QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -std=c++17 -fPIC#-x objective-c++
    QMAKE_LFLAGS += -mmacosx-version-min=10.7 -v -lstdc++
    LIBS += -lstdc++ -lc -lpthread
}

win32 {
    DEFINES += _X86_VC12_TARGET_
    DEFINES += MBCS _MBCS
 }

header_files.path = $${PROJECTDEPLOYDIR}/interfaces/
header_files.files = $$files($${PWD}/I*.h)

INSTALLS += header_files
DISTFILES +=     Makefile

OTHER_FILES += packagedependencies.txt

with_qtvs {
#NOTE : Must be placed at the end of the .pro
    include ($${QMAKE_REMAKEN_RULES_ROOT}/remaken_install_target.pri)
}
