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
		MSG_BOX("������ �� �� �������ϴ�");
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
		getline(linestream, strKey, L','); // Ű��
		getline(linestream, strData, L','); // ����
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
	wstring strCell; /* ���� �����͵� �����ϰ� ������ ���� */
	_tchar wcLetter; /* */
	_bool bInquotes = false; /* "���� �ȿ� �ִ��� üũ"*/

	/* �� ���ھ� �о ch�� ���� ������*/
	while (linestream.get(wcLetter))
	{
		/* ���� ch�� " ��� */
		if (wcLetter == L'"')
		{
			/* peek() == ���� ���ڰ� �������� Ȯ�� */
			/* ���� �������ڵ� " ��� */
			if (linestream.peek() == L'"') {
				// �̽��������� ����ǥ ("")�� "�� ��ȯ
				linestream.get(wcLetter); // �ϳ� �� �о ����
				strCell += L'"';
			}
			else {
				bInquotes = !bInquotes;
			}
		}
		else if (wcLetter == L',' && !bInquotes)
		{
			// ��ǥ�� ����ǥ �ۿ� �ִ� ��� �����ڷ� ���
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

