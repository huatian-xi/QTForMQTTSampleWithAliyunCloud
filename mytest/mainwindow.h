#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*
 * 你需要自行搭建QT MQTT环境
 * 使用环境Based on Qt 5.12.1 (MSVC 2015, 32 bit)
 * mqttSDK则使用官方提供的QT5的mqttSDK
 * @huatian-xi&github.com
 */
#include <QMainWindow>

#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLineEdit>

#include <qmqttclient.h>
#include <qmqttmessage.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void connectToMqtt();
    void onPublishClicked();

private:
    Ui::MainWindow *ui;
    QMqttClient *client;
    QPushButton *connectButton;
    QTextEdit *messageEdit;

    QLineEdit *topicLineEdit;
    QTextEdit *messageTextEdit;
    QPushButton *publishButton;


};

#endif // MAINWINDOW_H
