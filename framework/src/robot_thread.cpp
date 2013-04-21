#include "robot_thread.h"
#include "pthread.h"
void* RobotThread::Execute(void *_robThd)
{
	RobotThread* robThd = (RobotThread*)_robThd;
	robThd->Run();
	delete robThd;
	return NULL;
}

void RobotThread::Run()
{
	while (m_isRunning) {
		if (!m_dispatchQueue.get()) {
			m_dispatchQueue = ENEW xhn::list<xhn::static_string>;
		}
		xhn::list<xhn::static_string>* tmp = NULL;
		RobotManager::RobotArray::CheckoutHandle handle;
		Robot* nextActivedRobot = NULL;
		/// 开始循环
		do
		{
			nextActivedRobot = NULL;
			/// 先清除无用数据
			if (tmp) {
				delete tmp;
				tmp = NULL;
			}
			/// 重新克隆
			tmp = m_dispatchQueue->clone();
			/// 重新签出
			handle = RobotManager::Get()->Checkout();

			xhn::map<xhn::static_string, Robot*>* robots = handle.Get();

			if (m_curtActivedRobot) {
				/// 如果当前有激活的Robot，则将该Robot放在队列末尾
				xhn::static_string name = m_curtActivedRobot->GetName();
				robots->insert(xhn::make_pair(name, m_curtActivedRobot));
				tmp->push_back(name);
			}
			else {
				/// 什么也不做，尝试去获取m_curtActivedRobot
			}

			if (tmp->size()) {
				/// 如果队列尺寸不为0
				/// 提取出首个Robot
				xhn::static_string name = tmp->front();
				tmp->pop_front();
				/// 判断提取出的Robot是否是空闲的
				xhn::map<xhn::static_string, Robot*>::iterator iter = robots->find(name);
				if (iter != robots->end()) {
					/// 是的话将它赋给nextActivedRobot
					nextActivedRobot = iter->second;
					robots->erase(iter);
				}
				else {
					/// 否则将该Robot置于队列末尾
					tmp->push_back(name);
				}
			}
			else {
				/// 如果队列尺寸为0
				xhn::map<xhn::static_string, Robot*>::iterator iter = robots->begin();
				/// 则从robots里面取第一个Robot，激活它
				if (iter != robots->end()) {
					robots->erase(iter);
					nextActivedRobot = iter->second;
				}
				else {
					/// 什么也不做，尝试去提交
				}
			}
		} while (!RobotManager::Get()->Submit(handle));

		m_curtActivedRobot = nextActivedRobot;
		m_dispatchQueue = tmp;
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
    for (uint i = 0; i < m_robotThreads.size(); i++)
	{
		m_robotThreads[i]->Stop();
	}
}