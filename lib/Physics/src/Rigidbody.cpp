#include "Physics/Rigidbody.hpp"
#include "Physics/Shape.hpp"

#include "Core/Type.hpp"

#include <LinearMath/btMotionState.h>
#include <LinearMath/btDefaultMotionState.h>
#include <LinearMath/btTransform.h>

#include <BulletDynamics/Dynamics/btRigidBody.h>

#include <BulletCollision/CollisionShapes/btBoxShape.h>

namespace Gaze::Physics {
	struct Rigidbody::Impl
	{
		Unique<btRigidBody> handle;
		Unique<btMotionState> motionState;
		Shared<Shape> collisionShape;
	};

	Rigidbody::Rigidbody(Shared<Shape> collisionShape, float mass)
		: m_pImpl(new Impl())
	{
		auto* shape = static_cast<btBoxShape*>(collisionShape->Handle());
		auto inertia = btVector3(0, 0, 0);
		if (mass > 0) {
			shape->calculateLocalInertia(mass, inertia);
		}

		auto transform = btTransform();
		transform.setIdentity();
		transform.setOrigin({ 0, 0, 0 });
		m_pImpl->motionState = MakeUnique<btDefaultMotionState>(transform);
		auto info = btRigidBody::btRigidBodyConstructionInfo(
			mass,
			m_pImpl->motionState.get(),
			shape,
			inertia
		);
		m_pImpl->handle = MakeUnique<btRigidBody>(std::move(info));
		m_pImpl->collisionShape = collisionShape;
	}

	Rigidbody::~Rigidbody()
	{
		delete m_pImpl;
	}

	auto Rigidbody::Handle() const noexcept -> void*
	{
		return reinterpret_cast<void*>(m_pImpl->handle.get());
	}

	auto Rigidbody::SetOrigin(float x, float y, float z) noexcept -> void
	{
		m_pImpl->handle->getWorldTransform().setOrigin({ x, y, z });
		m_pImpl->handle->activate();
	}
	auto Rigidbody::Origin() const noexcept -> glm::vec3
	{
		const auto orig = m_pImpl->handle->getWorldTransform().getOrigin();
		return { orig.x(), orig.y(), orig.z() };
	}

	auto Rigidbody::SetRotation(const glm::vec3& axis, float radians) noexcept -> void
	{
		m_pImpl->handle->getWorldTransform().setRotation({ { axis.x, axis.y, axis.z }, radians });
		m_pImpl->handle->activate();
	}

	auto Rigidbody::RotationAxis() const noexcept -> glm::vec3
	{
		const auto axis = m_pImpl->handle->getWorldTransform().getRotation().getAxis();

		return { axis.x(), axis.y(), axis.z() };
	}

	auto Rigidbody::RotationAngle() const noexcept -> float
	{
		return m_pImpl->handle->getWorldTransform().getRotation().getAngle();
	}

	auto Rigidbody::Translate(const glm::vec3& translation) noexcept -> void
	{
		m_pImpl->handle->translate({ translation.x, translation.y, translation.z });
		m_pImpl->handle->activate();
	}
}
