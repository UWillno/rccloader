#ifndef MAINLOADER_H
#define MAINLOADER_H
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QSettings>
#include <QTranslator>
#include <QJsonObject>
#include <QUrlQuery>
#include "splashscreen.h"
#include <emscripten/bind.h>
#include <emscripten.h>
using namespace emscripten;



class MainLoader : public QObject
{
    Q_OBJECT
    // 当前语言
    Q_PROPERTY(int currentLang READ currentLang WRITE setCurrentLang NOTIFY currentLangChanged FINAL)

public:

    explicit MainLoader(QObject *parent = nullptr);

    enum Langs {zh_CN,en_US};

    int currentLang() const;
    void setCurrentLang(int newCurrentLang);

signals:

    void currentLangChanged();

    void loadRccFinished();

    void loadRccFailed();

public slots:
    // 设置语言
    void setLang(const int lang);
    // 复制
    void copy(QString text);

private slots:
    // 初始化qrc
    void initQrc();
    // 引擎加载
    void engineLoad();

private:
    // 请求的资源链接 wasm本身只是个加载器
    QString resUrl {"https://127.0.0.1/res"};
    QString originUrl {"https://127.0.0.1"};
    // 注册所有类型
    void registerTypes();
    // qml引擎
    QQmlApplicationEngine m_engine;
    // 属性持久化
    QSettings settings;
    // 当前语言
    int m_currentLang;
    // 翻译
    QTranslator translator;
    // 下载rcc/qrc
    void getRcc();

    QByteArray rccData;

    SplashScreen *splash;

    QNetworkAccessManager *manager;
};

#endif // MAINLOADER_H
