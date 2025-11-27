#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
// 一个简单的加载Widgets，其实可以删了 rcc相对于wasm很小的，早期字体是网络加载才这么写的
class SplashScreen : public QWidget {
    Q_OBJECT
public:
    explicit SplashScreen(QWidget *parent = nullptr);

    void start();
    void failed();

private:
    QProgressBar *progressBar;
    QLabel *status;
    QLabel *title;
    QLabel *copyright;
    void adjustGeo();
    QFont scaledFont(int basePointSize);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // SPLASHSCREEN_H
