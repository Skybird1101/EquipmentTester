
#ifndef _COMMAND_H_
#define  _COMMAND_H_
#pragma pack(push,1)


/*向FPGA发送的数据包格式*/
typedef struct _command_pack_to_fpga
{
	unsigned short frameFlag;//帧标识
	unsigned short frameLength;//帧长度
	unsigned char frameType;//帧类型标识
	unsigned char zuoPaoXuanZeKouJi;//左炮选择扣击
	unsigned char zuoPaoXuanZeYangDan;//左炮选择扬弹
	unsigned char youPaoXuanZeKouJi;//右炮选择扣击
	unsigned char youPaoXuanZeYangDan;//右炮选择扬弹
	unsigned char shootingButton;//射击按键
	unsigned char zuo1YanMuDan;//左1烟幕弹
	unsigned char zuo2YanMuDan;//左2烟幕弹
	unsigned char zuo3YanMuDan;//左3烟幕弹
	unsigned char zuo4YanMuDan;//左4烟幕弹
	unsigned char you1YanMuDan;//右1烟幕弹
	unsigned char you2YanMuDan;//右2烟幕弹
	unsigned char you3YanMuDan;//右3烟幕弹
	unsigned char you4YanMuDan;//右4烟幕弹
	unsigned char yangDanJiZhiShiZuo;//扬弹机指示左
	unsigned char yangDanJiZhiShiYou;//扬弹机指示右
	unsigned char youYangDanJiCount;//右扬弹机计数
	unsigned char zuoYangDanJiCount;//左扬弹机计数
	unsigned char danJinYouNeiGaoJing;//弹尽右内告警
	unsigned char danJinZuoNeiGaoJing;//弹尽左内告警
	unsigned char zuoZhuDanXiangDaoWei;//左主弹箱到位
	unsigned char zuoFuDanXiangDaoWei;//左副弹箱到位
	unsigned char youZhuDanXiangDaoWei;//右主弹箱到位
	unsigned char youFuDanXiangDaoWei;//左副弹箱到位
	unsigned char youKaiMenDaoWei;//右开门到位
	unsigned char reserve;//保留
	unsigned char checksum;//校验和

}CommanPacketToFpgaWithoutFixedFrequency;

/*接受从FPGA发送的数据包格式(状态信息)*/
typedef struct _command_pack_from_fpga
{
	unsigned short frameFlag;//帧标识
	unsigned short  frameLength;//帧长度
	unsigned char  type;//帧类型
	unsigned char  jiaoTaSheJianJian1;//脚踏设计按键1
	unsigned char  jiaoTaSheJianJian2;//脚踏设计按键2
	unsigned char  youSiYanMuDan;//右4烟幕弹
	unsigned char  youSanYanMuDan;//右3烟幕弹
	unsigned char  youErYanMuDan;//右2烟幕弹
	unsigned char  youYiYanMuDan; //右1烟幕弹
	unsigned char  zuoSiYanMuDan;//左4烟幕弹
	unsigned char  zuoSanYanMuDan;//左3烟幕弹
	unsigned char  zuoErYanMuDan;//左2烟幕弹
	unsigned char  zuoYiYanMuDan;//左1烟幕弹
	unsigned char  powerStationIndicate;//电站指示
	unsigned char  generalMangaeIndicate;//综合管理指示
	unsigned char  navigationPoseIndicate;//导航/位姿指示
	unsigned char  communicationEuqipIndicate;//通信装置指示
	unsigned char  ridarIndicate;//搜索雷达指示
	unsigned char  laserIndicate;//激光指示
	unsigned char  moveIndicate;//随动指示
	unsigned char  missleIndicate;//导弹指示
	unsigned char  measuerInitSpeed;//初速测量指示
	unsigned char  powerIndicate;//总电源指示
	unsigned char trackingLidarIndicate;//追踪雷达指示
	unsigned char fireControlIndicate;//火控机指示
	unsigned char heatProducerIndicate;//热成像仪指示
	unsigned char humanOperationIndicate;//炮手操作指示
	unsigned char tvIndicate;//电视指示
	unsigned char visualTrackingIndicate;//视频追踪指示
	unsigned char indicateLight;//舱门指示灯
	unsigned char danJinYouNeiGaoJing;//弹尽右内告警
	unsigned char danJinZuoNeiGaoJing;//弹尽左内告警
	unsigned char zuoKaiMenDaoWeiZhiShiDeng;//左开门到位指示灯
	unsigned char youKaiMenDaoWeiZhiShiDeng;//右开门到位指示灯
	unsigned char sheJieZhiShiDeng;//射界指示灯
	unsigned char zuoZhuDanXiangDaoWei;//左主弹箱到位
	unsigned char zuoFuDanXiangDaoWei;//左副弹箱到位
	unsigned char youZhuDanXiangDaoWei;//右主弹箱到位
	unsigned char youFuDanXiangDaoWei;//左副弹箱到位
	unsigned char gaoJiaoJiXianZhiShiDeng;//高角极限指示灯
	unsigned char gaoDiJiDongZhiShiDeng;//高低机动指示灯
	unsigned char gaoDiJieTuoZhiShiDeng;//高低解脱指示灯
	unsigned char diJiaoJiXianZhiShiDeng;//低角极限指示灯
	unsigned char fangWeiJiDongZhiShiDeng;//方位机动指示灯
	unsigned char fangWeiJieTuoZhiShiDeng;//方位解脱指示灯
	unsigned char zuoYaDanQiDaoWei;//左压弹器到位指示灯
	unsigned char youYaDanQiDaoWei;//右压弹器到位指示灯
	unsigned char reserve[13];//保留
	unsigned char cheksum;//校验和
	
}CommanPacketFromFpgaWithStateInfo;


/*FPGA-PC 波形包*/
typedef struct _commanpackwave
{
	unsigned short frameFlag;//帧标识
	unsigned short frameLength;//帧长度
	unsigned char type;//帧类型
	unsigned char zuoBoDanKongZhi;//左拨弹控制
	unsigned char youBoDanKongZhi;//右拨弹控制
	unsigned char zuoKaiMenKongZhi;//左开门控制
	unsigned char youKaiMenKongZhi;//右开门控制
	unsigned char zuoYaDanKongZhi;//左压弹控制
	unsigned char zuoJieTuoKongZhi;//左解脱控制
	unsigned char youJieTuoKongZhi;//右解脱控制
	unsigned char youYaDanKongZhi;//右压弹控制
	unsigned char zuoJiFaKongZhi;//左击发控制
	unsigned char youJiFaKongZhi;//右击发控制
	unsigned char zuoYangDanJiKongZhi;//左扬弹机控制
	unsigned char youYangDanJiKongZhi;//右扬弹机控制
	unsigned char reserve[14];
}CommanPacketFromFpgaWithWaveFigure;




/*CAN总线设置用到的命令*/
typedef struct _canBaud{
	unsigned short frameFlag;//帧标识
	unsigned char frameLength;//帧长度
	unsigned char type;//帧类型
	unsigned short baudRate;//用户设置的波特率
	unsigned char  timer;//用户设置的定时间隔
	unsigned char checkSum;//校验和
}CanConfigurations;



/*软复位命令*/
typedef struct _reset
{
	unsigned short frameFlag;//帧标识
	unsigned char frameLength;//帧长度
	unsigned char type;//帧类型
	unsigned int value;
	unsigned char reserve[7];
	unsigned char checksum;
}ReSet;
#pragma  pack(pop)
#endif
