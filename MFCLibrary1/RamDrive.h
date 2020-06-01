// RamDrive.h: interface for the QRamDrive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(RAMDRIVE_H)
#define RAMDRIVE_H

//编写人:沈宇亮  日期:2004-11-8
//功能: 本类用于实时数据的采集驱动与实时数据库之间的交换

//驱动内存包括四个栏目
//1. 接收命令区
//2. 实时数据区
//3. 结构传输区

#include <stdio.h>

#define		MAX_DATA_SIZE			2000	//实时数据区的缓冲大小设置
#define		MAX_STRUCT_NUM			25		//上行结构变量的缓冲大小设置
#define		MAX_STRUCT_NUM_DOWN		20		//下行结构变量的缓冲大小设置

#define  SHM_SIZE   25000					//共享内存区的大小

//size 21224
struct StructDrive
{
	int iVersion;					//共享区的版本
	char  chExistGetCmd;			//是否存在下传命令  0 不存在  1 存在
	char  chGetCmd[200];			//命令接收区
	int     iValue[MAX_DATA_SIZE];		//实时数据区
	int iStructNum;			//上传文本结构数据个数  当没有时为0 当存在时为具体的数目
	char chStruct[MAX_STRUCT_NUM][200];  //上传文本结构数据

	int iStructNumDown;		//下传文本结构数据个数  当没有时为0 当存在时为具体的数目
	char chStructDown[MAX_STRUCT_NUM_DOWN][200];  //下传文本结构数据
};

class QRamDrive
{
public:
	QRamDrive();
	virtual ~QRamDrive();

	bool CreateRam(int ramid);
	bool bRam;
	int iRamid;


	HANDLE hMapFile;
	LPVOID  lpMapAddress;

	StructDrive    * drive;
	void SetExit();   //send exit cmd
	void SetCmd(char * chCmd);
	void GetCmd(char * chCmd);
	bool GetExistCmd() {			//判断命令区是否存在命令
		if (drive == NULL)
			return false;
		else
			return   (drive->chExistGetCmd != 0);
	}

	void SetValue(int id, int iValue) {	//设置数据区值
		if (drive == NULL) return;
		drive->iValue[id] = iValue;
	}
	int GetValue(int id) {					//取数据区值
		if (drive == NULL) return 0;
		return  drive->iValue[id];
	}

	void SetStructNum(int iStructNum) { //设置上行结构变量个数
		if (drive == NULL) return;
		drive->iStructNum = iStructNum;
	}

	int GetStructNum() {	//取上行结构变量个数
		if (drive == NULL) return 0;
		return drive->iStructNum;
	}

	void SetStruct(int iNum, char * chMsg);// 设置上行结构变量

	char * GetStruct(int iNum) { return *(drive->chStruct + iNum); }//取上行结构变量

	int GetStructNumDown() { return drive->iStructNumDown; }//取下行结构变量个数
	void SetStructNumDown(int iNum) { drive->iStructNumDown = iNum; }//设置下行结构变量个数

	char * GetStructDown(int iNum) { return *(drive->chStructDown + iNum); }	//取下行结构变量
	void SetStructDown(int iNum, char * chMsgDown)	//设置下行结构变量
	{
		strcpy_s(*(drive->chStructDown + iNum), chMsgDown);
	}

	void ClearCmdMark() {		//清除命令标志
		if (drive == NULL)
			return;
		else
			drive->chExistGetCmd = 0;
	}

	char * GetCmdWithoutClearMark() { return drive->chGetCmd; }

};
#endif //
