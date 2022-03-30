#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QDateTime>
#include <QDebug>
#include <QtMath>
#include <QTimer>
#include <QMessageBox>

using namespace std;

const float pi=3.1415926;
bool flag=1,flag_2;
int swit=0;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //窗口设置
    setWindowTitle("Clock v1.0");
    resize(800,600);

    //初始化时钟
    QDateTime curTime=QDateTime::currentDateTime();
    QString now_time=curTime.toString("hhmmssyyyy-MM-dd");
    qDebug()<<now_time;
    string s;
    s=now_time.toStdString();
    cent=10*(s[4]-'0')+s[5]-'0';
    mini=10*(s[2]-'0')+s[3]-'0';
    hour=10*(s[0]-'0')+s[1]-'0';
    update();

    //时钟设置
    QTimer *timer=new QTimer(this);
    timer->start(1000);
    connect(timer,&QTimer::timeout,this,[=](){
        QDateTime curTime=QDateTime::currentDateTime();
        QString now_time=curTime.toString("hhmmss");
        QString now_date=curTime.toString("yyyy-MM-dd");
        qDebug()<<now_time;
        string s;
        s=now_time.toStdString();
        QString ap[2]={"上午","下午"};
        cent=10*(s[4]-'0')+s[5]-'0';
        mini=10*(s[2]-'0')+s[3]-'0';
        hour=10*(s[0]-'0')+s[1]-'0';
        if(hour==Alarm[2]&&mini==Alarm[1]&&cent==Alarm[0]){
            QMessageBox::information(this,"Alarm","设定时间到了！");
        }
        ui->lineEdit->setText("日期:"+now_date);
        ui->lineEdit_2->setText(ap[hour/12]);
        update();
    });

    //闹钟设置
    connect(ui->SetAlarm,&QPushButton::clicked,this,[=](){
        flag^=1;
        if(flag) {
            Alarm[0]=cent;
            Alarm[1]=mini;
            Alarm[2]=hour;
            timer->start(1000);
        }else {
            timer->stop();
            cent=Alarm[0];
            mini=Alarm[1];
            hour=Alarm[2];
        }
        ui->lineEdit_3->setText("Clock: "+QString::number(hour)+":"+QString::number(mini)+":"+QString::number(cent));
        update();
    });

    //转换时分秒针
    connect(ui->Tranform,&QPushButton::clicked,this,[=](){
        swit++;swit%=3;
    });

    //+
    connect(ui->Add,&QPushButton::clicked,this,[=](){
        if (swit==0) {
            cent++;
            if(cent==60){
                cent%=60,mini++;
            }
        }else if(swit==1){
            mini++;
            if(mini==60)
                mini%=60,hour++;
        }else {
            hour++;hour%=24;
        }
        ui->lineEdit_3->setText("Clock: "+QString::number(hour)+":"+QString::number(mini)+":"+QString::number(cent));
        update();
    });
    //-
    connect(ui->Minus,&QPushButton::clicked,this,[=](){
        if (swit==0) {
            cent--;
            if(cent==-1){
                cent+=60,mini--;
            }
        }else if(swit==1){
            mini--;
            if(mini==-1)
                mini+=60,hour--;
        }else {
            hour--;
            if(hour==-1) hour+=24;
        }
        ui->lineEdit_3->setText("Clock: "+QString::number(hour)+":"+QString::number(mini)+":"+QString::number(cent));
        update();
    });

    //计时器设置
    QTimer *timer_2=new QTimer(this);

    connect(timer_2,&QTimer::timeout,this,[=](){
        cent++;
        if(cent>=60) cent%=60, mini++;
        if(mini>=60) mini%=60, hour++;
        update();
        ui->lineEdit->setText(QString::number(hour)+":"+QString::number(mini)+":"+QString::number(cent));
    });

    //计时器
    connect(ui->TimeCal,&QPushButton::clicked,this,[=](){
       flag_2^=1;
       if(!flag_2){
           timer_2->stop();
           timer->start(1000);
       }else {
           timer->stop();
           cent=mini=hour=0;
           ui->lineEdit_2->clear();
       }
//       ui->lineEdit->setText("Stopping");
       update();
    });

    //开始
    connect(ui->Start,&QPushButton::clicked,this,[=](){
        timer_2->start(1000);
//        ui->lineEdit->setText("Going");
    });

    //停止
    connect(ui->Stop,&QPushButton::clicked,this,[=](){
        timer_2->stop();
//        ui->lineEdit->setText("Stopping");
    });

    //标记
    connect(ui->Flag,&QPushButton::clicked,this,[=](){
        ui->lineEdit_2->setText(QString::number(hour)+":"+QString::number(mini)+":"+QString::number(cent));
    });

    //重置
    connect(ui->Reset,&QPushButton::clicked,this,[=](){
        timer->stop();
        cent=mini=hour=0;
        update();
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::SetAlarm(){

}

void Widget::paintEvent(QPaintEvent *){

    QPainter painter(this);

    painter.setPen(QColor(0,160,233));

    //表盘绘画
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(QPoint(400,300),200,200);

    //字体设置
    QFont font;
    font.setFamily("SimSun");
    font.setPointSize(30);
    font.setItalic(false);
    painter.setFont(font);
    painter.setPen(QColor(165,29,14));

    //表盘数字
    for(int i=1;i<=12;i++){
        painter.drawText(QRect(QPoint((int)(400+175*sin(i/12.0*2*pi))-50,(int)(300-175*cos(i/12.0*2*pi))-25),QSize(100,50)),Qt::AlignCenter,QString::number(i));
    }

    painter.setPen(QColor(0,160,233));

    QPen pen;

    //秒针设置
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(QPoint(400,300),QPoint((int)(400+150*sin((cent/60.0)*2*pi)),(int)(300-150*cos(cent/60.0*2*pi))));

    //分针
    pen.setWidth(5);
    pen.setColor(QColor(11,45,23));
    painter.setPen(pen);
    painter.drawLine(QPoint(400,300),QPoint((int)(400+100*sin((mini/60.0)*2*pi)),(int)(300-100*cos(mini/60.0*2*pi))));

    //时针
    pen.setWidth(8);
    pen.setColor(QColor(100,100,100));
    painter.setPen(pen);
    painter.drawLine(QPoint(400,300),QPoint((int)(400+60*sin((hour/12.0)*2*pi)),(int)(300-60*cos(hour/12.0*2*pi))));

    //中心表盘
    QBrush brush(QColor(0,255,45));
    pen.setColor(QColor(0,0,0));
    pen.setWidth(2);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawEllipse(QPoint(400,300),10,10);
}
