TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    mygraphlib.cpp \
    myutils.cpp \
    geompack.cpp \
    cppalgorithm.cpp

HEADERS += \
    mygraphlib.h \
    myutils.h \
    geompack.h \
    thirdpartprograms.h \
    main.h \
    cppalgorithm.h
