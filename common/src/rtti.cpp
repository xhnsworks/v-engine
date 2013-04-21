#include "rtti.hpp"
#include "elog.h"
#include "xhn_utility.hpp"
/**
RTTI::RTTI (const char* typeName, const RTTI* parentRTTI)
: m_typeName(typeName)
{
    if (parentRTTI)
		m_parentRTTI = parentRTTI;
	else
		m_parentRTTI = this;
}
**/
ImplementRootRTTI(AAA);
ImplementRTTI(BBB, AAA);
void rtti_test()
{
	BBB b;
	const RTTI* rtti = b.GetRTTI();
	bool ret = b.IsKindOf<AAA>();
	AAA* a = b.DynamicCast<AAA>();
	elog("typename %s is a %x\n", rtti->GetTypeName(), a);
}