#include "serial_data_parse.h"
#include "serial_qt.h"

unsigned char recieve_buffer[READ_BUFFERSIZE] = {};
QMutex _mutex; //全局互斥锁
/***********************************************
*  @Name    : CImuCommand::serialInit
*  @Descrip : 串口初始化
*  @Notes   : None
***********************************************/
bool SDP::sdp::serialInit()
{
    try
    {
        sp.setPort("/dev/ttyUSB0");
        sp.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        sp.setTimeout(to);
        sp.open();
    }
    catch (serial::IOException &e)
    {
        ROS_ERROR_STREAM("Unable to open port!");
        std::cout << e.what() << std::endl;
        return -1;
    }

    if (sp.isOpen())
    {
        ROS_INFO_STREAM("serial Port initialized successful!");
        //cmdFrame(CMD_OUTPUT_200HZ);
        //sp.write(cmd_buffer, cmd_num);
    }
    else
    {
        return -1;
    }
    return true;
}

bool SDP::sdp::decodeFrame(unsigned char tmpBuffer[READ_BUFFERSIZE])
{
    if (tmpBuffer[0] == DATA_FrameHead && tmpBuffer[1] == DATA_FrameHead)
    //if (true)
    {
        //其他字节数据处理

        for (int i = 0; i < READ_BUFFERSIZE; i++)
        {
            std::cout << (unsigned int)tmpBuffer[i] << ' ';
        }
        std::cout << std::endl;

    }
    else
    {
        std::cout << "数据帧头错误" << std::endl;
        memset(tmpBuffer, 0, READ_BUFFERSIZE);
        return false;
    }
    return true;
}

/***********************************************
 *  @Name    : CImuCommand::serialInit
 *  @Descrip : 总运行程序
 *  @Notes   : None
 ***********************************************/
bool SDP::sdp::RUN(workthread *workthread_ptr)
{

    ros::NodeHandle nh;
    //ros::Publisher msg_pub = nh.advertise<sensor_msgs::Imu>("imumsg", 1000);

    //串口初始化
    serialInit();
    //ros::Rate loop_rate(50);
    while (ros::ok())
    {
        try
        {
            if (sp.available())
            {
                _mutex.lock();
                //读取数据
                sp.read(r_buffer, READ_BUFFERSIZE);
                //解析数据
                if (decodeFrame(r_buffer))
                {
                    for (int i = 0; i < READ_BUFFERSIZE; i++)
                    {
                        recieve_buffer[i] = r_buffer[i];
                    }
                    emit workthread_ptr->recieve_sig();
                }
                memset(r_buffer, 0, READ_BUFFERSIZE);
                _mutex.unlock();
            }
        }
        catch (serial::IOException &e)
        {
            sp.close();
            while (ros::ok)
            {
                if (!sp.isOpen())
                {
                    std::cout << "serial port is loss connected!" << std::endl;
                    std::cout << "try to reconnected..." << std::endl;
                    serialInit();
                }
                else
                {
                    std::cout << "reconnected successful!" << std::endl;
                    break;
                }
                //loop_rate.sleep();
                workthread::msleep(10);
            }
        }
        //loop_rate.sleep();
        workthread::msleep(10);
    }

    return true;
}

/***********************************************
 *  @Name    :workthread::run()
 *  @Descrip : 重构QThread run函数
 *  @Notes   : None
 ***********************************************/
void SDP::workthread::run()
{
    /*
    while (1)
    {
        ROS_INFO_STREAM("sleep...");
        QThread::sleep(1);
    }*/
    SDP::sdp Serial_Terminal;
    Serial_Terminal.RUN(this);
}