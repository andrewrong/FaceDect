#ifndef CAS_H
#define CAS_H
#include <fstream>
#include <string>
#include <vector>
#include "IntImage.h"
#include "AdaBoostClassifier.h"
#include "SimpleClassifier.h"
#include "MyStruct.h"
#include "ocv.h"

const int sx = 24;
const int sy = 24;

struct CascadeClassifier
{
	int count;
	AdaBoostClassifier* ac;

	CascadeClassifier();
	~CascadeClassifier();
	void Clear(void);
	CascadeClassifier& operator=(const CascadeClassifier& source);

	void WriteToFile(std::ofstream& f) const;
	void ReadFromFile(std::ifstream& f);
	void LoadDefaultCascade(std::string& cascade_filename);
	virtual void ApplyOriginalSize(IntImage& original,const std::string filename) ;
	void DrawResults(IntImage& image, const std::vector<MRect>& results) const;
};

void PostProcess(std::vector<MRect>& result, const int combine_min);
#endif