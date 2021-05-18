#pragma once
#include "common.h"

class NodeBase;

// slot used by node
struct Slot
{
	friend class NodeGraph;

public:
	enum class ValueType
	{
		svBOOL,
		svFLOAT,
		svVECTOR,
		svADDRESS
	};

	enum class Type
	{
		stINPUT,
		stOUTPUT
	};

private:
	// unique id used by imnodes
	// set by node graph
	int uid;

public:
	// key used when hashing node data
	// must be unique in the same node
	std::string valueKey;
	Type type;
	ValueType valueType;

	NodeBase* pOwner = nullptr;
	Slot* pCorrespond = nullptr;

public:
	Slot(const std::string& valueKey, Type type, ValueType valueType);

public:
	int GetUID() const { return uid; }
	bool ConnectTo(Slot* pOther);
};