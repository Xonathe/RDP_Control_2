#include "widget.h"
#include "ui_widget.h"
#include "ini.h"
#include "keygen.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
//    this->setWindowFlags(Qt::FramelessWindowHint); // отключить стандартную панель
    read_settings_ini();
    WSAStartup(MAKEWORD(2,2),&mWasData);
}


Widget::~Widget()
{
    delete ui;
}


/**
 * Проверка файла на существование
 * return: 0 если нет или 1 если есть
*/
bool file_exists (const char *fname) {
    return access(fname,0) != -1;
}


/**
 * Читает настройки программы
 * Если нет файла настроек, содает по умолчанию
 * Разрешает\запрещает локальные программы
 * Меняет разрешения окна
*/
void Widget::read_settings_ini() {
    bool settings_ini = file_exists("settings.ini");
    if (settings_ini == 0) {
        // запись файла ini
        inih::INIReader r;
        r.InsertEntry("programs", "local_programs", "False");
        r.InsertEntry("programs", "path", "\\\\192.168.0.6\\rdp\\progs.ini");
        inih::INIWriter::write("settings.ini", r);
    }
    inih::INIReader r{"settings.ini"};
    std::string local_programs = r.Get<std::string>("programs", "local_programs");
    std::string path_to_progs = r.Get<std::string>("programs", "path");
    if (local_programs == "True") {
        this->resize(271, 271);
        QString qpath = QString::fromLocal8Bit(path_to_progs.c_str());  // преобразование в QString
        fill_combobox(qpath);
    } else {
        this->resize(271, 230);
        ui->pushButton_4->hide();
        ui->comboBox->hide();
    }
}


/**
 * Проверяет наличие установленных программ по заданным путям
 * Заполняет комбобокс установленными программами
*/
void Widget::fill_combobox(QString path) {
    inih::INIReader r{path.toStdString()}; // преобразование в std::string

    for (int i = 1; i <= 1000; i++) {
        std::string s;
        try {
            s = r.Get<std::string>(std::to_string(i), "name");
        }
        catch (std::runtime_error const&) {
            break;
        }
        QString qs = QString::fromLocal8Bit(s.c_str()); // русификация строки
        QList<QString> name_progs;
        name_progs.append(qs);
        ui->comboBox->addItems(name_progs);
    }
}


/**
 * Запускает RDP с файлом конфигурации
*/
void Widget::on_pushButton_clicked()
{
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd);
    WinExec("mstsc.exe C:/Users/konkov_va/QtProjects/RDP_Control_2/Default.rdp", 1);
}


/**
 * Выходит из системы
*/
void Widget::on_pushButton_2_clicked()
{
    system("shutdown /L");
}


/**
 * Выключает компьютер
*/
void Widget::on_pushButton_3_clicked()
{
    system("shutdown /s /t 0");
}


/**
 * Получает имя запускаемой программы
 * Ищет по имени в списке и запускает
*/
void Widget::on_pushButton_4_clicked()
{
//    Widget::send_full_data();
    sWindow->show();
    this->close();
}


void Widget::send_full_data()
{
    std::string disk = get_system_disk_serial();
    std::string motherboard = get_motherboard_serial();
    std::string processor = get_processor_serial();

    QString data = QString::fromLocal8Bit(disk + " " + motherboard + " " + processor);
    qDebug() << data;

    mSocket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(mSocket == INVALID_SOCKET){
        return;
    }

    mAddrStr = "192.168.0.100:48888";
    char ip[1024] = {0};
    int port = 0;
    sscanf(mAddrStr.toUtf8().data(),"%[^:]:%d",ip,&port);
    SOCKADDR_IN addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons((u_short)port);
    addr.sin_addr.S_un.S_addr = inet_addr(ip);
    int val = ::connect(mSocket,(SOCKADDR*)&addr,sizeof(addr));
    if (val == SOCKET_ERROR) {
        return;
    }
    mSocketClient = new SocketClient(mSocket);
    mSocketClient->start();
    connect(this,&Widget::isSend,mSocketClient,&SocketClient::dealSend);
    connect(mSocketClient,&SocketClient::isMsg,this,&Widget::dealMsg);
    Widget::send_message(data);
}


// отправить сообщение серверу
void Widget::send_message(QString data)
{
    if(mSocket == INVALID_SOCKET){
        return;
    }
    emit isSend(data);
}


// принять сообщение от сервера
void Widget::dealMsg(QString msg)
{
    qDebug() << msg;
    if (msg == "OK\r"){
        qDebug() << "сервер сказал ОК";
    } else { qDebug() << "сервер сказал False"; }
    Widget::closeSocket();
}


// закрыть сокет
void Widget::closeSocket()
{
    closesocket(mSocket);
    WSACleanup();
    if(mSocketClient!=nullptr)
        mSocketClient->quit();
}
