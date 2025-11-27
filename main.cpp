#include "mainloader.h"

#include <QApplication>
#include <QCoreApplication>

const QString OrganizationName = "xxxxx";
const QString OrganizationDomain = "xxxx.com";
const QString ApplicationName = "xxxxx's Blog";
const QString ApplicationVersion = "1.0";
// QApplication *g_app;
// MainLoader *loaderPtr;


int main(int argc, char *argv[])
{
    // 如果能启动优先用这个 官方建议 近期版本我启动不了或有异常
    // g_app = new QApplication(argc, argv);
    // QCoreApplication::setOrganizationName(OrganizationName);
    // QCoreApplication::setOrganizationDomain(OrganizationDomain);
    // QCoreApplication::setApplicationName(ApplicationName);
    // QCoreApplication::setApplicationVersion(ApplicationVersion);
    // loaderPtr = new MainLoader();  // MainLoader loader;
    // return 0;

    // QT标准启动
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(OrganizationName);
    QCoreApplication::setOrganizationDomain(OrganizationDomain);
    QCoreApplication::setApplicationName(ApplicationName);
    QCoreApplication::setApplicationVersion(ApplicationVersion);
    MainLoader loader;
    return app.exec();
}
