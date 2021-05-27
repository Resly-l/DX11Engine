#pragma once
#include "animation.h"
#include "constant_buffer.h"

class Animator
{
private:
	struct Bone
	{
		uint32_t index; // index used in transform array
		std::string name;

		Matrix inverseBind; // bone space -> mesh space
		Matrix nodeTransform; // used as connection when there's no animation inbetween two bones

		std::vector<Bone*> childPtrs;
	};

	// used when calculating root motion
	struct TransformCapture
	{
		double elapsed;
		Decomposed transform;
	};

private:
	static constexpr size_t maxBones = 300;

	std::unordered_map<std::string, Bone> bones;
	Bone* pRootBone = nullptr;

	TransformCapture previousRootTransform;
	TransformCapture currentRootTransform;

	std::unordered_map<std::string, Animation> animations;
	Animation* pPreviousAnimation = nullptr;
	Animation* pCurrentAnimation; // current can't be nullptr

	const double swappingDuration = 0.3;
	double swappingElapsed = 0.0;

	std::vector<Matrix> boneTransforms{ maxBones }; // constant buffer data
	ArrayConstantBuffer boneTransformCB;

public:
	Animator(const aiScene& ai_scene);

public:
	const std::unordered_map<std::string, Bone>& GetBones() const { return bones; }

	bool SetAnimation(const std::string& animationName);
	Animation* GetCurrentAnimation() const { return pCurrentAnimation; };

	// returns if current animation finished
	void Animate(double deltaSeconds);
	void Bind() const;

	// change in root transform each frame
	Decomposed GetRootMotion() const;

private:
	void MapBones(const aiScene& ai_scene);
	void MapBoneHierarchy(Bone* pParent, const aiNode* pCorrespond);

	const aiNode* FindRootBone(const aiNode* pNode) const;
	static const aiNode* FindNode(const std::string& name, const aiNode* pParent);

	void ElapseActiveAnimations(double deltaSeconds);
	std::unordered_map<std::string, Matrix> InterpolateActiveAnimations();

	void FinalizeTransforms(Bone* pBone, Matrix parentTransform, const std::unordered_map<std::string, Matrix>& interpolations);
};