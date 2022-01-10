#include "hzpch.h"

#include "hazel/core/Application.h"
#include "hazel/utilities/PlatformUtilities.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Hazel {

	std::string FileDialogs::OpenFile(const char* filter, const char* path /* = ""*/) {
		HWND window = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
		
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR szPath[260] = { 0 };

		// Get current path
		if (path)
			strcpy_s(szPath, path);
		else {
			GetModuleFileNameA(NULL, szPath, sizeof(szPath));
			std::string::size_type pos = std::string(szPath).find_last_of("\\/");
			strcpy_s(szPath, std::string(szPath).substr(0, pos).c_str());
		}

		HZ_CORE_TRACE("Opening file dialog with path {0}", szPath);

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));

		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = window;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrInitialDir = szPath;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		// Fallback in case cancelled or closed dialog
		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter, const char* path /* = ""*/) {
		HWND window = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));

		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR szPath[260] = { 0 };

		// Get current path
		if (path)
			strcpy_s(szPath, path);
		else {
			GetModuleFileNameA(NULL, szPath, sizeof(szPath));
			std::string::size_type pos = std::string(szPath).find_last_of("\\/");
			strcpy_s(szPath, std::string(szPath).substr(0, pos).c_str());
		}

		HZ_CORE_TRACE("Opening file dialog with path {0}", szPath);

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));

		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = window;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.lpstrInitialDir = szPath;
		ofn.nFilterIndex = 1;
		// Extract default extension from filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		// Fallback in case cancelled or closed dialog
		return std::string();
	}
}