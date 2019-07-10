#include "widget.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QPixmap>

widget::widget(QWidget *parent) : QWidget(parent)
{
    m_button = new QPushButton("start");
    m_label = new QLabel;
    m_manager = new cyx::cyx_Manager();

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->addWidget(m_button);
    main_layout->addWidget(m_label);

    m_manager->moveToThread(&m_work);
    m_work.start();

    connect(m_button,SIGNAL(clicked()),this,SLOT(on_Click()));
    connect(m_manager,SIGNAL(sig_submit(QImage)),this,SLOT(on_submit(QImage)));
    connect(&m_work,SIGNAL(finished()),m_manager,SLOT(deleteLater()));
    connect(this,SIGNAL(sig_workstart()),m_manager,SLOT(slot_work()));

    this->resize(500,480);
}

void widget::on_Click(){
    if(m_button->text() == "start"){
        m_manager->m_stop = false;
        emit sig_workstart();
        m_button->setText("stop");
    }
    else{
        m_button->setText("start");
        m_manager->m_stop = true;
    }
}

void widget::on_submit(const QImage &img)
{
    if(img.isNull())
        return;
    m_label->setPixmap(QPixmap::fromImage(img));
}

widget::~widget(){
    if(m_manager)
        m_manager->m_stop = true;
    m_work.quit();
    m_work.wait(1000);
}
