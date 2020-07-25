#include "serial_qt.h"
#include "ui_serial_qt.h"
#include "serial_data_parse.h"
#include <QtWidgets/QListWidgetItem>

serial_qt::serial_qt(QWidget *parent) : QMainWindow(parent), ui(new Ui::serial_qt)
{
    ui->setupUi(this);
    SDP::sdp Serial_Terminal;
    //创建新线程
    workthread_ptr = new SDP::workthread();
    connect(ui->pushButton1, SIGNAL(clicked()), this, SLOT(open_or_close_thread()));
}

serial_qt::~serial_qt()
{
    //先终结，再退出，再等待完全退出，再回收资源
    if (workthread_ptr->isRunning())
    {
        workthread_ptr->terminate();
        workthread_ptr->wait();
    }
    delete workthread_ptr;
    delete ui;
}

//按钮控制打开或关闭串口子线程
void serial_qt::open_or_close_thread()
{
    static int isOpen = 0;
    if (!isOpen)
    {
        connect(workthread_ptr, SIGNAL(recieve_sig()), this, SLOT(recieve_data()));
        workthread_ptr->start();
        isOpen = 1;
        ui->pushButton1->setText("关闭线程");
        //ROS_INFO("child thread is started!\n");
        std::cout << "child thread is started!" << std::endl;
    }
    else
    {
        disconnect(workthread_ptr, SIGNAL(recieve_sig()), this, SLOT(recieve_data()));
        workthread_ptr->terminate();
        //workthread_ptr->exit();
        workthread_ptr->wait();
        isOpen = 0;
        ui->pushButton1->setText("打开线程");
        //ROS_INFO("child thread is terminated!\n");
        std::cout << "child thread is terminated!" << std::endl;
    }
}

//从子线程接收数据
void serial_qt::recieve_data()
{
    _mutex.lock();
    /*for (int i = 0; i < READ_BUFFERSIZE; i++)
    {
        std::cout << (unsigned int)recieve_buffer[i] << " ";
    }
    std::cout << std::endl;*/
    //更新显示数据
    QByteArray recieve_data = QByteArray((char *)recieve_buffer, READ_BUFFERSIZE);
    QString str = recieve_data.toHex(' ').toUpper();
    str += "\n";
    QListWidgetItem *list_widget_item = new QListWidgetItem();
    list_widget_item->setText(str);
    ui->listWidget->addItem(list_widget_item);
    int i = ui->listWidget->currentRow();
    ui->listWidget->setCurrentRow(++i);
    _mutex.unlock();
}