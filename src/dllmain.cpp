#include "vcproxy/vcruntime.hpp"

static std::string getModsFolder()
{
    std::string path = std::string(std::getenv("APPDATA")) + "\\Minecraft Bedrock\\mods";
    std::filesystem::create_directories(path);
    return path;
}

std::vector<std::string> getMods(const std::string& dir)
{
    std::vector<std::string> paths;

    for (auto &e : std::filesystem::directory_iterator(dir))
    {
        if (e.is_regular_file())
        {
            auto ext = e.path().extension().string();
            std::ranges::transform(ext, ext.begin(), ::tolower);
            if (ext == ".dll") paths.push_back(e.path().string());
        }
    }

    return paths;
}

static void loadMods()
{
    for (auto mods = getMods(getModsFolder()); auto &mod : mods)
        LoadLibraryA(mod.c_str());
}

BOOL WINAPI DllMain(HMODULE /* module */, DWORD reason,  LPVOID /* reserved */)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            proxy::init();
            loadMods();
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            proxy::shutdown();
            break;
        }

        default: break;
    }

    return TRUE;
}