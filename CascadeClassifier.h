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
extern REAL mean_min,mean_max,sq_min,sq_max,var_min,var_max;

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
	void LoadDefaultCascade(std::string& cascade_filename,std::string& cascade_filename_range);
	virtual void ApplyOriginalSize(IntImage& original,std::vector<MRect>& results) ;
	void DrawResults(IntImage& image, const std::vector<MRect>& results) const;
};

void PostProcess(std::vector<MRect>& result, const int combine_min);
#endif
