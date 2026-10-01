#pragma once
#include <filesystem>
#include <windows.h>
#include <shobjidl.h>
namespace TinyEditor {
    std::string SelectFolder(const std::string& defaultPath)
    {
        IFileDialog* dialog = nullptr;

        HRESULT hr = CoCreateInstance(
            CLSID_FileOpenDialog,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&dialog)
        );

        if (FAILED(hr))
            return {};

        DWORD options = 0;

        dialog->GetOptions(&options);

        dialog->SetOptions(
            options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM
        );

        if (!defaultPath.empty())
        {
            IShellItem* folder = nullptr;

            std::wstring widePath(
                defaultPath.begin(),
                defaultPath.end()
            );

            hr = SHCreateItemFromParsingName(
                widePath.c_str(),
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
            std::filesystem::path filesystemPath(path);
            result = filesystemPath.string();

            CoTaskMemFree(path);
        }

        item->Release();
        dialog->Release();

        return result;
    }
}