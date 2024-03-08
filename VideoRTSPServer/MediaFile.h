#pragma once
#include "CBuffer.h"
#include "Base.h"

class MediaFile
{
private:
	FILE* m_file;
	std::string m_filepath;
	long m_size;
	int m_type;				//�ļ�����
public:
	MediaFile();
	~MediaFile();
	int Open(const std::string& path, int nType);
	//���CBuffer��size=0�����ʾ��ȡ���
	CBuffer ReadOneFrame();
	void Close();
	//���ú󣬿������¶�ȡ֡��Ϣ
	void Reset();
private:
	CBuffer ReadH264Frame();
	//����-1��ʾ����ʧ��
	long FindH264Head();
};

