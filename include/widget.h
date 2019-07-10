#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QThread>
#include "manager.h"

class widget : public QWidget
{
    Q_OBJECT
public:
    explicit widget(QWidget *parent = 0);
    ~widget();

signals:
    void sig_workstart();

public slots:
    void on_Click();
    void on_submit(const QImage&);

private:
    QPushButton* m_button;
    QLabel* m_label;
    cyx::cyx_Manager* m_manager;
    QThread m_work;
};

#endif // WIDGET_H
