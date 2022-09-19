#ifndef WIDGET_H
#define WIDGET_H

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void receiveReplyGet(QNetworkReply *reply);

    void receiveReplyPost(QNetworkReply *reply);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;

    QNetworkAccessManager *managerGet;
    QNetworkAccessManager *managerPost;

    QNetworkRequest reqGet;
    QNetworkRequest reqPost;

    QNetworkReply *replyGet;
    QNetworkReply *replyPost;
};
#endif // WIDGET_H
