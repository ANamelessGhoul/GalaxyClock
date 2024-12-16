#include "local_time.h"
#include <time.h>
#include <cmath>
#include "raylib.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"      

Time GetLocalTime()
{
    double time = glfwGetTime();
    time_t now = static_cast<time_t>(time);

    struct tm *tm_now = localtime(&now);

    Time result;
    result.hours = tm_now->tm_hour;
    result.minutes = tm_now->tm_min;
    result.seconds = tm_now->tm_sec;
    double whole;
    result.milliseconds = modf(glfwGetTime(), &whole) * 1000.f;

    return result;
}

void InitTimer()
{
    time_t now;
    time(&now);

    glfwSetTime(now);

}
