#include "robot.h"

ImplementRootRTTI(Action);
ImplementRootRTTI(RobotCommandBase);
ImplementRTTI(RobotCommand, RobotCommandBase);
ImplementRTTI(RobotCommandReceipt, RobotCommandBase);
ImplementRootRTTI(Robot);

void Action::Do()
{
	Tick();
	DoImpl();
	Tock();
}
void Action::Tick()
{
	m_prevTimeCheckPoint = TimeCheckpoint::Tick();
}
void Action::Tock()
{
	TimeCheckpoint tcp = TimeCheckpoint::Tick();
	m_lastProcessingTime =
    TimeCheckpoint::CaleElapsedTime(m_prevTimeCheckPoint, tcp);
    
	m_prevTimeCheckPoint = tcp;
}

RobotManager* RobotManager::s_RobotManager = NULL;

void Robot::RunOnce()
{
	ActionQueue::iterator iter = m_actionQueue.begin();
	ActionQueue::iterator end = m_actionQueue.end();
	for (; iter != end; iter++) {
		ActionPtr act = *iter;
		act->Do();
		CommandProc();
	}
	CommandProc();
}

void Robot::CommandProc()
{
	ChannelMap::iterator iter = m_commandReceivingChannels.begin();
    ChannelMap::iterator end = m_commandReceivingChannels.end();
    for (; iter != end; iter++)
	{
		RobotCommandBase* cmdBase[2];
		euint size = 0;
		RWBuffer channel = iter->second;
		while (RWBuffer_Read(channel, (euint*)cmdBase, &size))
		{
			if (size >= sizeof(RobotCommandBase*)) {
				RobotCommand* cmd =
                cmdBase[0]->DynamicCast<RobotCommand>();
				if (cmd) {
                    CommandProcImpl(iter->first, cmd);
				    delete cmd;
					break;
				}
				RobotCommandReceipt* rec =
                cmdBase[0]->DynamicCast<RobotCommandReceipt>();
				if (rec) {
					CommandReceiptProcImpl(iter->first, rec);
					delete rec;
				}
			}
		}
	}
}

RobotManager::RobotManager()
{
	m_robots = ENEW xhn::list<Robot*>;
}

Robot* RobotManager::GetRobot(xhn::static_string robName)
{
    xhn::RWLock2::Instance inst = m_readwriteLock.GetReadLock();
    RobotMap::iterator iter = m_robotMap.find(robName);
    if (iter != m_robotMap.end()) {
        return iter->second;
    }
    else
        return NULL;
}

void RobotManager::MakeChannel(xhn::static_string sender,
                               xhn::static_string receiver)
{
    xhn::RWLock2::Instance inst = m_readwriteLock.GetWriteLock();
	RobotMap::iterator s = m_robotMap.find(sender);
	if (s == m_robotMap.end())
		return;
	RobotMap::iterator r = m_robotMap.find(receiver);
    if (r == m_robotMap.end())
		return;
	//////////////////////////////////////////////////////////////////////////
	Robot* sRob = s->second;
    Robot::ChannelMap::iterator iter =
    sRob->m_commandTransmissionChannels.find(receiver);
	RWBuffer channel = NULL;
	if (iter != sRob->m_commandTransmissionChannels.end())
		channel = iter->second;
	else {
		channel = RWBuffer_new(1024 * 1024);
		sRob->m_commandTransmissionChannels.insert(
            xhn::make_pair(receiver, channel)
        );
	}
	//////////////////////////////////////////////////////////////////////////
	Robot* rRob = r->second;
	iter = rRob->m_commandReceivingChannels.find(sender);
    if (iter != rRob->m_commandReceivingChannels.end())
		return;
	else
		rRob->m_commandReceivingChannels.insert(
            xhn::make_pair(sender, channel)
        );
}
void RobotManager::BreakChannel(xhn::static_string sender,
                                xhn::static_string receiver)
{
	xhn::RWLock2::Instance inst = m_readwriteLock.GetWriteLock();
	RobotMap::iterator r = m_robotMap.find(receiver);
	if (r != m_robotMap.end()) {
		Robot* rRob = r->second;
		rRob->m_commandReceivingChannels.erase(sender);
	}
	RobotMap::iterator s = m_robotMap.find(sender);
	if (s != m_robotMap.end()) {
		Robot* sRob = r->second;
		Robot::ChannelMap::iterator iter =
        sRob->m_commandTransmissionChannels.find(receiver);
        
		if (iter != sRob->m_commandTransmissionChannels.end()) {
			RobotCommand* cmd[2];
			euint size = 0;
			RWBuffer channel = iter->second;
			while (RWBuffer_Read(channel, (euint*)cmd, &size)) {
				if (size >= sizeof(RobotCommand*))
					delete cmd[0];
			}
			RWBuffer_delete(iter->second);
		}
	}
}
RWBuffer RobotManager::GetChannel(xhn::static_string sender,
                                  xhn::static_string receiver)
{
    xhn::RWLock2::Instance inst = m_readwriteLock.GetReadLock();
	RobotMap::iterator s = m_robotMap.find(sender);
	if (s == m_robotMap.end())
		return NULL;
	Robot* sRob = s->second;
    
	Robot::ChannelMap::iterator iter =
    sRob->m_commandTransmissionChannels.find(receiver);
    
	if (iter != sRob->m_commandTransmissionChannels.end())
		return iter->second;
	else
		return NULL;
}
void RobotManager::Init()
{
	if (!s_RobotManager)
		s_RobotManager = ENEW RobotManager;
}
void RobotManager::Dest()
{
	if (s_RobotManager) {
		delete s_RobotManager;
		s_RobotManager = NULL;
	}
}
RobotManager* RobotManager::Get()
{
	return s_RobotManager;
}
Robot* RobotManager::Remove(xhn::static_string robName)
{
    xhn::RWLock2::Instance inst =  m_readwriteLock.GetWriteLock();

    xhn::list< Robot*>::iterator iter = m_robots->begin();
	xhn::list< Robot*>::iterator end = m_robots->end();
	for (; iter != end; iter++) {
		if ((*iter)->GetName() == robName) {
			Robot* ret = *iter;
			m_robots->remove(iter);
			return ret;
		}
	}
	return NULL;
}