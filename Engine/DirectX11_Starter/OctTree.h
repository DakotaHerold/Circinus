#include <stdio.h>
#include <string.h>
#include <iostream>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "BoundRenderer.h"

// COTD Entry submitted by Paul Nettle [midnight@FluidStudios.com]
// Corresponds with an Ask MidNight response (http://www.flipcode.com/askmid/)

// -----------------------------------------------------------------------------
// This defines a callback for traversal
// -----------------------------------------------------------------------------
class   Octree;
typedef bool(*callback)(const Octree &o, void *data);

// -----------------------------------------------------------------------------
// This defines a point class (it's incomplete, but the data elements are there)
// -----------------------------------------------------------------------------
class   Point
{
public:
	Point(float xParam, float yParam, float zParam, int nParam)
	{
		x = xParam;
		y = yParam;
		z = zParam;
		n = nParam; 
		dirty = false; 
	}
	float          x, y, z;        // Position
	unsigned int      n;              // User's unique identifier
	unsigned int    code;           // Used during octree generation
	bool dirty; 

	// Insert typical operators, such as *, +, -, etc.
	Point operator*(const DirectX::XMFLOAT3 vec) 
	{
		Point p(x,y,z,n); 
		p.code = code; 
		p.x *= vec.x; 
		p.y *= vec.y; 
		p.z *= vec.z;
		return p; 
	}

	void Move(float xParam, float yParam, float zParam)
	{
		x += xParam; 
		y += yParam; 
		z += zParam;
		dirty = true; 
	}
};

// -----------------------------------------------------------------------------
// This defines a cubic bounding volume (center, radius)
// -----------------------------------------------------------------------------

//typedef struct
//{
//	Point           center;         // Center of a cubic bounding volume
//	double          radius;         // Radius of a cubic bounding volume
//} Bounds;

// -----------------------------------------------------------------------------
// The octree class -- almost real code!
// -----------------------------------------------------------------------------

class   Octree
{
public:
	// Construction/Destruction

	Octree();
	virtual                         ~Octree();

	// Accessors

	inline   Point * const * points() const { return _points; }
	inline   unsigned int    pointCount() const { return _pointCount; }

	// Implementation
	virtual const   bool            build(Point **points,
		const unsigned int count,
		const unsigned int threshold,
		const unsigned int maximumDepth,
		const DirectX::BoundingOrientedBox &bounds,
		Octree* parentTree,
		const unsigned int currentDepth = 0
		);
	//static  const   Bounds          calcCubicBounds(const Point * const * points,
	//	const unsigned int count);
	virtual const   bool            traverse(callback proc, void *data) const;
	virtual void Update(); 
	virtual bool checkRebuild(); 

protected:
	Octree*					_parent; 
	Octree                  *_child[8];
	unsigned int            _pointCount;
	//unsigned int			_prevPointCount; 
	Point                   **_points;
	Point                   _center;
	DirectX::BoundingOrientedBox _box; 
	bool					isLeaf; 
	int						currDepth; 
	int						treeThreshold;
	int						treeMaxDepth; 
};