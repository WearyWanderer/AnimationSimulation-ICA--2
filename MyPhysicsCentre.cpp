#include "MyPhysicsCentre.hpp"
#include <tyga/BasicWorldClock.hpp>
#include <tyga/Log.hpp>

//namespace your_name_here
//{

PhysicsObject::PhysicsObject() : mass(0)
{
}

tyga::Vector3 PhysicsObject::
position() const
{
	auto actor = this->Actor();
	if (actor != nullptr) {
		const auto& xform = actor->Transformation();
		return tyga::Vector3(xform._30, xform._31, xform._32);
	}
	return tyga::Vector3();
}

PhysicsPlane::PhysicsPlane()
{
}

tyga::Vector3 PhysicsPlane::
normal() const
{
	auto actor = this->Actor();
	if (actor != nullptr) {
		const auto& xform = actor->Transformation();
		return tyga::Vector3(xform._20, xform._21, xform._22);
	}
	return tyga::Vector3();
}

PhysicsSphere::PhysicsSphere() : radius(1)
{
}

PhysicsBox::PhysicsBox()
{
}

tyga::Vector3 PhysicsBox::
U() const
{
	auto actor = this->Actor();
	if (actor != nullptr) {
		const auto& xform = actor->Transformation();
		return tyga::Vector3(xform._00, xform._01, xform._02);
	}
	return tyga::Vector3();
}

tyga::Vector3 PhysicsBox::
V() const
{
	auto actor = this->Actor();
	if (actor != nullptr) {
		const auto& xform = actor->Transformation();
		return tyga::Vector3(xform._10, xform._11, xform._12);
	}
	return tyga::Vector3();
}

tyga::Vector3 PhysicsBox::
W() const
{
	auto actor = this->Actor();
	if (actor != nullptr) {
		const auto& xform = actor->Transformation();
		return tyga::Vector3(xform._20, xform._21, xform._22);
	}
	return tyga::Vector3();
}

std::shared_ptr<MyPhysicsCentre> MyPhysicsCentre::default_centre_;

std::shared_ptr<MyPhysicsCentre> MyPhysicsCentre::
defaultCentre()
{
	if (default_centre_ == nullptr) {
		default_centre_ = std::make_shared<MyPhysicsCentre>();
	}
	return default_centre_;
}

MyPhysicsCentre::
MyPhysicsCentre()
{
}

MyPhysicsCentre::
~MyPhysicsCentre()
{
}

template<typename T> static void
_prune(std::list<T>& list)
{
	for (auto it = list.begin(); it != list.end();) {
		if (it->expired()) {
			it = list.erase(it);
		}
		else {
			it++;
		}
	}
}

void MyPhysicsCentre::
pruneDeadObjects()
{
	_prune(planes_);
	_prune(spheres_);
	_prune(boxes_);
}

std::shared_ptr<PhysicsPlane> MyPhysicsCentre::
newPlane()
{
	auto new_plane = std::make_shared<PhysicsPlane>();
	planes_.push_back(new_plane);
	return new_plane;
}

std::shared_ptr<PhysicsSphere> MyPhysicsCentre::
newSphere()
{
	auto new_sphere = std::make_shared<PhysicsSphere>();
	spheres_.push_back(new_sphere);
	return new_sphere;
}

std::shared_ptr<PhysicsBox> MyPhysicsCentre::
newBox()
{
	auto new_box = std::make_shared<PhysicsBox>();
	boxes_.push_back(new_box);
	return new_box;
}

void MyPhysicsCentre::
runloopWillBegin()
{
	for (auto s_iter = spheres_.begin(); s_iter != spheres_.end(); s_iter++) //copied 
	{
		// only continue if a strong reference is available
		if (s_iter->expired()) continue;
		auto sphere = s_iter->lock();

		// very crude ground plane collision detection and response
		// TODO: this could be improved
		const auto& p = sphere->position();
		if (p.y < sphere->radius) {
			sphere->velocity.y = 0;
			auto actor = sphere->Actor();
			if (actor != nullptr) {
				auto xform = actor->Transformation();
				xform._31 = sphere->radius;
				actor->setTransformation(xform);
			}
		}

		for (auto b_iter = boxes_.begin(); b_iter != boxes_.end(); b_iter++) //copied the above layout for boxes, checking inside here for each sphere as there is only one box on the badger
		{
			// only continue if a strong reference is available
			if (b_iter->expired()) continue;
			auto box = b_iter->lock();

			float dist = tyga::length(box->position() - sphere->position()); //relative distance(length) between box and sphere

			tyga::Vector3 dir = tyga::unit(box->position() - sphere->position()); //get direction vector

			float radius = std::abs(tyga::dot(box->U(), dir)) + std::abs(tyga::dot(box->V(), dir)) + std::abs(tyga::dot(box->W(), dir)); //plane radius of the badger boxes

			if (dist < (radius + sphere->radius)) //If the length is less than radius of box and sphere, a collision must be occuring
			{
				sphere->collisionOccured = true;
			}
		}

	}
}

void MyPhysicsCentre::
runloopExecuteTask()
{
	const float time = tyga::BasicWorldClock::CurrentTime();
	const float delta_time = tyga::BasicWorldClock::CurrentTickInterval();

	for (auto ptr : spheres_) {
		// only continue if a strong reference is available
		if (ptr.expired()) continue;
		auto model = ptr.lock();

		// only continue if the model is attached to an actor
		auto actor = model->Actor();
		if (actor == nullptr) continue;

		// TODO: perform the physics (kinetic motion) update

		tyga::Vector3 acceleration = utilAyre::ApplyGravity(model->force) / model->mass; //Newton's good ol' 2nd Law - apply gravity straight away to the acceleration so that the mines eventually fall

		model->velocity += acceleration * delta_time; //integrate the acceleration with time

		tyga::Vector3 newPos = utilAyre::EulerVec(model->position(), delta_time, model->velocity); //use the calculated euler of this frame to lerp the toymine's position

		// TODO: update the actor's transformation
		actor->setTransformation(utilAyre::Translate(newPos));
		// reset the force
		model->force = tyga::Vector3(0, 0, 0);
	}
}

void MyPhysicsCentre::
runloopDidEnd()
{
	pruneDeadObjects();
}

//} // end namespace tyga
