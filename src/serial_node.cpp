#include "serial_data_parse.h"
#include "serial_qt.h"
#include <QtWidgets/QApplication>

int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	ros::init(argc, argv, "serial_node");
	serial_qt q;
	q.show();
	return a.exec();
}
