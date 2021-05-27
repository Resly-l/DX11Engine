#include "animator.h"
#include "console.h"

Animator::Animator(const aiScene& ai_scene)
	:
	boneTransformCB(ConstantType::ctBONE_TRANSFORM, ShaderBindFlag::bfBIND_VS, Matrix{}, maxBones)
{
	MapBones(ai_scene);

	auto pRootBoneNode = FindRootBone(ai_scene.mRootNode);
	pRootBone = &bones[pRootBoneNode->mName.C_Str()];

	MapBoneHierarchy(pRootBone, pRootBoneNode);

	for (uint32_t i = 0u; i < ai_scene.mNumAnimations; i++)
	{
		const auto& ai_animation = *ai_scene.mAnimations[i];
		animations.emplace(ai_animation.mName.C_Str(), Animation(ai_animation));
	}

	pCurrentAnimation = &(animations.begin()->second);
}

bool Animator::SetAnimation(const std::string& animationName)
{
	if (auto it = animations.find(animationName); it != animations.end())
	{
		if (pCurrentAnimation->name == animationName)
		{
			return false;
		}

		pPreviousAnimation = pCurrentAnimation;
		pPreviousAnimation->SetLoop(false);

		pCurrentAnimation = &(it->second);
		pCurrentAnimation->elapsed = 0.0;
		swappingElapsed = 0.0;

		return true;
	}

	Console::AddLog({ Log::Type::ltWARNING, "animation not found : " + animationName });
	return false;
}

void Animator::Animate(double deltaSeconds)
{
	previousRootTransform.elapsed = pCurrentAnimation->elapsed;
	previousRootTransform.transform = pCurrentAnimation->InterpolateKeyframes()[pRootBone->name].Decompose();

	ElapseActiveAnimations(deltaSeconds);

	currentRootTransform.elapsed = pCurrentAnimation->elapsed;
	currentRootTransform.transform = pCurrentAnimation->InterpolateKeyframes()[pRootBone->name].Decompose();

	// manually setting root bone transform to final buffer without root motion
	boneTransforms[pRootBone->index] = pRootBone->inverseBind;

	const auto interpolations = InterpolateActiveAnimations();

	for (auto pChild : pRootBone->childPtrs)
	{
		FinalizeTransforms(pChild, Matrix::Identity(), interpolations);
	}

	boneTransformCB.Update(boneTransforms);
}

void Animator::Bind() const
{
	boneTransformCB.Bind();
}

Decomposed Animator::GetRootMotion() const
{
	Decomposed rootMotion;

	if (previousRootTransform.elapsed < currentRootTransform.elapsed)
	{
		rootMotion.scale = currentRootTransform.transform.scale - previousRootTransform.transform.scale;
		rootMotion.angle = currentRootTransform.transform.angle - previousRootTransform.transform.angle;
		rootMotion.position = currentRootTransform.transform.position - previousRootTransform.transform.position;
	}
	else
	{
		const auto nodeTransform = pRootBone->nodeTransform.Decompose();

		rootMotion.scale = currentRootTransform.transform.scale - nodeTransform.scale;
		rootMotion.angle = currentRootTransform.transform.angle - nodeTransform.angle;
		rootMotion.position = currentRootTransform.transform.position - nodeTransform.position;
	}

	return rootMotion;
}

void Animator::MapBones(const aiScene& ai_scene)
{
	uint32_t boneIndex = 0;

	for (uint32_t i = 0; i < ai_scene.mNumMeshes; i++)
	{
		for (uint32_t j = 0; j < ai_scene.mMeshes[i]->mNumBones; j++)
		{
			const auto& ai_bone = *ai_scene.mMeshes[i]->mBones[j];
			const std::string boneName(ai_bone.mName.C_Str());

			if (auto it = bones.find(boneName); it == bones.end())
			{
				Matrix inverseBind = reinterpret_cast<const Matrix&>(ai_bone.mOffsetMatrix).GetTransposed();
				Matrix nodeTransform = reinterpret_cast<const Matrix&>(FindNode(boneName, ai_scene.mRootNode)->mTransformation).GetTransposed();

				bones[boneName] = Bone(boneIndex++, boneName, inverseBind, nodeTransform);
			}
		}
	}
}

void Animator::MapBoneHierarchy(Bone* pParent, const aiNode* pCorrespond)
{
	for (uint32_t i = 0u; i < pCorrespond->mNumChildren; i++)
	{
		if (auto it = bones.find(pCorrespond->mChildren[i]->mName.C_Str()); it != bones.end())
		{
			pParent->childPtrs.push_back(&it->second);
			MapBoneHierarchy(pParent->childPtrs.back(), pCorrespond->mChildren[i]);
		}
		else
		{
			MapBoneHierarchy(pParent, pCorrespond->mChildren[i]);
		}
	}
}

const aiNode* Animator::FindRootBone(const aiNode* pNode) const
{
	if (pNode->mParent)
	{
		auto it = bones.find(pNode->mName.C_Str());
		auto parent_it = bones.find(pNode->mParent->mName.C_Str());

		// if bone has no parent bone
		if (it != bones.end() && parent_it == bones.end())
		{
			return pNode;
		}
	}

	for (uint32_t i = 0; i < pNode->mNumChildren; i++)
	{
		if (auto pFound = FindRootBone(pNode->mChildren[i]))
		{
			return pFound;
		}
	}

	return nullptr;
}

const aiNode* Animator::FindNode(const std::string& name, const aiNode* pParent)
{
	if (pParent->mName.C_Str() == name)
	{
		return pParent;
	}

	for (uint32_t i = 0; i < pParent->mNumChildren; i++)
	{
		if (auto pFound = FindNode(name, pParent->mChildren[i]))
		{
			return pFound;
		}
	}

	return nullptr;
}

void Animator::ElapseActiveAnimations(double deltaSeconds)
{
	if (pPreviousAnimation)
	{
		// if swapping ended or previous animation finished, no need to interpolate current w/ previous
		if ((swappingElapsed += deltaSeconds) > swappingDuration || pPreviousAnimation->Elapse(deltaSeconds))
		{
			pPreviousAnimation = nullptr;
		}
	}

	pCurrentAnimation->Elapse(deltaSeconds);
}

std::unordered_map<std::string, Matrix> Animator::InterpolateActiveAnimations()
{
	std::unordered_map<std::string, Matrix> interpolations = pCurrentAnimation->InterpolateKeyframes();

	if (pPreviousAnimation)
	{
		const auto previousInterpolations = pPreviousAnimation->InterpolateKeyframes();
		const double alpha = 1.0 - (swappingElapsed / swappingDuration);

		for (auto& [boneName, transform] : interpolations)
		{
			transform = transform.Interpolate(previousInterpolations.at(boneName), (float)alpha);
		}
	}
	
	return interpolations;
}

void Animator::FinalizeTransforms(Bone* pBone, Matrix parentTransform, const std::unordered_map<std::string, Matrix>& interpolations)
{
	const auto it = interpolations.find(pBone->name);

	const auto transform = it != interpolations.end() ? it->second : pBone->nodeTransform;
	const auto concatenated = transform * parentTransform;

	boneTransforms[pBone->index] = pBone->inverseBind * concatenated;

	for (auto& pChild : pBone->childPtrs)
	{
		FinalizeTransforms(pChild, concatenated, interpolations);
	}
}