APP_NAME = TheRipper

CONFIG += qt warn_on cascades10

QT += network

include(config.pri)

LIBS += -lbb -lbbsystem -lbbdata -lbbnetwork
