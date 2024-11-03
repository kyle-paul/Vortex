#include "Graphics/Mesh.h"

namespace Vortex
{
    namespace Utils {
        std::vector<std::string> split(const std::string &line, const std::string &delimiter, std::vector<std::string> &res)
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

    Mesh::Mesh()
    {

    }

    Mesh::~Mesh()
    {
        
    }


    void Mesh::Init(const std::string &filepath)
    {
        RegisterMesh(filepath);

        // float vertices[8 * 3] = {
        //     -0.5f,  0.5f, -0.5f,
        //     0.5f,  0.5f, -0.5f,
        //     0.5f, -0.5f, -0.5f,
        //     -0.5f, -0.5f, -0.5f,
        //     -0.5f, -0.5f,  0.5f,
        //     0.5f, -0.5f,  0.5f,
        //     0.5f,  0.5f,  0.5f,
        //     -0.5f,  0.5f,  0.5f
        // };


        // uint32_t faces[12 * 3] = {
        //     0, 1, 2,
        //     0, 2, 3, 
        //     4, 5, 6,
        //     4, 6, 7, 
        //     0, 3, 4,
        //     0, 4, 7, 
        //     1, 6, 5,
        //     1, 5, 2, 
        //     2, 5, 4, 
        //     2, 4, 3, 
        //     7, 6, 1,
        //     7, 1, 0
        // };

        VA = VertexArray::Create();

        Ref<VertexBuffer> VB = VertexBuffer::Create(pos.data(), pos.size() * sizeof(float));
        VB->SetLayout({
            { ShaderDataType::Float3, "a_Position" }
        });
        VA->AddVertexBuffer(VB);
        
        Ref<IndexBuffer> IB = IndexBuffer::Create(faces.data(), faces.size());
        VA->SetIndexBuffer(IB);
    };

    void Mesh::RegisterMesh(const std::string &filepath)
    {
        std::ifstream file(filepath.c_str());
        if (!file.is_open())
        {
            std::cerr << "Error: Failed to open file: " << filepath << "\n";
            throw std::runtime_error("Failed to open file");
        }

        std::string line;
        std::vector<std::string> words;
        words.reserve(8);

        while(std::getline(file, line))
        {
            line.erase(0, line.find_first_not_of(" \t"));
            if (line.empty() || line[0] == '#')  continue;

            if (line.substr(0, 2) == "v ")
            {
                words.clear();
                Utils::split(line, " ", words);
                if (words.empty()) continue;
                VX_CORE_INFO("v = {0} {1} {2}", words[1], words[2], words[3]);
                pos.push_back(std::stof(words[1]));
                pos.push_back(std::stof(words[2]));
                pos.push_back(std::stof(words[3]));
            }

            else if (line.substr(0, 2) == "vn")
            {
                words.clear();
                Utils::split(line, " ", words);
                if (words.empty()) continue;
                VX_CORE_INFO("vn = {0} {1} {2}", words[1], words[2], words[3]);
                norms.push_back(std::stof(words[1]));
                norms.push_back(std::stof(words[2]));
                norms.push_back(std::stof(words[3]));
            }

            else if (line.substr(0, 2) == "vt")
            {
                words.clear();
                Utils::split(line, " ", words);
                if (words.empty()) continue;
                VX_CORE_INFO("vt = {0} {1}", words[1], words[2]);
                texts.push_back(std::stof(words[1]));
                texts.push_back(std::stof(words[2]));
            }

            else if (line[0] == 'f') 
            {
                words.clear();
                Utils::split(line, " ", words);
                if (words.empty()) continue;
                VX_CORE_INFO("f = {0} {1} {2}", words[1], words[2], words[3]);

                for (size_t i = 1; i < words.size(); ++i)
                {
                    std::vector<std::string> parts;
                    Utils::split(words[i], "/", parts);
                    if (!parts.empty() && !parts[0].empty())
                    {
                        faces.push_back(std::stoi(parts[0]) - 1);
                    }
                }
            }
        }        
    }
}