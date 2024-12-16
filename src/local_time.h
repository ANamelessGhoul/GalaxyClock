#ifndef TIME_H
#define TIME_H

struct Time{
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
    unsigned int milliseconds;
};

Time GetLocalTime();
void InitTimer();


#endif // TIME_H