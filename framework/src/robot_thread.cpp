#include "robot_thread.h"
#include "pthread.h"
void* RobotThread::Execute(void *_robThd)
{
	RobotThread* robThd = (RobotThread*)_robThd;
	robThd->Run();
	delete robThd;
	return NULL;
}

#include <vector>
#include <string>

void RobotThread::Run()
{
	while (m_isRunning) {
		RobotManager::RobotArray::CheckoutHandle handle;
		Robot* nextActivedRobot = NULL;
		/// loop begin
		do
		{
			nextActivedRobot = NULL;
			/// checkout robot array
			handle = RobotManager::Get()->Checkout();

			xhn::list<Robot*>* robots = handle.Get();
			if (m_curtActivedRobot) {
				robots->push_back(m_curtActivedRobot);
			}
			if (robots->size()) {
				nextActivedRobot = robots->front();
				robots->pop_front();
			}
		} while (!RobotManager::Get()->Submit(handle));

		m_curtActivedRobot = nextActivedRobot;
		if (m_curtActivedRobot) {
			m_curtActivedRobot->DoAction();
		}
	}
}

RobotThreadManager* RobotThreadManager::s_RobotThreadManager = NULL;
void RobotThreadManager::Init()
{
	s_RobotThreadManager = ENEW RobotThreadManager;
}
RobotThreadManager* RobotThreadManager::Get()
{
	return s_RobotThreadManager;
}

void RobotThreadManager::AddRobotThread()
{
	RobotThread* robThd = ENEW RobotThread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&robThd->m_thread,
                   NULL,
                   RobotThread::Execute,
                   (void *) robThd);
	pthread_attr_destroy(&attr);
	m_robotThreads.push_back(robThd);
}

void RobotThreadManager::StopAllRobotThread()
{
    for (euint i = 0; i < m_robotThreads.size(); i++)
	{
		m_robotThreads[i]->Stop();
	}
}