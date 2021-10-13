#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EquipmentTester.h"
#include <QMessageBox>
#include <QUdpSocket>
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include "frlight.h"
#include "SwitchControl.h"
#include "Command.h"
#include "QPower.h"
#include "qcustomplot.h"
class EquipmentTester : public QMainWindow
{
    Q_OBJECT

public:
	EquipmentTester(QWidget* parent = Q_NULLPTR);

    /*将所有的Slider设置成0和1*/
    void InitSwitch();

    /*设置QLable的字体*/
    void InitFont();
	/*计算校验和*/
    unsigned int getChecksum(unsigned char*buf,unsigned int size);

    ~EquipmentTester();
public slots:

    /*CAN总线设置区域的Ok和Cancel键*/
    void on_button_box_ok();
    void on_button_box_cancel();

	/*UDP发送和接收的槽函数*/
    void sendByUdp();
    void RecvByUdp();

    
	/*定时读取数据绘图*/
    void dynamPlot();

    /*绑定插座号和对应的软件界面*/
    void bindTree(QTreeWidgetItem* item,int count);

    /*左炮选择扣击的槽函数*/
    void zuoPaoXuanZeKouJiSlot();

    /*左炮选择扬弹的槽函数*/
    void zuoPaoXuanZeYangDanSlot();

    /*右炮选择扣击的槽函数*/
    void youPaoXuanZeKouJiSlot();

    /*右炮选择扬弹的槽函数*/
    void youPaoXuanZeYangDanSlot();

    /*射击按键槽函数*/
    void shootingButtonSlot();

    /*左1烟幕弹槽函数*/
    void zuo1YanMuDanSlot();

    /*左2烟幕弹槽函数*/
    void zuo2YanMuDanSlot();

    /*左3烟幕弹槽函数*/
    void zuo3YanMuDanSlot();

    /*左3烟幕弹槽函数*/
    void zuo4YanMuDanSlot();

    /*右1烟幕弹槽函数*/
    void you1YanMuDanSlot();

    /*右2烟幕弹槽函数*/
    void you2YanMuDanSlot();

    /*右3烟幕弹槽函数*/
    void you3YanMuDanSlot();

    /*右4烟幕弹槽函数*/
    void you4YanMuDanSlot();

    /*扬弹机指示左槽函数*/
    void yangDanJiZhiShiZuoSlot();

    /*扬弹机指示右槽函数*/
    void yangDanJiZhiShiYouSlot();

    /*处理FPGA发送的状态信息，在界面上完成显示*/
    void handleStateInfo();

    /*处理FPGA发送的波形信息，在界面上完成显示*/
    void hanndleWaveInfo();

    /*软复位*/
    void resetCAN();
signals:

    /*处理状态信息信号*/
    void handleStateInfoSignal();
	/*处理波形数据信号*/
    void handleWaveInforSignal();
private:
    Ui::EquipmentTesterClass ui;

    /*保存当前设置的波特率*/
    int baudRate;

    /*保存波特率和字符串的映射关系*/
    QMap<QString, int>baudMap;

    /*保存发送时刻*/
    unsigned long long sendTimer;

    /*UDP套接字*/
    QUdpSocket* udpSocket;

    /*定时器间隔*/
    QTimer* DataTimer;

    /*保存各个信号*/
    QStringList xs1;
    QStringList xs2;
    QStringList xs3;
    QStringList xs4;
    QStringList xs5;
    QStringList xs6;
    QStringList xs7;
    QStringList xs8;
    QStringList xs9;
    QStringList xs10;
    QStringList xs11;
    QStringList xs12;
    QStringList xs13;

    /*和FPGA通信的端口号*/
    const int portNumber = 8888;

    /*和FPGA通信中用到的命令*/

    CanConfigurations* can_configurations_;//配置CAN口
    CommanPacketToFpgaWithoutFixedFrequency* commandPackSend;//发送给FPGA的命令
    CommanPacketFromFpgaWithStateInfo* commandPackRecvState;//收到的状态信息
    CommanPacketFromFpgaWithWaveFigure* commandPackRecvWaveFigure;//收到的波形

    ReSet* reset;//复位命令

    /*判断命令发送成功的flag*/
    int flagOfCommanSendStatus;
};
