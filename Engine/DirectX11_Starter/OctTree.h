#pragma once

#include "CollisionDetection.h"
#include "RigidBody.h"
#include "ComponentManager.h"
#include "Transform.h"
#include <vector>

using namespace DirectX;

class OctTree
{
public:
	OctTree(BoundingBox region);
	OctTree();
	BoundingBox m_region;
	std::vector<Transform*> m_objects = ComponentManager::current->root->children;
	/// <summary>
	/// These are items which we're waiting to insert into the datastructure. 
	/// We want to accrue as many objects in here as possible before we inject them into the tree. This is slightly more cache friendly.
	/// </summary>
	//Queue<T> m_pendingInsertion = new Queue<T>();

	/// <summary>
	/// These are all of the possible child octants for this node in the tree.
	/// </summary>
	OctTree* m_childNode[8];

	/// <summary>
	/// This is a bitmask indicating which child nodes are actively being used.
	/// It adds slightly more complexity, but is faster for performance since there is only one comparison instead of 8.
	/// </summary>
	byte m_activeNodes = 0;

	/// <summary>
	/// The minumum size for enclosing region is a 1x1x1 cube.
	/// </summary>
	const int MIN_SIZE = 1;

	/// <summary>
	/// this is how many frames we'll wait before deleting an empty tree branch. Note that this is not a constant. The maximum lifespan doubles
	/// every time a node is reused, until it hits a hard coded constant of 64
	/// </summary>
	int m_maxLifespan = 8;          //
	int m_curLife = -1;             //this is how much time we have left

									/// <summary>
									/// A reference to the parent node is sometimes required. If we are a node and we realize that we no longer have items contained within ourselves,
									/// we need to let our parent know that we're empty so that it can delete us.
									/// </summary>
	OctTree* _parent;

	bool m_treeReady = false;       //the tree has a few objects which need to be inserted before it is complete
	bool m_treeBuilt = false;       //there is no pre-existing tree yet.


										   /*Note: we want to avoid allocating memory for as long as possible since there can be lots of nodes.*/
										   /// <summary>
										   /// Creates an oct tree which encloses the given region and contains the provided objects.
										   /// </summary>
										   /// <param name="region">The bounding region for the oct tree.</param>
										   /// <param name="objList">The list of objects contained within the bounding region</param>
	void Update(float gameTime);
	void Add<T>(List<T> ItemList) where T : Physical;
	void Add<T>(T Item) where T: Physical;
	void Remove<T>(T Item) where T : Physical;
	List<IntersectionRecord> AllIntersections(Ray intersectionRay);
	IntersectionRecord NearestIntersection(Ray intersectionRay, Physical.PhysicalType type = Physical.PhysicalType.ALL);
	List<IntersectionRecord> AllIntersections(Ray intersectionRay, Physical.PhysicalType type = Physical.PhysicalType.ALL);
	List<IntersectionRecord> AllIntersections(BoundingFrustum region, Physical.PhysicalType type = Physical.PhysicalType.ALL);


private:
	OctTree(BoundingBox region, std::vector<Transform*> m_objects);
	void Insert<T>(T Item) where T : Physical;
	void BuildTree();
	OctTree CreateNode(BoundingBox region, List<Physical> objList);
	OctTree CreateNode(BoundingBox region, Physical Item);
	void UpdateTree();
	void FindEnclosingBox();
	void FindEnclosingCube();
	List<IntersectionRecord> GetIntersection(BoundingFrustum frustum, Physical.PhysicalType type = Physical.PhysicalType.ALL);
	List<IntersectionRecord> GetIntersection(Ray intersectRay, Physical.PhysicalType type = Physical.PhysicalType.ALL);
	List<IntersectionRecord> GetIntersection(List<Physical> parentObjs, Physical.PhysicalType type = Physical.PhysicalType.ALL);
	bool IsRoot;
	bool HasChildren;
	bool IsEmpty;


	//void Render(PrimitiveBatch pb); render the tree
};