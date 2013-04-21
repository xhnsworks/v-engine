#ifndef RESOURCE_NODE_H
#define RESOURCE_NODE_H
#include "common.h"
#include "etypes.h"
#include "emem.hpp"
#include "xhn_map.hpp"
#include "xhn_set.hpp"
#include <map>
#include <vector>

enum Era
{
	Neoproterozoic, // ��Ԫ�Ŵ�
	Paleozoic, // ������
	Mesozoic, // ������
	Cenozoic, // ������
};
class ResourceNode : public MemObject
{
private:
	static STD_NAMESPACE::set<ResourceNode*> s_deletedResNodes;
protected:
	vptr m_mem;
	bool m_isRootNode;
	bool m_isDeleted;
	STD_NAMESPACE::set<ResourceNode*> m_outputs;
	STD_NAMESPACE::set<ResourceNode*> m_inputs;
public:
    ResourceNode(vptr mem);
	ResourceNode();
	virtual ~ResourceNode();
	bool HasInput()
	{
		return m_inputs.size() != 0;
	}
	void Attach(ResourceNode* node)
	{
		m_outputs.insert(node);
		node->m_inputs.insert(this);
	}
	void MarkDeleted()
	{
		s_deletedResNodes.insert(this);
	}
	vptr Get()
	{
		return m_mem;
	}
	bool _TrackBack(ResourceNode* node, STD_NAMESPACE::set<ResourceNode*>& trackBuffer);
	void TrackBack();
    static void Update();
};
#endif
