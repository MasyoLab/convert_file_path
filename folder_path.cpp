//==========================================================================
// �t�H���_�T��[folder_path.cpp]
// author: MasyoLab
//==========================================================================
#include "folder_path.hpp"

std::list<std::string> folder_path::get() noexcept(false)
{
	HANDLE hFind = nullptr;
	WIN32_FIND_DATA win32fd; //defined at Windwos.h
	std::list<std::string> folder_names;

	ZeroMemory(&win32fd, sizeof(WIN32_FIND_DATA));

	hFind = FindFirstFile("*", &win32fd);

	if (win32fd.dwFileAttributes == 0) {
		FindClose(hFind);
		return folder_names;
	}

	if (hFind == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("file not found");
		return folder_names;
	}

	do {
		if (win32fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			std::string str_ = win32fd.cFileName;
			if (str_ != "." && str_ != "..")
				folder_names.emplace_back(str_);
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return folder_names;
}

std::list<std::string> folder_path::get_in(const std::string& folder_name) noexcept(false)
{
	HANDLE hFind = nullptr;
	WIN32_FIND_DATA win32fd;//defined at Windwos.h
	std::list<std::string> folder_names;

	ZeroMemory(&win32fd, sizeof(WIN32_FIND_DATA));

	std::string _folder = folder_name + "/*";

	hFind = FindFirstFile(_folder.c_str(), &win32fd);

	if (win32fd.dwFileAttributes == 0) {
		FindClose(hFind);
		return folder_names;
	}

	if (hFind == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("file not found");
		return folder_names;
	}

	do {
		if (win32fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			std::string str_ = win32fd.cFileName;
			if (str_ != "." && str_ != "..")
				folder_names.emplace_back(folder_name + "/" + str_);
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return folder_names;
}

std::list<std::string> folder_path::get_in_recursive(const std::list<std::string>& path) noexcept(false)
{
	std::list<std::string> str_folder_new;
	std::list<std::string> str_folder;

	for (auto& itr1 : path)
		for (auto& itr2 : get_in(itr1))
			str_folder_new.emplace_back(itr2);

	for (auto& itr : path)
		str_folder.emplace_back(itr);
	for (auto& itr : str_folder_new)
		str_folder.emplace_back(itr);

	// �ċA�O�Ƀf�[�^�̍œK��
	str_folder.sort(std::greater<std::string>());
	str_folder.unique();

	// �T������f�[�^�����݂���ꍇ�f�[�^�������p���ċA����
	if (str_folder_new.size() != 0)
		for (auto& itr : get_in_recursive(str_folder_new))
			str_folder.emplace_back(itr);

	// �f�[�^��߂��O�ɍœK��
	str_folder.sort(std::greater<std::string>());
	str_folder.unique();

	return str_folder;
}

std::list<std::string> folder_path::get_all() noexcept(false)
{
	return get_in_recursive(get());
}
