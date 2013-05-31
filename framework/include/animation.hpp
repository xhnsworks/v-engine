#ifndef ANIMATION_HPP
#define ANIMATION_HPP
///**********************************************************************///
///                           include begin                              ///
///**********************************************************************///
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
///**********************************************************************///
///                           include end                                ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class AnimationRobot;
class AnimTimer
{
public:
    virtual double GetCurrentTime() = 0;
    virtual double Tick() = 0;
    virtual void Tock() = 0;
};

template <
typename ATTR_CLONER,
typename ATTR_LOADER,
typename INTERPOLATION,
typename COMMAND_SENDER
>
class Animation
{
public:
	typedef xhn::map<double, Attribute*> TimeLine;
	typedef typename xhn::map<double, Attribute*>::iterator Frame;
    typedef typename xhn::map<double, Attribute*>::const_iterator ConstFrame;
    typedef typename xhn::map<double, Attribute*>::const_reverse_iterator
    ConstReverseFrame;
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
	bool m_isLooped;
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
		, m_isLooped(true)
	{}
	Animation(Attribute::Type attrType, AnimTimer* timer)
		: m_attrType(attrType)
		, m_prevFrame(NULL, 0)
		, m_beginTime(0.0)
		, m_curtTime(0.0)
		, m_periodCount(0)
		, m_status(Stopped)
		, m_timer(timer)
		, m_isLooped(true)
	{}
	Animation(AttributeHandle target,
              Attribute::Type attrType,
              AnimTimer* timer)
		: m_target(target)
		, m_attrType(attrType)
		, m_prevFrame(NULL, 0)
		, m_beginTime(0.0)
		, m_curtTime(0.0)
		, m_periodCount(0)
		, m_status(Stopped)
		, m_timer(timer)
		, m_isLooped(true)
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
		, m_isLooped(true)
	{}
	~Animation()
	{
		Clear();
	}
	void InsertKeyFrame(double time, Attribute* attr) {
		m_timeLine.insert(
            xhn::make_pair(time, m_attrCloner(attr, m_attrType))
        );
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
				if (m_isLooped) {
					m_prevFrame = m_timeLine.begin();
					Attribute* attr =
						m_attrCloner(m_prevFrame->second, m_attrType);
					m_commandSender(m_target, attr, m_attrType);
				}
				else {
					Stop();
				}
				return;
			}
			double realTotelElapsedTime =
            totelElapsedTime - ((double)periodCount * periodTime);

			Frame nextFrame = m_prevFrame;
			nextFrame++;
			if (nextFrame == m_timeLine.end()) {
				if (m_isLooped) {
					Attribute* attr =
						m_attrCloner(m_prevFrame->second, m_attrType);
					m_commandSender(m_target, attr, m_attrType);
					m_prevFrame = m_timeLine.begin();
				}
				else {
					Stop();
				}
				return;
			}

			while (realTotelElapsedTime > nextFrame->first) {
				m_prevFrame++;
				nextFrame++;
				if (nextFrame == m_timeLine.end()) {
					if (m_isLooped) {
						Attribute* attr =
							m_attrCloner(m_prevFrame->second, m_attrType);
						m_commandSender(m_target, attr, m_attrType);
						m_prevFrame = m_timeLine.begin();
					}
					else {
						Stop();
					}
					return;
				}
			}

			if (nextFrame != m_timeLine.end()) {
				double prevTime = m_prevFrame->first;
				double nextTime = nextFrame->first;
				double timeBlock0 = realTotelElapsedTime - prevTime;
				double timeBlock1 = nextTime - realTotelElapsedTime;
				double ratio = timeBlock0 / (timeBlock0 + timeBlock1);
				Attribute* attr =
                m_interpolation(m_prevFrame->second,
                                nextFrame->second,
                                ratio,
                                m_attrType);
				m_commandSender(m_target, attr, m_attrType);
			}
		}
	}
	void Stop() {
		if (m_timeLine.size()) {
			Frame beginFrame = m_timeLine.begin();
			Attribute* attr =
            m_attrCloner(beginFrame->second, m_attrType);
		    m_commandSender(m_target, attr, m_attrType);
			m_prevFrame = m_timeLine.end();
			m_curtTime = 0.0;
		}
		m_status = Stopped;
	}
	void LoadFromXMLNode(const pugi::xml_node& node) {
		Attribute::Type srcType;
		_LoadAttributeType(srcType, node, xhn::string("type"));
		pugi::xml_attribute isLooped = node.attribute("is_looped");
		m_isLooped = isLooped.as_bool();
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
		Frame iter = m_timeLine.begin();
		Frame end = m_timeLine.end();
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
			ConstFrame begin = m_timeLine.begin();
			ConstReverseFrame end = m_timeLine.rbegin();
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
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class FAttrCloneProc
{
public:
    inline Attribute* operator() (Attribute* a, Attribute::Type type) {
		switch (type)
		{
		case Attribute::Float:
			return ENEW FloatAttr( *((FloatAttr*)a) );
		case Attribute::Float2:
            return ENEW Float2Attr( *((Float2Attr*)a) );
		case Attribute::Float3:
			return ENEW Float3Attr( *((Float3Attr*)a) );
		case Attribute::Float4:
			return ENEW Float4Attr( *((Float4Attr*)a) );
		default:
			return NULL;
		}
	}
};
class FAttrLoaderProc
{
public:
	inline Attribute* operator () (const pugi::xml_node& from,
                                   const Attribute::Type type) {
		switch (type)
		{
		case Attribute::Float:
			{
				FloatAttr* ret = ENEW FloatAttr;
				_LoadEFloat(*ret, from, xhn::string("value"));
				return ret;
			}
			break;
		case Attribute::Float2:
			{
				Float2Attr* ret = ENEW Float2Attr;
				_LoadEFloat2(*ret, from, xhn::string("value"));
				return ret;
			}
			break;
		case Attribute::Float3:
			{
				Float3Attr* ret = ENEW Float3Attr;
				_LoadEFloat3(*ret, from, xhn::string("value"));
				return ret;
			}
			break;
		case Attribute::Float4:
			{
				Float4Attr* ret = ENEW Float4Attr;
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
	inline Attribute* operator() (Attribute* a,
                                  Attribute* b,
                                  double factor,
                                  Attribute::Type type) {
        switch (type)
		{
		case Attribute::Float:
		    {
			    FloatAttr* ret = ENEW FloatAttr;
			    *ret = FloatAttr::Lerp(
                    *((FloatAttr*)a),
                    *((FloatAttr*)b),
                    (float)factor
                );
			    return ret;
			}
		case Attribute::Float2:
			{
				Float2Attr* ret = ENEW Float2Attr;
				*ret = Float2Attr::Lerp(
                    *((Float2Attr*)a),
                    *((Float2Attr*)b),
                    (float)factor
                );
				return ret;
			}
		case Attribute::Float3:
			{
				Float3Attr* ret = ENEW Float3Attr;
				*ret = Float3Attr::Lerp(
                    *((Float3Attr*)a),
                    *((Float3Attr*)b),
                    (float)factor
                );
				return ret;
			}
		case Attribute::Float4:
			{
				Float4Attr* ret = ENEW Float4Attr;
				*ret = Float4Attr::Lerp(
                    *((Float4Attr*)a),
                    *((Float4Attr*)b),
                    (float)factor
                );
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
    FCommandSenderProc()
	{}
public:
	inline void operator () (AttributeHandle tgt,
                             Attribute* v,
                             Attribute::Type type) {
		switch (type)
		{
		case Attribute::Float:
			///*(tgt.GetAttribute<FloatAttr>()) = *((FloatAttr*)v);
			tgt.SetAttribute((FloatAttr*)v);
			break;
		case Attribute::Float2:
			///*(tgt.GetAttribute<Float2Attr>()) = *((Float2Attr*)v);
			tgt.SetAttribute((Float2Attr*)v);
			break;
		case Attribute::Float3:
			///*(tgt.GetAttribute<Float3Attr>()) = *((Float3Attr*)v);
			tgt.SetAttribute((Float3Attr*)v);
			break;
		case Attribute::Float4:
			///*(tgt.GetAttribute<Float4Attr>()) = *((Float4Attr*)v);
			tgt.SetAttribute((Float4Attr*)v);
			break;
		}
		delete v;
	}
};
typedef Animation<
FAttrCloneProc,
FAttrLoaderProc,
FInterpolationProc,
FCommandSenderProc
> AnimationInstance;
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class AnimAction : public Action, public AnimTimer
{
	DeclareRTTI;
public:
	AnimAction(AnimationInstance* anim)
		: m_animation(anim)
		, m_timer(0.0)
	{
		anim->m_timer = this;
		m_prevCheckpoint = TimeCheckpoint::Tick();
	}
	virtual double GetCurrentTime();
	virtual double Tick();
	virtual void Tock();
	AnimationInstance* m_animation;
	double m_timer;
	TimeCheckpoint m_prevCheckpoint;
	virtual void DoImpl() {
        double t = Tick();
		m_animation->Update(t);
		Tock();
	}
};

///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///

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
    AnimStatusChangeReceipt(int animID,
                            AnimationStatus prevStatus,
                            AnimationStatus curtStatus)
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

///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class AnimationRobot : public Robot
{
	DeclareRTTI;
public:
	int m_animationStamp;
	
    typedef xhn::map< int, AnimationInstance > AnimationMap;
	AnimationMap m_animations;
public:
	AnimationRobot();
	virtual xhn::static_string GetName();
	virtual void InitChannels();
	virtual void CommandProcImpl(xhn::static_string sender,
                                 RobotCommand* command);
	virtual void CommandReceiptProcImpl(xhn::static_string sender,
                                        RobotCommandReceipt* receipt);
	int CreateAnimation(AttributeHandle attr, Attribute::Type attrType);
	void LoadAnimation(int animID,
                       XMLResourcePtr file,
                       xhn::static_string animName);
	void StopAnimation(int animID);
	void DestroyAnimation(int animID);
	AnimationStatus GetAnimationStatus(int animID);
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif
