#include "animation.hpp"
#include "render_system.h"
ImplementRTTI(AnimAction, Action);
ImplementRTTI(AnimationRobot, Robot);
ImplementRTTI(CreateAnimCommand, RobotCommand);
ImplementRTTI(StopAnimCommand, RobotCommand);
ImplementRTTI(AnimStatusChangeReceipt, RobotCommandReceipt);

bool CreateAnimCommand::Test(Robot* exeRob)
{
	return true;
}
void CreateAnimCommand::Do(Robot* exeRob, xhn::static_string sender)
{
	AnimationRobot* animRob = exeRob->DynamicCast<AnimationRobot>();
	if (animRob) {
		int animID = animRob->CreateAnimation(m_attrHandle, m_attrType);
		xhn::static_string emptyStr;
		if (m_animFileName != emptyStr && m_animName != emptyStr)
		{
	        XMLResourcePtr cfg =
            RenderSystem_load_gui_config(m_animFileName);
			animRob->LoadAnimation(
                animID,
                cfg,
                m_animName
            );
		}
		RWBuffer receiptChannel =
        RobotManager::Get()->GetChannel(animRob->GetName(), sender);
		if (receiptChannel) {
			AnimStatusChangeReceipt* rec =
            ENEW AnimStatusChangeReceipt(
                animID,
                NotExist,
                animRob->GetAnimationStatus(animID)
            );
			RWBuffer_Write(
                receiptChannel,
                (const euint*)&rec,
                sizeof(rec)
            );
		}
	}
}

bool StopAnimCommand::Test(Robot* exeRob)
{
    return true;
}
void StopAnimCommand::Do(Robot* exeRob, xhn::static_string sender)
{
	AnimationRobot* animRob = exeRob->DynamicCast<AnimationRobot>();
	if (animRob) {
		
		animRob->StopAnimation(m_animID);
        
		RWBuffer receiptChannel =
        RobotManager::Get()->GetChannel(
            animRob->GetName(),
            sender
        );
        
		if (receiptChannel) {
			AnimStatusChangeReceipt* rec =
            ENEW AnimStatusChangeReceipt(
                m_animID,
                NotExist,
                animRob->GetAnimationStatus(m_animID)
            );
            
			RWBuffer_Write(
                receiptChannel,
                (const euint*)&rec,
                sizeof(rec)
            );
		}
	}
}

AnimationRobot::AnimationRobot()
: m_animationStamp(0)
, m_timer(0.0)
{
	m_prevCheckpoint = TimeCheckpoint::Tick();
}
double AnimationRobot::GetCurrentTime()
{
    return m_timer;
}
double AnimationRobot::Tick()
{
	TimeCheckpoint checkpoint = TimeCheckpoint::Tick();
	double elapsedTime =
    TimeCheckpoint::CaleElapsedTime(m_prevCheckpoint, checkpoint);
	m_prevCheckpoint = checkpoint;
	m_timer += elapsedTime;
	return elapsedTime;
}
void AnimationRobot::Tock()
{
}
xhn::static_string AnimationRobot::GetName()
{
    return "AnimationRobot";
}
void AnimationRobot::InitChannels()
{
	RobotManager::Get()->MakeChannel("RenderRobot",
                                     "AnimationRobot");
	RobotManager::Get()->MakeChannel("AnimationRobot",
                                     "RenderRobot");
}

void AnimationRobot::CommandProcImpl(xhn::static_string sender,
                                     RobotCommand* command)
{
    CreateAnimCommand* cac = command->DynamicCast<CreateAnimCommand>();
	if (cac) {
        cac->Do(this, sender);
	}
}
void AnimationRobot::CommandReceiptProcImpl(xhn::static_string sender,
                                            RobotCommandReceipt* receipt)
{
}
int AnimationRobot::CreateAnimation(AttributeHandle attr, Attribute::Type attrType)
{
	int ret = -1;
	AnimationInstance anim(attr, attrType, this);
	AnimationMap::iterator iter =
    m_animations.insert(xhn::make_pair(m_animationStamp, anim));
	ActionPtr act = ENEW AnimAction(this, &iter->second);
	m_actionQueue.push_back(act);
	ret = m_animationStamp;
	m_animationStamp++;

	return ret;
}
void AnimationRobot::LoadAnimation(int animID, XMLResourcePtr file,
                                   xhn::static_string animName)
{
    AnimationMap::iterator iter = m_animations.find(animID);
	if (iter != m_animations.end()) {
		pugi::xml_document& doc = file->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node animations = root.child("animations");
		pugi::xml_node animNode = animations.child(animName.c_str());
		if (animNode) {
			pugi::xml_node::iterator timelineIter = animNode.begin();
			pugi::xml_node::iterator timelineEnd = animNode.end();
			if (timelineIter != timelineEnd) {
				pugi::xml_node timelineNode = *timelineIter;
				iter->second.LoadFromXMLNode(timelineNode);
				iter->second.Play();
			}
		}
	}
}
void AnimationRobot::StopAnimation(int animID)
{
	AnimationMap::iterator iter = m_animations.find(animID);
	if (iter != m_animations.end()) {
		iter->second.Stop();
	}
}

void AnimationRobot::DestroyAnimation(int animID)
{
	AnimationMap::iterator iter = m_animations.find(animID);
	if (iter != m_animations.end()) {
		xhn::vector<ActionPtr>::iterator actIter = m_actionQueue.begin();
		xhn::vector<ActionPtr>::iterator actEnd = m_actionQueue.end();
		for (; actIter != actEnd; actIter++) {
			ActionPtr act = *actIter;
			{
				AnimAction* aa = act->DynamicCast<AnimAction>();
				if (aa && aa->m_animation == &iter->second) {
					m_actionQueue.erase(actIter);
					break;
				}
			}
		}
		m_animations.erase(iter);
	}
}

AnimationStatus AnimationRobot::GetAnimationStatus(int animID)
{
	AnimationMap::iterator iter = m_animations.find(animID);
	if (iter != m_animations.end()) {
		return iter->second.GetStatus();
	}
	else {
		return NotExist;
	}
}
