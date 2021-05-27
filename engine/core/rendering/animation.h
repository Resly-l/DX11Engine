#pragma once
#include "common.h"

class Animation
{
	friend class Animator;
	
private:
	struct Key
	{
		double time;
		Vector value;
	};

	struct Keyframe
	{
		std::string boneName; // bone affected by this keyframe

		std::vector<Key> scalingKeys;
		std::vector<Key> rotationKeys;
		std::vector<Key> translationKeys;
	};

private:
	const std::string name;

	const double duration;
	double elapsed = 0.0;
	double ticksPerSecond;
	bool loop = true;

	std::vector<Keyframe> keyframes;

public:
	Animation(const aiAnimation& ai_animation);

public:
	std::string GetName() const { return name; }

	void SetLoop(bool loop) { this->loop = loop; }
	bool IsLooping() const { return loop; }
	bool IsFinished() const { return !loop && elapsed >= duration; }

private:
	// returns if elapsed > duration
	bool Elapse(double deltaSeconds);
	std::unordered_map<std::string, Matrix> InterpolateKeyframes();

	void ParseKeyframes(const aiAnimation& ai_animation);

	static size_t FindPreviousKeyIndex(const std::vector<Key>& keys, double elapsed);
	static Matrix InterpolateKey(const std::vector<Key>& keys, bool loop, double elapsed, std::function<Matrix(const Vector&, const Vector&, float)> func);
};