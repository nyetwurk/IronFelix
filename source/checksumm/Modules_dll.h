#define GetLibVer "Lunteg"
#define GetKnownECU "Vupsen"
#define RecogniseECU "Pupsen"
#define GetChksNum "Kuzya"
#define FixChkSum "Pchelenok"
#define GetChkSum "Shnuk"
	struct ECU_info
		{
			LPVOID EcuBuffer;   // ��������� �� ����� � ����������� ������
			unsigned int EcuFileSize;	// ������ �����
			char ECU_Descr[100]; // �������� ������� ��������
			int ChkSumNumber;	// ���������� �������� �� � ��������
			int BadChkSumNumber; //���������� ������������ �� � ��������
		} ;


typedef char* (*GetLibVer_Imp)();
GetLibVer_Imp GetLibVer_proc; // ���������� �� ����������. ���������� ������ � ��������� ������
typedef char* (*GetKnownECU_Imp)();
GetKnownECU_Imp GetKnownECU_proc; // ������ �������� ECU � ����������. ���������� ������ �� ������� ECU
typedef int (*RecogniseECU_Imp)(ECU_info *);
RecogniseECU_Imp RecogniseECU_proc; // �������� ECU. ���� ��������, ���������� 1
typedef int (*GetChksNum_Imp)();
GetChksNum_Imp GetChksNum_proc; // ���������� ���������� �� � ��������. ���������� ���-�� ��������� ��
typedef int (*FixChkSum_Imp)(ECU_info *);
FixChkSum_Imp FixChkSum_proc; // ������������ ����������� �����. ���������� ���-�� ������������ ��
typedef int (*GetChkSum_Imp)();
GetChkSum_Imp GetChkSum_proc; // �������� �� � ��������. ���������� 1 ���� ��� �� �����