// RamDrive.cpp: implementation of the QRamDrive class.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "ramdrive.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
QRamDrive::QRamDrive()
{

	int isize;
	isize = sizeof(StructDrive);
	drive = NULL;
	bRam = false;
	lpMapAddress = NULL;
	hMapFile = NULL;

}

QRamDrive::~QRamDrive()
{

	if (lpMapAddress)  UnmapViewOfFile(lpMapAddress);
	if (hMapFile) CloseHandle(hMapFile);

}

//按指定的编号创建共享内存区
bool QRamDrive::CreateRam(int ramid)
{
	char chRamName[30];
	sprintf_s(chRamName, "RAM%03d", ramid);
	CString strRam;
	strRam.Format(_T("RAM%03d"), ramid);

	if (lpMapAddress)  UnmapViewOfFile(lpMapAddress);
	if (hMapFile) CloseHandle(hMapFile);

	HANDLE hFile = (HANDLE)0xFFFFFFFF;

	hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, SHM_SIZE, strRam);
	if (hMapFile == NULL) {
		//TRACE( _T("创建共享内存出错\r\n") );
		char chMsg[100];
		strcpy_s(chMsg, "共享内存申请错误，请与程序供应商联系");
		::MessageBox(NULL, (LPCTSTR)chMsg, NULL, MB_OK);
		lpMapAddress = NULL;
		bRam = false;
		return false;
	}
	lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	drive = (StructDrive *)lpMapAddress;

	bRam = true;
	drive->iVersion = 1;
	iRamid = ramid;
	return true;
}

// 设置命令区的命令串
// char * chCmd 下送的命令串
void QRamDrive::SetCmd(char * chCmd)
{
	strcpy_s(drive->chGetCmd, chCmd);
	drive->chExistGetCmd = 1;
}

//取共享内存区的命令串
//char * chCmd 获取命令内容的缓冲区
void QRamDrive::GetCmd(char * chCmd)
{
	strcpy_s(chCmd, 200, drive->chGetCmd);
	drive->chExistGetCmd = 0;
}

//设置上传的结构变量
//int iNum  结构变量序号   char * chMsg  结构变量内容
void QRamDrive::SetStruct(int iNum, char * chMsg)
{
	if (drive == NULL) return;
	strcpy_s(drive->chStruct[iNum], chMsg);
}

//设置设置命令区的内容为EXIT
void QRamDrive::SetExit()
{
	strcpy_s(drive->chGetCmd, "EXIT");
	drive->chExistGetCmd = 1;
}


/*
使用范例

QRamDrive  ram  ;		//声明共享内存类变量
ram.CreateRam( 0 );		//创建0号共享内存
ram.SetValue( 1, 100 ) ;	//给1号实时变量赋值100

int iValue ;
iValue = ram.GetValue( 1 ) ;  //取1号实时变量,  应为100

ram.SetCmd(  "control 1 1 " ) ;设置命令区的内容

char chCmd[200];
if( ram.GetExistCmd(  ) )	//如果存在命令
	ram.GetCmd( chCmd );	//取命令内容入缓冲区

ram.SetStruct( 0, "soe 2006 5 15 08 09  30  6 0  1 " );// 设置0号上行结构变量
ram.SetStructNum( 1 );  //设置上行结构变量数目为1 , 上位程序根据此1取数

*/
