#include "Physics/World.hpp"
#include "Physics/Rigidbody.hpp"

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>

#include <vector>

namespace Gaze::Physics {
	struct World::Impl
	{
		Mem::Unique<btDefaultCollisionConfiguration>     config;
		Mem::Unique<btCollisionDispatcher>               dispatcher;
		Mem::Unique<btDbvtBroadphase>                    broadphase;
		Mem::Unique<btSequentialImpulseConstraintSolver> solver;
		Mem::Unique<btDiscreteDynamicsWorld>             world;
		std::vector<Mem::Shared<Rigidbody>>              rigidbodies;
	};

	World::World()
		: m_pImpl(new Impl())
	{
		m_pImpl->config     = Mem::MakeUnique<btDefaultCollisionConfiguration>();
		m_pImpl->dispatcher = Mem::MakeUnique<btCollisionDispatcher>(m_pImpl->config.get());
		m_pImpl->broadphase = Mem::MakeUnique<btDbvtBroadphase>();
		m_pImpl->solver     = Mem::MakeUnique<btSequentialImpulseConstraintSolver>();
		m_pImpl->world      = Mem::MakeUnique<btDiscreteDynamicsWorld>(
			m_pImpl->dispatcher.get(),
			m_pImpl->broadphase.get(),
			m_pImpl->solver.get(),
			m_pImpl->config.get()
		);

		m_pImpl->world->setGravity({ 0.F, -9.8067F, 0.F });
	}

	World::~World()
	{
		delete m_pImpl;
	}

	auto World::Update(F64 timestep) -> void
	{
		m_pImpl->world->stepSimulation(btScalar(timestep));
	}

	auto World::AddRigidbody(Mem::Shared<Rigidbody> body) -> void
	{
		m_pImpl->world->addRigidBody(static_cast<btRigidBody*>(body->Handle()));
		m_pImpl->rigidbodies.emplace_back(std::move(body));
	}

	auto World::Handle() const noexcept -> void*
	{
		return reinterpret_cast<void*>(m_pImpl->world.get());
	}
}
