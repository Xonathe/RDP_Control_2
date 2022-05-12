#ifndef REQWIN_H
#define REQWIN_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Reqwin; }
QT_END_NAMESPACE

class Reqwin : public QWidget
{
    Q_OBJECT

public:
    explicit Reqwin(QWidget *parent = 0);
        ~Reqwin();
signals:
    void reqwin_signal();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Reqwin *ui;
};

#endif // REQWIN_H
