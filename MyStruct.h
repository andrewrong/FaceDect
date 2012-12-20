#ifndef MYSTRUCT_H
#define MYSTRUCT_H
struct MRect
{
    long left;
    long top;
    long right;
    long bottom;

    public:
    long Height() const
    {
	return bottom - top;
    }
    long Width() const
    {
	return right - left;
    }
    bool IntersectRect(MRect& source1,MRect& source2);
    void UnionRect(MRect& s1,MRect& s2);
};

struct MSize
{
    long cx;
    long cy;
	MSize(long x,long y)
	{
		cx = x;
		cy = y;
	}
};
#endif
