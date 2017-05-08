#include "OctTree.h"
// -----------------------------------------------------------------------------
// Construction -- Just "nullify" the class
// -----------------------------------------------------------------------------
Octree::Octree()
	: _pointCount(0), _points(NULL), _center(0.0f, 0.0f, 0.0f, 0), _radius(0.0f)
{
	memset(_child, 0, sizeof(_child));
}

// -----------------------------------------------------------------------------
// Destruction -- free up memory
// -----------------------------------------------------------------------------
Octree::~Octree()
{
	delete[] _points;
}

// -----------------------------------------------------------------------------
// Build the octree
// -----------------------------------------------------------------------------
const   bool    Octree::build(Point **points,
	const unsigned int count,
	const unsigned int threshold,
	const unsigned int maximumDepth,
	const DirectX::BoundingOrientedBox &bounds,
	const unsigned int currentDepth)
{
	// Store bounding volume for visualization purposes
	box = bounds; 

	// You know you're a leaf when...
	//
	// 1. The number of points is <= the threshold
	// 2. We've recursed too deep into the tree
	//    (currentDepth >= maximumDepth)
	//
	//    NOTE: We specifically use ">=" for the depth comparison so that we
	//          can set the maximumDepth depth to 0 if we want a tree with
	//          no depth.

	if (count <= threshold || currentDepth >= maximumDepth)
	{
		// Just store the points in the node, making it a leaf

		_pointCount = count;
		_points = new Point *[count];
		memcpy(_points, points, sizeof(Point *) * count);
		return true;
	}

	// We'll need this (see comment further down in this source)
	// Initialize all values to -1
	int    childPointCounts[8] = { -1, -1, -1, -1, -1, -1, -1,-1 };

	// Classify each point to a child node
	for (unsigned int i = 0; i < count; i++)
	{

		// Current point
		Point   &p = *points[i];

		// Center of this node
		const DirectX::XMFLOAT3 &c = bounds.Center;

		// Here, we need to know which child each point belongs to. To
		// do this, we build an index into the _child[] array using the
		// relative position of the point to the center of the current
		// node

		p.code = 0;
		if (p.x > c.x) p.code |= 1;
		if (p.y > c.y) p.code |= 2;
		if (p.z > c.z) p.code |= 4;

		// We'll need to keep track of how many points get stuck in each
		// child so we'll just keep track of it here, since we have the
		// information handy.

		//childPointCounts[p.code]++;
		childPointCounts[p.code] = i; 
	}

	// Recursively call build() for each of the 8 children
	for (int i = 0; i < 8; i++)
	{
		// Don't bother going any further if there aren't any points for
		// this child in which case the value is -1 
		if (childPointCounts[i] == -1) continue;

		// Allocate the child
		_child[i] = new Octree();

		// Allocate a list of points that were coded JUST for this child
		// only
		//std::cout << childPointCounts[i] << std::endl;
		Point   **newList = new Point *[childPointCounts[i]];

		// Go through the input list of points and copy over the points
		// that were coded for this child
		Point **ptr = newList;

		for (unsigned int j = 0; j < count; j++)
		{
			if (points[j]->code == i)
			{
				*ptr = points[j];
				ptr++;
			}
		}

		// At this point, we have a list of points that will belong to
		// this child node. We'll want to remove any point with a
		// duplicate 'n' in them...
		//
		// [We won't need to reallocate the list, since it's temporary]
		int     newCount = 0;
		for (unsigned int j = 0; j < childPointCounts[i]; j++)
		{
			// Remove duplicates from newList
			// ...
			// Keep track of the new count in 'newCount'

			if (points[j]->n == newList[j]->n)
			{
				//newList[j]--; 
			}
			
			newCount++;	
			/*if (points[j]->n == j)
			{
				*newList = points[j];
				newList--;
				newCount--;
			}*/
		}

		// Generate a new bounding volume -- We do this with a touch of
		// trickery...
		//
		// We use a table of offsets. These offsets determine where a
		// node is, relative to it's parent. So, for example, if want to
		// generate the bottom-left-rear (-x, -y, -z) child for a node,
		// we use (-1, -1, -1).
		// 
		// However, since the radius of a child is always half of its
		// parent's, we use a table of 0.5, rather than 1.0.
		// 
		// These values are stored the following table. Note that this
		// won't compile because it assumes Points are structs, but you
		// get the idea.
		Point   boundsOffsetTable[8] =
		{
			{ -0.5f, -0.5f, -0.5f, 1 },
			{ +0.5f, -0.5f, -0.5f, 2 },
			{ -0.5f, +0.5f, -0.5f, 3 },
			{ +0.5f, +0.5f, -0.5f, 4 },
			{ -0.5f, -0.5f, +0.5f, 5 },
			{ +0.5f, -0.5f, +0.5f, 6 },
			{ -0.5f, +0.5f, +0.5f, 7 },
			{ +0.5f, +0.5f, +0.5f, 8 }
		};

		// Calculate our offset from the center of the parent's node to
		// the center of the child's node

		Point offset = boundsOffsetTable[i] * bounds.Extents;

		// Create a new Bounds, with the center offset and half the
		// radius
		DirectX::BoundingOrientedBox newBounds;
		newBounds.Extents.x = bounds.Extents.x * 0.5f;
		newBounds.Extents.y = bounds.Extents.y * 0.5f;
		newBounds.Extents.z = bounds.Extents.z * 0.5f;

		newBounds.Center.x = bounds.Center.x + offset.x;
		newBounds.Center.y = bounds.Center.y + offset.y;
		newBounds.Center.z = bounds.Center.z + offset.z;


		// Recurse
		_child[i]->build(newList, newCount, threshold, maximumDepth,
			newBounds, currentDepth + 1);

		// Clean up -- FIX ME
		//for (int i = 0; i < childPointCounts[i]; i++)
		//{
		//	delete[] newList[i];
		//}
		//delete[] newList;
		
	}

	return true;
}

// -----------------------------------------------------------------------------
// Determine the [cubic] bounding volume for a set of points
// -----------------------------------------------------------------------------
//const Bounds Octree::calcCubicBounds(const Point * const * points,
//	const unsigned int count)
//{
//	// What we'll give to the caller
//
//	Bounds  b;
//
//	// Determine min/max of the given set of points
//
//	Point   min = *points[0];
//	Point   max = *points[0];
//
//	for (unsigned int i = 1; i < count; i++)
//	{
//		const Point &p = *points[i];
//		if (p.x < min.x) min.x = p.x;
//		if (p.y < min.y) min.y = p.y;
//		if (p.z < min.z) min.z = p.z;
//		if (p.x > max.x) max.x = p.x;
//		if (p.y > max.y) max.y = p.y;
//		if (p.z > max.z) max.z = p.z;
//	}
//
//	// The radius of the volume (dimensions in each direction)
//
//	Point   radius = max - min;
//
//	// Find the center of this space
//
//	b.center = min + radius * 0.5;
//
//	// We want a CUBIC space. By this, I mean we want a bounding cube, not
//	// just a bounding box. We already have the center, we just need a
//	// radius that contains the entire volume. To do this, we find the
//	// maxumum value of the radius' X/Y/Z components and use that
//
//	b.radius = radius.x;
//	if (b.radius < radius.y) b.radius = radius.y;
//	if (b.radius < radius.z) b.radius = radius.z;
//
//	// Done
//
//	return b;
//}

// -----------------------------------------------------------------------------
// Generic tree traversal
// -----------------------------------------------------------------------------
const bool Octree::traverse(callback proc, void *data) const
{
	// Call the callback for this node (if the callback returns false, then
	// stop traversing.

	if (!proc(*this, data)) return false;

	// If I'm a node, recursively traverse my children

	if (!_pointCount)
	{
		for (unsigned int i = 0; i < 8; i++)
		{
			// We store incomplete trees (i.e. we're not guaranteed
			// that a node has all 8 children)

			if (!_child[i]) continue;

			if (!_child[i]->traverse(proc, data)) return false;
		}
	}

	return true;
}

void Octree::Update()
{
	// Render box itself 
	BoundRenderer::instance()->Draw(box);

	// Render points as small boxes 
	for (unsigned int i = 0; i < pointCount(); i++) {
		DirectX::BoundingBox aabb(
			DirectX::XMFLOAT3(points()[i]->x, points()[i]->y, points()[i]->z),
			DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		DirectX::BoundingOrientedBox pointBox;
		DirectX::BoundingOrientedBox::CreateFromBoundingBox(pointBox, aabb);
		pointBox.Transform(pointBox, 1,
			DirectX::XMQuaternionRotationRollPitchYaw(0, 0, 0)
			, DirectX::XMVectorSet(0, 0, 0, 0));
		BoundRenderer::instance()->Draw(pointBox);
	}

	// Repeat for all children 
	for (Octree* tree : _child)
	{
		if (tree != nullptr)
		{
			tree->Update();
		}
	}
}

