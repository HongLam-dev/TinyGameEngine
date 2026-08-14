#include "GameObject.h"
namespace TinyEngine
{
	GameObject& Component::GetOwner()
	{
		return owner;
	}
}