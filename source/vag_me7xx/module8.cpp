//---------------------------------------------------------------------------

#include <windows.h>
//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------

#pragma argsused
#include <intsafe.h>
#include <strsafe.h>

#include <Modules_exp.h>

#define ECUFILESIZE1 0x80000
#define ECUFILESIZE2 0x100000

int FindFixCRC(ECU_info* dllECUinfo, int FixCRC);
//Ищет или исправляет КС
//Если FixCRC=1 то исправляет
//Возвращает 1 если успешно, 0 если ошибка



int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}
//---------------------------------------------------------------------------

char* GetLibVer()
{
	char* Ver_str="VAG Bosch ME7.XX ECU v.1.0.0.1";
	return Ver_str;
}

int RecogniseECU (ECU_info* dllECUinfo)
{
	unsigned int i, s_crc, FindOffset, HexCount1, HexCount2, HexCount3 ;
	char* FileDescr="VAG Bosch ME7.XX";
//	char cf_crc[30];

	unsigned char HexBytes1[]={0x00, 0x00, 0x80, 0x00, 0xFF, 0xFB, 0x80, 0x00, 0x00, 0x00, 0x82, 0x00, 0xFF, 0xFF};
	unsigned char HexBytes2[]={0xF0, 0x54, 0x5C, 0x25, 0x20, 0x54, 0x5C, 0x25, 0x20, 0x54, 0x5C, 0x45, 0xF0};
	unsigned char HexBytes3[]={0xF0, 0x74, 0x08, 0x71, 0xF0, 0x58, 0x20, 0x75, 0xF2, 0xF4};
	HexCount1=14;
	HexCount2=13;
	HexCount3=10;

	DWORD Buffer= (DWORD)dllECUinfo->EcuBuffer;


	//Распознаем ECU
	if (dllECUinfo->EcuFileSize!=ECUFILESIZE1 && dllECUinfo->EcuFileSize!=ECUFILESIZE2)
		{
			return 0;
		}

	FindOffset=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , HexBytes1, HexCount1);
	if (FindOffset==0xFFFFFFFF)
		{
			return 0;
		}
	FindOffset=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , HexBytes2, HexCount2);
	if (FindOffset==0xFFFFFFFF)
		{
				FindOffset=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , HexBytes3, HexCount3);
				if (FindOffset==0xFFFFFFFF)
					{
						return 0;
                    }
		}

	if (!FindFixCRC(dllECUinfo, 0))
	{
		return 0;
	}

/*	for (i = 0; i < dllECUinfo->EcuFileSize; i++)
		{
			f_crc+= *(unsigned char*)(Buffer+i);
		}
*/
	StringCbLength(FileDescr, 50, &s_crc);
	StringCbCopyN (dllECUinfo->ECU_Descr, 100, FileDescr, s_crc);
/*	IntToChar (f_crc, cf_crc);
	StringCbLength(cf_crc, 50, &s_crc);
	StringCbCatN (dllECUinfo->ECU_Descr, 100, cf_crc, s_crc+1 );
*/
/*	FindOffset=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , HexBytes, HexCount);
	f_crc=SummInt8(0, dllECUinfo->EcuFileSize-1, (unsigned char*)Buffer );
	s_crc=SummInt16Intel(0, dllECUinfo->EcuFileSize-1, (unsigned char*)Buffer );
	s_crc=SummInt16Mot(0, dllECUinfo->EcuFileSize-1, (unsigned char*)Buffer );
*/


	return 1;

}

int FixChkSum(ECU_info* dllECUinfo)
{
	unsigned int FindOffset, HexCount1, HexCount2, HexCount3;
	unsigned char HexBytes1[]={0x00, 0x00, 0x80, 0x00, 0xFF, 0xFB, 0x80, 0x00, 0x00, 0x00, 0x82, 0x00, 0xFF, 0xFF};
	unsigned char HexBytes2[]={0xF0, 0x54, 0x5C, 0x25, 0x20, 0x54, 0x5C, 0x25, 0x20, 0x54, 0x5C, 0x45, 0xF0};
	unsigned char HexBytes3[]={0xF0, 0x74, 0x08, 0x71, 0xF0, 0x58, 0x20, 0x75, 0xF2, 0xF4};
	HexCount1=14;
	HexCount2=13;
	HexCount3=10;

	DWORD Buffer= (DWORD)dllECUinfo->EcuBuffer;

	//Распознаем ECU
	if (dllECUinfo->EcuFileSize!=ECUFILESIZE1 && dllECUinfo->EcuFileSize!=ECUFILESIZE2)
		{
			return 0;
		}

	FindOffset=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , HexBytes1, HexCount1);
	if (FindOffset==0xFFFFFFFF)
		{
			return 0;
		}
	FindOffset=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , HexBytes2, HexCount2);
	if (FindOffset==0xFFFFFFFF)
		{
				FindOffset=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , HexBytes3, HexCount3);
				if (FindOffset==0xFFFFFFFF)
					{
						return 0;
                    }
		}

	if (!FindFixCRC(dllECUinfo, 1))
	{
		return 0;
	}
	return dllECUinfo->BadChkSumNumber;

}

int FindFixCRC(ECU_info* dllECUinfo, int FixCRC)
{
	 unsigned int FindOffset, EcuSubType, StartAdr1, EndAdr1, StartAdr2, EndAdr2, StartAdr3, EndAdr3, StartAdr4, EndAdr4, StartAdr5, EndAdr5, ChkSumAdr1, ChkSumAdr2, ChkSumAdr3, ChkSumAdr4, CS_off, CS_count;
	 unsigned int ChkSumm1=0, ChkSumm2=0, ChkSumm3=0, ChkSumm4=0, CalcSumm1, CalcSumm2, CalcSumm3, CalcSumm4, TotalSumm=0, BadSumm=0, MultCSAdr;
	 //Signature Block CheckSumm
	 //Signature for old ME7.X 512Kb, 1024Kb ECU
	 unsigned char EcuBlockSig1[]={0x20, 0x54, 0xC4, 0x50, 0x02, 0x00, 0xF0, 0xC8, 0xF0, 0xD9, 0xD4, 0xE0, 0x02, 0x00};
	 unsigned int EcuHexCount1=14;
	 unsigned char EcuBlockSig2[]={0x08, 0x41, 0xF0, 0x58, 0x20, 0x45, 0xC4, 0x40, 0x02, 0x00, 0xF0, 0xC8, 0xF0, 0xD9, 0xD4, 0xE0, 0x02, 0x00};
	 unsigned int EcuHexCount2=18;
	 unsigned char EcuBlockSig3[]={0x88, 0x50, 0x88, 0x40, 0xF0, 0xC8, 0xF0, 0xD9, 0xD4, 0xE0, 0x06, 0x00, 0xDA};
	 unsigned int EcuHexCount3=13;
	 //Signature for new ME7.X 1024Kb ECU
	 unsigned char EcuBlockSig4[]={0xF0, 0x74, 0x08, 0x71, 0xF0, 0x58, 0x20, 0x75, 0xF2, 0xF4};
	 unsigned int EcuHexCount4=10;
	 unsigned char CSummSig1[]={0xF0, 0x54, 0x5C, 0x25, 0x20, 0x54, 0x5C, 0x25, 0x20, 0x54, 0x5C, 0x45, 0xF0, 0x85};
	 unsigned char CSummSig2[]={0xF0, 0x54, 0x5C, 0x25, 0x20, 0x54, 0x5C, 0x25, 0x20, 0x54, 0x5C, 0x45, 0xF0, 0xA5};
	 unsigned int HexCount=14;
	 unsigned char CSummSig3[]={0x00, 0x00, 0x00, 0x00, 0xFF, 0x3F, 0x00, 0x00};
	 unsigned char CSummSig4[]={0x00, 0xC0, 0x87, 0x00, 0xFF, 0xFF, 0x87, 0x00};
	 unsigned char CSummSig5[]={0x00, 0xC0, 0x8F, 0x00, 0xFF, 0xFF, 0x8F, 0x00};
	 unsigned char CSummSig6[]={0xE6, 0xF4, 0xFF, 0xFF, 0xE6, 0xF5, 0xFF, 0xFF};
	 unsigned char CSummSig7[]={0x56, 0xF4, 0xFF, 0xFF, 0x56, 0xF5, 0xFF, 0xFF};
	 unsigned int HexCount2=8;


	 DWORD Buffer1 = (DWORD)dllECUinfo->EcuBuffer;
	 unsigned char* Buffer= (unsigned char*)(Buffer1);

	//Recognise ECU type
	//EcuSubType
	// 1- old ME7.x
	// 2- old ME7.x
	// 3- new ME7.x calc CRC32 3 times in 5 zones
	// 4- new ME7.x calc CRC32 3 times in 5 zones and calc Int8 in 4 zones
	// 5- new ME7.x calc CRC32 3 times in 5 zones and calc Int8 in 4 zones and second CRC32 Hash Table
	// 6- new ME7.x calc Int8 1 times in 4 zones
	do
	{
	FindOffset=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig1, EcuHexCount1);
	if (FindOffset!=0xFFFFFFFF)
		{
			EcuSubType=1;
			break;
		}
	FindOffset=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig4, EcuHexCount4);
	if (FindOffset!=0xFFFFFFFF)
		{
			EcuSubType=6;
			break;
		}
	FindOffset=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
	if (FindOffset!=0xFFFFFFFF)
		{
			FindOffset=FindHexStr(FindOffset+EcuHexCount2, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
			if (FindOffset!=0xFFFFFFFF)
				{
					FindOffset=FindHexStr(FindOffset+EcuHexCount2, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
					if (FindOffset!=0xFFFFFFFF)
						{
							EcuSubType=2;
							FindOffset=FindHexStr(FindOffset+EcuHexCount2, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
							if (FindOffset==0xFFFFFFFF)
								{
									break;
								}
							else
								{
									FindOffset=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig3, EcuHexCount3);
									if (FindOffset!=0xFFFFFFFF)
										{
											EcuSubType=5;
											break;
										}
									else
										{
											EcuSubType=3;
											break;
                                        }
                                }
						}
					else {return 0;}
				}
			else {return 0;}
		}
	 else {return 0;}

	} while (true);


	//Calc and check CRC32 CheckSumm
	switch(EcuSubType)
	{
		case 1:
		//Detect CRC32 Addresses
		 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig6, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr1=*(Buffer+CS_off+0x22)+(*(Buffer+CS_off+0x23)<<8)+(*(Buffer+CS_off+0x26)<<16)+(*(Buffer+CS_off+0x27)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}
		 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig6, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr2=*(Buffer+CS_off+0x22)+(*(Buffer+CS_off+0x23)<<8)+(*(Buffer+CS_off+0x26)<<16)+(*(Buffer+CS_off+0x27)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}
		 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig6, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr3=*(Buffer+CS_off+0x22)+(*(Buffer+CS_off+0x23)<<8)+(*(Buffer+CS_off+0x26)<<16)+(*(Buffer+CS_off+0x27)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}

		 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig1, HexCount);
		 if (CS_off!=0xFFFFFFFF)
			{
			 StartAdr1=0x0000+(*(Buffer+CS_off+0x14)<<16)+(*(Buffer+CS_off+0x15)<<24)-0x800000;
			 EndAdr1=*(Buffer+CS_off+0x38)+(*(Buffer+CS_off+0x39)<<8)+(*(Buffer+CS_off+0x14)<<16)+(*(Buffer+CS_off+0x15)<<24)-0x800001;
				 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig1, HexCount);
				 if (CS_off!=0xFFFFFFFF)
					{
					 StartAdr2=*(Buffer+CS_off+0x12)+(*(Buffer+CS_off+0x13)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800000;
					 EndAdr2=*(Buffer+CS_off+0x3A)+(*(Buffer+CS_off+0x3B)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800001;
					}
				 else
					{
						 return 0;
					}
				 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig1, HexCount);
				 if (CS_off!=0xFFFFFFFF)
					{
					 StartAdr3=*(Buffer+CS_off+0x12)+(*(Buffer+CS_off+0x13)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800000;
					 EndAdr3=*(Buffer+CS_off+0x3A)+(*(Buffer+CS_off+0x3B)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800001;
					}
				 else
					{
						 return 0;
					}
			}
         else {return 0;};

			TotalSumm++;
			ChkSumm1=Read4ByteIntel(Buffer+ChkSumAdr1);
			CalcSumm1=CalcCRC32(StartAdr1, EndAdr1, Buffer);
				 if (ChkSumm1!=CalcSumm1)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm1, Buffer+ChkSumAdr1);
							}
						BadSumm++;
					}
			TotalSumm++;
			ChkSumm2=Read4ByteIntel(Buffer+ChkSumAdr2);
			CalcSumm2=CalcCRC32(StartAdr2, EndAdr2, Buffer);
				 if (ChkSumm2!=CalcSumm2)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm2, Buffer+ChkSumAdr2);
							}
						BadSumm++;
					}
			TotalSumm++;
			ChkSumm3=Read4ByteIntel(Buffer+ChkSumAdr3);
			CalcSumm3=CalcCRC32(StartAdr3, EndAdr3, Buffer);
				 if (ChkSumm3!=CalcSumm3)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm3, Buffer+ChkSumAdr3);
							}
						BadSumm++;
					}

		break;

		case 2:
		//Detect CRC32 Addresses
		 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig6, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr1=*(Buffer+CS_off+0x22)+(*(Buffer+CS_off+0x23)<<8)+(*(Buffer+CS_off+0x26)<<16)+(*(Buffer+CS_off+0x27)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}
		 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig6, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr2=*(Buffer+CS_off+0x22)+(*(Buffer+CS_off+0x23)<<8)+(*(Buffer+CS_off+0x26)<<16)+(*(Buffer+CS_off+0x27)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}
		 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig6, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr3=*(Buffer+CS_off+0x22)+(*(Buffer+CS_off+0x23)<<8)+(*(Buffer+CS_off+0x26)<<16)+(*(Buffer+CS_off+0x27)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}

		 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig2, HexCount);
		 if (CS_off!=0xFFFFFFFF)
			{
			 StartAdr1=*(Buffer+CS_off-0x0E)+(*(Buffer+CS_off-0x0D)<<8)+(*(Buffer+CS_off-0x0A)<<16)+(*(Buffer+CS_off-0x09)<<24)-0x800000;
			 EndAdr1=*(Buffer+CS_off+0x16)+(*(Buffer+CS_off+0x17)<<8)+(*(Buffer+CS_off+0x1A)<<16)+(*(Buffer+CS_off+0x1B)<<24)-0x800000;
				 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig2, HexCount);
				 if (CS_off!=0xFFFFFFFF)
					{
					 StartAdr2=*(Buffer+CS_off-0x0E)+(*(Buffer+CS_off-0x0D)<<8)+(*(Buffer+CS_off-0x0A)<<16)+(*(Buffer+CS_off-0x09)<<24)-0x800000;
					 EndAdr2=*(Buffer+CS_off+0x16)+(*(Buffer+CS_off+0x17)<<8)+(*(Buffer+CS_off+0x1A)<<16)+(*(Buffer+CS_off+0x1B)<<24)-0x800000;
					}
				 else
					{
						 return 0;
					}
				 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig2, HexCount);
				 if (CS_off!=0xFFFFFFFF)
					{
					 StartAdr3=*(Buffer+CS_off-0x0E)+(*(Buffer+CS_off-0x0D)<<8)+(*(Buffer+CS_off-0x0A)<<16)+(*(Buffer+CS_off-0x09)<<24)-0x800000;
					 EndAdr3=*(Buffer+CS_off+0x16)+(*(Buffer+CS_off+0x17)<<8)+(*(Buffer+CS_off+0x1A)<<16)+(*(Buffer+CS_off+0x1B)<<24)-0x800000;
					}
				 else
					{
						 return 0;
					}
			 }
		  else {return 0;}

			TotalSumm++;
			ChkSumm1=Read4ByteIntel(Buffer+ChkSumAdr1);
			CalcSumm1=CalcCRC32(StartAdr1, EndAdr1, Buffer);
				 if (ChkSumm1!=CalcSumm1)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm1, Buffer+ChkSumAdr1);
							}
						BadSumm++;
					}
			TotalSumm++;
			ChkSumm2=Read4ByteIntel(Buffer+ChkSumAdr2);
			CalcSumm2=CalcCRC32(StartAdr2, EndAdr2, Buffer);
				 if (ChkSumm2!=CalcSumm2)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm2, Buffer+ChkSumAdr2);
							}
						BadSumm++;
					}
			TotalSumm++;
			ChkSumm3=Read4ByteIntel(Buffer+ChkSumAdr3);
			CalcSumm3=CalcCRC32(StartAdr3, EndAdr3, Buffer);
				 if (ChkSumm3!=CalcSumm3)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm3, Buffer+ChkSumAdr3);
							}
						BadSumm++;
					}

		break;

		case 3:
		 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig7, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr1=*(Buffer+CS_off+0x12)+(*(Buffer+CS_off+0x13)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}
		 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig7, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr2=*(Buffer+CS_off+0x12)+(*(Buffer+CS_off+0x13)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}
		 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig6, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr3=*(Buffer+CS_off+0x22)+(*(Buffer+CS_off+0x23)<<8)+(*(Buffer+CS_off+0x26)<<16)+(*(Buffer+CS_off+0x27)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}

		 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 StartAdr2=*(Buffer+CS_off-0x4C)+(*(Buffer+CS_off-0x4B)<<8)+(*(Buffer+CS_off-0x48)<<16)+(*(Buffer+CS_off-0x47)<<24)-0x800000;
			 EndAdr2=*(Buffer+CS_off-0x28)+(*(Buffer+CS_off-0x27)<<8)+(*(Buffer+CS_off-0x24)<<16)+(*(Buffer+CS_off-0x23)<<24)-0x800000;
				 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
				 if (CS_off!=0xFFFFFFFF)
					{
					 StartAdr3=*(Buffer+CS_off-0x4C)+(*(Buffer+CS_off-0x4B)<<8)+(*(Buffer+CS_off-0x48)<<16)+(*(Buffer+CS_off-0x47)<<24)-0x800000;
					 EndAdr3=*(Buffer+CS_off-0x28)+(*(Buffer+CS_off-0x27)<<8)+(*(Buffer+CS_off-0x24)<<16)+(*(Buffer+CS_off-0x23)<<24)-0x800000;
					}
				 else
					{
						 return 0;
					}
					 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
					 if (CS_off!=0xFFFFFFFF)
						{
						 StartAdr4=*(Buffer+CS_off-0x50)+(*(Buffer+CS_off-0x4F)<<8)+(*(Buffer+CS_off-0x4C)<<16)+(*(Buffer+CS_off-0x4B)<<24)-0x800000;
						 EndAdr4=*(Buffer+CS_off-0x2C)+(*(Buffer+CS_off-0x2B)<<8)+(*(Buffer+CS_off-0x28)<<16)+(*(Buffer+CS_off-0x27)<<24)-0x800000;
						}
					 else
						{
							 return 0;
						}
						 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
						 if (CS_off!=0xFFFFFFFF)
							{
							 StartAdr5=*(Buffer+CS_off-0x4C)+(*(Buffer+CS_off-0x4B)<<8)+(*(Buffer+CS_off-0x48)<<16)+(*(Buffer+CS_off-0x47)<<24)-0x800000;
							 EndAdr5=*(Buffer+CS_off-0x28)+(*(Buffer+CS_off-0x27)<<8)+(*(Buffer+CS_off-0x24)<<16)+(*(Buffer+CS_off-0x23)<<24)-0x800000;
							 StartAdr1=*(Buffer+CS_off+0x2F4)+(*(Buffer+CS_off+0x2F5)<<8)+(*(Buffer+CS_off+0x2F8)<<16)+(*(Buffer+CS_off+0x2F9)<<24)-0x800000;
							 EndAdr1=StartAdr1+(*(Buffer+CS_off+0x2FB)>>4 & 0xF) -1;

							}
						 else
							{
								 return 0;
							}
			 }
		  else {return 0;}

			TotalSumm++;
			ChkSumm1=Read4ByteIntel(Buffer+ChkSumAdr1);
			CalcSumm1=CalcCRC32(StartAdr1, EndAdr1, Buffer);
			CalcSumm1=CalcCRC32Cont(StartAdr2, EndAdr2, Buffer, CalcSumm1);
				 if (ChkSumm1!=CalcSumm1)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm1, Buffer+ChkSumAdr1);
							}
						BadSumm++;
					}
			TotalSumm++;
			ChkSumm2=Read4ByteIntel(Buffer+ChkSumAdr2);
			CalcSumm2=CalcCRC32Cont(StartAdr3, EndAdr3, Buffer, CalcSumm1);
				 if (ChkSumm2!=CalcSumm2)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm2, Buffer+ChkSumAdr2);
							}
						BadSumm++;
					}
			TotalSumm++;
			ChkSumm3=Read4ByteIntel(Buffer+ChkSumAdr3);
			CalcSumm3=CalcCRC32Cont(StartAdr4, EndAdr4, Buffer, CalcSumm2);
			CalcSumm3=CalcCRC32Cont(StartAdr5, EndAdr5, Buffer, CalcSumm3);
				 if (ChkSumm3!=CalcSumm3)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm3, Buffer+ChkSumAdr3);
							}
						BadSumm++;
					}


		break;

		case 4:

		break;

		case 5:
		 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig7, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr1=*(Buffer+CS_off+0x12)+(*(Buffer+CS_off+0x13)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}
		 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig7, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr2=*(Buffer+CS_off+0x12)+(*(Buffer+CS_off+0x13)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}
		 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig6, HexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr3=*(Buffer+CS_off+0x22)+(*(Buffer+CS_off+0x23)<<8)+(*(Buffer+CS_off+0x26)<<16)+(*(Buffer+CS_off+0x27)<<24)-0x800000;
			 ChkSumAdr4=*(Buffer+CS_off+0x1F0)+(*(Buffer+CS_off+0x1F1)<<8)+(*(Buffer+CS_off+0x1F4)<<16)+(*(Buffer+CS_off+0x1F5)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}

		 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
		 if (CS_off!=0xFFFFFFFF)
			{
			 StartAdr2=*(Buffer+CS_off-0x4C)+(*(Buffer+CS_off-0x4B)<<8)+(*(Buffer+CS_off-0x48)<<16)+(*(Buffer+CS_off-0x47)<<24)-0x800000;
			 EndAdr2=*(Buffer+CS_off-0x28)+(*(Buffer+CS_off-0x27)<<8)+(*(Buffer+CS_off-0x24)<<16)+(*(Buffer+CS_off-0x23)<<24)-0x800000;
				 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
				 if (CS_off!=0xFFFFFFFF)
					{
					 StartAdr3=*(Buffer+CS_off-0x4C)+(*(Buffer+CS_off-0x4B)<<8)+(*(Buffer+CS_off-0x48)<<16)+(*(Buffer+CS_off-0x47)<<24)-0x800000;
					 EndAdr3=*(Buffer+CS_off-0x28)+(*(Buffer+CS_off-0x27)<<8)+(*(Buffer+CS_off-0x24)<<16)+(*(Buffer+CS_off-0x23)<<24)-0x800000;
					}
				 else
					{
						 return 0;
					}
					 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
					 if (CS_off!=0xFFFFFFFF)
						{
						 StartAdr4=*(Buffer+CS_off-0x50)+(*(Buffer+CS_off-0x4F)<<8)+(*(Buffer+CS_off-0x4C)<<16)+(*(Buffer+CS_off-0x4B)<<24)-0x800000;
						 EndAdr4=*(Buffer+CS_off-0x2C)+(*(Buffer+CS_off-0x2B)<<8)+(*(Buffer+CS_off-0x28)<<16)+(*(Buffer+CS_off-0x27)<<24)-0x800000;
						}
					 else
						{
							 return 0;
						}
						 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig2, EcuHexCount2);
						 if (CS_off!=0xFFFFFFFF)
							{
							 StartAdr5=*(Buffer+CS_off-0x4C)+(*(Buffer+CS_off-0x4B)<<8)+(*(Buffer+CS_off-0x48)<<16)+(*(Buffer+CS_off-0x47)<<24)-0x800000;
							 EndAdr5=*(Buffer+CS_off-0x28)+(*(Buffer+CS_off-0x27)<<8)+(*(Buffer+CS_off-0x24)<<16)+(*(Buffer+CS_off-0x23)<<24)-0x800000;
							 StartAdr1=*(Buffer+CS_off+0x448)+(*(Buffer+CS_off+0x449)<<8)+(*(Buffer+CS_off+0x44C)<<16)+(*(Buffer+CS_off+0x44D)<<24)-0x800000;
							 EndAdr1=StartAdr1+(*(Buffer+CS_off+0x44F)>>4 & 0xF) -1;

							}
						 else
							{
								 return 0;
							}
			 }
		  else {return 0;}

			TotalSumm++;
			ChkSumm1=Read4ByteIntel(Buffer+ChkSumAdr1);
			CalcSumm1=CalcCRC32(StartAdr1, EndAdr1, Buffer);
			CalcSumm1=CalcCRC32Cont(StartAdr2, EndAdr2, Buffer, CalcSumm1);
				 if (ChkSumm1!=CalcSumm1)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm1, Buffer+ChkSumAdr1);
							}
						BadSumm++;
					}
			TotalSumm++;
			ChkSumm2=Read4ByteIntel(Buffer+ChkSumAdr2);
			CalcSumm2=CalcCRC32Cont(StartAdr3, EndAdr3, Buffer, CalcSumm1);
				 if (ChkSumm2!=CalcSumm2)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm2, Buffer+ChkSumAdr2);
							}
						BadSumm++;
					}
			TotalSumm++;
			ChkSumm3=Read4ByteIntel(Buffer+ChkSumAdr3);
			CalcSumm3=CalcCRC32Cont(StartAdr4, EndAdr4, Buffer, CalcSumm2);
			CalcSumm3=CalcCRC32Cont(StartAdr5, EndAdr5, Buffer, CalcSumm3);
				 if (ChkSumm3!=CalcSumm3)
					{
						if (FixCRC)
							{
								Write4ByteIntel(CalcSumm3, Buffer+ChkSumAdr3);
							}
						BadSumm++;
					}

			TotalSumm++;
			ChkSumm4=Read4ByteIntel(Buffer+ChkSumAdr4);
			CalcSumm4=SummInt8(StartAdr2, EndAdr2, Buffer);
			CalcSumm4+=SummInt8(StartAdr3, EndAdr3, Buffer);
			CalcSumm4+=SummInt8(StartAdr4, EndAdr4, Buffer);
			CalcSumm4+=SummInt8(StartAdr5, EndAdr5, Buffer);

				 if (ChkSumm4==(Read4ByteIntel(Buffer+ChkSumAdr4+4)^0xFFFFFFFF))
					{
					 if (ChkSumm4!=CalcSumm4)
						{
							if (FixCRC)
								{
									Write4ByteIntel(CalcSumm4, Buffer+ChkSumAdr4);
									Write4ByteIntel(CalcSumm4^0xFFFFFFFF, Buffer+ChkSumAdr4+4);
								}
							BadSumm++;
						}
					 }
				 else
				 {
					if (FixCRC)
						{
							Write4ByteIntel(CalcSumm4, Buffer+ChkSumAdr4);
							Write4ByteIntel(CalcSumm4^0xFFFFFFFF, Buffer+ChkSumAdr4+4);
						}
					BadSumm++;
				 }


		break;


		case 6:

		 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig4, EcuHexCount4);
		 if (CS_off!=0xFFFFFFFF)
			{
			 ChkSumAdr1=*(Buffer+CS_off+0x242)+(*(Buffer+CS_off+0x243)<<8)+(*(Buffer+CS_off+0x246)<<16)+(*(Buffer+CS_off+0x247)<<24)-0x800000;
			}
		 else
			{
				 return 0;
			}
		 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig4, EcuHexCount4);
		 if (CS_off!=0xFFFFFFFF)
			{
			 StartAdr1=*(Buffer+CS_off-0x44)+(*(Buffer+CS_off-0x43)<<8)+(*(Buffer+CS_off-0x40)<<16)+(*(Buffer+CS_off-0x3F)<<24)-0x800000;
			 EndAdr1=*(Buffer+CS_off-0x24)+(*(Buffer+CS_off-0x23)<<8)+(*(Buffer+CS_off-0x20)<<16)+(*(Buffer+CS_off-0x1F)<<24)-0x800000;
				 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig4, EcuHexCount4);
				 if (CS_off!=0xFFFFFFFF)
					{
					 StartAdr2=*(Buffer+CS_off-0x44)+(*(Buffer+CS_off-0x43)<<8)+(*(Buffer+CS_off-0x40)<<16)+(*(Buffer+CS_off-0x3F)<<24)-0x800000;
					 EndAdr2=*(Buffer+CS_off-0x24)+(*(Buffer+CS_off-0x23)<<8)+(*(Buffer+CS_off-0x20)<<16)+(*(Buffer+CS_off-0x1F)<<24)-0x800000;
					}
				 else
					{
						 return 0;
					}
					 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig4, EcuHexCount4);
					 if (CS_off!=0xFFFFFFFF)
						{
						 StartAdr3=*(Buffer+CS_off-0x48)+(*(Buffer+CS_off-0x47)<<8)+(*(Buffer+CS_off-0x44)<<16)+(*(Buffer+CS_off-0x43)<<24)-0x800000;
						 EndAdr3=*(Buffer+CS_off-0x28)+(*(Buffer+CS_off-0x27)<<8)+(*(Buffer+CS_off-0x24)<<16)+(*(Buffer+CS_off-0x23)<<24)-0x800000;
						}
					 else
						{
							 return 0;
						}
						 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , EcuBlockSig4, EcuHexCount4);
						 if (CS_off!=0xFFFFFFFF)
							{
							 StartAdr4=*(Buffer+CS_off-0x44)+(*(Buffer+CS_off-0x43)<<8)+(*(Buffer+CS_off-0x40)<<16)+(*(Buffer+CS_off-0x3F)<<24)-0x800000;
							 EndAdr4=*(Buffer+CS_off-0x24)+(*(Buffer+CS_off-0x23)<<8)+(*(Buffer+CS_off-0x20)<<16)+(*(Buffer+CS_off-0x1F)<<24)-0x800000;
							}
						 else
							{
								 return 0;
							}
			 }
		  else {return 0;}

  			TotalSumm++;
			ChkSumm1=Read4ByteIntel(Buffer+ChkSumAdr1);
			CalcSumm1=SummInt8(StartAdr1, EndAdr1, Buffer);
			CalcSumm1+=SummInt8(StartAdr2, EndAdr2, Buffer);
			CalcSumm1+=SummInt8(StartAdr3, EndAdr3, Buffer);
			CalcSumm1+=SummInt8(StartAdr4, EndAdr4, Buffer);

				 if (ChkSumm1==(Read4ByteIntel(Buffer+ChkSumAdr1+4)^0xFFFFFFFF))
					{
					 if (ChkSumm1!=CalcSumm1)
						{
							if (FixCRC)
								{
									Write4ByteIntel(CalcSumm1, Buffer+ChkSumAdr1);
									Write4ByteIntel(CalcSumm1^0xFFFFFFFF, Buffer+ChkSumAdr1+4);
								}
							BadSumm++;
						}
					 }
				 else
				 {
					if (FixCRC)
						{
							Write4ByteIntel(CalcSumm1, Buffer+ChkSumAdr1);
							Write4ByteIntel(CalcSumm1^0xFFFFFFFF, Buffer+ChkSumAdr1+4);
						}
					BadSumm++;
				 }
		break;
	}



	 //Calc and check Block Checksumm (803c)
	 StartAdr1=0x0000;
	 EndAdr1=Read4ByteIntel(Buffer+0x8030)-0x800000;
	 StartAdr2=Read4ByteIntel(Buffer+0x8034)+2-0x800000;
	 EndAdr2=Read4ByteIntel(Buffer+0x8014)-0x800000;
	 ChkSumAdr1=0x803C;


		TotalSumm++;
		ChkSumm1=*(Buffer+ChkSumAdr1)+(*(Buffer+ChkSumAdr1+1)<<8);
		CalcSumm1=SummBlock(StartAdr1, EndAdr1, Buffer);
		CalcSumm1+=SummBlock(StartAdr2, EndAdr2, Buffer);
		CalcSumm1&=0xFFFF;
			 if (ChkSumm1!=CalcSumm1)
				{
					if (FixCRC)
						{
							*(Buffer+ChkSumAdr1)=CalcSumm1;
							*(Buffer+ChkSumAdr1+1)=CalcSumm1>>8;
						}
					BadSumm++;
				}

	//Cacl and check full block checksumm
	StartAdr1=0x000000;
	EndAdr1=0x00FBFF;
	StartAdr2=0x20000;
	if (dllECUinfo->EcuFileSize==ECUFILESIZE1)
	{
		EndAdr2=0x07FFFF;
		ChkSumAdr1=0x07FFE0;
	}
	if (dllECUinfo->EcuFileSize==ECUFILESIZE2)
	{
		EndAdr2=0x0FFFFF;
		ChkSumAdr1=0x0FFFE0;
	}

	 ChkSumm1= Read4ByteIntel(Buffer+ChkSumAdr1);
	 CalcSumm1=SummInt16Intel(StartAdr1, EndAdr1, Buffer);
	 CalcSumm1+=SummInt16Intel(StartAdr2, EndAdr2, Buffer);

	 TotalSumm++;


	 if (ChkSumm1==(Read4ByteIntel(Buffer+ChkSumAdr1+4)^0xFFFFFFFF))
	 {
		 if (ChkSumm1!=CalcSumm1)
			{
				if (FixCRC)
					{
						Write4ByteIntel(CalcSumm1, Buffer+ChkSumAdr1);
						Write4ByteIntel(CalcSumm1^0xFFFFFFFF, Buffer+ChkSumAdr1+4);
					}
				BadSumm++;
			}
	 }
	 else
	 {
		if (FixCRC)
			{
				Write4ByteIntel(CalcSumm1, Buffer+ChkSumAdr1);
				Write4ByteIntel(CalcSumm1^0xFFFFFFFF, Buffer+ChkSumAdr1+4);
			}
		BadSumm++;
	 }



	 //Cacl and check multipoint checksumm for block with checsumm at 0x1E0
	StartAdr2=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig3, HexCount2);
	if (StartAdr2==0x01E0)
		{
			EndAdr2=0x1FF;
			 for (MultCSAdr = StartAdr2; MultCSAdr < EndAdr2; MultCSAdr+=0x10)
			 {

				StartAdr1=Read4ByteIntel(Buffer+MultCSAdr);
				EndAdr1=Read4ByteIntel(Buffer+MultCSAdr+4);
			 if ( StartAdr1>=0x800000)
			 {
				 StartAdr1-=0x800000;
				 EndAdr1-=0x800000;

				TotalSumm++;
			 if (EndAdr1<=dllECUinfo->EcuFileSize)
			 {
				ChkSumm1= Read4ByteIntel(Buffer+MultCSAdr+8);
				CalcSumm1=SummInt16Intel(StartAdr1, EndAdr1, Buffer);
				 if (ChkSumm1==(Read4ByteIntel(Buffer+MultCSAdr+12)^0xFFFFFFFF))
					{
					 if (ChkSumm1!=CalcSumm1)
						{
							if (FixCRC)
								{
									Write4ByteIntel(CalcSumm1, Buffer+MultCSAdr+8);
									Write4ByteIntel(CalcSumm1^0xFFFFFFFF, Buffer+MultCSAdr+12);
								}
							BadSumm++;
						}
					 }
				 else
				 {
					if (FixCRC)
						{
							Write4ByteIntel(CalcSumm1, Buffer+MultCSAdr+8);
							Write4ByteIntel(CalcSumm1^0xFFFFFFFF, Buffer+MultCSAdr+12);
						}
					BadSumm++;
				 }
			 }
			 else break;
			 }
			 }

		}

	//Cacl and check multipoint checksumm for main block
	StartAdr2=FindHexStr(0x200, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig3, HexCount2);
	if (StartAdr2==0xFFFFFFFF)
		{
			return 0;
		}
	if (dllECUinfo->EcuFileSize==ECUFILESIZE1)
	{
		EndAdr2=FindHexStr(StartAdr2+0x01E0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig4, HexCount2);
		if (EndAdr2==0xFFFFFFFF)
			{
				return 0;
			}
		EndAdr2+=0x0F;
	}
	if (dllECUinfo->EcuFileSize==ECUFILESIZE2)
	{
		EndAdr2=FindHexStr(StartAdr2+0x01E0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig5, HexCount2);
		if (EndAdr2==0xFFFFFFFF)
			{
			EndAdr2=FindHexStr(StartAdr2+0x01E0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig4, HexCount2);
			if (EndAdr2==0xFFFFFFFF)
				{
					return 0;
				}
			}
		EndAdr2+=0x0F;
	}

	 for (MultCSAdr = StartAdr2; MultCSAdr < EndAdr2; MultCSAdr+=0x10)
	 {

		StartAdr1=Read4ByteIntel(Buffer+MultCSAdr);
		EndAdr1=Read4ByteIntel(Buffer+MultCSAdr+4);
	 if ( StartAdr1>=0x800000)
	 {
		 StartAdr1-=0x800000;
		 EndAdr1-=0x800000;

		TotalSumm++;
	 if (EndAdr1<=dllECUinfo->EcuFileSize)
	 {
		ChkSumm1= Read4ByteIntel(Buffer+MultCSAdr+8);
		CalcSumm1=SummInt16Intel(StartAdr1, EndAdr1, Buffer);
		 if (ChkSumm1==(Read4ByteIntel(Buffer+MultCSAdr+12)^0xFFFFFFFF))
			{
			 if (ChkSumm1!=CalcSumm1)
				{
					if (FixCRC)
						{
							Write4ByteIntel(CalcSumm1, Buffer+MultCSAdr+8);
							Write4ByteIntel(CalcSumm1^0xFFFFFFFF, Buffer+MultCSAdr+12);
						}
					BadSumm++;
				}
			 }
		 else
		 {
			if (FixCRC)
				{
					Write4ByteIntel(CalcSumm1, Buffer+MultCSAdr+8);
					Write4ByteIntel(CalcSumm1^0xFFFFFFFF, Buffer+MultCSAdr+12);
				}
			BadSumm++;
		 }
	 }
	 else break;
	 }
	 }

/*	 //Calc and check CRC32 CheckSumm
	 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig6, HexCount2);
	 if (CS_off!=0xFFFFFFFF)
		{
		 ChkSumAdr1=*(Buffer+CS_off+0x22)+(*(Buffer+CS_off+0x23)<<8)+(*(Buffer+CS_off+0x26)<<16)+(*(Buffer+CS_off+0x27)<<24)-0x800000;
		}
	 else
		{
			 return 0;
		}
	 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig6, HexCount2);
	 if (CS_off!=0xFFFFFFFF)
		{
		 ChkSumAdr2=*(Buffer+CS_off+0x22)+(*(Buffer+CS_off+0x23)<<8)+(*(Buffer+CS_off+0x26)<<16)+(*(Buffer+CS_off+0x27)<<24)-0x800000;
		}
	 else
		{
			 return 0;
		}

	 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig6, HexCount2);
	 if (CS_off!=0xFFFFFFFF)
		{
		 ChkSumAdr3=*(Buffer+CS_off+0x22)+(*(Buffer+CS_off+0x23)<<8)+(*(Buffer+CS_off+0x26)<<16)+(*(Buffer+CS_off+0x27)<<24)-0x800000;
		}
	 else
		{
			 return 0;
		}

	 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig1, HexCount);
	 if (CS_off!=0xFFFFFFFF)
		{
		 StartAdr1=0x0000+(*(Buffer+CS_off+0x14)<<16)+(*(Buffer+CS_off+0x15)<<24)-0x800000;
		 EndAdr1=*(Buffer+CS_off+0x38)+(*(Buffer+CS_off+0x39)<<8)+(*(Buffer+CS_off+0x14)<<16)+(*(Buffer+CS_off+0x15)<<24)-0x800001;
			 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig1, HexCount);
			 if (CS_off!=0xFFFFFFFF)
				{
				 StartAdr2=*(Buffer+CS_off+0x12)+(*(Buffer+CS_off+0x13)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800000;
				 EndAdr2=*(Buffer+CS_off+0x3A)+(*(Buffer+CS_off+0x3B)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800001;
				}
			 else
				{
					 return 0;
				}
			 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig1, HexCount);
			 if (CS_off!=0xFFFFFFFF)
				{
				 StartAdr3=*(Buffer+CS_off+0x12)+(*(Buffer+CS_off+0x13)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800000;
				 EndAdr3=*(Buffer+CS_off+0x3A)+(*(Buffer+CS_off+0x3B)<<8)+(*(Buffer+CS_off+0x16)<<16)+(*(Buffer+CS_off+0x17)<<24)-0x800001;
				}
			 else
				{
					 return 0;
				}
		}
	 else
		{
	 CS_off=FindHexStr(0, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig2, HexCount);
	 if (CS_off!=0xFFFFFFFF)
		{
		 StartAdr1=*(Buffer+CS_off-0x0E)+(*(Buffer+CS_off-0x0D)<<8)+(*(Buffer+CS_off-0x0A)<<16)+(*(Buffer+CS_off-0x09)<<24)-0x800000;
		 EndAdr1=*(Buffer+CS_off+0x16)+(*(Buffer+CS_off+0x17)<<8)+(*(Buffer+CS_off+0x1A)<<16)+(*(Buffer+CS_off+0x1B)<<24)-0x800000;
			 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig2, HexCount);
			 if (CS_off!=0xFFFFFFFF)
				{
				 StartAdr2=*(Buffer+CS_off-0x0E)+(*(Buffer+CS_off-0x0D)<<8)+(*(Buffer+CS_off-0x0A)<<16)+(*(Buffer+CS_off-0x09)<<24)-0x800000;
				 EndAdr2=*(Buffer+CS_off+0x16)+(*(Buffer+CS_off+0x17)<<8)+(*(Buffer+CS_off+0x1A)<<16)+(*(Buffer+CS_off+0x1B)<<24)-0x800000;
				}
			 else
				{
					 return 0;
				}
			 CS_off=FindHexStr(CS_off+8, dllECUinfo->EcuFileSize, (unsigned char*)Buffer , CSummSig2, HexCount);
			 if (CS_off!=0xFFFFFFFF)
				{
				 StartAdr3=*(Buffer+CS_off-0x0E)+(*(Buffer+CS_off-0x0D)<<8)+(*(Buffer+CS_off-0x0A)<<16)+(*(Buffer+CS_off-0x09)<<24)-0x800000;
				 EndAdr3=*(Buffer+CS_off+0x16)+(*(Buffer+CS_off+0x17)<<8)+(*(Buffer+CS_off+0x1A)<<16)+(*(Buffer+CS_off+0x1B)<<24)-0x800000;
				}
			 else
				{
					 return 0;
				}
		}
	 else
		{
			 return 0;
		}
		}

		TotalSumm++;
		ChkSumm1=Read4ByteIntel(Buffer+ChkSumAdr1);
		CalcSumm1=CalcCRC32(StartAdr1, EndAdr1, Buffer);
			 if (ChkSumm1!=CalcSumm1)
				{
					if (FixCRC)
						{
							Write4ByteIntel(CalcSumm1, Buffer+ChkSumAdr1);
						}
					BadSumm++;
				}
		ChkSumm2=Read4ByteIntel(Buffer+ChkSumAdr2);
		CalcSumm2=CalcCRC32(StartAdr2, EndAdr2, Buffer);
			 if (ChkSumm2!=CalcSumm2)
				{
					if (FixCRC)
						{
							Write4ByteIntel(CalcSumm2, Buffer+ChkSumAdr2);
						}
					BadSumm++;
				}
		ChkSumm3=Read4ByteIntel(Buffer+ChkSumAdr3);
		CalcSumm3=CalcCRC32(StartAdr3, EndAdr3, Buffer);
			 if (ChkSumm3!=CalcSumm3)
				{
					if (FixCRC)
						{
							Write4ByteIntel(CalcSumm3, Buffer+ChkSumAdr3);
						}
					BadSumm++;
				}
*/

	 dllECUinfo->BadChkSumNumber=BadSumm;  //Bad CheckSumm number in dump
	 dllECUinfo->ChkSumNumber=TotalSumm;  //CheckSumm number in dump
	 return 1;
}


unsigned int FindHexStr(unsigned int SearchOffset, unsigned int BufferSize, unsigned char *SearchBuffer, unsigned char *HexBytes, unsigned int HexCount)
{
	unsigned int i, h_bytes, s_count=0;
	for (i = SearchOffset; i < BufferSize-HexCount; i++)
		{
			if (*(SearchBuffer+i)==*HexBytes)
				{
					s_count=1;
					if (HexCount==1)
						{
							return i;
						}
					for (h_bytes = 1; h_bytes < HexCount; h_bytes++)
						{
							if (*(SearchBuffer+i+h_bytes)==*(HexBytes+h_bytes))
								{
								   s_count++;
								}
							else
								{
									s_count=0;
									break;
								}
						}
					 if (s_count==HexCount)
						{
							return i;
						}
				}
		}
	return 0xFFFFFFFF;
}

unsigned int SummInt8(unsigned int StartInt8, unsigned int EndInt8, unsigned char *SearchBuffer)
{
	unsigned int summ=0, i;
	for (i = StartInt8; i <= EndInt8; i++)
		{
			summ+= *(SearchBuffer+i);
		}
	return summ;
}

unsigned int SummInt16Intel(unsigned int StartInt16, unsigned int EndInt16, unsigned char *SearchBuffer)
{
	unsigned int summ=0, i;
	for (i = StartInt16; i < EndInt16; i=i+2)
		{
			summ+= *(SearchBuffer+i);
			summ+= *(SearchBuffer+i+1)<<8;
		}
	return summ;
}

unsigned int SummInt16Mot(unsigned int StartInt16, unsigned int EndInt16, unsigned char *SearchBuffer)
{
	unsigned int summ=0, i;
	for (i = StartInt16; i < EndInt16; i=i+2)
		{
			summ+= *(SearchBuffer+i+1);
			summ+= *(SearchBuffer+i)<<8;
		}
	return summ;
}

unsigned int Read4ByteIntel (unsigned char *ReadAddress)
{
	unsigned int  ReadedDword;
	ReadedDword= *(ReadAddress)+(*(ReadAddress+1)<<8)+(*(ReadAddress+2)<<16)+(*(ReadAddress+3)<<24);
	return ReadedDword;
}

void Write4ByteIntel (unsigned int WrDword, unsigned char *WriteAddress)
{
	*WriteAddress= WrDword;
	*(WriteAddress+1)= WrDword>>8;
	*(WriteAddress+2)= WrDword>>16;
	*(WriteAddress+3)= WrDword>>24;
}

unsigned int SummBlock(unsigned int StartBlock, unsigned int EndBlock, unsigned char *SearchBuffer)
{
	unsigned int summ=0, i;
	for (i = StartBlock; i < EndBlock; i=i+0x2000)
		{
			summ+= *(SearchBuffer+i);
			summ+= *(SearchBuffer+i+1)<<8;
			summ+= *(SearchBuffer+i+0x1FFE);
			summ+= *(SearchBuffer+i+0x1FFF)<<8;
		}
	return summ;

}

unsigned int Crc32Table[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
	0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
	0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};


unsigned int CalcCRC32(unsigned int StartBlock, unsigned int EndBlock, unsigned char *Buffer)
{
	unsigned int crc = 0xFFFFFFFF;
	unsigned int i;
	for (i = 0; i <= (EndBlock-StartBlock); i++)
	{
			crc = (crc >> 8) ^ Crc32Table[(crc ^ *(Buffer+StartBlock+i)) & 0xFF];
	}
	return crc ^ 0xFFFFFFFF;
}

unsigned int CalcCRC32Cont(unsigned int StartBlock, unsigned int EndBlock, unsigned char *Buffer, unsigned int crc_start)
{
	unsigned int crc = crc_start^ 0xFFFFFFFF;
	unsigned int i;
	for (i = 0; i <= (EndBlock-StartBlock); i++)
	{
			crc = (crc >> 8) ^ Crc32Table[(crc ^ *(Buffer+StartBlock+i)) & 0xFF];
	}
	return crc ^ 0xFFFFFFFF;
}


