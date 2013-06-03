#include "animation.hpp"
#include "render_system.h"
#include "xhn_exception.hpp"
ImplementRTTI(AnimAction, Action);
ImplementRTTI(AnimationRobot, Robot);
ImplementRTTI(CreateAnimCommand, RobotCommand);
ImplementRTTI(PlayAnimCommand, RobotCommand);
ImplementRTTI(StopAnimCommand, RobotCommand);
ImplementRTTI(AnimStatusChangeReceipt, RobotCommandReceipt);

double AnimAction::GetCurrentTime()
{
	return m_timer;
}
double AnimAction::Tick()
{
	TimeCheckpoint checkpoint = TimeCheckpoint::Tick();
	double elapsedTime =
		TimeCheckpoint::CaleElapsedTime(m_prevCheckpoint, checkpoint);
	m_prevCheckpoint = checkpoint;
	m_timer += elapsedTime;
	return elapsedTime;
}
void AnimAction::Tock()
{
}

bool CreateAnimCommand::Test(Robot* exeRob)
{
	return true;
}
void CreateAnimCommand::Do(Robot* exeRob, xhn::static_string sender)
{
	AnimationRobot* animRob = exeRob->DynamicCast<AnimationRobot>();
	if (animRob) {
		animRob->CreateAnimation(m_animName, m_attrHandle, m_attrType);
		xhn::static_string emptyStr;
		if (m_animFileName != emptyStr && m_animName != emptyStr)
		{
	        XMLResourcePtr cfg =
            RenderSystem_load_animation_config(m_animFileName);
			animRob->LoadAnimation(
                cfg,
                m_animName,
				m_isPlayNow
            );
		}
		RWBuffer receiptChannel =
        RobotManager::Get()->GetChannel(animRob->GetName(), sender);
		if (receiptChannel) {
			AnimStatusChangeReceipt* rec =
            ENEW AnimStatusChangeReceipt(
                m_animName,
                NotExist,
                animRob->GetAnimationStatus(m_animName)
            );
			RWBuffer_Write(
                receiptChannel,
                (const euint*)&rec,
                sizeof(rec)
            );
		}
	}
}

bool PlayAnimCommand::Test(Robot* exeRob)
{
	return true;
}
void PlayAnimCommand::Do(Robot* exeRob, xhn::static_string sender)
{
	AnimationRobot* animRob = exeRob->DynamicCast<AnimationRobot>();
	if (animRob) {

		AnimationStatus prevStatus = animRob->GetAnimationStatus(m_animName);
		animRob->PlayAnimation(m_animName);

		RWBuffer receiptChannel =
			RobotManager::Get()->GetChannel(
			animRob->GetName(),
			sender
			);

		if (receiptChannel) {
			AnimStatusChangeReceipt* rec =
				ENEW AnimStatusChangeReceipt(
				m_animName,
				prevStatus,
				animRob->GetAnimationStatus(m_animName)
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
		
		AnimationStatus prevStatus = animRob->GetAnimationStatus(m_animName);
		animRob->StopAnimation(m_animName);
        
		RWBuffer receiptChannel =
        RobotManager::Get()->GetChannel(
            animRob->GetName(),
            sender
        );
        
		if (receiptChannel) {
			AnimStatusChangeReceipt* rec =
            ENEW AnimStatusChangeReceipt(
                m_animName,
                prevStatus,
                animRob->GetAnimationStatus(m_animName)
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
	PlayAnimCommand* pac = command->DynamicCast<PlayAnimCommand>();
	if (pac) {
		pac->Do(this, sender);
	}
}
void AnimationRobot::CommandReceiptProcImpl(xhn::static_string sender,
                                            RobotCommandReceipt* receipt)
{
}
void AnimationRobot::CreateAnimation(xhn::static_string animName, AttributeHandle attr, Attribute::Type attrType)
{
	if (m_animations.find(animName) != m_animations.end()) {
		VEngineExce(ObjectNameAlreadyExistedException, "animation name is already existed");
	}
	AnimationInstance anim(attr, attrType, NULL);
	AnimationMap::iterator iter =
    m_animations.insert(xhn::make_pair(animName, anim));
	ActionPtr act = ENEW AnimAction(&iter->second);
	m_actionQueue.push_back(act);
}
void AnimationRobot::LoadAnimation(XMLResourcePtr file,
                                   xhn::static_string animName,
								   bool playNow)
{
    AnimationMap::iterator iter = m_animations.find(animName);
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
				if (playNow)
				    iter->second.Play();
			}
		}
	}
}
void AnimationRobot::PlayAnimation(xhn::static_string animName)
{
	AnimationMap::iterator iter = m_animations.find(animName);
	if (iter != m_animations.end()) {
		iter->second.Play();
	}
}
void AnimationRobot::StopAnimation(xhn::static_string animName)
{
	AnimationMap::iterator iter = m_animations.find(animName);
	if (iter != m_animations.end()) {
		iter->second.Stop();
	}
}

void AnimationRobot::DestroyAnimation(xhn::static_string animName)
{
	AnimationMap::iterator iter = m_animations.find(animName);
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

AnimationStatus AnimationRobot::GetAnimationStatus(xhn::static_string animName)
{
	AnimationMap::iterator iter = m_animations.find(animName);
	if (iter != m_animations.end()) {
		return iter->second.GetStatus();
	}
	else {
		return NotExist;
	}
}
