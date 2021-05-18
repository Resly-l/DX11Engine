#include "slot.h"

Slot::Slot(const std::string& valueKey, Type type, ValueType valueType)
	:
	valueKey(valueKey),
	type(type),
	valueType(valueType)
{}

bool Slot::ConnectTo(Slot* pOther)
{
	if (type != pOther->type && valueType == pOther->valueType &&
		pCorrespond == nullptr && pOther->pCorrespond == nullptr)
	{
		pCorrespond = pOther;
		pOther->pCorrespond = this;

		return true;
	}

	return false;
}