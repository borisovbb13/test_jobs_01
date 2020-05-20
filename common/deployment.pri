EOL = $$escape_expand(\n\t)

win32:        BUILD_BITS = "win"
else:mac:     BUILD_BITS = "mac"
else:android: BUILD_BITS = "android"
else:unix:    BUILD_BITS = "linux"
else:         BUILD_BITS = "unknown"

BUILD_BITS = $$join(BUILD_BITS,,,-$${QT_ARCH})

DESTDIR = $$shell_path($$absolute_path($$PWD/../bin))

build_pass:CONFIG(debug, debug|release) {
    BUILD_BITS = $$join(BUILD_BITS,,,d)
    DESTDIR = $$join(DESTDIR,,,_debug)
} else {
    DESTDIR = $$join(DESTDIR,,,_release)
}

TARGET = $$join(TARGET,,,-$${BUILD_BITS})

DEFINES *= BUILD_BITS=\\\"$${BUILD_BITS}\\\"

MOC_DIR     = $$shell_path($$absolute_path($$PWD/../build/$${TARGET}))
OBJECTS_DIR = $$shell_path($$absolute_path($$PWD/../build/$${TARGET}))
RCC_DIR     = $$shell_path($$absolute_path($$PWD/../build/$${TARGET}))
UI_DIR      = $$shell_path($$absolute_path($$PWD/../build/$${TARGET}))

INCLUDEPATH += $$shell_path($$absolute_path($$PWD/../include))
INCLUDEPATH += $$shell_path($$absolute_path($$PWD/..))

LIBS += -L$$shell_path($$absolute_path($$PWD/../lib))
LIBS += -L$$DESTDIR

win32 {
    equals(TEMPLATE, "lib") {
        TARGET_EXT = ".dll"
    } else {
        TARGET_EXT = ".exe"
    }

    !equals(TARGET, aidriller_common-$${BUILD_BITS}) {
        QMAKE_POST_LINK += $$shell_quote($$shell_path($$[QT_INSTALL_BINS]/windeployqt))

        contains(QT, sql):             QMAKE_POST_LINK += -sql
        contains(QT, network):         QMAKE_POST_LINK += -network
        contains(QT, opengl):          QMAKE_POST_LINK += -opengl
        contains(QT, qml):             QMAKE_POST_LINK += -qml
        contains(QT, quick):           QMAKE_POST_LINK += -quick
        contains(QT, widgets):         QMAKE_POST_LINK += -widgets
        contains(QT, quickwidgets):    QMAKE_POST_LINK += -quickwidgets
        contains(QT, qml) | contains(QT, quick) | contains(QT, quickwidgets): QMAKE_POST_LINK += --qmldir $$[QT_INSTALL_QML]

        QMAKE_POST_LINK += $$shell_quote($$shell_path($${DESTDIR}/$${TARGET}$${TARGET_EXT})) $$EOL

        contains(QT, charts) {
            build_pass:CONFIG(debug, debug|release) {
                QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_quote($$shell_path($$[QT_INSTALL_BINS]/Qt5Chartsd.dll)) $$shell_quote($$shell_path($${DESTDIR})) $$EOL
            } else {
                QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_quote($$shell_path($$[QT_INSTALL_BINS]/Qt5Charts.dll)) $$shell_quote($$shell_path($${DESTDIR})) $$EOL
            }

            QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_quote($$shell_path($$[QT_INSTALL_QML]/QtCharts)) $$shell_quote($$shell_path($${DESTDIR})) $$EOL
        }
    }
}

QMAKE_TARGET_COMPANY = "Undefined"
QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2020 Boris B. Borisov"
