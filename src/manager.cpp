#include "manager.h"
#include <cmath>
#include <QDebug>
#include <QThread>
#include <QImageWriter>

cyx::cyx_Manager::cyx_Manager(const QString& path,QObject* parent):QObject(parent),m_gamer_col(QColor(56,59,102)),
m_back_col(QColor()),m_value_img(QImage()),m_speed(0.75),m_stop(false),m_gamer_pos(QPoint()),m_target_pos(QPoint()){
    SetImage(path);
}

cyx::cyx_Manager::~cyx_Manager(){

}

QPoint cyx::cyx_Manager::GetCerten(){
    if(m_img.isNull())
        return QPoint();
    int x = m_img.width();
    int y = m_img.height();
    return QPoint(x/2,y/2);
}

QImage cyx::cyx_Manager::SetImage(const QImage& img){
    QImage old = m_img;
    m_img = img;
    return old;
}

QImage cyx::cyx_Manager::SetImage(const QString &path){
    if(path != ""){
        QImage img(path);
        return SetImage(img);
    }
    m_img = QImage();
    return m_img;
}

void cyx::cyx_Manager::GetValueImage(){
    int y,w,h;
    QPoint point = GetCerten();
    y = point.y() - 300;
    w = m_img.width();
    h = 600;
    m_value_img = m_img.copy(0,y,w,h);
    m_back_col = GetBackGroundCol();
}

QColor cyx::cyx_Manager::GetBackGroundCol(){
    if(!m_value_img.isNull())
        return m_value_img.pixelColor(5,5);
    return QColor();
}

void cyx::cyx_Manager::GetGamerPos(){
    int w = m_value_img.width();    //1080
    int h = m_value_img.height();   //600

    for(int i = 0;i < w;++i){
        for(int j = h-1;j >= h/2;--j){
            QColor cur = m_value_img.pixelColor(i,j);
            if(cur == m_gamer_col){
                //这里判断颜色相同即可
                m_gamer_pos = QPoint(i,j);
                qDebug() << "gamer:" << m_gamer_pos;
                return;
           }
        }
    }
}

void cyx::cyx_Manager::GetTarPos(){
    int w = m_value_img.width();
    int h = m_value_img.height();

    //按宽遍历，也就是遍历完一行再遍历下一行
    for(int j = 0;j <= h/2;++j){
        for(int i = 0; i < w; ++i){
            QColor cur = m_value_img.pixelColor(i,j);
            if(isSameCol(m_back_col,cur))
                continue;
            if(isSameCol(m_gamer_col,cur) && i >= m_gamer_pos.x()-40 && i <= m_gamer_pos.x()+40)
                continue;
            m_target_pos = QPoint(i,j);
            qDebug() << "target:" << m_target_pos;
            return;
        }
    }
}

bool cyx::cyx_Manager::isSameCol(const QColor& src,const QColor &des){
    int cr,cg,cb;
    int r,g,b;
    src.getRgb(&r,&g,&b);
    des.getRgb(&cr,&cg,&cb);

    int tmp = pow((cr-r),2) + pow((cg-g),2) + pow((cb-b),2);
    double dis = sqrt(tmp);
    if(dis < 20)
        return true;
    return false;
}

void cyx::cyx_Manager::SaveValueImg(){
    if(m_value_img.isNull())
        return;
    QImageWriter imgwriter("D:/tmp/qtstudy/weixin_fuzhu/value_cap.png");
    imgwriter.write(m_value_img);
    qDebug() << "图片保存成功";
}

bool cyx::cyx_Manager::SaveScreen(){
    system("cls");                                      //清屏
    system("adb shell screencap -p /sdcard/cap.png");   //获取手机屏幕当前内容
    system("adb pull /sdcard/cap.png D:/tmp/qtstudy/weixin_fuzhu/debug/cap.png");                 //保存到电脑
    SetImage("D:/tmp/qtstudy/weixin_fuzhu/debug/cap.png");
    return true;
}

bool cyx::cyx_Manager::Swipe(){
    int t = (int)(m_dis/m_speed);
    qDebug() << "dis:" << m_dis << "t:" << t;
    char cmd[50];
    sprintf(cmd,"adb shell input swipe 500 500 500 500 %d",t);
    system(cmd);
    return true;
}

void cyx::cyx_Manager::GetDis(){
    GetGamerPos();
    GetTarPos();

    m_target_pos = QPoint(m_target_pos.x(),m_target_pos.y()+60);
    int tmp = pow((m_gamer_pos.x()-m_target_pos.x()),2) + pow((m_gamer_pos.y()-m_target_pos.y()),2);
    m_dis = (int)sqrt(tmp);
}

void cyx::cyx_Manager::slot_work(){
    while(!m_stop){
        SaveScreen();
        GetValueImage();
        GetDis();
        emit sig_submit(m_value_img);
        Swipe();
    }
}
