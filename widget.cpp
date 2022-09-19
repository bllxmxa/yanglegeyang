#include "widget.h"
#include "ui_widget.h"

#include <QIntValidator>
#include <QMessageBox>


QString uid;
QString avatar;
int status;
int num = 0;
int errnum = 0;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    ui->textEdit_2->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);

    ui->lineEdit_2->setValidator(new QIntValidator(1,1000000000,this));  // [1,1000]范围内的整数输入

    managerGet = new QNetworkAccessManager(this);
    managerPost = new QNetworkAccessManager(this);
    connect(managerGet, &QNetworkAccessManager::finished, this, &Widget::receiveReplyGet);
    connect(managerPost,&QNetworkAccessManager::finished,this,&Widget::receiveReplyPost);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    status = 0;
    //设置url
    reqGet.setUrl(QUrl("http://cat-match.easygame2021.com/sheep/v1/game/user_info?uid=" + ui->lineEdit->text() +
                       "&t=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE2OTQzMjcyNDYsIm5iZiI6MTY2MzIyNTA0NiwiaWF0IjoxNjYzMjIzMjQ2LCJqdGkiOiJDTTpjYXRfbWF0Y2g6bHQxMjM0NTYiLCJvcGVuX2lkIjoiIiwidWlkIjo4MzU0MzAxNCwiZGVidWciOiIiLCJsYW5nIjoiIn0.5qpiRRjxwUmN1U8Qst8dFBMWMQyWi26DcfTgHIITZds&content-type=application%2Fjson&User-Agent=Mozilla%2F5.0%20(iPhone%3B%20CPU%20iPhone%20OS%2015_6%20like%20Mac%20OS%20X)%20AppleWebKit%2F605.1.15%20(KHTML%2C%20like%20Gecko)%20Mobile%2F15E148%20MicroMessenger%2F8.0.28(0x18001c26)%20NetType%2FWIFI%20Language%2Fzh_CNh2"));

    reqGet.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1; WOW64; rv:46.0) Gecko/20100101 Firefox/46.0");
    // 发送get请求
    replyGet = managerGet->get(reqGet);
}


// 接收http响应 GET
void Widget::receiveReplyGet(QNetworkReply *reply)
{
    // 获取状态码
    //int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    // 获取响应数据
    QString reply_data = reply->readAll();

    if(status == 0){
        QJsonDocument jdoc = QJsonDocument::fromJson(reply_data.toUtf8());
        QJsonObject jobject = jdoc.object();
        int err_code = jobject["err_code"].toInt();

        if(reply_data != "" && (err_code == 0)){
            ui->textEdit_2->append("UID获取成功");
            if(jobject.contains("data"))                //第一级对象
            {
                QJsonValue value = jobject.value("data");
                jobject = value.toObject();
                uid = jobject["wx_open_id"].toString();
                avatar = jobject["avatar"].toString();
                ui->lineEdit_3->clear();
                ui->lineEdit_3->insert(uid);
                ui->pushButton_3->setEnabled(true);
            }

        }else{
            ui->textEdit_2->append("UID获取失败");
            ui->pushButton_3->setEnabled(false);
        }
    }else{
        QJsonDocument jdoc = QJsonDocument::fromJson(reply_data.toUtf8());
        QJsonObject jobject = jdoc.object();
        int err_code = jobject["err_code"].toInt();
        if(reply_data != "" && (err_code == 0)){
            ui->textEdit_2->append("第" + QString::number(num) + "次通关成功刷入");
        }else{
            ui->textEdit_2->append("第" + QString::number(num) + "次通关刷入失败");
            errnum++;
        }
        num++;
        if(num == ui->lineEdit_2->text().toInt()){
            ui->pushButton_2->setEnabled(true);
            ui->textEdit_2->append("通关次数刷入完成，失败:" + QString::number(errnum) + "次");
        }
    }
}

//接受HTTP响应POST
void Widget::receiveReplyPost(QNetworkReply *reply)
{
    // 获取状态码
    //int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    // 获取响应数据
    QString reply_data = reply->readAll();
    QJsonDocument jdoc = QJsonDocument::fromJson(reply_data.toUtf8());
    QJsonObject jobject = jdoc.object();
    int err_code = jobject["err_code"].toInt();

    if(reply_data != "" && (err_code == 0)){
        ui->textEdit_2->append("TOKEN获取成功");
        if(jobject.contains("data"))                //第一级对象
        {
            QJsonValue value = jobject.value("data");
            jobject = value.toObject();
            QString token = jobject["token"].toString();
            ui->textEdit->clear();
            ui->textEdit->append(token);
            ui->pushButton_3->setEnabled(false);
        }

    }else{
        ui->textEdit_2->append("TOKEN获取失败");
        QMessageBox::warning(this,"error","Token获取失败，手动填写Token",QMessageBox::Ok);
        ui->textEdit->setReadOnly(false);
    }
}

void Widget::on_pushButton_3_clicked()
{

    QString t = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE2OTQyNDcwNjAsIm5iZiI6MTY2MzE0NDg2MCwiaWF0IjoxNjYzMTQzMDYwLCJqdGkiOiJDTTpjYXRfbWF0Y2g6bHQxMjM0NTYiLCJvcGVuX2lkIjoiIiwidWlkIjoyNjY2NzQwOSwiZGVidWciOiIiLCJsYW5nIjoiIn0.P4lqjKuo6ENufg6RDGNnzsZHGEHHy5B_CLl0sx4KxSU";
    // 设置url 发送POST请求
    reqPost.setUrl(QUrl("http://cat-match.easygame2021.com/sheep/v1/user/login_oppo"));
    //设置请求数据
    QString POSTmsg =
            "uid=" + uid +"&avatar=1&nick_name=1&sex=1&t=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE2OTQzMjcyNDYsIm5iZiI6MTY2MzIyNTA0NiwiaWF0IjoxNjYzMjIzMjQ2LCJqdGkiOiJDTTpjYXRfbWF0Y2g6bHQxMjM0NTYiLCJvcGVuX2lkIjoiIiwidWlkIjo4MzU0MzAxNCwiZGVidWciOiIiLCJsYW5nIjoiIn0.5qpiRRjxwUmN1U8Qst8dFBMWMQyWi26DcfTgHIITZds&content-type=application%2Fjson&User-Agent=Mozilla%2F5.0+(iPhone%3B+CPU+iPhone+OS+15_6+like+Mac+OS+X)+AppleWebKit%2F605.1.15+(KHTML%2C+like+Gecko)+Mobile%2F15E148+MicroMessenger%2F8.0.28(0x18001c26)+NetType%2FWIFI+Language%2Fzh_CN";
    // 发送post请求
    reqPost.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1; WOW64; rv:46.0) Gecko/20100101 Firefox/46.0");
    replyPost = managerPost->post(reqPost, POSTmsg.toUtf8());
}


void Widget::on_pushButton_2_clicked()
{
    status = 1;
    if(ui->textEdit->toPlainText() == ""){
        QMessageBox::warning(this,"error","Token不能为空",QMessageBox::Ok);
    }else{
        ui->textEdit_2->append("开始刷入通关次数：" + ui->lineEdit_2->text() + "次");
        ui->pushButton_2->setEnabled(false);
        num = 0;
        QVariant v;
        for (int var = 0; var < ui->lineEdit_2->text().toInt(); ++var) {
            v.setValue("t=" + ui->textEdit->toPlainText().toUtf8());
            reqGet.setUrl(QUrl("https://cat-match.easygame2021.com/sheep/v1/game/game_over?rank_sco"
                               "re=1&rank_state=1&rank_time=1&rank_role=1&skin=1"));
            reqGet.setRawHeader("Cookie", v.toByteArray());
            replyGet = managerGet->get(reqGet);
        }
    }
}

