#include "splashscreen.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QMovie>
#include <QApplication>
#include <QScreen>
#include <QPainterPath>

SplashScreen::SplashScreen(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowOpacity(0.9);
    setMinimumSize(1,1);
    connect(qApp->primaryScreen(),&QScreen::geometryChanged,this,[&]{
        adjustGeo();
    });
    title = new QLabel("XXXXXXXXXXX", this);
    title->setStyleSheet("color: white;");
    title->setAlignment(Qt::AlignLeft);
    // 早期我中文字体也是网络请求的，所以用的英文，现在字体编译进wasm了所以可以用中文
    status = new QLabel("加载中……", this);
    status->setStyleSheet("color: white;");
    status->setAlignment(Qt::AlignLeft);
    progressBar = new QProgressBar(this);
    progressBar->setTextVisible(false);
    progressBar->setRange(0, 0);  // 非确定进度条
    progressBar->setFixedHeight(4);
    progressBar->setStyleSheet("QProgressBar::chunk { background-color: #66CCFF; }");
    copyright = new QLabel("©版权相关文字", this);
    copyright->setStyleSheet("color: white;");
    copyright->setAlignment(Qt::AlignRight);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addWidget(status);
    layout->addWidget(progressBar);
    layout->addStretch();
    layout->addWidget(copyright);
    layout->setContentsMargins(30, 30, 30, 20);
    setLayout(layout);
    adjustGeo();
}

void SplashScreen::failed()
{
    status->setText("资源加载失败!");
    status->setStyleSheet("color: red;");
    progressBar->setVisible(false);
}

void SplashScreen::adjustGeo()
{
    auto screen = qApp->primaryScreen();
    auto screenGeo = screen->geometry();
    auto size = screenGeo.size();
    qreal height,width;
    if(screen->isLandscape(screen->orientation())){
        height = screenGeo.height() / 2;
        width = height / 0.5;
    }else{
        width = screenGeo.width() * 0.9;
        height = width *0.5;
    }
    qreal x = (size.width() - width)/2;
    qreal y = (size.height() - height)/2;
    setGeometry(x,y,width,height);
    title->setFont(scaledFont(64));
    status->setFont(scaledFont(44));
    copyright->setFont(scaledFont(24));
}

QFont SplashScreen::scaledFont(int basePointSize) {
    qreal dpi = qApp->primaryScreen()->logicalDotsPerInch();
    qreal dpiScale = dpi / 96.0;
    int windowWidth = this->width();
    qreal widthScale = qreal(windowWidth) / 1280.0;
    qreal finalScale = widthScale / dpiScale;
    finalScale = qBound(0.3, finalScale, 2.0);
    QFont font;
    font.setPointSizeF(basePointSize * finalScale);
    return font;
}

void SplashScreen::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(this->rect(), 10, 10);

    painter.fillPath(path, QColor(29,28,40));
}
