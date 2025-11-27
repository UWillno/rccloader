#include "mainloader.h"

#include "qclipboard.h"
#include "splashscreen.h"

#include <emscripten/bind.h>
#include <QFontDatabase>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QResource>
#include <QQmlContext>
#include <emscripten.h>
using namespace emscripten;

EM_JS(char*, get_origin, (), {
    var url = window.location.origin;
    return stringToNewUTF8(url);
});
EM_JS(char*, get_page_url, (), {
    var url = window.location.search;
    return stringToNewUTF8(url);
});

MainLoader::MainLoader(QObject *parent)
    : QObject{parent}
{
    // 有JSPI时可以启用
    // QSettings::setDefaultFormat(QSettings::WebIndexedDBFormat);
    manager = new QNetworkAccessManager(this);
    splash = new SplashScreen();
    // 加载中文字体
    int fontId = QFontDatabase::addApplicationFont(":/MiSans-Regular.ttf");
    if (fontId != -1) {
        qDebug() << "字体加载成功";
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(family);
        qApp->setFont(font);
    }

    if(settings.value("Lang").isValid()){
        m_currentLang = settings.value("Lang").toInt();
    }else{
        m_currentLang = 0;
    }
    connect(this,&MainLoader::loadRccFinished,this,[&]{
        splash->close();
        splash->deleteLater();
        initQrc();
        registerTypes();
        engineLoad();
    });
    connect(this,&MainLoader::currentLangChanged,this,[&]{
        switch (m_currentLang) {
        case en_US:{
            qApp->installTranslator(&translator);
            break;
        }
        default:{
            qApp->removeTranslator(&translator);
            break;
        }
        }
        m_engine.retranslate();
    });
    connect(this,&MainLoader::loadRccFailed,this,[&]{
        splash->failed();
    });
    getRcc();
}

void MainLoader::initQrc()
{
    // 如果qrc有字体可以这里加载
    // 加载翻译
    (void)translator.load(":/xxx.qm");
}

void MainLoader::engineLoad()
{
    switch (m_currentLang) {
    case en_US:{
        qApp->installTranslator(&translator);
        break;
    }
    default:{
        qApp->removeTranslator(&translator);
        break;
    }
    }
    m_engine.load("qrc:/main.qml");
}



void MainLoader::setLang(const int lang)
{
    setCurrentLang(lang);
}

void MainLoader::registerTypes()
{
    m_engine.rootContext()->setContextProperty("MainLoader",this);
    // 资源路径
    m_engine.rootContext()->setContextProperty("RESURL",resUrl);
    m_engine.rootContext()->setContextProperty("OriginUrl",originUrl);
    // 注册别的cpp对象

}

void MainLoader::getRcc()
{
    splash->show();
    QFontDatabase::addApplicationFont(":/ttf/MiSans-Regular.ttf");
    // 可以固定死，我是因为有多个镜像站，固定死就没意义了
    char *str = get_origin();
    originUrl = QString(str);
    free(str);
    resUrl = originUrl + "/res"; // 资源目录
    const QString RCCURL = resUrl + "/你的qml.rcc";

    QNetworkReply *reply =  manager->get(QNetworkRequest(QUrl(RCCURL)));
    QObject::connect(reply, &QNetworkReply::finished, this, [=]() {
            rccData = reply->readAll();
            if(QResource::registerResource(reinterpret_cast<const uchar *>(rccData.constData()))){
                emit loadRccFinished();
            }else{
                emit loadRccFailed();
            }
            reply->deleteLater();
        });

}

void MainLoader::copy(QString text)
{
    qApp->clipboard()->setText(text);
}


int MainLoader::currentLang() const
{
    return m_currentLang;
}

void MainLoader::setCurrentLang(int newCurrentLang)
{
    if (m_currentLang == newCurrentLang)
        return;
    m_currentLang = newCurrentLang;
    settings.setValue("Lang",m_currentLang);
    emit currentLangChanged();
}

