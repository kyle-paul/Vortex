#pragma once
#include <Vortex/PreHeaders.h>

namespace Vortex
{
    namespace Utils {
        inline std::vector<std::string> split(const std::string &line, const std::string &delimiter, std::vector<std::string> &res)
        {
            size_t pos = 0;
            std::string token;
            std::string tempLine = line;
            while ((pos = tempLine.find(delimiter)) != std::string::npos)
            {
                token = tempLine.substr(0, pos);
                res.push_back(token);
                tempLine.erase(0, pos + delimiter.size());
            }
            res.push_back(tempLine);
            return res;
        }
    };
}