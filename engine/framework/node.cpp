#include "node.h"

void NodeBase::SupplyDependents()
{
	for (auto& output : outputs)
	{
		if (auto pDependent = output.pCorrespond)
		{
			pDependent->pOwner->data[pDependent->valueKey] = data[output.valueKey];
		}
	}
}