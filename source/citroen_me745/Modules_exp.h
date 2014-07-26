#define GetLibVer Lunteg
#define GetKnownECU Vupsen
#define RecogniseECU Pupsen
#define GetChksNum Kuzya
#define FixChkSum Pchelenok
#define GetChkSum Shnuk

	struct ECU_info
		{
			LPVOID EcuBuffer;   // ��������� �� ����� � ����������� ������
			unsigned int EcuFileSize;	// ������ �����
			char ECU_Descr[100]; // �������� ������� ��������
			int ChkSumNumber;	// ���������� �������� �� � ��������
			int BadChkSumNumber; //���������� ������������ �� � ��������
		} ;


#define DLL_SPEC extern "C" __declspec(dllexport)

DLL_SPEC char* GetLibVer(); // ���������� �� ����������. ���������� ������ � ��������� ������
DLL_SPEC char* GetKnownECU(); // ������ �������� ECU � ����������. ���������� ������ �� ������� ECU
DLL_SPEC int RecogniseECU(ECU_info *); // �������� ECU. ���� ��������, ���������� 1
DLL_SPEC int GetChksNum(); // ���������� ���������� �� � ��������. ���������� ���-�� ��������� ��
DLL_SPEC int FixChkSum(ECU_info *); // ������������ ����������� �����. ���������� ���-�� ������������ ��
DLL_SPEC int GetChkSum(); // �������� �� � ��������. ���������� 1 ���� ��� �� �����

unsigned int FindHexStr(unsigned int SearchOffset, unsigned int BufferSize, unsigned char *SearchBuffer, unsigned char *HexBytes, unsigned int HexCount);
//����� ������������������ ���� � �����. ���������� �������� �� ������ ������
//SearchOffset �������� �� ������ ����� ��� ������ ������
//BufferSize ������ ������
//SearchBuffer ��������� �� ����� � ������
//HexBytes ��������� �� ������������������ ������� ����
//HexCount ���������� ���� ��� ������

unsigned int SummInt8(unsigned int StartInt8, unsigned int EndInt8, unsigned char *SearchBuffer);
//������� ���������� ����� � ��������� ������� ��  StartInt8 �� EndInt8
// ���������� ������������ �����

unsigned int SummInt16Intel(unsigned int StartInt16, unsigned int EndInt16, unsigned char *SearchBuffer);
//������� ����� Word � ��������� ������� ��  StartInt16 �� EndInt16 � ������� Intel
// ���������� ������������ �����

unsigned int SummInt16Mot(unsigned int StartInt16, unsigned int EndInt16, unsigned char *SearchBuffer);
//������� ����� Word � ��������� ������� ��  StartInt16 �� EndInt16 � ������� Motorola
// ���������� ������������ �����

unsigned int Read4ByteIntel (unsigned char *ReadAddress);
//��������� � ������ ReadAddress 4 ����� � ������� ����� � ���������� ��

void Write4ByteIntel (unsigned int WrDword, unsigned char *WriteAddress);
//���������� �� ������ WriteAddress 4 ����� � ������� �����

unsigned int SummBlock(unsigned int StartBlock, unsigned int EndBlock, unsigned char *SearchBuffer);
//������� ��������� ����� (803C) � ��������� ������� ��  StartBlock �� EndBlock � ������� Intel
// ���������� ������������ �����

unsigned int CalcCRC32(unsigned int StartBlock, unsigned int EndBlock, unsigned char *Buffer);
//������� CRC32 ������� �� �������� StartBlock �� ������ ������ �� �������� EndBlock

unsigned int CalcCRC32Cont(unsigned int StartBlock, unsigned int EndBlock, unsigned char *Buffer, unsigned int crc_start);
//������� ����������� CRC32 ������� �� �������� StartBlock �� ������ ������ �� �������� EndBlock
//� ������ ����������� �������� CRC32 crc_start
