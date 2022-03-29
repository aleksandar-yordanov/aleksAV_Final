QT       += core gui charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32-msvc* {
    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS_WINDOWS += $$quote( /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\" )
}

LIBS += kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

CONFIG += c++17

DEFINES -= UNICODE
DEFINES += _MBCS
DEFINES += _CRT_SECURE_NO_WARNINGS

RC_ICONS = avLogo.ico

QMAKE_MACOSX_DEPLOYMENT_TARGET = 12.0

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    C:\Users\alexs\Desktop\QTprojects\asio-1.22.1\include \
    C:\Users\alexs\Desktop\QTprojects\json\single_include\nlohmann

SOURCES += \
    Entropy.cpp \
    PEdump.cpp \
    accountwindow.cpp \
    common.cpp \
    debugwindow.cpp \
    directorytraversal.cpp \
    errorbox.cpp \
    filemanager.cpp \
    filescanner.cpp \
    httprequest.cpp \
    installverifier.cpp \
    license.cpp \
    loginmanager.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    memscanner.cpp \
    privilegeescalation.cpp \
    resetwindow.cpp \
    scanmanager.cpp \
    statsmanager.cpp \
    stringrequest.cpp \
    threattype.cpp \
    versioninformation.cpp

HEADERS += \
    Entropy.h \
    PEdump.h \
    accountwindow.h \
    common.h \
    debugwindow.h \
    directorytraversal.h \
    errorbox.h \
    filemanager.h \
    filescanner.h \
    httprequest.h \
    includes.h \
    installverifier.h \
    license.h \
    loginmanager.h \
    loginwindow.h \
    mainwindow.h \
    memscanner.h \
    privilegeescalation.h \
    resetwindow.h \
    robin_hood.h \
    scanmanager.h \
    statsmanager.h \
    stringrequest.h \
    threadsafequeue.h \
    threattype.h \
    versioninformation.h

FORMS += \
    accountwindow.ui \
    debugwindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    resetwindow.ui \
    versioninformation.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
