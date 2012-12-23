#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <float.h>
using namespace std;

#include "AdaBoostClassifier.h"

//#include "CascadeClassifier.h"


AdaBoostClassifier::AdaBoostClassifier():count(0),thresh(0.0),alphas(NULL),scs(NULL)
{
}

AdaBoostClassifier::~AdaBoostClassifier()
{
	Clear();
}

void AdaBoostClassifier::Clear()
{
	delete[] scs; scs=NULL;
	delete[] alphas; alphas=NULL;
	count = 0;
	thresh = 0.0;
}

AdaBoostClassifier& AdaBoostClassifier::operator=(const AdaBoostClassifier& source)
{
	InitToGivenSize(source.count);
	count = source.count;
	thresh = source.thresh;
	memcpy(alphas,source.alphas,sizeof(*alphas)*count);
	for(int i=0;i<count;i++) scs[i] = source.scs[i];
	return *this;
}

void AdaBoostClassifier::InitToGivenSize(const int size)
{
	Clear();
	count = 0;
	thresh = 0.0;
	alphas = new REAL[size]; ASSERT(alphas!=NULL);
	scs = new SimpleClassifier[size]; ASSERT(scs!=NULL);
}

void AdaBoostClassifier::WriteToFile(ofstream& f) const
{
	f<<count<<endl;
	f<<thresh<<endl;
	for(int i=0;i<count;i++) f<<alphas[i]<<" ";
	f<<endl;
	for(int i=0;i<count;i++) scs[i].WriteToFile(f);
	f<<endl;
}

void AdaBoostClassifier::ReadFromFile(ifstream& f)
{
	Clear();
	f>>count; ASSERT(count>0);
	f.ignore(256,'\n');
	scs = new SimpleClassifier[count]; ASSERT(scs!=NULL);
	alphas = new REAL[count]; ASSERT(alphas!=NULL);
	
	f>>thresh; f.ignore(256,'\n');
	for(int i=0;i<count;i++) f>>alphas[i]; f.ignore(256,'\n');
	for(int i=0;i<count;i++) scs[i].ReadFromFile(f); f.ignore(256,'\n');
}