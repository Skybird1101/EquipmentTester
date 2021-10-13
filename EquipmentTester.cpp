#include "EquipmentTester.h"

EquipmentTester::EquipmentTester(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	udpSocket = new QUdpSocket();
	udpSocket->bind(QHostAddress::LocalHost, 8888);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, [=](){on_button_box_ok();});
	connect(ui.buttonBox, &QDialogButtonBox::rejected, [=](){on_button_box_cancel();});
	connect(udpSocket, &QUdpSocket::readyRead, this, &EquipmentTester::RecvByUdp);
	
	InitSwitch();
	//InitFont();

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK2312"));

	/*��Ӷ���ڵ�*/
	QList<QTreeWidgetItem*>topLevelItems;
	QString text;
	for(int i=1;i<=13;i++)
	{
		text = "XS" + QString::number(i);
		QTreeWidgetItem* topItem = new QTreeWidgetItem(ui.treeWidget);
		topItem->setText(0, text);
		topLevelItems.append(topItem);
	}
	ui.treeWidget->addTopLevelItems(topLevelItems);

	///*������еĽӲ������*/
	//QList<QTreeWidgetItem*>childrenItems;
	//QStringList type;
	//type << "XCG18T4K1P1" << "XCE24T26K1P1(W)" << "XCE18T10K1P1" << "XCE24T19K1P1(W)" << "XCE24T26K1P1" << "XCE12T3K1P1" << "XCE12T3K1P1" << "XCE12T3K1P1(W)" << "XCE22T12K1P1" << "XCE24T19K1P1" << "XCE27T30K1P1"<<"XCE22T12K1P1(W)"<<"";

	//for(int i=1;i<topLevelItems.size();i++)
	//{
	//	QTreeWidgetItem *item=new QTreeWidgetItem(topLevelItems[i]);
	//	item->setText(0, type[i]);
	//
	//}
	connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &EquipmentTester::bindTree);
	
	/*�������п����õ�BaudRate*/
	QStringList baudRateItem;
	baudRateItem<< "1MBit/s" << "800kBit/s" << "500kBit/s" << "250kBit/s" << "125kBit/s" << "100kBit/s"<<"95.238kBit/s"<<"83.333kBit/s" <<"50kBit/s" << "47.619kBit/s" << "33.333kBit/s" << "20kBit/s" << "10kBit/s" << "5kBit/s";

	baudMap.insert("1MBit/s", 1000000);
	baudMap.insert("800kBit/s", 800000);
	baudMap.insert("500kBit/s", 500000);
	baudMap.insert("250kBit/s", 250000);
	baudMap.insert("125kBit/s", 125000);
	baudMap.insert("100kBit/s", 100000);
	baudMap.insert("95.238kBit/s",95238 );
	baudMap.insert("83.333kBit/s",83333);
	baudMap.insert("50kBit/s",50000);
	baudMap.insert("47.619kBit/s", 47619);
	baudMap.insert("33.333kBit/s", 33333);
	baudMap.insert("20kBit/s", 20000);
	baudMap.insert("10kBit/s", 10000);
	baudMap.insert("5kBit/s", 5000);

	/*����lineEditֻ����������*/
	QRegExp regx("[1-9][0-9]+$");
	QValidator* validator = new QRegExpValidator(regx, ui.lineEdit);
	ui.lineEdit->setValidator(validator);
	ui.comboBox->addItems(baudRateItem);

	ui.buttonBox->button(QDialogButtonBox::Ok)->setText(QString::fromLocal8Bit("ȷ��"));
	ui.buttonBox->button(QDialogButtonBox::Cancel)->setText(QString::fromLocal8Bit("ȡ��"));

	DataTimer = new QTimer(this);
	connect(DataTimer, &QTimer::timeout, this, [=](){dynamPlot();});
	/*��ʱ���������Ϊ1ms*/
	DataTimer->start(1);

	ui.widget->addGraph();

	
	/*��ʼ��CANͨ������*/
	can_configurations_ = new CanConfigurations[8];
	commandPackSend = new CommanPacketToFpgaWithoutFixedFrequency[32];
	reset = new ReSet[16];
	commandPackRecvState = new CommanPacketFromFpgaWithStateInfo[64];

	connect(ui.pushButton_2, &QPushButton::clicked, this, [=]() {resetCAN(); });
	connect(ui.horizontalSlider, &QSlider::valueChanged, this, [=]() {zuoPaoXuanZeKouJiSlot();});
	connect(ui.horizontalSlider_2, &QSlider::valueChanged, this, [=](){youPaoXuanZeKouJiSlot();});
	connect(ui.horizontalSlider_3, &QSlider::valueChanged, this, [=](){zuoPaoXuanZeYangDanSlot();});
	connect(ui.horizontalSlider_4, &QSlider::valueChanged, this, [=](){youPaoXuanZeYangDanSlot();});
	connect(ui.verticalSlider, &QSlider::valueChanged, this, [=]()
	{
			zuo1YanMuDanSlot();
			zuo2YanMuDanSlot();
			zuo3YanMuDanSlot();
			zuo4YanMuDanSlot();
			you1YanMuDanSlot();
			you2YanMuDanSlot();
			you3YanMuDanSlot();
			you4YanMuDanSlot();
	});
	connect(ui.verticalSlider_2, &QSlider::valueChanged, this, [=]() {yangDanJiZhiShiYouSlot(); });
	connect(ui.verticalSlider_2, &QSlider::valueChanged, this, [=]() {yangDanJiZhiShiZuoSlot(); });
	connect(ui.pushButton, &QPushButton::clicked, this, [=]() {shootingButtonSlot(); });
	connect(this,&EquipmentTester::handleStateInfoSignal, this, [=](){EquipmentTester::handleStateInfo();});
	connect(this, &EquipmentTester::handleWaveInforSignal, this, [=]() {hanndleWaveInfo(); });

	
}


void EquipmentTester::on_button_box_ok()
{
	if (ui.lineEdit->text().size() != 0)
	{
		sendTimer = ui.lineEdit->text().toULongLong();
		ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz")+ " "+QString::fromLocal8Bit("��ʱ������óɹ�\n"));
	}
	if (baudMap.value(ui.comboBox->currentText()) != 0)
	{
		baudRate = baudMap.value(ui.comboBox->currentText());
		ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " "+QString::fromLocal8Bit("���������óɹ�\n"));
	}
	memset(can_configurations_, 0, sizeof(CanConfigurations));

	/*�����������*/
	can_configurations_->frameLength = 0x08;
	can_configurations_->frameFlag = 0xEB91;
	can_configurations_->baudRate = baudRate;
	can_configurations_->type = 0x04;
	can_configurations_->timer = sendTimer;
	can_configurations_->checkSum = 0x00;
	ui.textBrowser->repaint();
	udpSocket->writeDatagram((const char* )(can_configurations_), QHostAddress::LocalHost,7777);
	QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("���óɹ�"));

}

void EquipmentTester::on_button_box_cancel()
{
	QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����ȡ��"));
}

void EquipmentTester::RecvByUdp()
{
	QByteArray data;
	while (udpSocket->hasPendingDatagrams())
	{
		data.resize(udpSocket->pendingDatagramSize());
		udpSocket->readDatagram(data.data(), data.size());
	}
	unsigned char *buf=(unsigned char*)data.data();
	/*�ж����ݱ�ʶ�����Ͳ�ͬ���ź�*/
	if(buf[3]==0x01)
	{
		this->commandPackRecvState = (CommanPacketFromFpgaWithStateInfo*)buf;
		emit handleStateInfoSignal();
		
		
	}
	if(buf[3]==0x02)
	{
		this->commandPackRecvWaveFigure = (CommanPacketFromFpgaWithWaveFigure*)buf;
		emit handleWaveInforSignal();
		
	}

}

void EquipmentTester::sendByUdp()
{
	
}

void EquipmentTester::dynamPlot()
{
	//key�ĵ�λ����
	double key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
	qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);
	//ʹ�������������������
	double value0 = qrand() % 2;

	//double value1 = qrand() % 80;
	
	ui.widget->graph(0)->addData(key, value0);//�������1������1
	//ui.widget->graph(0)->removeFromLegend(,\)
	ui.widget->graph(0)->removeDataBefore(key - 1);

	ui.widget->graph(0)->rescaleValueAxis();

	ui.widget->graph(0)->setLineStyle(QCPGraph::lsStepCenter);

	//�����8����ָ������ʱ����Ϊ8�룬�����Ҫ��������ʾ�����ʱ��
	//�Ͱ�8����Ϊ�Ƚϴ�ֵ������Ҫ��ʾ60�룬�Ǿ͸ĳ�60��
	//��ʱremoveDataBefore(key-8)�е�8ҲҪ�ĳ�60������������ʾ��������
	ui.widget->xAxis->setRange(key + 0.25, 1, Qt::AlignRight);//�趨x��ķ�Χ
	ui.widget->replot();
}

EquipmentTester::~EquipmentTester()
{
	delete can_configurations_;
	delete commandPackSend;
	delete commandPackRecvState;
	delete reset;
}
unsigned EquipmentTester::getChecksum(unsigned char* buf, unsigned size)
{
	int i = 0;
	unsigned char checksum = 0;
	if ((buf == NULL) || (size == 0))
	{
		return 0;
	}
	for (int i = 0; i < size; i++)
	{
		checksum = (checksum + buf[i]);
	}
	return checksum;
}

void EquipmentTester::bindTree(QTreeWidgetItem* item,int count)
{
	QString itemtext = item->text(count);
	if (item->text(count) == "XS2")
		ui.stackedWidget->setCurrentIndex(0);
	else if (item->text(count) == "XS3")
		ui.stackedWidget->setCurrentIndex(1);
	else if (item->text(count) == "XS4")
		ui.stackedWidget->setCurrentIndex(2);
	else if (item->text(count) == "XS5")
		ui.stackedWidget->setCurrentIndex(3);
	else if (item->text(count) == "XS9")
		ui.stackedWidget->setCurrentIndex(4);
	else if (item->text(count) == "XS10")
		ui.stackedWidget->setCurrentIndex(5);
	else if (item->text(count) == "XS11")
		ui.stackedWidget->setCurrentIndex(6);
	else if (item->text(count) == "XS12")
		ui.stackedWidget->setCurrentIndex(7);
	else if (item->text(count) == "XS13")
		ui.stackedWidget->setCurrentIndex(8);
}
void EquipmentTester::InitSwitch()
{
	ui.horizontalSlider->setMinimum(0);
	ui.horizontalSlider->setMaximum(1);

	ui.horizontalSlider_2->setMinimum(0);
	ui.horizontalSlider_2->setMaximum(1);

	ui.horizontalSlider_3->setMinimum(0);
	ui.horizontalSlider_3->setMaximum(1);

	ui.horizontalSlider_4->setMinimum(0);
	ui.horizontalSlider_4->setMaximum(1);

	ui.verticalSlider->setMinimum(0);
	ui.verticalSlider->setMaximum(1);

	ui.verticalSlider_2->setMinimum(0);
	ui.verticalSlider_2->setMaximum(1);

	
}

void EquipmentTester::InitFont()
{
	QFont font("SongTi", 15, 40);
	ui.label_6->setFont(font);
}

void EquipmentTester::zuoPaoXuanZeKouJiSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum = 0;
	if(ui.horizontalSlider->value()==1)
	{
	
		commandPackSend->zuoPaoXuanZeKouJi = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("������ѡ��ۻ����سɹ�\n"));
			ui.textBrowser->repaint();
		}
		else
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("������ѡ��ۻ�����ʧ��\n"));
			ui.textBrowser->repaint();
		}
	}
	else
	{
		commandPackSend->zuoPaoXuanZeKouJi = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus = udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر�����ѡ��ۻ����سɹ�\n"));
			ui.textBrowser->repaint();
		}
		else
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر�����ѡ��ۻ�����ʧ��\n"));
			ui.textBrowser->repaint();
		}
	}

}
void EquipmentTester::zuoPaoXuanZeYangDanSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum = 0;
	if (ui.horizontalSlider_3->value() == 1)
	{
		commandPackSend->zuoPaoXuanZeYangDan = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("������ѡ���ﵯ���سɹ�\n"));
			ui.textBrowser->repaint();
		}
		else
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("������ѡ���ﵯ����ʧ��\n"));
			ui.textBrowser->repaint();
		}
	}	
	else
	{
		commandPackSend->zuoPaoXuanZeYangDan = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر�����ѡ���ﵯ���سɹ�\n"));
			ui.textBrowser->repaint();
		}
		else
		{
				ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر�����ѡ���ﵯ����ʧ��\n"));
		ui.textBrowser->repaint();
		}
	
	}
	
}

void EquipmentTester::youPaoXuanZeYangDanSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum = 0;
	if (ui.horizontalSlider_4->value() == 1)
	{
		commandPackSend->youPaoXuanZeYangDan = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("������ѡ���ﵯ���سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
	else
	{
		commandPackSend->youPaoXuanZeYangDan = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر�����ѡ���ﵯ���سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
}
void EquipmentTester::youPaoXuanZeKouJiSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum = 0;
	if (ui.horizontalSlider_2->value() == 1)
	{
		commandPackSend->youPaoXuanZeKouJi = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("������ѡ��ۻ����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
	else
	{
		commandPackSend->youPaoXuanZeKouJi = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر�����ѡ��ۻ����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
}

void EquipmentTester::shootingButtonSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum = getChecksum((unsigned char*)commandPackSend, 32);
	commandPackSend->shootingButton = 1;
	commandPackSend->checksum = checksum;
	flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
	if (flagOfCommanSendStatus > 0)
	{
		ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("��������ť\n"));
		ui.textBrowser->repaint();
	}
}

void EquipmentTester::zuo1YanMuDanSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum;
	if (ui.verticalSlider->value() == 1)
	{
		commandPackSend->zuo1YanMuDan = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("����1��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
	else
	{
		commandPackSend->zuo1YanMuDan = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if(flagOfCommanSendStatus>0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر���1��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
}
void EquipmentTester::zuo2YanMuDanSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum;
	if (ui.verticalSlider->value() == 1)
	{
		commandPackSend->zuo2YanMuDan = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("����2��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
	else
	{
		commandPackSend->zuo2YanMuDan = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if(flagOfCommanSendStatus>0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر���2��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}

}
void EquipmentTester::zuo3YanMuDanSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum;
	if (ui.verticalSlider->value() == 1)
	{
		commandPackSend->zuo3YanMuDan = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("����3��Ļ�����سɹ�\n"));
		ui.textBrowser->repaint();
	}
	else
	{
		commandPackSend->zuo3YanMuDan = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if(flagOfCommanSendStatus>0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر���3��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}

}

void EquipmentTester::zuo4YanMuDanSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum;
	if (ui.verticalSlider->value() == 1)
	{
		commandPackSend->zuo4YanMuDan = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("����4��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
	else
	{
		commandPackSend->zuo4YanMuDan = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر���4��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
}
void EquipmentTester::you1YanMuDanSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum;
	if (ui.verticalSlider->value() == 1)
	{
		commandPackSend->you1YanMuDan = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("����1��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
	else
	{
		commandPackSend->you1YanMuDan = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر���1��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
}

void EquipmentTester::you2YanMuDanSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum;
	if (ui.verticalSlider->value() == 1)
	{
		commandPackSend->you2YanMuDan = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("����2��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
	else
	{
		commandPackSend->you2YanMuDan = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر���2��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
}

void EquipmentTester::you3YanMuDanSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum;
	if (ui.verticalSlider->value() == 1)
	{
		commandPackSend->you3YanMuDan = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("����3��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
	else
	{
		commandPackSend->you3YanMuDan = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر���3��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
}
void EquipmentTester::you4YanMuDanSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum;
	if (ui.verticalSlider->value() == 1)
	{
		commandPackSend->you4YanMuDan = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("����4��Ļ�����سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
	else
	{
		commandPackSend->you4YanMuDan = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=	udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
		ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر���4��Ļ�����سɹ�\n"));
		ui.textBrowser->repaint();
		}

	}
}
void EquipmentTester::yangDanJiZhiShiYouSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum;
	if (ui.verticalSlider->value() == 1)
	{
		commandPackSend->yangDanJiZhiShiYou = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("���ﵯָʾ�ҿ��سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
	else
	{
		commandPackSend->you4YanMuDan = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر��ﵯָʾ�ҿ��سɹ�\n"));
			ui.textBrowser->repaint();
		}
	}
}
void EquipmentTester::yangDanJiZhiShiZuoSlot()
{
	memset(commandPackSend, 0, sizeof(CommanPacketToFpgaWithoutFixedFrequency));
	commandPackSend->frameFlag = 0xEB90;
	commandPackSend->frameLength = 0x0100;
	commandPackSend->frameType = 0x03;
	unsigned char checksum;
	if (ui.verticalSlider_2->value() == 1)
	{
		commandPackSend->yangDanJiZhiShiZuo = 1;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("���ﵯָʾ�󿪹سɹ�\n"));
			ui.textBrowser->repaint();
		}
		else
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("���ﵯָʾ�󿪹�ʧ��\n"));
			ui.textBrowser->repaint();
		}
		
	}
	else
	{
		commandPackSend->yangDanJiZhiShiZuo = 0;
		checksum = getChecksum((unsigned char*)commandPackSend, 32);
		commandPackSend->checksum = checksum;
		flagOfCommanSendStatus = udpSocket->writeDatagram((const char*)commandPackSend, 32, QHostAddress::LocalHost, 7777);
		if (flagOfCommanSendStatus > 0)
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر��ﵯָʾ�󿪹سɹ�\n"));
			ui.textBrowser->repaint();
		}
		else
		{
			ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("�ر��ﵯָʾ�󿪹�ʧ��\n"));
			ui.textBrowser->repaint();
		}
	}
}
void EquipmentTester::handleStateInfo()
{
	/*��̤��ư���1*/
	if(commandPackRecvState->jiaoTaSheJianJian1==1)
	{
		ui.widget_6->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_6->set_state(FrLight::kMalfunction);
	}

	/*��̤�������2*/
	if (commandPackRecvState->jiaoTaSheJianJian2 == 1)
	{
		ui.widget_7->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_7->set_state(FrLight::kMalfunction);
	}

	/*������Ļ��*/
	if(commandPackRecvState->youSiYanMuDan==1)
	{
		ui.widget_8->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_8->set_state(FrLight::kMalfunction);
	}

	/*������Ļ��*/
	if(commandPackRecvState->youSanYanMuDan==1)
	{
		ui.widget_9->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_9->set_state(FrLight::kMalfunction);
	}

	/*�Ҷ���Ļ��*/
	if(commandPackRecvState->youErYanMuDan==1)
	{
		ui.widget_10->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_10->set_state(FrLight::kMalfunction);
	}

	/*��1��Ļ��*/
	if(commandPackRecvState->youYiYanMuDan==1)
	{
		ui.widget_11->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_11->set_state(FrLight::kMalfunction);
	}

	/*��4��Ļ��*/
	if(commandPackRecvState->zuoSiYanMuDan==1)
	{
		ui.widget_12->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_12->set_state(FrLight::kMalfunction);
	}

	/*��3��Ļ��*/
	if(commandPackRecvState->zuoSanYanMuDan==1)
	{
		ui.widget_13->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_13->set_state(FrLight::kMalfunction);
	}

	/*��2��Ļ��*/
	if(commandPackRecvState->zuoErYanMuDan==1)
	{
		ui.widget_14->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_14->set_state(FrLight::kMalfunction);
	}

	/*��1��Ļ��*/
	if(commandPackRecvState->zuoYiYanMuDan==1)
	{
		ui.widget_15->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_15->set_state(FrLight::kMalfunction);
	}

	/*��վָʾ*/
	if(commandPackRecvState->powerStationIndicate==1)
	{
		ui.widget_16->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_16->set_state(FrLight::kMalfunction);
	}


	/*�ۺϹ���ָʾ*/
	if(commandPackRecvState->generalMangaeIndicate==1)
	{
		ui.widget_17->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_17->set_state(FrLight::kMalfunction);
	}


	/*����ָʾ*/
	if(commandPackRecvState->navigationPoseIndicate==1)
	{
		ui.widget_18->set_state(FrLight::kNormal);
	}
	else
	{
	ui.widget_18->set_state(FrLight::kMalfunction);
	}


	/*ͨ��װ��ָʾ*/
	if(commandPackRecvState->communicationEuqipIndicate==1)
	{
		ui.widget_19->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_19->set_state(FrLight::kMalfunction);
	}

	/*�����״�ָʾ*/
	if(commandPackRecvState->ridarIndicate==1)
	{
		ui.widget_20->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_20->set_state(FrLight::kMalfunction);
	}

	/*����ָʾ*/
	if(commandPackRecvState->laserIndicate==1)
	{
		ui.widget_21->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_21->set_state(FrLight::kMalfunction);
	}

	/*�涯ָʾ*/
	if(commandPackRecvState->moveIndicate==1)
	{
		ui.widget_22->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_22->set_state(FrLight::kMalfunction);
	}

	/*����ָʾ*/
	if(commandPackRecvState->missleIndicate==1)
	{
		ui.widget_23->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_23->set_state(FrLight::kMalfunction);
	}
	/*���ٲ���ָʾ*/
	if(commandPackRecvState->measuerInitSpeed==1)
	{
		ui.widget_24->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_24->set_state(FrLight::kMalfunction);
	}


	/*�ܵ�Դָʾ*/
	if(commandPackRecvState->powerIndicate==1)
	{
		ui.widget_25->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_25->set_state(FrLight::kMalfunction);
	}

	/*�����״�ָʾ*/
	if(commandPackRecvState->trackingLidarIndicate==1)
	{
		ui.widget_26->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_26->set_state(FrLight::kMalfunction);
	}

	/*��ػ�ָʾ*/
	if(commandPackRecvState->fireControlIndicate==1)
	{
		ui.widget_27->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_27->set_state(FrLight::kMalfunction);
	}

	/*�ȳ�����ָʾ*/
	if(commandPackRecvState->heatProducerIndicate==1)
	{
		ui.widget_28->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_28->set_state(FrLight::kMalfunction);
	}


	/*���ֲ���ָʾ*/
	if(commandPackRecvState->humanOperationIndicate==1)
	{
		ui.widget_29->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_29->set_state(FrLight::kMalfunction);
	}

	/*����ָʾ*/
	if(commandPackRecvState->tvIndicate==1)
	{
		ui.widget_30->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_30->set_state(FrLight::kMalfunction);
	}

	/*�ۻ���Դ��ʾ*/


	/*�������ڸ澯*/
	if(commandPackRecvState->danJinYouNeiGaoJing==1)
	{
		ui.widget_33->set_state(FrLight::kNormal);
		ui.widget_32->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_32->set_state(FrLight::kMalfunction);
		ui.widget_33->set_state(FrLight::kMalfunction);
	}

	/*���ŵ�λָʾ��*/
	if (commandPackRecvState->zuoKaiMenDaoWeiZhiShiDeng == 1)
	{
		ui.widget_34->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_34->set_state(FrLight::kMalfunction);
	}

	/*�ҿ��ŵ�λָʾ��*/
	if (commandPackRecvState->youKaiMenDaoWeiZhiShiDeng == 1)
	{
		ui.widget_35->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_35->set_state(FrLight::kMalfunction);
	}


	/*���ָʾ��*/
	if (commandPackRecvState->sheJieZhiShiDeng == 1)
	{
		ui.widget_36->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_36->set_state(FrLight::kMalfunction);
	}

	/*�������䵽λ*/
	if (commandPackRecvState->zuoZhuDanXiangDaoWei == 1)
	{
		ui.widget_37->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_37->set_state(FrLight::kMalfunction);
	}

	/*�������䵽λ*/
	if (commandPackRecvState->youZhuDanXiangDaoWei == 1)
	{
		ui.widget_38->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_38->set_state(FrLight::kMalfunction);
	}

	/*�󸱵��䵽λ*/
	if (commandPackRecvState->zuoFuDanXiangDaoWei == 1)
	{
		ui.widget_39->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_39->set_state(FrLight::kMalfunction);
	}

	/*�Ҹ����䵽λ*/
	if (commandPackRecvState->youFuDanXiangDaoWei == 1)
	{
		ui.widget_40->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_40->set_state(FrLight::kMalfunction);
	}


	/*�߽Ǽ���ָʾ��*/
	if (commandPackRecvState->gaoJiaoJiXianZhiShiDeng == 1)
	{
		ui.widget_41->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_41->set_state(FrLight::kMalfunction);
	}

	/*�ͽǼ���ָʾ��*/
	if (commandPackRecvState->gaoJiaoJiXianZhiShiDeng == 1)
	{
		ui.widget_44->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_44->set_state(FrLight::kMalfunction);
	}
	/*�ߵͻ���ָʾ��*/
	if (commandPackRecvState->gaoDiJiDongZhiShiDeng == 1)
	{
		ui.widget_42->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_42->set_state(FrLight::kMalfunction);
	}


	/*�ߵͻ���ָʾ��*/
	if (commandPackRecvState->gaoDiJieTuoZhiShiDeng == 1)
	{
		ui.widget_43->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_43->set_state(FrLight::kMalfunction);
	}

	/*��λ����ָʾ��*/
	if (commandPackRecvState->fangWeiJiDongZhiShiDeng == 1)
	{
		ui.widget_45->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_45->set_state(FrLight::kMalfunction);
	}

	/*��λ����ָʾ��*/
	if (commandPackRecvState->fangWeiJieTuoZhiShiDeng == 1)
	{
		ui.widget_46->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_46->set_state(FrLight::kMalfunction);
	}


	///*����ָʾ��*/
	//if (commandPackRecvState->indicateLight == 1)
	//{
	//	ui.widget_47->set_state(FrLight::kNormal);
	//}
	//else
	//{
	//	ui.widget_47->set_state(FrLight::kMalfunction);
	//}


	/*��ѹ������λ*/
	if (commandPackRecvState->zuoYaDanQiDaoWei == 1)
	{
		ui.widget_48->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_48->set_state(FrLight::kMalfunction);
	}

	
	/*��ѹ������λ*/
	if (commandPackRecvState->youYaDanQiDaoWei == 1)
	{
		ui.widget_49->set_state(FrLight::kNormal);
	}
	else
	{
		ui.widget_49->set_state(FrLight::kMalfunction);
	}

}

void EquipmentTester::hanndleWaveInfo()
{
	
}
void EquipmentTester::resetCAN()
{
	memset(reset, 0, 16);
	reset->frameFlag = 0x7D23;
	reset->frameLength = 0x08;
	reset->type = 0x05;
	reset->value = 0xAA5555AA;
	unsigned char checksum = getChecksum((unsigned char*)reset, 16);
	reset->checksum = checksum;
	flagOfCommanSendStatus=udpSocket->writeDatagram((const char*)reset, 16, QHostAddress::LocalHost, 7777);
	if (flagOfCommanSendStatus > 0)
	{
		ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("��λ�ɹ�\n"));
	}
	else
	{
		ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") + " " + QString::fromLocal8Bit("��λʧ��\n"));
	}
}





