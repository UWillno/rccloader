# 添加你需要的模块，这只是我用过的 后续版本可能还会减少或增加，
QT += quick widgets quickcontrols2 network graphs websockets sql gui core qml
# 看文档加模块
# https://doc.qt.io/qt-6/wasm.html
# 要用多线程就加上 concurrent
# QT += multimedia 单线程可能还没加回来 ?
# charts 6.10之后被弃用，而且不能与graphs同时存在 链接会异常据说Qt7修复
wasm {
CONFIG(release, debug|release){
DEFINES += QT_NO_DEBUG_OUTPUT
QMAKE_CXXFLAGS += -Oz -flto
QMAKE_LFLAGS += -flto
}
}


CONFIG += c++17 cmdline
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp

HEADERS +=

# RESOURCES += \
#     import.qrc

# DISTFILES += import.qml
