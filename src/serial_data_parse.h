/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2020,三环智能
 *
 *  @File    : serial_data_parse.h
 *  @Brief   : 232串口数据解析
 *
 *  @Version : V1.0
 *  @Date    : 2020/07/15

 *	@History :
 *    Author : 朱殷晔
 *    Descrip: 假设帧头0xaa,数据格式：帧头+指令+校验和
 *************************************************************************************************************/

#ifndef _SERIAL_DATA_PARSE_H_
#define _SERIAL_DATA_PARSE_H_

#include <iostream>
#include "serial/serial.h"
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include "serial_test/serial.h"
#include <sensor_msgs/Imu.h>
#include <QtCore/QThread>
#include <QtWidgets/QMainWindow>
#include <QtCore/QMutex>

#define READ_BUFFERSIZE 44
#define DATA_FrameHead (unsigned char)0xAA

extern unsigned char recieve_buffer[READ_BUFFERSIZE];
extern QMutex _mutex;//全局互斥锁

//命名空间 串口解析
namespace SDP
{
	class sdp;
	class workthread;
	class sdp
	{
	private:
		serial::Serial sp; //串口对象

	public:
		sdp(){};
		~sdp(){};

		/***********************************************
		 *  @Name    : CImuCommand::cmdFrame
		 *  @Descrip : 向IMU发送一帧指令
         *  @in      : 指令
         *  @out     : 原始数据
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool cmdFrame(unsigned char imucmd);
		/***********************************************
		 *  @Name    : CImuCommand::muliteCmdFrame
		 *  @Descrip : 向IMU发送三个指令，查询输出
		 *  @Para    : [in]三个指令
		 *	@Para    : [in/out]	v_ucData：原始数据帧
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool muliteCmdFrame(unsigned char imucmd1, unsigned char imucmd2, unsigned char imucmd3);

		/***********************************************
		 *  @Name    : CImuCommand::decodeFrame
		 *  @Descrip : 解码
		 *  @Notes   : None
		 ***********************************************/
		bool decodeFrame(unsigned char tmpBuffer[READ_BUFFERSIZE]);

		/***********************************************
		 *  @Name    : CImuCommand::serialInit
		 *  @Descrip : 串口初始化
		 *  @Notes   : None
		 ***********************************************/
		bool serialInit();

		/***********************************************
		 *  @Name    : CImuCommand::serialInit
		 *  @Descrip : 总运行程序
		 *  @Notes   : None
		 ***********************************************/
		bool RUN(SDP::workthread *);

	public:
		//unsigned char cmd_buffer[cmd_num];
		//unsigned char mulit_cmd_buffer[mulitCmd_num];
		unsigned char r_buffer[READ_BUFFERSIZE];
	};

	class workthread : public QThread
	{
		//继承Qt的类Q_OBJECT不能少
		Q_OBJECT
	public:
		workthread(){};
		~workthread(){};

	protected:
		void run();
	signals:
		void recieve_sig();
	};
} // namespace SDP
#endif