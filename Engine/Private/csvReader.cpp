#include "csvReader.h"
#include "GameInstance.h"
#include "LevelData.h"

#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>

CcsvReader::CcsvReader()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CcsvReader::Readcsv(const _wstring& strcsvPath, CLevelData* pDatas)
{
	wifstream fin;

//#pragma push_macro("new")
//#undef new
//
//	fin.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
//
//#pragma pop_macro("new")

	fin.open(strcsvPath, ios::in);

	if (!fin.is_open()) {
		MSG_BOX("파일을 열 수 없었읍니다");
		return E_FAIL;
	}

	wstring strLine;
	getline(fin, strLine);

	_wstring strKey;
	_wstring strData;
	_byte* pData = { nullptr };
	_byte* pAddress = { nullptr };

	while (getline(fin, strLine))
	{
		wstringstream linestream(strLine);
		getline(linestream, strKey, L','); // 키값
		getline(linestream, strData, L','); // 갯수
		_uint iNum = stoi(strData);

		pData = (_byte*)malloc(iNum * sizeof(_float));
		pAddress = pData;
		_float fCell = { 0.f };

		for (_uint i = 0; i < iNum; i++)
		{
			getline(linestream, strData, L',');
			if (strData.empty())
				continue;
			fCell = stof(strData);
			if (nullptr != pAddress)
			{
				memcpy(pAddress, &fCell, sizeof(_float));
				pAddress += sizeof(_float);
			}
		}

		if (FAILED(pDatas->Add_Data(strKey, pData)))
			return E_FAIL;
	}

	fin.close();
	return S_OK;
}

_wstring CcsvReader::Parsing_csv(wstringstream& linestream)
{
	wstring strCell; /* 읽은 데이터들 저장하고 리턴할 변수 */
	_tchar wcLetter; /* */
	_bool bInquotes = false; /* "여기 안에 있는지 체크"*/

	/* 한 글자씩 읽어서 ch에 저장 시켜줌*/
	while (linestream.get(wcLetter))
	{
		/* 만약 ch가 " 라면 */
		if (wcLetter == L'"')
		{
			/* peek() == 다음 문자가 무엇인지 확인 */
			/* 만약 다음문자도 " 라면 */
			if (linestream.peek() == L'"') {
				// 이스케이프된 따옴표 ("")를 "로 변환
				linestream.get(wcLetter); // 하나 더 읽어서 버림
				strCell += L'"';
			}
			else {
				bInquotes = !bInquotes;
			}
		}
		else if (wcLetter == L',' && !bInquotes)
		{
			// 쉼표는 따옴표 밖에 있는 경우 구분자로 사용
			break;
		}
		else {
			strCell += wcLetter;
		}
	}

	return strCell;
}

bool CcsvReader::IsNum(const _wstring& strCell)
{
	if (strCell.empty())
		return false;

	_bool bHasDot = { false };
	_uint i = { };

	if (strCell[0] == L'-')
		i = 1;

	for (; i < strCell.length(); i++)
	{
		if (strCell[i] == L'.')
		{
			if (bHasDot)
				return false;

			bHasDot = true;
		}
		else if (!iswdigit(strCell[i]))
		{
			return false;
		}
	}

	return true;
}

CcsvReader* CcsvReader::Create()
{
	return new CcsvReader;
}

void CcsvReader::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}

