#include "reqwin.h"
#include "ui_reqwin.h"

Reqwin::Reqwin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Reqwin)
{
    ui->setupUi(this);
}

Reqwin::~Reqwin()
{
    delete ui;
}

void Reqwin::on_pushButton_clicked()
{
    this->close();      // Закрываем окно
    emit reqwin_signal(); // И вызываем сигнал на открытие главного окна
}
