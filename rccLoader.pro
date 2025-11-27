
# 添加你需要的模块，这只是我用过的 后续版本可能还会减少或增加，
QT += quick sql widgets multimedia quickcontrols2 graphs websockets
# 要用多线程就加上 concurrent
# charts 6.10之后被弃用，而且不能与graphs同时存在
CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainloader.cpp \
    splashscreen.cpp

HEADERS += \
    mainloader.h \
    splashscreen.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    font.qrc
