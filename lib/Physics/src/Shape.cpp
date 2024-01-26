#include "Physics/Shape.hpp"

#include "Core/Type.hpp"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"

namespace Gaze::Physics {
	struct Shape::Impl
	{
		Unique<btCollisionShape> handle;
	};

	Shape::Shape()
		: m_pImpl(new Impl())
	{
	}

	Shape::~Shape()
	{
		delete m_pImpl;
	}

	auto Shape::Handle() const noexcept -> void*
	{
		return reinterpret_cast<void*>(m_pImpl->handle.get());
	}

	BoxShape::BoxShape(float width, float height, float depth)
	{
		m_pImpl->handle = MakeUnique<btBoxShape>(btVector3(width / 2, height / 2, depth / 2));
	}
}
