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
		/// ��ʼѭ��
		do
		{
			nextActivedRobot = NULL;
			/// �������������
			if (tmp) {
				delete tmp;
				tmp = NULL;
			}
			/// ���¿�¡
			tmp = m_dispatchQueue->clone();
			/// ����ǩ��
			handle = RobotManager::Get()->Checkout();

			xhn::map<xhn::static_string, Robot*>* robots = handle.Get();

			if (m_curtActivedRobot) {
				/// �����ǰ�м����Robot���򽫸�Robot���ڶ���ĩβ
				xhn::static_string name = m_curtActivedRobot->GetName();
				robots->insert(xhn::make_pair(name, m_curtActivedRobot));
				tmp->push_back(name);
			}
			else {
				/// ʲôҲ����������ȥ��ȡm_curtActivedRobot
			}

			if (tmp->size()) {
				/// ������гߴ粻Ϊ0
				/// ��ȡ���׸�Robot
				xhn::static_string name = tmp->front();
				tmp->pop_front();
				/// �ж���ȡ����Robot�Ƿ��ǿ��е�
				xhn::map<xhn::static_string, Robot*>::iterator iter = robots->find(name);
				if (iter != robots->end()) {
					/// �ǵĻ���������nextActivedRobot
					nextActivedRobot = iter->second;
					robots->erase(iter);
				}
				else {
					/// ���򽫸�Robot���ڶ���ĩβ
					tmp->push_back(name);
				}
			}
			else {
				/// ������гߴ�Ϊ0
				xhn::map<xhn::static_string, Robot*>::iterator iter = robots->begin();
				/// ���robots����ȡ��һ��Robot��������
				if (iter != robots->end()) {
					robots->erase(iter);
					nextActivedRobot = iter->second;
				}
				else {
					/// ʲôҲ����������ȥ�ύ
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