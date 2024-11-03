#include "Graphics/Shape.h"

namespace Vortex
{
    Shape::Shape(BasicType type) : m_Type(type)
    {
        switch(type)
        {
            case(BasicType::Square) : {
                RegisterShape("assets/Meshes/BasicType/Square.obj");
                break;
            }

            case(BasicType::Triangle) : {
                RegisterShape("assets/Meshes/BasicType/Triangle.obj");
                break;
            }

            case(BasicType::Circle) : {
                RegisterShape("assets/Meshes/BasicType/Circle.obj");
                break;
            }

            case(BasicType::Cube) : {
                RegisterShape("assets/Meshes/BasicType/Cube.obj");
                break;
            }
        }

        VA = VertexArray::Create();

        Ref<VertexBuffer> VB = VertexBuffer::Create(verts.data(), verts.size() * sizeof(float));
		VB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normals" },
			{ ShaderDataType::Float2, "a_TextCoord" },
		});
		VA->AddVertexBuffer(VB);

		Ref<IndexBuffer> IB = IndexBuffer::Create(indices.data(), indices.size());
		VA->SetIndexBuffer(IB);
    }

    Shape::~Shape()
    {

    }

    void Shape::RegisterShape(const std::string &filepath)
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

        int norm_idx = 0;
        int text_idx = 0;

        while(std::getline(file, line))
        {
            line.erase(0, line.find_first_not_of(" \t"));
            if (line.empty() || line[0] == '#')  continue;

            if (line.substr(0, 2) == "v ")
            {
                words.clear();
                Utils::split(line, " ", words);
                if (words.empty()) continue;

                verts.push_back(std::stof(words[1]));
                verts.push_back(std::stof(words[2]));
                verts.push_back(std::stof(words[3]));

                verts.push_back(0);
                verts.push_back(0);
                verts.push_back(0);

                verts.push_back(0);
                verts.push_back(0);
            }

            else if (line.substr(0, 2) == "vn")
            {
                words.clear();
                Utils::split(line, " ", words);
                if (words.empty()) continue;

                verts[norm_idx*8 + 3] = std::stof(words[1]);
                verts[norm_idx*8 + 4] = std::stof(words[2]);
                verts[norm_idx*8 + 5] = std::stof(words[3]);
                norm_idx++;
            }

            else if (line.substr(0, 2) == "vt")
            {
                words.clear();
                Utils::split(line, " ", words);
                if (words.empty()) continue;

                verts[text_idx*8 + 6] = std::stof(words[1]);
                verts[text_idx*8 + 7] = std::stof(words[2]);
                text_idx++;
            }

            else if (line[0] == 'f') 
            {
                words.clear();
                Utils::split(line, " ", words);
                if (words.empty()) continue;

                for (size_t i = 1; i < words.size(); ++i)
                {
                    std::vector<std::string> parts;
                    Utils::split(words[i], "/", parts);
                    if (!parts.empty() && !parts[0].empty())
                    {
                        indices.push_back(std::stoi(parts[0]) - 1);
                    }
                }
            }
        }
    }
}