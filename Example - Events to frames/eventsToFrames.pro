TEMPLATE = app

QT += qml quick
CONFIG += c++11

HEADERS += /usr/local/include/chameleon/backgroundCleaner.hpp \
           /usr/local/include/chameleon/changeDetectionDisplay.hpp \
           /usr/local/include/chameleon/logarithmicDisplay.hpp \
           /usr/local/include/chameleon/frameGenerator.hpp
SOURCES += main.cpp
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lpthread
RESOURCES += qml.qrc
