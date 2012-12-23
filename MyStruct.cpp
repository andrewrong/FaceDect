
#include "MyStruct.h"
long max(long left,long right)
{
    if(left > right)
    {
	return left;
    }
    else
    {
	return right;
    }
}


long min(long left,long right)
{
    if(left < right)
    {
	return left;
    }
    else
    {
	return right;
    }
}

bool MRect::IntersectRect(MRect& source1,MRect& source2)
{
    long maxX = max(source1.left,source2.left); 
    long maxY = max(source1.top,source2.top);
    long minX = min(source1.right,source2.right);
    long minY = min(source1.bottom,source2.bottom);

    if(maxX < minX && maxY < minY)
    {
	this->top = maxY;
	this->left = maxX;
	this->bottom = minY;
	this->right = minX;

	return 1;
    }
    else
    {
	this->top = this->left = this->bottom = this->right = 0;
	return 0;
    }
}

void MRect::UnionRect(MRect& source1,MRect& source2)
{
    long minX = min(source1.left,source2.left); 
    long minY = min(source1.top,source2.top);
    long maxX = max(source1.right,source2.right);
    long maxY = max(source1.bottom,source2.bottom);

    this->top = minY;
    this->left = minX;
    this->bottom = maxY;
    this->right = maxX;
}
