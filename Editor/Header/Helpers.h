#pragma once
#include <filesystem>
#include <windows.h>
#include <shobjidl.h>
namespace TinyEditor {
    std::string SelectSaveFile(const std::string& defaultPath)
    {
        IFileDialog* dialog = nullptr;

        HRESULT hr = CoCreateInstance(
            CLSID_FileSaveDialog,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&dialog)
        );

        if (FAILED(hr))
            return {};

        DWORD options = 0;
        dialog->GetOptions(&options);

        dialog->SetOptions(
            options | FOS_FORCEFILESYSTEM
        );

        // Only allow .tge files
        COMDLG_FILTERSPEC fileTypes[] =
        {
            { L"TinyGameEngine Scene", L"*.tge" }
        };

        dialog->SetFileTypes(
            ARRAYSIZE(fileTypes),
            fileTypes
        );

        dialog->SetDefaultExtension(L"tge");

        // Start in the requested folder
        if (!defaultPath.empty())
        {
            IShellItem* folder = nullptr;

            std::filesystem::path path(defaultPath);

            hr = SHCreateItemFromParsingName(
                path.c_str(),
                nullptr,
                IID_PPV_ARGS(&folder)
            );

            if (SUCCEEDED(hr))
            {
                dialog->SetFolder(folder);
                folder->Release();
            }
        }

        hr = dialog->Show(nullptr);

        if (FAILED(hr))
        {
            dialog->Release();
            return {};
        }

        IShellItem* item = nullptr;

        hr = dialog->GetResult(&item);

        if (FAILED(hr))
        {
            dialog->Release();
            return {};
        }

        PWSTR path = nullptr;

        hr = item->GetDisplayName(
            SIGDN_FILESYSPATH,
            &path
        );

        std::string result;

        if (SUCCEEDED(hr))
        {
            result = std::filesystem::path(path).string();
            CoTaskMemFree(path);
        }

        item->Release();
        dialog->Release();

        return result;
    }
}