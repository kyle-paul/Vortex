#include "Vortex.h"
#include "Vortex/PreHeaders.h"

namespace Vortex
{
    class ChessModel
    {    
    public:
        static void SubmitPlayer(Ref<Scene> ActiveScene, std::string filepath, std::string &name, glm::vec3 &position, bool opponent);
    };
}