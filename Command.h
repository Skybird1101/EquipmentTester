
#ifndef _COMMAND_H_
#define  _COMMAND_H_
#pragma pack(push,1)


/*��FPGA���͵����ݰ���ʽ*/
typedef struct _command_pack_to_fpga
{
	unsigned short frameFlag;//֡��ʶ
	unsigned short frameLength;//֡����
	unsigned char frameType;//֡���ͱ�ʶ
	unsigned char zuoPaoXuanZeKouJi;//����ѡ��ۻ�
	unsigned char zuoPaoXuanZeYangDan;//����ѡ���ﵯ
	unsigned char youPaoXuanZeKouJi;//����ѡ��ۻ�
	unsigned char youPaoXuanZeYangDan;//����ѡ���ﵯ
	unsigned char shootingButton;//�������
	unsigned char zuo1YanMuDan;//��1��Ļ��
	unsigned char zuo2YanMuDan;//��2��Ļ��
	unsigned char zuo3YanMuDan;//��3��Ļ��
	unsigned char zuo4YanMuDan;//��4��Ļ��
	unsigned char you1YanMuDan;//��1��Ļ��
	unsigned char you2YanMuDan;//��2��Ļ��
	unsigned char you3YanMuDan;//��3��Ļ��
	unsigned char you4YanMuDan;//��4��Ļ��
	unsigned char yangDanJiZhiShiZuo;//�ﵯ��ָʾ��
	unsigned char yangDanJiZhiShiYou;//�ﵯ��ָʾ��
	unsigned char youYangDanJiCount;//���ﵯ������
	unsigned char zuoYangDanJiCount;//���ﵯ������
	unsigned char danJinYouNeiGaoJing;//�������ڸ澯
	unsigned char danJinZuoNeiGaoJing;//�������ڸ澯
	unsigned char zuoZhuDanXiangDaoWei;//�������䵽λ
	unsigned char zuoFuDanXiangDaoWei;//�󸱵��䵽λ
	unsigned char youZhuDanXiangDaoWei;//�������䵽λ
	unsigned char youFuDanXiangDaoWei;//�󸱵��䵽λ
	unsigned char youKaiMenDaoWei;//�ҿ��ŵ�λ
	unsigned char reserve;//����
	unsigned char checksum;//У���

}CommanPacketToFpgaWithoutFixedFrequency;

/*���ܴ�FPGA���͵����ݰ���ʽ(״̬��Ϣ)*/
typedef struct _command_pack_from_fpga
{
	unsigned short frameFlag;//֡��ʶ
	unsigned short  frameLength;//֡����
	unsigned char  type;//֡����
	unsigned char  jiaoTaSheJianJian1;//��̤��ư���1
	unsigned char  jiaoTaSheJianJian2;//��̤��ư���2
	unsigned char  youSiYanMuDan;//��4��Ļ��
	unsigned char  youSanYanMuDan;//��3��Ļ��
	unsigned char  youErYanMuDan;//��2��Ļ��
	unsigned char  youYiYanMuDan; //��1��Ļ��
	unsigned char  zuoSiYanMuDan;//��4��Ļ��
	unsigned char  zuoSanYanMuDan;//��3��Ļ��
	unsigned char  zuoErYanMuDan;//��2��Ļ��
	unsigned char  zuoYiYanMuDan;//��1��Ļ��
	unsigned char  powerStationIndicate;//��վָʾ
	unsigned char  generalMangaeIndicate;//�ۺϹ���ָʾ
	unsigned char  navigationPoseIndicate;//����/λ��ָʾ
	unsigned char  communicationEuqipIndicate;//ͨ��װ��ָʾ
	unsigned char  ridarIndicate;//�����״�ָʾ
	unsigned char  laserIndicate;//����ָʾ
	unsigned char  moveIndicate;//�涯ָʾ
	unsigned char  missleIndicate;//����ָʾ
	unsigned char  measuerInitSpeed;//���ٲ���ָʾ
	unsigned char  powerIndicate;//�ܵ�Դָʾ
	unsigned char trackingLidarIndicate;//׷���״�ָʾ
	unsigned char fireControlIndicate;//��ػ�ָʾ
	unsigned char heatProducerIndicate;//�ȳ�����ָʾ
	unsigned char humanOperationIndicate;//���ֲ���ָʾ
	unsigned char tvIndicate;//����ָʾ
	unsigned char visualTrackingIndicate;//��Ƶ׷��ָʾ
	unsigned char indicateLight;//����ָʾ��
	unsigned char danJinYouNeiGaoJing;//�������ڸ澯
	unsigned char danJinZuoNeiGaoJing;//�������ڸ澯
	unsigned char zuoKaiMenDaoWeiZhiShiDeng;//���ŵ�λָʾ��
	unsigned char youKaiMenDaoWeiZhiShiDeng;//�ҿ��ŵ�λָʾ��
	unsigned char sheJieZhiShiDeng;//���ָʾ��
	unsigned char zuoZhuDanXiangDaoWei;//�������䵽λ
	unsigned char zuoFuDanXiangDaoWei;//�󸱵��䵽λ
	unsigned char youZhuDanXiangDaoWei;//�������䵽λ
	unsigned char youFuDanXiangDaoWei;//�󸱵��䵽λ
	unsigned char gaoJiaoJiXianZhiShiDeng;//�߽Ǽ���ָʾ��
	unsigned char gaoDiJiDongZhiShiDeng;//�ߵͻ���ָʾ��
	unsigned char gaoDiJieTuoZhiShiDeng;//�ߵͽ���ָʾ��
	unsigned char diJiaoJiXianZhiShiDeng;//�ͽǼ���ָʾ��
	unsigned char fangWeiJiDongZhiShiDeng;//��λ����ָʾ��
	unsigned char fangWeiJieTuoZhiShiDeng;//��λ����ָʾ��
	unsigned char zuoYaDanQiDaoWei;//��ѹ������λָʾ��
	unsigned char youYaDanQiDaoWei;//��ѹ������λָʾ��
	unsigned char reserve[13];//����
	unsigned char cheksum;//У���
	
}CommanPacketFromFpgaWithStateInfo;


/*FPGA-PC ���ΰ�*/
typedef struct _commanpackwave
{
	unsigned short frameFlag;//֡��ʶ
	unsigned short frameLength;//֡����
	unsigned char type;//֡����
	unsigned char zuoBoDanKongZhi;//�󲦵�����
	unsigned char youBoDanKongZhi;//�Ҳ�������
	unsigned char zuoKaiMenKongZhi;//���ſ���
	unsigned char youKaiMenKongZhi;//�ҿ��ſ���
	unsigned char zuoYaDanKongZhi;//��ѹ������
	unsigned char zuoJieTuoKongZhi;//����ѿ���
	unsigned char youJieTuoKongZhi;//�ҽ��ѿ���
	unsigned char youYaDanKongZhi;//��ѹ������
	unsigned char zuoJiFaKongZhi;//���������
	unsigned char youJiFaKongZhi;//�һ�������
	unsigned char zuoYangDanJiKongZhi;//���ﵯ������
	unsigned char youYangDanJiKongZhi;//���ﵯ������
	unsigned char reserve[14];
}CommanPacketFromFpgaWithWaveFigure;




/*CAN���������õ�������*/
typedef struct _canBaud{
	unsigned short frameFlag;//֡��ʶ
	unsigned char frameLength;//֡����
	unsigned char type;//֡����
	unsigned short baudRate;//�û����õĲ�����
	unsigned char  timer;//�û����õĶ�ʱ���
	unsigned char checkSum;//У���
}CanConfigurations;



/*��λ����*/
typedef struct _reset
{
	unsigned short frameFlag;//֡��ʶ
	unsigned char frameLength;//֡����
	unsigned char type;//֡����
	unsigned int value;
	unsigned char reserve[7];
	unsigned char checksum;
}ReSet;
#pragma  pack(pop)
#endif
