TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

# Added 
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lpthread -lSDL2


