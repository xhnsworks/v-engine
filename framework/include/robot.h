#ifndef ROBOT_H
#define ROBOT_H
#include "common.h"
#include "etypes.h"
#include "xhn_vector.hpp"
#include "xhn_set.hpp"
#include "xhn_map.hpp"
#include "xhn_static_string.hpp"
#include "xhn_lock.hpp"
#include "emem.hpp"
#include "xhn_smart_ptr.hpp"
#include "rtti.hpp"
#include "rwbuffer.h"
#include "timer.h"
class Robot;
class Action : public RefObject
{
	DeclareRootRTTI;
public:
	TimeCheckpoint m_prevTimeCheckPoint;
	double m_lastProcessingTime;
public:
	Action()
		: m_lastProcessingTime(0.0)
	{}
	virtual ~Action() {}
	virtual void DoImpl() = 0;
	void Do();
	void Tick();
    void Tock();
	inline double GetLastProcessingTime() {
		return m_lastProcessingTime;
	}
};
typedef xhn::SmartPtr<Action> ActionPtr;

class RobotCommandBase : public MemObject
{
    DeclareRootRTTI;
};

class RobotCommand : public RobotCommandBase
{
	DeclareRTTI;
public:
	virtual ~RobotCommand() {}
	virtual bool Test(Robot* exeRob) = 0;
	virtual void Do(Robot* exeRob, xhn::static_string sender) = 0;
};

class RobotCommandReceipt : public RobotCommandBase
{
    DeclareRTTI;
public:
	virtual ~RobotCommandReceipt() {}
	virtual var ShowDetails() = 0;
};

class Robot : public MemObject
{
    DeclareRootRTTI;
public:
	///pthread_t m_thread;
	xhn::vector<ActionPtr> m_actionQueue;
	int m_curtActionIndex;
	xhn::map<xhn::static_string, RWBuffer> m_commandReceivingChannels;
	xhn::map<xhn::static_string, RWBuffer> m_commandTransmissionChannels;
public:
	Robot()
		: m_curtActionIndex(-1) {
		///memset(&m_thread, 0, sizeof(m_thread));
	}
	virtual ~Robot() {}
	inline void AddAction(ActionPtr act) {
		 m_actionQueue.push_back(act);
	}
	friend class RobotManager;
public:
	void RunOnce();
	void CommandProc();
	virtual void CommandProcImpl(xhn::static_string sender, RobotCommand* command) {}
	virtual void CommandReceiptProcImpl(xhn::static_string sender, RobotCommandReceipt* receipt) {}
	virtual xhn::static_string GetName() = 0;
	virtual void InitChannels() {}
	inline void Next() {
		if (m_actionQueue.size()) {
			m_curtActionIndex++;
			if ((euint)m_curtActionIndex >= m_actionQueue.size())
				m_curtActionIndex = 0;
		}
		else
			m_curtActionIndex = -1;
	}
	inline ActionPtr GetCurrnetAction() {
		ActionPtr ret;
		if (m_actionQueue.size()) {
			if (m_curtActionIndex < 0)
				m_curtActionIndex = 0;
			if ((euint)m_curtActionIndex >= m_actionQueue.size())
				m_curtActionIndex = 0;
			ret = m_actionQueue[m_curtActionIndex];
		}
		return ret;
	}
	inline void DoAction() {
		ActionPtr act = GetCurrnetAction();
		if (act.get()) act->Do();
		Next();
		CommandProc();
	}
};

class RobotManager : public MemObject
{
private:
    static RobotManager* s_RobotManager;
public:
    xhn::RWLock2 m_readwriteLock;
	typedef xhn::SmartPtr< xhn::map<xhn::static_string, Robot*> > RobotArray;
	/// ’‚¡Ω∏ˆ»›∆˜–Ë“™—œ∏ÒÕ¨≤Ω
	RobotArray m_robots;
	xhn::map<xhn::static_string, Robot*> m_robotMap;
public:
	RobotManager();

	template<typename R>
	R* AddRobot() {
		R* rob = ENEW R;
		{
			xhn::RWLock2::Instance inst =  m_readwriteLock.GetWriteLock();
			m_robots->insert(xhn::make_pair(rob->GetName(), (Robot*)rob));
			m_robotMap.insert(xhn::make_pair(rob->GetName(), (Robot*)rob));
		}
		rob->InitChannels();
		return rob;
	}
	template<typename R, typename P0>
	R* AddRobot(P0 p0) {
		R* rob = ENEW R(p0);
		{
			xhn::RWLock2::Instance inst =  m_readwriteLock.GetWriteLock();
			m_robots->insert(xhn::make_pair(rob->GetName(), (Robot*)rob));
			m_robotMap.insert(xhn::make_pair(rob->GetName(), (Robot*)rob));
		}
		rob->InitChannels();
		return rob;
	}
    Robot* GetRobot(xhn::static_string robName);
	void MakeChannel(xhn::static_string sender, xhn::static_string receiver);
	void BreakChannel(xhn::static_string sender, xhn::static_string receiver);
	RWBuffer GetChannel(xhn::static_string sender, xhn::static_string receiver);
	static void Init();
	static void Dest();
	static RobotManager* Get();
	inline RobotArray::CheckoutHandle Checkout() {
		xhn::RWLock2::Instance inst = m_readwriteLock.GetReadLock();
		return m_robots.Checkout();
	}
	inline bool Submit(RobotArray::CheckoutHandle& handle) {
		xhn::FGarbageCollectProc< xhn::map<xhn::static_string, Robot*> > gc;
		bool ret = false;
		{
			xhn::RWLock2::Instance inst = m_readwriteLock.GetWriteLock();
			ret = m_robots.Submit(handle, gc);
		}
		return ret;
	}
    /// remove a robot, who is no longer be controlled by a manager, but it also in m_robotMap
    Robot* Remove(xhn::static_string robName);
};
#endif
