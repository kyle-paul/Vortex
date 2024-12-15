#include "Vortex.h"

namespace Vortex
{
    class ChessManager
    {
    public:
        static void Render(Ref<Scene> ActiveScene);

    private:
        static void RenderBoard();
    };
}