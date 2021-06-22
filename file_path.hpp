//==========================================================================
// �t�@�C���T��[file_path.hpp]
// author: MasyoLab
//==========================================================================
#pragma once

//==========================================================================
// include
//==========================================================================
#include <Windows.h>
#include <list>
#include <string>
#include <stdexcept>

namespace file_path {
    /**
    @brief �t�@�C���T��
    @param dir_name [in] �T���Ώۃf�B���N�g��
    @param extension [in] �g���q�w��
    @param set_ [in]
    @return �T������
    */
    std::list<std::string> get_in(const std::string& dir_name, const std::string& extension, std::string set_ = "\\*.") noexcept(false);

    /**
    @brief �t�@�C���T��
    @param extension [in] �g���q�w��
    @return �T������
    */
    std::list<std::string> get(const std::string& extension) noexcept(false);
}
