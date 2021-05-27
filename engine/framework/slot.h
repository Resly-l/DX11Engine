#pragma once
#include "common.h"

class NodeBase;

// slot used by node
class Slot
{
	friend class NodeGraph;

public:
	enum class ValueType
	{
		svBOOL,
		svFLOAT,
		svSTRING,
		svVECTOR,
		svMATRIX,
		svADDRESS
	};

	enum class Type
	{
		stINPUT,
		stOUTPUT
	};

public:
	// key used when hashing node data
	// must be unique in the same node
	const std::string valueKey;
	const Type type;
	const ValueType valueType;

	NodeBase* pOwner = nullptr;
	Slot* pCorrespond = nullptr;

private:
	// unique id used by imnodes
	// set by node graph
	int uid;

public:
	Slot(std::string valueKey, Type type, ValueType valueType);

public:
	int GetUID() const { return uid; }

	bool ConnectTo(Slot* pOther);
	void RemoveConnection();
};