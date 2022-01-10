#pragma once

#include <string>

namespace Hazel {

	class FileDialogs {
	public:
		static std::string OpenFile(const char *filter, const char* path = nullptr);
		static std::string SaveFile(const char* filter, const char* path = nullptr);
	};
}