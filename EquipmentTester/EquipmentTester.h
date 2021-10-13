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

    /*�����е�Slider���ó�0��1*/
    void InitSwitch();

    /*����QLable������*/
    void InitFont();
	/*����У���*/
    unsigned int getChecksum(unsigned char*buf,unsigned int size);

    ~EquipmentTester();
public slots:

    /*CAN�������������Ok��Cancel��*/
    void on_button_box_ok();
    void on_button_box_cancel();

	/*UDP���ͺͽ��յĲۺ���*/
    void sendByUdp();
    void RecvByUdp();

    
	/*��ʱ��ȡ���ݻ�ͼ*/
    void dynamPlot();

    /*�󶨲����źͶ�Ӧ���������*/
    void bindTree(QTreeWidgetItem* item,int count);

    /*����ѡ��ۻ��Ĳۺ���*/
    void zuoPaoXuanZeKouJiSlot();

    /*����ѡ���ﵯ�Ĳۺ���*/
    void zuoPaoXuanZeYangDanSlot();

    /*����ѡ��ۻ��Ĳۺ���*/
    void youPaoXuanZeKouJiSlot();

    /*����ѡ���ﵯ�Ĳۺ���*/
    void youPaoXuanZeYangDanSlot();

    /*��������ۺ���*/
    void shootingButtonSlot();

    /*��1��Ļ���ۺ���*/
    void zuo1YanMuDanSlot();

    /*��2��Ļ���ۺ���*/
    void zuo2YanMuDanSlot();

    /*��3��Ļ���ۺ���*/
    void zuo3YanMuDanSlot();

    /*��3��Ļ���ۺ���*/
    void zuo4YanMuDanSlot();

    /*��1��Ļ���ۺ���*/
    void you1YanMuDanSlot();

    /*��2��Ļ���ۺ���*/
    void you2YanMuDanSlot();

    /*��3��Ļ���ۺ���*/
    void you3YanMuDanSlot();

    /*��4��Ļ���ۺ���*/
    void you4YanMuDanSlot();

    /*�ﵯ��ָʾ��ۺ���*/
    void yangDanJiZhiShiZuoSlot();

    /*�ﵯ��ָʾ�Ҳۺ���*/
    void yangDanJiZhiShiYouSlot();

    /*����FPGA���͵�״̬��Ϣ���ڽ����������ʾ*/
    void handleStateInfo();

    /*����FPGA���͵Ĳ�����Ϣ���ڽ����������ʾ*/
    void hanndleWaveInfo();

    /*��λ*/
    void resetCAN();
signals:

    /*����״̬��Ϣ�ź�*/
    void handleStateInfoSignal();
	/*�����������ź�*/
    void handleWaveInforSignal();
private:
    Ui::EquipmentTesterClass ui;

    /*���浱ǰ���õĲ�����*/
    int baudRate;

    /*���沨���ʺ��ַ�����ӳ���ϵ*/
    QMap<QString, int>baudMap;

    /*���淢��ʱ��*/
    unsigned long long sendTimer;

    /*UDP�׽���*/
    QUdpSocket* udpSocket;

    /*��ʱ�����*/
    QTimer* DataTimer;

    /*��������ź�*/
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

    /*��FPGAͨ�ŵĶ˿ں�*/
    const int portNumber = 8888;

    /*��FPGAͨ�����õ�������*/

    CanConfigurations* can_configurations_;//����CAN��
    CommanPacketToFpgaWithoutFixedFrequency* commandPackSend;//���͸�FPGA������
    CommanPacketFromFpgaWithStateInfo* commandPackRecvState;//�յ���״̬��Ϣ
    CommanPacketFromFpgaWithWaveFigure* commandPackRecvWaveFigure;//�յ��Ĳ���

    ReSet* reset;//��λ����

    /*�ж�����ͳɹ���flag*/
    int flagOfCommanSendStatus;
};
