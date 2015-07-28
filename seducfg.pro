#QMAKE_CXX	= "ccache $$QMAKE_CXX"

QT		+= core gui widgets serialport network

SOURCES		+= mainwindow.cpp about.cpp donation.cpp
HEADERS		+= mainwindow.h   about.h   donation.h

FORMS		+= res/ui/mainwindow.ui res/ui/about.ui res/ui/donation.ui
RESOURCES	+= res/seducfg.qrc

TRANSLATIONS	+= lng/seducfg_de.ts

unix:!macx {
QMAKE_LFLAGS	+= -Wl,-rpath=.
}

win32 {
RC_ICONS	+= res/ico/app.ico
}

macx {
ICON		+= res/ico/app.icns
}
