#ifndef MANAGER_H
#define MANAGER_H
#include <QImage>
#include <QPoint>
#include <QColor>
#include <QObject>

namespace cyx{

class cyx_Manager:public QObject{
    Q_OBJECT

public:
    cyx_Manager(const QString& path = "",QObject* parent = nullptr);
    ~cyx_Manager();
    QImage  SetImage(const QImage& img);
    QImage  SetImage(const QString& path);
    void    GetValueImage();                        //获取有效图片
    void  GetGamerPos();                            //获取游戏玩家位置
    void  GetTarPos();                               //获取目标点位置
    bool    isSameCol(const QColor& src,const QColor& des);           //判断指定颜色是否相同
    bool    SaveScreen();                           //保存手机屏幕
    bool    Swipe();                                //按压手机屏幕的某一点
    void    GetDis();                               //获取人物与目标点的距离

signals:
    void sig_submit(const QImage&);

public slots:
    void slot_work();
private:
    QPoint GetCerten();
    QColor  GetBackGroundCol();                     //获取有效图片的背景颜色
    void SaveValueImg();

private:
    QImage  m_img;
    QColor  m_gamer_col;
    QColor  m_back_col;
    double  m_speed;                                //单位时间按压积累的力量
    double  m_dis;                                  //人物与目标点的距离
    QPoint  m_gamer_pos;                            //人物位置
    QPoint  m_target_pos;                           //目标位置

public:
    QImage m_value_img;
    bool    m_stop;                                 //是否继续


};

}
#endif // MANAGER_H
