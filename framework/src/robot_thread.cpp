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
		/**
		if (!m_dispatchQueue.get()) {
			m_dispatchQueue = ENEW xhn::list<xhn::static_string>;
		}
		**/
		///xhn::list<xhn::static_string>* tmp = NULL;
		RobotManager::RobotArray::CheckoutHandle handle;
		Robot* nextActivedRobot = NULL;
		/// loop begin
		do
		{
			nextActivedRobot = NULL;
			/// delete cloned queue
			/**
			if (tmp) {
				delete tmp;
				tmp = NULL;
			}
			/// try to clone queue
			tmp = m_dispatchQueue->clone();
			**/
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
/**
			if (m_curtActivedRobot) {
				/// current actived robot push to robot array's tail
				xhn::static_string name = m_curtActivedRobot->GetName();
				robots->insert(xhn::make_pair(name, m_curtActivedRobot));
				tmp->push_back(name);
			}
			else {
				/// m_curtActivedRobot is null, do nothing
			}

			if (tmp->size()) {
				/// get the head of robot array's head
				xhn::static_string name = tmp->front();
				tmp->pop_front();

				xhn::map<xhn::static_string, Robot*>::iterator iter = robots->find(name);
				if (iter != robots->end()) {
                    /// next actived robot is queue head
                    nextActivedRobot = iter->second;
					/// push current actived robot to queue tail
					///tmp->push_back(iter->second->GetName());
					robots->erase(iter);
				}
				else {
                    /// robot array is empty, that next actived robot is current actived robot
					tmp->push_back(name);
                    nextActivedRobot = m_curtActivedRobot;
				}
			}
			else {
				xhn::map<xhn::static_string, Robot*>::iterator iter = robots->begin();
				
				if (iter != robots->end()) {
					robots->erase(iter);
					nextActivedRobot = iter->second;
				}
				else {
					/// do nothing
				}
			}
**/
		} while (!RobotManager::Get()->Submit(handle));

		m_curtActivedRobot = nextActivedRobot;
		///m_dispatchQueue = tmp;
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
	pthread_create(&robThd->m_thread, NULL, RobotThread::Execute, (void *) robThd);
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