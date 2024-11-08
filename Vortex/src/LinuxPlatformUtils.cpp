#include "Vortex/PreHeaders.h"
#include "Vortex/Utils/PlatformUtils.h"
#include <GLFW/glfw3.h>
#include <array>
#include <memory>

namespace Vortex
{
    std::string FileDialogs::OpenFile(const char* filter)
    {
#ifdef _WIN32
        // Windows-specific code for opening file dialog
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return std::string();

#else
        // Linux-specific code using Zenity
        char command[512];
        const char* startDir = "/home/pc/dev/engine/Sandbox/assets/Scenes";
        snprintf(command, sizeof(command), "zenity --file-selection --file-filter='%s' --filename='%s/' 2>/dev/null", filter, startDir);
        
        std::array<char, 128> buffer;
        std::string result;
        
        auto pclose_lambda = [](FILE* f) { pclose(f); };
        std::unique_ptr<FILE, decltype(pclose_lambda)> pipe(popen(command, "r"), pclose_lambda);
        
        if (!pipe) return std::string();
        
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }
        result.erase(result.find_last_not_of("\n") + 1);
        return result;
#endif
    }

    std::string FileDialogs::SaveFile(const char* filter)
    {
#ifdef _WIN32
        // Windows-specific code for saving file dialog
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetSaveFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return std::string();

#else
        // Linux-specific code using Zenity for save dialog
        char command[512];
        const char* startDir = "/home/pc/dev/engine/Sandbox/assets/Scenes";
        snprintf(command, sizeof(command), "zenity --file-selection --file-filter='%s' --filename='%s/' 2>/dev/null", filter, startDir);

        std::array<char, 128> buffer;
        std::string result;
        
        auto pclose_lambda = [](FILE* f) { pclose(f); };
        std::unique_ptr<FILE, decltype(pclose_lambda)> pipe(popen(command, "r"), pclose_lambda);

        if (!pipe) return std::string();

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }
        result.erase(result.find_last_not_of("\n") + 1);
        return result;
#endif
    }
}