#include "ChessManager.h"
#include "ChessModel.h"
#include "Vortex/Scene/Component.h"

namespace Vortex
{
    struct ChessPiece
    {
        std::string name;
        glm::vec3 position;
        bool opponent=false;

    };

    std::unordered_map<std::string, ChessPiece> m_ChessAssets = {
        // White pieces
        {"assets/Meshes/models/Chess/Bishop.obj", {"White Bishop", glm::vec3(-2.0f, 0.0f, -7.0f), false}},
        {"assets/Meshes/models/Chess/King.obj", {"White King", glm::vec3(0.0f, 0.0f, -7.0f), false}},
        {"assets/Meshes/models/Chess/Knight.obj", {"White Knight", glm::vec3(-1.0f, 0.0f, -7.0f), false}},
        {"assets/Meshes/models/Chess/Pawn.obj", {"White Pawn", glm::vec3(-3.0f, 0.0f, -6.0f), false}},
        {"assets/Meshes/models/Chess/Queen.obj", {"White Queen", glm::vec3(1.0f, 0.0f, -7.0f), false}},
        {"assets/Meshes/models/Chess/Rook.obj", {"White Rook", glm::vec3(-4.0f, 0.0f, -7.0f), false}},
        // White Pawns
        {"assets/Meshes/models/Chess/Pawn.obj", {"Pawn 1", glm::vec3(-4.0f, 0.0f, -6.0f), false}},
        {"assets/Meshes/models/Chess/Pawn.obj", {"Pawn 2", glm::vec3(-3.0f, 0.0f, -6.0f), false}},
        {"assets/Meshes/models/Chess/Pawn.obj", {"Pawn 3", glm::vec3(-2.0f, 0.0f, -6.0f), false}},
        {"assets/Meshes/models/Chess/Pawn.obj", {"Pawn 4", glm::vec3(-1.0f, 0.0f, -6.0f), false}},
        {"assets/Meshes/models/Chess/Pawn.obj", {"Pawn 5", glm::vec3(0.0f, 0.0f, -6.0f), false}},
        {"assets/Meshes/models/Chess/Pawn.obj", {"Pawn 6", glm::vec3(1.0f, 0.0f, -6.0f), false}},
        {"assets/Meshes/models/Chess/Pawn.obj", {"Pawn 7", glm::vec3(2.0f, 0.0f, -6.0f), false}},
        {"assets/Meshes/models/Chess/Pawn.obj", {"Pawn 8", glm::vec3(3.0f, 0.0f, -6.0f), false}},

        // Black pieces
        {"assets/Meshes/models/Opponent/Bishop.obj", {"Black Bishop", glm::vec3(-2.0f, 0.0f, 0.0f), true}},
        {"assets/Meshes/models/Opponent/King.obj", {"Black King", glm::vec3(0.0f, 0.0f, 0.0f), true}},
        {"assets/Meshes/models/Opponent/Knight.obj", {"Black Knight", glm::vec3(-1.0f, 0.0f, 0.0f), true}},
        {"assets/Meshes/models/Opponent/Pawn.obj", {"Black Pawn", glm::vec3(-3.0f, 0.0f, 1.0f), true}},
        {"assets/Meshes/models/Opponent/Queen.obj", {"Black Queen", glm::vec3(1.0f, 0.0f, 0.0f), true}},
        {"assets/Meshes/models/Opponent/Rook.obj", {"Black Rook", glm::vec3(-4.0f, 0.0f, 0.0f), true}},
        // Black Pawns
        {"assets/Meshes/models/Opponent/Pawn.obj", {"Pawn 1", glm::vec3(-4.0f, 0.0f, 1.0f), true}},
        {"assets/Meshes/models/Opponent/Pawn.obj", {"Pawn 2", glm::vec3(-3.0f, 0.0f, 1.0f), true}},
        {"assets/Meshes/models/Opponent/Pawn.obj", {"Pawn 3", glm::vec3(-2.0f, 0.0f, 1.0f), true}},
        {"assets/Meshes/models/Opponent/Pawn.obj", {"Pawn 4", glm::vec3(-1.0f, 0.0f, 1.0f), true}},
        {"assets/Meshes/models/Opponent/Pawn.obj", {"Pawn 5", glm::vec3(0.0f, 0.0f, 1.0f), true}},
        {"assets/Meshes/models/Opponent/Pawn.obj", {"Pawn 6", glm::vec3(1.0f, 0.0f, 1.0f), true}},
        {"assets/Meshes/models/Opponent/Pawn.obj", {"Pawn 7", glm::vec3(2.0f, 0.0f, 1.0f), true}},
        {"assets/Meshes/models/Opponent/Pawn.obj", {"Pawn 8", glm::vec3(3.0f, 0.0f, 1.0f), true}}
    };

    void ChessManager::Render(Ref<Scene> ActiveScene)
    {
        for (auto &[filepath, chesspiece] : m_ChessAssets)
        {
            ChessModel::SubmitPlayer(ActiveScene, filepath, chesspiece.name, chesspiece.position, chesspiece.opponent);
        }

        // Define the size of the chessboard
        constexpr int boardSize = 8;
        constexpr float cubeSize = 1.0f;
        constexpr float offset = -((boardSize - 1) * cubeSize) / 2.0f;

        // Loop through rows and columns to create the chessboard
        for (int row = 0; row < boardSize; ++row)
        {
            for (int col = 0; col < boardSize; ++col)
            {
                // Create a new cube entity
                auto cube = ActiveScene->CreateEntity("Cube");

                // Add the Shape component
                Shape shapeObj = Shape(Shape::BasicType::Cube);
                cube.AddComponent<ShapeComponent>(shapeObj);

                // Add the Sprite Renderer component for color
                auto& spriteRenderer = cube.AddComponent<SpriteRendererComponent>();
                
                // Alternate colors based on the row and column index
                if ((row + col) % 2 == 0)
                {
                    spriteRenderer.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // White
                }
                else
                {
                    spriteRenderer.Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Black
                }

                // Position the cube
                auto& transform = cube.GetComponent<TransformComponent>();
                transform.Translation = glm::vec3(col * cubeSize + offset, -1.0f, row * cubeSize + offset);
                transform.Scale = glm::vec3(cubeSize, cubeSize, cubeSize);
            }
        }
    }
}