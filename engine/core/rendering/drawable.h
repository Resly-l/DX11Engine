#pragma once

class Drawable
{
public:
	virtual ~Drawable() = default;

public:
	virtual void Draw() abstract;
};