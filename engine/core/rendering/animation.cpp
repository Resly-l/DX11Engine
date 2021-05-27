#include "animation.h"

Animation::Animation(const aiAnimation& ai_animation)
	:
	name(ai_animation.mName.C_Str()),
	ticksPerSecond(ai_animation.mTicksPerSecond),
	duration(ai_animation.mDuration)
{
	ParseKeyframes(ai_animation);
}

bool Animation::Elapse(double deltaSeconds)
{
	if ((elapsed += deltaSeconds * ticksPerSecond) > duration)
	{
		elapsed = loop ? fmod(elapsed, duration) : elapsed;
		
		return true;
	}

	return false;
}

std::unordered_map<std::string, Matrix> Animation::InterpolateKeyframes()
{
	std::unordered_map<std::string, Matrix> boneTransforms;

	for (auto& keyframe : keyframes)
	{
		Matrix boneTransform;
			
		boneTransform *= InterpolateKey(keyframe.scalingKeys, loop, elapsed,
			[](const Vector& lhs, const Vector& rhs, float alpha)
			{
				return Matrix::Scaling(lhs.Lerp(rhs, alpha));
			});

		boneTransform *= InterpolateKey(keyframe.rotationKeys, loop, elapsed,
			[](const Vector& lhs, const Vector& rhs, float alpha)
			{
				return Matrix::RotationQuaternion(lhs.Slerp(rhs, alpha));
			});

		boneTransform *= InterpolateKey(keyframe.translationKeys, loop, elapsed,
			[](const Vector& lhs, const Vector& rhs, float alpha)
			{
				return Matrix::Translation(lhs.Lerp(rhs, alpha));
			});

		boneTransforms[keyframe.boneName] = boneTransform;
	}

	return boneTransforms;
}

void Animation::ParseKeyframes(const aiAnimation& ai_animation)
{
	keyframes.clear();
	keyframes.reserve(ai_animation.mNumChannels);

	for (uint32_t i = 0; i < ai_animation.mNumChannels; i++)
	{
		const aiNodeAnim* pNodeAnim = ai_animation.mChannels[i];

		keyframes.push_back({ pNodeAnim->mNodeName.C_Str() });

		keyframes[i].scalingKeys.reserve(pNodeAnim->mNumScalingKeys);
		keyframes[i].rotationKeys.reserve(pNodeAnim->mNumRotationKeys);
		keyframes[i].translationKeys.reserve(pNodeAnim->mNumPositionKeys);

		for (uint32_t j = 0; j < pNodeAnim->mNumScalingKeys; j++)
		{
			const aiVectorKey& ai_key = pNodeAnim->mScalingKeys[j];
			keyframes[i].scalingKeys.push_back({ ai_key.mTime, Vector(ai_key.mValue.x, ai_key.mValue.y, ai_key.mValue.z, 0.0f) });
		}

		for (uint32_t j = 0; j < pNodeAnim->mNumRotationKeys; j++)
		{
			const aiQuatKey& ai_key = pNodeAnim->mRotationKeys[j];
			keyframes[i].rotationKeys.push_back({ ai_key.mTime, Vector(ai_key.mValue.x, ai_key.mValue.y, ai_key.mValue.z, ai_key.mValue.w) });
		}

		for (uint32_t j = 0; j < pNodeAnim->mNumPositionKeys; j++)
		{
			const aiVectorKey& ai_key = pNodeAnim->mPositionKeys[j];
			keyframes[i].translationKeys.push_back({ ai_key.mTime, Vector(ai_key.mValue.x, ai_key.mValue.y, ai_key.mValue.z, 0.0f) });
		}
	}
}

size_t Animation::FindPreviousKeyIndex(const std::vector<Key>& keys, double elapsed)
{
	size_t step = keys.size() / 2;
	size_t index = step;

	// binary search
	while (true)
	{
		if (elapsed >= keys[index - 1].time && elapsed <= keys[index].time)
		{
			return index - 1u;
		}

		step = std::max(step / 2, (size_t)1);
		keys[index].time >= elapsed ? index -= step : index += step;
	}
}

Matrix Animation::InterpolateKey(const std::vector<Key>& keys, bool loop, double elapsed, std::function<Matrix(const Vector&, const Vector&, float)> func)
{
	const double animTime = std::min(elapsed, keys.back().time);
	const auto keyIndex = FindPreviousKeyIndex(keys, animTime);
	const double alpha = (animTime - keys[keyIndex].time) / (keys[keyIndex + 1].time - keys[keyIndex].time);

	return func(keys[keyIndex].value, keys[keyIndex + 1].value, (float)alpha);
}