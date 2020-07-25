#ifndef SERIAL_QT_H
#define SERIAL_QT_H

#include "serial_data_parse.h"

namespace Ui
{
    class serial_qt;
}

class serial_qt : public QMainWindow
{
    Q_OBJECT

public:
    explicit serial_qt(QWidget *parent = nullptr);
    ~serial_qt();
public slots:
    void open_or_close_thread();
    void recieve_data();

private:
    Ui::serial_qt *ui;
    SDP::workthread *workthread_ptr;
};

#endif // SERIAL_QT_H
