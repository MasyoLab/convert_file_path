//==========================================================================
// �t�@�C���T��[file_path.cpp]
// author: MasyoLab
//==========================================================================
#include "file_path.hpp"

std::list<std::string> file_path::get_in(const std::string& dir_name, const std::string& extension, std::string set_) noexcept(false)
{
	HANDLE hFind = nullptr;
	WIN32_FIND_DATA win32fd;//defined at Windwos.h
	std::list<std::string> file_names;

	ZeroMemory(&win32fd, sizeof(WIN32_FIND_DATA));

	//�g���q�̐ݒ�
	std::string search_name = dir_name + set_ + extension;

	hFind = FindFirstFile(search_name.c_str(), &win32fd);

	if (win32fd.dwFileAttributes == 0) {
		FindClose(hFind);
		return file_names;
	}

	if (hFind == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("file not found");
		return file_names;
	}

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {}
		else
			file_names.push_back(win32fd.cFileName);
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return file_names;
}

std::list<std::string> file_path::get(const std::string& extension) noexcept(false)
{
	return get_in("", extension, "*.");
}