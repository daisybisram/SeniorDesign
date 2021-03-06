INCLUDEPATH += $${PWD}

SOURCES     += $${PWD}/gpiocontrol.cpp \
    $$PWD/adccontrol.cpp \
    $$PWD/spicontrol.cpp

HEADERS     += $${PWD}/gpiocontrol.h \
    $$PWD/adccontrol.h \
    $$PWD/daccontrol.h \
    $$PWD/spicontrol.h

FORMS       += $${PWD}/gpiocontrol.ui

RESOURCES   += $${PWD}
