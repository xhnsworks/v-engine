//
//  attribute.cpp
//  ecg
//
//  Created by 徐 海宁 on 13-5-24.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#include "attribute.h"

ImplementRootRTTI(Attribute);
ImplementRTTI(FloatAttr, Attribute);
ImplementRTTI(Float2Attr, FloatAttr);
ImplementRTTI(Float3Attr, Float2Attr);
ImplementRTTI(Float4Attr, Float3Attr);

component Attribute::ComponentFormat(component comp)
{
	switch(comp)
	{
	case CompX:
	case CompY:
	case CompZ:
	case CompW:
		return comp;
	case CompR:
		return CompX;
	case CompG:
		return CompY;
	case CompB:
		return CompZ;
	case CompA:
		return CompW;
	default:
		return comp;
	}
}
euint Attribute::GetNumComponents(Attribute* attr)
{
	const RTTI* rtti = attr->GetRTTI();
	if (rtti == &FloatAttr::s_RTTI)
		return 1;
	else if (rtti == &Float2Attr::s_RTTI)
		return 2;
	else if (rtti == &Float3Attr::s_RTTI)
		return 3;
	else if (rtti == &Float4Attr::s_RTTI)
		return 4;
	else
		return 0;
}

void Attribute::Copy(Attribute* dst, Attribute* src, component_index bridge)
{
	euint numComps0 = GetNumComponents(dst);
	euint numComps1 = GetNumComponents(src);
	euint numComps2 = bridge.num_comps;
	euint numComps = numComps0;
	if (numComps1 < numComps)
		numComps = numComps1;
	if (numComps2 < numComps)
		numComps = numComps2;
	switch (numComps)
	{
	case 1:
		{
			FloatAttr* ftDst = dst->DynamicCast<FloatAttr>();
			FloatAttr* ftSrc = src->DynamicCast<FloatAttr>();
            component comp = ComponentFormat(bridge.comps[0]);
			if (comp < CompY)
				ftDst->x = ftSrc->x;
		}
		break;
	case 2:
		{
			Float2Attr* ft2Dst = dst->DynamicCast<Float2Attr>();
			Float2Attr* ft2Src = src->DynamicCast<Float2Attr>();
			component comp = ComponentFormat(bridge.comps[0]);
			switch (comp)
			{
			case CompX:
				ft2Dst->x = ft2Src->x;
				break;
			case CompY:
                ft2Dst->x = ft2Src->y;
                break;
			default:
				break;
			}
			comp = ComponentFormat(bridge.comps[1]);
			switch (comp)
			{
			case CompX:
				ft2Dst->y = ft2Src->x;
				break;
			case CompY:
				ft2Dst->y = ft2Src->y;
				break;
			default:
				break;
			}
		}
		break;
	case 3:
		{
			Float3Attr* ft3Dst = dst->DynamicCast<Float3Attr>();
			Float3Attr* ft3Src = src->DynamicCast<Float3Attr>();
			component comp = ComponentFormat(bridge.comps[0]);
			switch (comp)
			{
			case CompX:
				ft3Dst->x = ft3Src->x;
				break;
			case CompY:
				ft3Dst->x = ft3Src->y;
				break;
			case CompZ:
				ft3Dst->x = ft3Src->z;
				break;
			default:
				break;
			}
			comp = ComponentFormat(bridge.comps[1]);
			switch (comp)
			{
			case CompX:
				ft3Dst->y = ft3Src->x;
				break;
			case CompY:
				ft3Dst->y = ft3Src->y;
				break;
			case CompZ:
				ft3Dst->y = ft3Src->z;
				break;
			default:
				break;
			}
			comp = ComponentFormat(bridge.comps[2]);
			switch (comp)
			{
			case CompX:
				ft3Dst->z = ft3Src->x;
				break;
			case CompY:
				ft3Dst->z = ft3Src->y;
				break;
			case CompZ:
				ft3Dst->z = ft3Src->z;
				break;
			default:
				break;
			}
		}
		break;
	case 4:
		{
			Float4Attr* ft4Dst = dst->DynamicCast<Float4Attr>();
			Float4Attr* ft4Src = src->DynamicCast<Float4Attr>();
			component comp = ComponentFormat(bridge.comps[0]);
			switch (comp)
			{
			case CompX:
				ft4Dst->x = ft4Src->x;
				break;
			case CompY:
				ft4Dst->x = ft4Src->y;
				break;
			case CompZ:
				ft4Dst->x = ft4Src->z;
				break;
			case CompW:
				ft4Dst->x = ft4Src->w;
				break;
			}
			comp = ComponentFormat(bridge.comps[1]);
			switch (comp)
			{
			case CompX:
				ft4Dst->y = ft4Src->x;
				break;
			case CompY:
				ft4Dst->y = ft4Src->y;
				break;
			case CompZ:
				ft4Dst->y = ft4Src->z;
				break;
			case CompW:
				ft4Dst->y = ft4Src->w;
				break;
			}
			comp = ComponentFormat(bridge.comps[2]);
			switch (comp)
			{
			case CompX:
				ft4Dst->z = ft4Src->x;
				break;
			case CompY:
				ft4Dst->z = ft4Src->y;
				break;
			case CompZ:
				ft4Dst->z = ft4Src->z;
				break;
			case CompW:
				ft4Dst->z = ft4Src->w;
				break;
			}
			comp = ComponentFormat(bridge.comps[3]);
			switch (comp)
			{
			case CompX:
				ft4Dst->w = ft4Src->x;
				break;
			case CompY:
				ft4Dst->w = ft4Src->y;
				break;
			case CompZ:
				ft4Dst->w = ft4Src->z;
				break;
			case CompW:
				ft4Dst->w = ft4Src->w;
				break;
			}
		}
		break;
	default:
		break;
	}
}

void Attribute::GetBridge(Attribute* attr, component_index& bridge)
{
	const RTTI* rtti = attr->GetRTTI();
	if (rtti == &FloatAttr::s_RTTI) {
		bridge.num_comps = 1;
		bridge.comps[0] = CompX;
	}
	else if (rtti == &Float2Attr::s_RTTI) {
		bridge.num_comps = 2;
		bridge.comps[0] = CompX;
		bridge.comps[1] = CompY;
	}
	else if (rtti == &Float3Attr::s_RTTI) {
		bridge.num_comps = 3;
		bridge.comps[0] = CompX;
		bridge.comps[1] = CompY;
		bridge.comps[2] = CompZ;
	}
	else if (rtti == &Float4Attr::s_RTTI) {
		bridge.num_comps = 4;
		bridge.comps[0] = CompX;
		bridge.comps[1] = CompY;
		bridge.comps[2] = CompZ;
		bridge.comps[3] = CompW;
	}
	else
		bridge.num_comps = 0;
}

component_index Attribute::GetReverseBridge(component_index bridge)
{
	for (euint i = 0; i < bridge.num_comps; i++) {
		bridge.comps[i] = ComponentFormat(bridge.comps[i]);
	}
    component_index ret;
	ret.num_comps = bridge.num_comps;
	for (euint i = 0; i < bridge.num_comps; i++) {
        ret.comps[(euint)bridge.comps[i]] = (component)i;
	}
	return ret;
}