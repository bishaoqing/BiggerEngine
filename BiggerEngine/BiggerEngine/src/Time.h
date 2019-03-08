#ifndef _H_TIME_H_
#define _H_TIME_H_

class Time {
public:
	static float deltaTime;
	static float lastTime;
};

float Time::deltaTime = 0;
float Time::lastTime = 0;

#endif
