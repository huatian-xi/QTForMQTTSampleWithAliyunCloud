#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化UI组件
    connectButton = new QPushButton("连接到MQTT", this);
    messageEdit = new QTextEdit(this);
    messageEdit->setReadOnly(true);
    messageEdit->setPlaceholderText(tr("在此接收订阅topic"));


    publishButton = new QPushButton("发布", this);
    topicLineEdit = new QLineEdit(this);
    topicLineEdit->setPlaceholderText(tr("在此输入主题"));
    messageTextEdit = new QTextEdit(this);
    messageTextEdit->setPlaceholderText(tr("在此输入消息"));

    QVBoxLayout *layout = new QVBoxLayout(ui->centralWidget);
    layout->addWidget(connectButton);
    layout->addWidget(messageEdit);
    layout->addWidget(publishButton);
    layout->addWidget(topicLineEdit);
    layout->addWidget(messageTextEdit);

    // 设置MQTT客户端
    client = new QMqttClient(this);
    client->setHostname("iot-yours.mqtt.iothub.aliyuncs.com");
    client->setPort(1883);
    client->setClientId("ProductKey.ProductName|securemode=114514,signmethod=1919810,timestamp=114514|");
    client->setUsername("ProductName&ProductKey");
    client->setPassword("yours password 1145141919810");

    // 连接信号和槽
    connect(client, &QMqttClient::connected, this, &MainWindow::onConnected);
    connect(client, &QMqttClient::disconnected, this, &MainWindow::onDisconnected);
    connect(client, &QMqttClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectToMqtt);
    connect(publishButton, &QPushButton::clicked, this, &MainWindow::onPublishClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToMqtt()
{
    if (client->state() == QMqttClient::Disconnected) {
        client->connectToHost();
    } else {
        client->disconnectFromHost();
    }
}

void MainWindow::onConnected()
{
    ui->statusBar->showMessage("MQTT 代理已连接");
    connectButton->setText("断开连接");
    client->subscribe(QMqttTopicFilter("your/topic"));
}


void MainWindow::onDisconnected()
{
    ui->statusBar->showMessage("MQTT 代理已断开");
    connectButton->setText("连接到MQTT");
}

void MainWindow::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    // 使用 QMqttTopicName 的 name() 方法获取主题名称
    QString topicNameStr = topic.name(); // 获取主题名称
    QString payloadStr = QString::fromUtf8(message);

    messageEdit->append(QString("已收到来自主题: %1\r\n有效消息载荷: %2")
                        .arg(topicNameStr)
                        .arg(payloadStr));
}


void MainWindow::onPublishClicked()
{
    QString topic = topicLineEdit->text();
    QString messageContent = messageTextEdit->toPlainText();

    // 将消息内容转换为 QByteArray
    QByteArray message = messageContent.toUtf8();

    // 定义主题和消息的QoS等级
    QMqttTopicName qTopic(topic);
    quint8 qos = 0; // 根据需要设置 QoS

    // 发布消息
    qint32 messageId = client->publish(qTopic, message, qos, false);

    if (messageId == -1) {
        // 处理错误
        qDebug() << "消息发布失败";
    } else {
        // 可以在这里显示消息ID或者成功的消息
        qDebug() << "消息已发布 使用 ID:" << messageId;
    }
}
