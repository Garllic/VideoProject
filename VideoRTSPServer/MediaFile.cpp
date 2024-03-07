#include "MediaFile.h"

MediaFile::MediaFile()
{
	m_file = NULL;
	m_size = -1;
	m_type = -1;
}

MediaFile::~MediaFile()
{
	if (m_file != NULL) {
		fclose(m_file);
	}
	delete m_file;
}

int MediaFile::Open(const std::string& path, int nType)
{
	m_file = fopen(path.c_str(), "rb");
	if (m_file == NULL) {
		ATLTRACE("文件“%s”打开失败\r\n", path.c_str());
		return -1;
	}
	m_type = nType;
	m_filepath = path;
	fseek(m_file, 0, SEEK_END);
	m_size = ftell(m_file);
	fseek(m_file, 0, SEEK_SET);

	return 0;
}

CBuffer MediaFile::ReadOneFrame()
{
	CBuffer ret;
	switch (m_type)
	{
	case 96:
		ret = ReadH264Frame();
	default:
		break;
	}

	return ret;
}

void MediaFile::Close()
{
	m_size = -1;
	m_type = -1;
	m_filepath.resize(0);
	if (m_file != NULL) {
		FILE* t = m_file;
		m_file = NULL;
		fclose(t);
	}
}

void MediaFile::Reset()
{
	fseek(m_file, 0, SEEK_SET);
}

CBuffer MediaFile::ReadH264Frame()
{
	long fHead = 0, ftail = 0;
	if (m_file) {
		fHead = FindH264Head();			//帧头
		if (fHead == -1) {
			return CBuffer();
		}
		fseek(m_file, 4, SEEK_CUR);
		ftail = FindH264Head();			//下一帧帧头
		if (ftail == -1) {
			ftail = m_size;
		}
		CBuffer ret(ftail - fHead);
		fseek(m_file, fHead, SEEK_SET);
		fread((void*)ret.c_str(), 1, ftail - fHead, m_file);	//读取一帧
		return ret;
	}

	return CBuffer();
}

long MediaFile::FindH264Head()
{
	//字节匹配00 00 00 01
	while (!feof(m_file)) {
		char c = 0x7F;
		while (!feof(m_file)) {
			c = fgetc(m_file);
			if (c == 0)break;
		}
		if (!feof(m_file)) {
			c = fgetc(m_file);
			if (c == 0) {
				c = fgetc(m_file);
				if (c == 0) {
					c = fgetc(m_file);
					if (c == 1) {
						fseek(m_file, -4, SEEK_CUR);
						return ftell(m_file);
					}
				}
			}
		}
	}

	return -1;
}
