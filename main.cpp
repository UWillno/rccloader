/* 只保证基础功能可用
 * 不用多余的文件和类，wasm只是当作一个加载器，
 * 准备所需的模块，所有功能均在QML上实现，不依赖CPP，避免需要重新编译
 * 理论都可以远程，但是确保cors
 */

#include <QResource>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QPushButton>
#include <QScreen>
#ifdef Q_OS_WASM
#include <emscripten.h>
EM_JS(char*, get_search, (), {
    return stringToNewUTF8(window.location.search);
});
#endif


QString rccUrl,entryQml = "qrc:/Main.qml",qmlUrl;
static QByteArray rccData,inlineCode;

void exit(){
    // 突然觉得刷新不太好
    // #ifdef Q_OS_WASM
    //     emscripten_run_script("location.reload()");
    // #else
    //     qApp->exit(-1);
    // #endif
    qApp->exit(-1);
}
void load(QQmlApplicationEngine &engine){

    if(QResource::registerResource(reinterpret_cast<const uchar *>(rccData.constData()))){
        qDebug() << "注册成功";
        engine.load(entryQml);

    }
    else{
        qDebug() << "注册失败";
        exit();
    }
}

void loadFromFile(QQmlApplicationEngine &engine){
    // 全屏容器
    QWidget *overlay = new QWidget();

    QObject::connect(qApp, &QCoreApplication::aboutToQuit,
                     overlay, &QObject::deleteLater);
    // 覆盖整个屏幕
    QRect screenRect = qApp->primaryScreen()->geometry();
    overlay->setGeometry(screenRect);

    // 全屏按钮
    QPushButton *openFileDialogButton = new QPushButton(overlay);
    openFileDialogButton->setGeometry(overlay->rect());
    // 不要用中文，字体建议放rcc/qml中加载
    openFileDialogButton->setText("LoadFromFile");
    QObject::connect(openFileDialogButton, &QPushButton::clicked,[&]{
        QFileDialog::getOpenFileContent("Rcc/Qml (*.qml *.rcc)",
                                        [&engine](const QString &fileName, const QByteArray &fileContent) {
                                            if (fileName.isEmpty()) {
                                                qDebug() << "文件为空";
                                                // No file was selected
                                            } else {
                                                qDebug() << fileName;
                                                if (fileName.endsWith(".rcc")) {
                                                    qDebug() << ".rcc";
                                                    // rcc
                                                    rccData = fileContent;
                                                    load(engine);
                                                } else if (fileName.endsWith(".qml")) {
                                                    // qml
                                                    qDebug() << ".qml";
                                                    inlineCode = fileContent;
                                                    engine.loadData(inlineCode);
                                                }
                                            }
                                        }
                                        );
    });
    overlay->lower();
    overlay->show();

}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // 可以xhr直接请求qrc文件 避免需要写C++
    qputenv("QML_XHR_ALLOW_FILE_READ","1");
#ifdef Q_OS_WASM
    QString query;
    {
        char *c = get_search();
        if (c) {
            query = QString::fromUtf8(c);
            free(c);
        }
    }
    qDebug() << query;
    if (!query.isEmpty() && query != "?"){
        QUrlQuery q(query.slice(1));
        rccUrl   = q.queryItemValue("rcc");
        if(!q.queryItemValue("entry").isEmpty()){
            entryQml = q.queryItemValue("entry");
            entryQml = entryQml.startsWith("qrc:")
                           ? entryQml
                           : QStringLiteral("qrc:/%1").arg(entryQml);
        }
        QString code = q.queryItemValue("code");
        QString b64 = q.queryItemValue("code_b64");
        if(!code.isEmpty()) inlineCode = code.toUtf8() ;
        if(!b64.isEmpty())  inlineCode =  QByteArray::fromBase64(b64.toUtf8());
        qmlUrl = q.queryItemValue("qml");
    }
#endif
    QQmlApplicationEngine engine;
    qDebug() << rccUrl << entryQml << inlineCode;
    // 网络加载
    if(!rccUrl.isEmpty()){
        qDebug() << "从rccUrl加载";
        QNetworkAccessManager *manager = new QNetworkAccessManager(&app);
        QNetworkRequest req((QUrl(rccUrl)));
        auto *reply = manager->get(req);
        QObject::connect(reply, &QNetworkReply::finished, &engine, [&]() {
            if (reply->error() != QNetworkReply::NoError) {
                qDebug() << "请求异常";
                exit();
                reply->deleteLater();
                return;
            }
            rccData = reply->readAll();
            load(engine);
            reply->deleteLater();
        });
    }else if(!qmlUrl.isEmpty()){
        qDebug() << "从qmlUrl加载";
        QNetworkAccessManager *manager = new QNetworkAccessManager(&app);
        QNetworkRequest req((QUrl(qmlUrl)));
        auto *reply = manager->get(req);
        QObject::connect(reply, &QNetworkReply::finished, &engine, [&]() {
            if (reply->error() != QNetworkReply::NoError) {
                qDebug() << "请求异常";
                exit();
                reply->deleteLater();
                return;
            }
            inlineCode = reply->readAll();
            engine.loadData(inlineCode);
            reply->deleteLater();
        });
    }else if(!inlineCode.isEmpty()){
        // 从代码加载
        engine.loadData(inlineCode);
    }else {
        // 从文件加载 wasm限制，建议有个交互
        loadFromFile(engine);
    }

    return app.exec();
}
