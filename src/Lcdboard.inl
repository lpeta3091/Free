#include <filesystem>

using namespace std;
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
    return "Not found.";
}

inline vector<string> split(const string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


