#include <btBulletDynamicsCommon.h>

class PhysicsComponent {
public:
    PhysicsComponent(btScalar mass, btCollisionShape* shape, const btVector3& initialPosition) {
        btVector3 localInertia(0, 0, 0);
        shape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), initialPosition));
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, shape, localInertia);

        rigidBody = new btRigidBody(rigidBodyCI);
    }

    ~PhysicsComponent() {
        delete rigidBody->getMotionState();
        delete rigidBody->getCollisionShape();
        delete rigidBody;
    }

    void addToWorld(btDiscreteDynamicsWorld* world) {
        world->addRigidBody(rigidBody);
    }

    void removeFromWorld(btDiscreteDynamicsWorld* world) {
        world->removeRigidBody(rigidBody);
    }

    void applyGravity() {
        rigidBody->applyCentralForce(btVector3(0, -9.81, 0));
    }

    btRigidBody* getRigidBody() {
        return rigidBody;
    }

private:
    btRigidBody* rigidBody;
};