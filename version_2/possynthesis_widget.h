#ifndef POSSYNTHESIS_WIDGET_H
#define POSSYNTHESIS_WIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QWebFrame>

#include <all_include.h>

namespace Ui {
class PosSynthesis_Widget;
}

class PosSynthesis_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit PosSynthesis_Widget(QWidget *parent = 0);
    ~PosSynthesis_Widget();

private slots:
    void on_synthesis_BT_clicked();

    void on_mapShow_BT_clicked();

private:
    Ui::PosSynthesis_Widget *ui;

signals:
    void showLatAndLon(float lat,float lon);
};

#endif // POSSYNTHESIS_WIDGET_H
