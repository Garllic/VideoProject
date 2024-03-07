#pragma once
#include "CBuffer.h"
#include "Base.h"

class MediaFile
{
private:
	FILE* m_file;
	std::string m_filepath;
	long m_size;
	int m_type;				//文件类型
public:
	MediaFile();
	~MediaFile();
	int Open(const std::string& path, int nType);
	//如果CBuffer的size=0，则表示读取完毕
	CBuffer ReadOneFrame();
	void Close();
	//重置后，可以重新读取帧信息
	void Reset();
private:
	CBuffer ReadH264Frame();
	//返回-1表示查找失败
	long FindH264Head();
};

