#ifndef ANIMATION_HPP
#define ANIMATION_HPP
///*************************************************************************************************************************///
///                                                     include begin                                                       ///
///*************************************************************************************************************************///
#include "common.h"
#include "etypes.h"
#include "xhn_map.hpp"
#include "pugixml.hpp"
#include "eassert.h"
#include "robot.h"
#include "float_base.h"
#include "timer.h"
#include "rwbuffer.h"
#include "render_robot.h"
#include "ecg2pugi.h"
#include "xml_resource.h"
#include "attribute.h"
///*************************************************************************************************************************///
///                                                     include end                                                         ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class AnimationRobot;
class AnimTimer
{
public:
    virtual double GetCurrentTime() = 0;
    virtual double Tick() = 0;
    virtual void Tock() = 0;
};

template <typename ATTR_CLONER, typename ATTR_LOADER, typename INTERPOLATION, typename COMMAND_SENDER>
class Animation
{
public:
	typedef xhn::map<double, Attribute*> TimeLine;
	typedef typename xhn::map<double, Attribute*>::iterator Frame;
public:
	AttributeHandle m_target;
	Attribute::Type m_attrType;
	TimeLine m_timeLine;
	Frame m_prevFrame;
	double m_beginTime;
	double m_curtTime;
	int m_periodCount;
    AnimationStatus m_status;
	AnimTimer* m_timer;
	ATTR_CLONER m_attrCloner;
	ATTR_LOADER m_attrLoader;
	INTERPOLATION m_interpolation;
	COMMAND_SENDER m_commandSender;
	Animation()
		: m_attrType(Attribute::Float)
		, m_prevFrame(NULL, 0)
		, m_beginTime(0.0)
		, m_curtTime(0.0)
		, m_periodCount(0)
		, m_status(Stopped)
		, m_timer(NULL)
	{}
	Animation(Attribute::Type attrType, AnimTimer* timer)
		: m_attrType(attrType)
		, m_prevFrame(NULL, 0)
		, m_beginTime(0.0)
		, m_curtTime(0.0)
		, m_periodCount(0)
		, m_status(Stopped)
		, m_timer(timer)
	{}
	Animation(AttributeHandle target, Attribute::Type attrType, AnimTimer* timer)
		: m_target(target)
		, m_attrType(attrType)
		, m_prevFrame(NULL, 0)
		, m_beginTime(0.0)
		, m_curtTime(0.0)
		, m_periodCount(0)
		, m_status(Stopped)
		, m_timer(timer)
	{}
	Animation(const Animation& anim)
		: m_target(anim.m_target)
		, m_attrType(anim.m_attrType)
		, m_prevFrame(NULL, 0)
		, m_beginTime(0.0)
		, m_curtTime(0.0)
		, m_periodCount(0)
		, m_status(anim.m_status)
		, m_timer(anim.m_timer)
	{}
	~Animation()
	{
		Clear();
	}
	void InsertKeyFrame(double time, Attribute* attr) {
		m_timeLine.insert(xhn::make_pair(time, m_attrCloner(attr, m_attrType)));
		Frame beginFrame = m_timeLine.begin();
		m_beginTime = beginFrame->first;
	}
	void Update(double elapsedTime) {
		if (m_status != Playing)
			return;
		if (m_prevFrame != m_timeLine.end()) {
			m_curtTime += elapsedTime;

			double periodTime = GetPeriodTime();
			double totelElapsedTime = m_curtTime - m_beginTime;
			int periodCount = (int)(totelElapsedTime / periodTime);
			if (periodCount > m_periodCount) {
				m_periodCount = periodCount;
				m_prevFrame = m_timeLine.begin();
				Attribute* attr = m_attrCloner(m_prevFrame->second, m_attrType);
				m_commandSender(m_target, attr, m_attrType);
				return;
			}
			double realTotelElapsedTime = totelElapsedTime - ((double)periodCount * periodTime);

			Frame nextFrame = m_prevFrame;
			nextFrame++;
			if (nextFrame == m_timeLine.end()) {
				Attribute* attr = m_attrCloner(m_prevFrame->second, m_attrType);
				m_commandSender(m_target, attr, m_attrType);
				m_prevFrame = m_timeLine.begin();
				///m_beginTime = m_curtTime;
				return;
			}

			while (realTotelElapsedTime > nextFrame->first) {
				m_prevFrame++;
				nextFrame++;
				if (nextFrame == m_timeLine.end()) {
					Attribute* attr = m_attrCloner(m_prevFrame->second, m_attrType);
					m_commandSender(m_target, attr, m_attrType);
					m_prevFrame = m_timeLine.begin();
					///m_beginTime = m_curtTime;
					return;
				}
			}

			if (nextFrame != m_timeLine.end()) {
				double prevTime = m_prevFrame->first;
				double nextTime = nextFrame->first;
				double timeBlock0 = realTotelElapsedTime - prevTime;
				double timeBlock1 = nextTime - realTotelElapsedTime;
				double ratio = timeBlock0 / (timeBlock0 + timeBlock1);
				Attribute* attr = m_interpolation(m_prevFrame->second, nextFrame->second, ratio, m_attrType);
				m_commandSender(m_target, attr, m_attrType);
			}
		}
	}
	void Stop() {
		if (m_timeLine.size()) {
			Frame beginFrame = m_timeLine.begin();
			Attribute* attr = m_attrCloner(beginFrame->second, m_attrType);
		    m_commandSender(m_target, attr, m_attrType);
			m_prevFrame = m_timeLine.end();
			m_curtTime = 0.0;
		}
		m_status = Stopped;
	}
	void LoadFromXMLNode(const pugi::xml_node& node) {
		Attribute::Type srcType;
		_LoadAttributeType(srcType, node, xhn::string("type"));
		if (srcType != m_attrType)
			return;
		Clear();
		pugi::xml_node::iterator iter = node.begin();
		pugi::xml_node::iterator end = node.end();
		for (; iter != end; iter++) {
			pugi::xml_node keyframe = *iter;
			pugi::xml_attribute time = keyframe.attribute("time");
			Attribute* attr = m_attrLoader(keyframe, m_attrType);
			m_timeLine.insert(xhn::make_pair(s_to_us(time.as_double()), attr));
		}
		m_prevFrame = m_timeLine.begin();
	}
	void Clear()
	{
		typename xhn::map<double, Attribute*>::iterator iter = m_timeLine.begin();
		typename xhn::map<double, Attribute*>::iterator end = m_timeLine.end();
		for (; iter != end; iter++)
		{
			delete iter->second;
		}
		m_timeLine.clear();
		m_curtTime = 0.0;
		m_beginTime = 0.0;
		m_prevFrame = m_timeLine.end();
		m_status = Stopped;
	}
	double GetPeriodTime() const {
		double ret = 0.0;
		if (m_timeLine.size() > 1) {
			typename xhn::map<double, Attribute*>::const_iterator begin = m_timeLine.begin();
			typename xhn::map<double, Attribute*>::const_reverse_iterator end = m_timeLine.rbegin();
			double beginTime = begin->first;
			double endTime = end->first;
			ret = endTime - beginTime;
		}
		if (ret <= 0.0)
			ret = 0.001;
		return ret;
	}
	void Resume() {
		if (m_status != Paused)
			return;
		m_status = Playing;
	}
	void Play() {
		if (m_status == Paused || m_status == Stopped) {
			m_beginTime = m_timer->GetCurrentTime();
			m_prevFrame = m_timeLine.begin();
			m_curtTime = m_beginTime;
			m_status = Playing;
		}
	}
	AnimationStatus GetStatus() const {
		return m_status;
	}
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class FAttrCloneProc
{
public:
    inline Attribute* operator() (Attribute* a, Attribute::Type type) {
		switch (type)
		{
		case Attribute::Float:
			return ENEW EFloat( *((EFloat*)a) );
		case Attribute::Float2:
            return ENEW EFloat2( *((EFloat2*)a) );
		case Attribute::Float3:
			return ENEW EFloat3( *((EFloat3*)a) );
		case Attribute::Float4:
			return ENEW EFloat4( *((EFloat4*)a) );
		default:
			return NULL;
		}
	}
};
class FAttrLoaderProc
{
public:
	inline Attribute* operator () (const pugi::xml_node& from, const Attribute::Type type) {
		switch (type)
		{
		case Attribute::Float:
			{
				EFloat* ret = ENEW EFloat;
				_LoadEFloat(*ret, from, xhn::string("value"));
				return ret;
			}
			break;
		case Attribute::Float2:
			{
				EFloat2* ret = ENEW EFloat2;
				_LoadEFloat2(*ret, from, xhn::string("value"));
				return ret;
			}
			break;
		case Attribute::Float3:
			{
				EFloat3* ret = ENEW EFloat3;
				_LoadEFloat3(*ret, from, xhn::string("value"));
				return ret;
			}
			break;
		case Attribute::Float4:
			{
				EFloat4* ret = ENEW EFloat4;
				_LoadEFloat4(*ret, from, xhn::string("value"));
				return ret;
			}
			break;
		default:
			return NULL;
		}
	}
};
class FInterpolationProc
{
public:
	inline Attribute* operator() (Attribute* a, Attribute* b, double factor, Attribute::Type type) {
        switch (type)
		{
		case Attribute::Float:
		    {
			    EFloat* ret = ENEW EFloat;
			    *ret = EFloat::Lerp( *((EFloat*)a), *((EFloat*)b), (float)factor );
			    return ret;
			}
		case Attribute::Float2:
			{
				EFloat2* ret = ENEW EFloat2;
				*ret = EFloat2::Lerp( *((EFloat2*)a), *((EFloat2*)b), (float)factor );
				return ret;
			}
		case Attribute::Float3:
			{
				EFloat3* ret = ENEW EFloat3;
				*ret = EFloat3::Lerp( *((EFloat3*)a), *((EFloat3*)b), (float)factor );
				return ret;
			}
		case Attribute::Float4:
			{
				EFloat4* ret = ENEW EFloat4;
				*ret = EFloat4::Lerp( *((EFloat4*)a), *((EFloat4*)b), (float)factor );
				return ret;
			}
		default:
			return NULL;
		}
	}
};

class FCommandSenderProc
{
public:
	RWBuffer m_commandChannel;
public:
    FCommandSenderProc()
	{
		m_commandChannel = RobotManager::Get()->GetChannel("AnimationRobot", "RenderRobot");
	}
public:
	inline void operator () (AttributeHandle tgt, Attribute* v, Attribute::Type type) {
		xhn::RWLock::Instance inst = tgt.GetWriteLock();
		switch (type)
		{
		case Attribute::Float:
			*((EFloat*)tgt.GetAttribute()) = *((EFloat*)v);
			break;
		case Attribute::Float2:
			*((EFloat2*)tgt.GetAttribute()) = *((EFloat2*)v);
			break;
		case Attribute::Float3:
			*((EFloat3*)tgt.GetAttribute()) = *((EFloat3*)v);
			break;
		case Attribute::Float4:
			*((EFloat4*)tgt.GetAttribute()) = *((EFloat4*)v);
			break;
		}
		delete v;
	}
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class AnimAction : public Action
{
	DeclareRTTI;
public:
	AnimAction(AnimTimer* timer, Animation<FAttrCloneProc, FAttrLoaderProc, FInterpolationProc, FCommandSenderProc>* anim)
		: m_timer(timer)
		, m_animation(anim)
	{}
	AnimTimer* m_timer;
	Animation<FAttrCloneProc, FAttrLoaderProc, FInterpolationProc, FCommandSenderProc>* m_animation;
	virtual void DoImpl() {
        double t = m_timer->Tick();
		m_animation->Update(t);
		m_timer->Tock();
	}
};

///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///

class CreateAnimCommand : public RobotCommand
{
	DeclareRTTI;
public:
	AttributeHandle m_attrHandle;
	Attribute::Type m_attrType;
	xhn::static_string m_animFileName;
	xhn::static_string m_animName;
	xhn::static_string m_animResGrp;
public:
	CreateAnimCommand(AttributeHandle attr, Attribute::Type attrType)
		: m_attrHandle(attr)
		, m_attrType(attrType)
	{}
	virtual bool Test(Robot* exeRob);
	virtual void Do(Robot* exeRob, xhn::static_string sender);
};

struct AnimInfo
{
    int animID;
	AnimationStatus prevStatus;
	AnimationStatus cureStatus;
	AnimInfo(int id, AnimationStatus ps, AnimationStatus cs)
		: animID(id)
		, prevStatus(ps)
		, cureStatus(cs)
	{}
};
class AnimStatusChangeReceipt : public RobotCommandReceipt
{
	DeclareRTTI;
public:
	AnimInfo m_animInfo;
    AnimStatusChangeReceipt(int animID, AnimationStatus prevStatus, AnimationStatus curtStatus)
		: m_animInfo(animID, prevStatus, curtStatus)
	{}
	virtual var ShowDetails() {
		var ret;
		ret.vptr_var = &m_animInfo;
		return ret;
	}
};

class StopAnimCommand : public RobotCommand
{
	DeclareRTTI;
public:
	int m_animID;
public:
	StopAnimCommand(int animID)
		: m_animID(animID)
	{}
	virtual bool Test(Robot* exeRob);
	virtual void Do(Robot* exeRob, xhn::static_string sender);
};

///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class AnimationRobot : public Robot, public AnimTimer
{
	DeclareRTTI;
public:
	int m_animationStamp;
	double m_timer;
	TimeCheckpoint m_prevCheckpoint;
	xhn::map< int, Animation<FAttrCloneProc, FAttrLoaderProc, FInterpolationProc, FCommandSenderProc> > m_animations;
public:
	AnimationRobot();
	virtual double GetCurrentTime();
	virtual double Tick();
	virtual void Tock();
	virtual xhn::static_string GetName();
	virtual void InitChannels();
	virtual void CommandProcImpl(xhn::static_string sender, RobotCommand* command);
	virtual void CommandReceiptProcImpl(xhn::static_string sender, RobotCommandReceipt* receipt);
	int CreateAnimation(AttributeHandle attr, Attribute::Type attrType);
	void LoadAnimation(int animID, XMLResourcePtr file, xhn::static_string animName);
	void StopAnimation(int animID);
	void DestroyAnimation(int animID);
	AnimationStatus GetAnimationStatus(int animID);
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
#endif
