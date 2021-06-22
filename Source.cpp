//==========================================================================
// convert_file_path
// Get latest version at https://github.com/MasyoLab/convert_file_path
//==========================================================================

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define _GLIBCXX_DEBUG
#include <time.h>
#include <iostream>
#include <chrono>
#include <direct.h>

#include "data_save.hpp"
#include "file_path.hpp"
#include "folder_path.hpp"
#include "text_load.hpp"

//==========================================================================
// �N���X��`
//==========================================================================
class contena_dc
{
public:
	contena_dc() {
		m_str_name.clear();
		m_str_path.clear();
		m_str_extension.clear();
	}
	contena_dc(const std::string& str_name, const std::string& str_path, const std::string& str_extension) {
		for (auto& itr : str_name)
		{
			// �󔒂�u�������܂�
			if (itr == ' ') {
				m_str_name += "_";
				continue;
			}
			m_str_name += itr;
		}
		m_str_path = str_path;
		m_str_extension = str_extension;
	}
	~contena_dc() {
		m_str_name.clear();
		m_str_path.clear();
		m_str_extension.clear();
	}
public:
	std::string m_str_name; // �t�@�C����
	std::string m_str_path; // �t�@�C���p�X
	std::string m_str_extension; // �g���q
};

//==========================================================================
// �^��`
//==========================================================================
using list_contena_dc = std::list<contena_dc>;
using list_string = std::list<std::string>;

//==========================================================================
// �֐���`
//==========================================================================

//==========================================================================
/**
@brief �t�@�C���T��
@param dir_name [in] �T���Ώۃf�B���N�g��
@param extension [in] �g���q�w��
@param set_ [in]
*/
list_contena_dc search(
	const std::string& dir_name,
	const list_string& extension,
	const std::string set_);
//==========================================================================

//==========================================================================
/**
@brief �w�b�_�[����
@param create_day [in] ������
@param create_time [in] ��������
@param data_dc [in] �ǂݎ�茋��
@param extension [in] �g���q�w��
*/
void create_header(
	const std::string& create_day,
	const std::string& create_time,
	const list_contena_dc& data_dc,
	const list_string& extension);
//==========================================================================

//==========================================================================
/**
@brief ���O����
@param start_ms [in] �����J�n����
@param end_ms [in] �����I������
@param create_day [in] ������
@param create_time [in] ��������
@param directory [in] �T�������f�B���N�g��
@param data_dc [in] �ǂݎ�茋��
*/
void create_log(
	float start_ms,
	float end_ms,
	const std::string& create_day,
	const std::string& create_time,
	const list_string& directory,
	const list_contena_dc& data_dc);
//==========================================================================

//==========================================================================
/**
@brief ���Ԏ擾
@return ����
*/
float get_time_sec();
//==========================================================================

//=============================================================================
// main
//=============================================================================
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	list_contena_dc data_dc; // �f�[�^�i�[�p
	time_t timer;
	tm* date;
	std::string create_day; // ������
	std::string create_time; // ��������
	char str[256] = { 0 };
	float start_ms = get_time_sec();

	//=============================================================================
	// �������s�J�n���Ԃ��L�^
	//=============================================================================
	timer = time(nullptr); // �o�ߎ��Ԃ��擾
	date = localtime(&timer); // �o�ߎ��Ԃ����Ԃ�\���\���� date �ɕϊ�
	strftime(str, sizeof(str), "%Y/%m/%d", date); // �N���������b
	create_day = str;
	strftime(str, sizeof(str), "%H:%M:%S", date); // �N���������b
	create_time = str;

	//=============================================================================
	// ���C������
	//=============================================================================

	//=============================================================================
	// �f�B���N�g���T��
	//=============================================================================

	// �g���q�ǂݎ��
	auto extension = text_load::get("�g���q�w��.txt");

	// �����f�B���N�g������T��
	auto directory = folder_path::get_all();

	// �f�B���N�g���w��Ȃ�
	data_dc = search("", extension, "*.");

	// �f�B���N�g���w�肠��
	for (auto& itr1 : directory)
		// ���o�����f�B���N�g�����̑S�f�[�^��T������
		// �擾�������ʂ�ǉ�����
		for (auto& itr2 : search(itr1, extension, "\\*."))
			data_dc.push_back(itr2);

	// �o�͏���
	create_header(create_day, create_time, data_dc, extension);

	// ���O�o��
	create_log(start_ms, get_time_sec(), create_day, create_time, directory, data_dc);

	return std::system("PAUSE");
}

list_contena_dc search(
	const std::string& dir_name,
	const list_string& extension,
	const std::string set_)
{
	list_contena_dc data_dc; // �f�[�^�i�[�p

	// �T���f�[�^�L�^
	for (auto& itr1 : extension) {

		// �S�T���Ώۃf�B���N�g����������
		// �f�[�^�̊i�[
		for (auto& itr2 : file_path::get_in(dir_name, itr1, set_)) {
			std::string _file_name; // �t�@�C����
			std::string _path_name; // �g���q
			bool bchange = false;

			// �g���q�r��
			for (auto& itr3 : itr2) {
				std::string str_;
				str_ = itr3;

				if (str_ == ".")
					bchange = true;
				if (bchange == false && str_ != ".")
					_file_name += str_;
				else if (bchange == true && str_ != ".")
					_path_name += str_;
			}

			if (dir_name == "")
				data_dc.emplace_back(_file_name, dir_name + itr2, _path_name);
			else if (dir_name != "")
				data_dc.emplace_back(_file_name, dir_name + "/" + itr2, _path_name);
		}
	}
	return data_dc;
}

void create_header(
	const std::string& create_day,
	const std::string& create_time,
	const list_contena_dc& data_dc,
	const list_string& extension)
{
	list_string header_data; // �w�b�_�[�̓��e
	const std::string _define = "#define ";
	const std::string _ifndef = "#ifndef ";
	const std::string _endif = "#endif ";
	std::string _extension;
	int n_count = 0;

	header_data.push_back("//=============================================================================");
	header_data.push_back("// resource file path");
	header_data.push_back("// " + create_day);
	header_data.push_back("// " + create_time);
	header_data.push_back("//=============================================================================");
	header_data.push_back("#pragma once");

	for (auto& itr1 : extension) {
		for (auto& itr2 : data_dc) {
			if (itr1 == itr2.m_str_extension) {

				std::string header_text; // �e�L�X�g�����p

				if (_extension != itr1) {
					_extension = itr1;
					header_data.push_back("");
					header_data.push_back("//=============================================================================");
					header_data.push_back("// filename extension [." + _extension + "]");
					header_data.push_back("//=============================================================================");
					header_data.push_back("");
				}

				header_text = "RESOURCE_";
				header_text += itr2.m_str_name;
				header_text += "_";
				header_text += itr2.m_str_extension;
				header_data.push_back(_ifndef + header_text);
				header_data.push_back(_define + header_text + " " + '"' + itr2.m_str_path + '"');
				header_data.push_back(_endif + "// !" + header_text);
				header_text.clear();
			}
		}
	}

	// �o��
	list_string str_link;

	// txt�f�[�^�̐���
	str_link.push_back(std::to_string(data_dc.size()));
	for (auto& itr : data_dc)
		// �i�[
		str_link.push_back(itr.m_str_path);

	// �ۑ�
	data_save::save("resource_list.txt", str_link);
	data_save::save("resource_list.h", header_data);
}

void create_log(
	float start_ms,
	float end_ms,
	const std::string& create_day,
	const std::string& create_time,
	const list_string& directory,
	const list_contena_dc& data_dc)
{
	std::string Generation_time = "GenerationTime : " + std::to_string(end_ms - start_ms) + " ms";
	list_string log_data;
	log_data.push_back("");
	log_data.push_back("=========== convert_file_path ===========");
	log_data.push_back("*** " + create_day);
	log_data.push_back("*** " + create_time);
	log_data.push_back("*** " + Generation_time);
	log_data.push_back("*** Number of detected data : " + std::to_string((int)data_dc.size()));
	log_data.push_back("");
	log_data.push_back("=========== We are generating logs ===========");
	log_data.push_back("");
	log_data.push_back("���������������� Folder import list ����������������");
	log_data.push_back("");
	for (auto& itr : directory)
		log_data.push_back("[" + itr + "]" + "�����o���܂���");
	log_data.push_back("");
	log_data.push_back("���������������� File import list ����������������");
	log_data.push_back("");
	for (auto& itr : data_dc)
		log_data.push_back("[" + itr.m_str_path + "]" + "�����o���܂���");
	log_data.push_back("");
	log_data.push_back("=========== Log generation has ended ===========");
	log_data.push_back("");
	log_data.push_back("**** Crash! ****");
	log_data.push_back("");

	// ���O�̕ۑ�
	data_save::save("convert_file_path.log", log_data);

	// �o��
	for (auto& itr : log_data)
		std::cout << itr << std::endl;
}

float get_time_sec()
{
	return static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()) / (float)1000000000;
}
