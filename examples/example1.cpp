#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;


inline std::string findPath(const std::string ppath, const std::string name)
{
    std::string res;
    for (const auto & entry : fs::directory_iterator(ppath))
    {
                res = entry.path();
                std::size_t found = res.find(name);
                if (found!=std::string::npos) return res;
    }
    return "";
}

int main()
{
    std::string strn = "setup.xml";
    std::string ppath = "/home/user/Work/lcdboard";
    std::cout << "Found at: " << findPath(ppath, strn) << '\n';
}
