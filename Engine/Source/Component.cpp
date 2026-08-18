#include "GameObject.h"
namespace TinyEngine
{
	const GameObject& Component::GetOwner() const
	{
		return owner;
	}

	GameObject& Component::GetOwner()
	{
		return owner;
	}
}