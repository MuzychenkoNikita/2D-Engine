#include "Utils.hpp"
#include "Core.hpp"

namespace Engine::Utils
{
void OnTimerAction(void (*action), float timer) {
    static float timePassed = 0.0f;
    if (timePassed < timer)
        timePassed+= Engine::Core::DeltaTime;
    else
        action;
}
const char* IntToConstChar(int integer) {
    char buffer[200];
    sprintf(buffer, "%d", integer);
    const char* c_str_representation = buffer;
    return c_str_representation;
}
}
