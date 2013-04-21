#ifndef ROBOT_THREAD_H
#define ROBOT_THREAD_H
#include "xhn_static_string.hpp"
#include "xhn_list.hpp"
#include "xhn_vector.hpp"
#include "xhn_smart_ptr.hpp"
#include "robot.h"
#include "emem.hpp"
class RobotThread : public MemObject
{
public:
	pthread_t m_thread;
	xhn::SmartPtr< xhn::list<xhn::static_string> > m_dispatchQueue;
	Robot* m_curtActivedRobot;
	bool m_isRunning;
public:
	RobotThread()
		: m_curtActivedRobot(NULL)
		, m_isRunning(true)
	{}
	static void* Execute(void *_robThd);
	void Run();
	inline void Stop() {
		m_isRunning = false;
	}
};

class RobotThreadManager : public MemObject
{
private:
	static RobotThreadManager* s_RobotThreadManager;
public:
	xhn::vector<RobotThread*> m_robotThreads;
	void AddRobotThread();
	void StopAllRobotThread();
	static void Init();
	static RobotThreadManager* Get();
};
#endif