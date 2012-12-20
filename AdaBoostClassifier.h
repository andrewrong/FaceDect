#ifndef ADA_H
#define ADA_H
#include <fstream>
#include "IntImage.h"
#include "SimpleClassifier.h"
#include "MyStruct.h"
#include "ocv.h"


enum GOAL_METHODS {GOAL_METHOD_MIN_ERROR = 1, GOAL_METHOD_VALID_DR = 2, GOAL_METHOD_FIX_FP = 3};

struct AdaBoostClassifier
{
	int count;
	SimpleClassifier* scs;
	REAL* alphas;
	REAL thresh;

	AdaBoostClassifier();
	~AdaBoostClassifier();
	void Clear(void);
	AdaBoostClassifier& operator=(const AdaBoostClassifier& source);
	
	void WriteToFile(std::ofstream& f) const;
	void ReadFromFile(std::ifstream& f);
	void InitToGivenSize(const int size);

	inline REAL GetValue(const IntImage& im) const;
	inline int ApplyImagePatch(const IntImage& im) const;
};

REAL AdaBoostClassifier::GetValue(const IntImage& im) const
{
	int i;
	REAL value;

	value = 0.0;
	for(i=0;i<count;i++) value += alphas[i]*scs[i].Apply(im);
	
	return value;
}

int AdaBoostClassifier::ApplyImagePatch(const IntImage& im) const
{
	return (GetValue(im)>=thresh)?1:0;
}
#endif
