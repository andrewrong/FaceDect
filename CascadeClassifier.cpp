
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
using namespace std;
#include "CascadeClassifier.h"

REAL mean_min,mean_max,sq_min,sq_max,var_min,var_max;

CascadeClassifier::CascadeClassifier():count(0),ac(NULL)
{
}

CascadeClassifier::~CascadeClassifier()
{
	Clear();	
}

void CascadeClassifier::Clear()
{
	count = 0;
	delete[] ac; ac=NULL;
}

CascadeClassifier& CascadeClassifier::operator=(const CascadeClassifier& source)
{
	Clear();
	count = source.count;
	int max_nodes = 50;
	ac = new AdaBoostClassifier[max_nodes]; ASSERT(ac!=NULL);
	for(int i=0;i<count;i++) ac[i] = source.ac[i];
	return *this;
}

void CascadeClassifier::ReadFromFile(ifstream& f)
{
	Clear();
	f>>count; f.ignore(256,'\n');
	int max_nodes = 50;
	ac = new AdaBoostClassifier[max_nodes]; ASSERT(ac!=NULL);
	for(int i=0;i<count;i++) ac[i].ReadFromFile(f);
}

void CascadeClassifier::WriteToFile(ofstream& f) const
{
	f<<count<<endl;
	for(int i=0;i<count;i++) ac[i].WriteToFile(f);
}

void CascadeClassifier::LoadDefaultCascade(string& cascade_filename,string& cascade_filename_range)
{
	ifstream f;
	ifstream frange;
	f.open(cascade_filename.c_str());
	frange.open(cascade_filename_range.c_str());
	frange>>mean_min>>mean_max>>sq_min>>sq_max>>var_min>>var_max;
	ReadFromFile(f);
	f.close();
	frange.close();
}

void CascadeClassifier::DrawResults(IntImage& image,const vector<MRect>& results) const
{
	int i;
	unsigned int k;
	int x1,x2,y1,y2;

	for(k=0;k<results.size();k++)
	{
		y1 = (results[k].top>=0)?results[k].top:0; 
		y1 = (results[k].top<image.height)?results[k].top:(image.height-1);
		y2 = (results[k].bottom>=0)?results[k].bottom:0;
		y2 = (results[k].bottom<image.height)?results[k].bottom:(image.height-1);
		x1 = (results[k].left>=0)?results[k].left:0;
		x1 = (results[k].left<image.width)?results[k].left:(image.width-1);
		x2 = (results[k].right>=0)?results[k].right:0;
		x2 = (results[k].right<image.width)?results[k].right:(image.width-1);	
		for(i=y1;i<=y2;i++) 
		{
			image.data[i][x1] = 255;
			image.data[i][x2] = 255;
		}
		for(i=x1;i<=x2;i++)
		{
			image.data[y1][i] = 255;
			image.data[y2][i] = 255;
		}
	}
}

void CascadeClassifier::ApplyOriginalSize(IntImage& original,vector<MRect>& results) 
{
	IntImage procface;
	IntImage image,square;
	REAL sq,ex,value;
	int result;
	MRect rect;
	REAL ratio;

	procface.Copy(original);
	ratio = 1.0;
	results.clear();
	REAL paddedsize = REAL(1)/REAL((sx+1)*(sy+1));
	while((procface.height>sx+1) && (procface.width>sy+1))
	{
		procface.CalcSquareAndIntegral(square,image);
		for(int i=0,size_x=image.height-sx;i<size_x;i+=1)
			for(int j=0,size_y=image.width-sy;j<size_y;j+=1)
			{
				ex = image.data[i+sx][j+sy]+image.data[i][j]-image.data[i+sx][j]-image.data[i][j+sy];
				if(ex<mean_min || ex>mean_max) continue;
				sq = square.data[i+sx][j+sy]+square.data[i][j]-square.data[i+sx][j]-square.data[i][j+sy];
				if(sq<sq_min) continue;
				ex *= paddedsize;
				ex = ex * ex;
				sq *= paddedsize;
				sq = sq - ex;
				ASSERT(sq>=0);
				if(sq>0) sq = sqrt(sq); else sq = 1.0;
				if(sq<var_min) continue;
				result = 1;
				for(int k=0;k<count;k++)
				{
					value = 0.0;
					for(int t=0,size=ac[k].count;t<size;t++)
					{
						REAL f1 = 0;
						REAL** p = image.data + i;
						SimpleClassifier& s = ac[k].scs[t];

						switch(s.type)
						{
							case 0:
								f1 =   p[s.x1][j+s.y3] - p[s.x1][j+s.y1] + p[s.x3][j+s.y3] - p[s.x3][j+s.y1]
									+ REAL(2)*(p[s.x2][j+s.y1] - p[s.x2][j+s.y3]);
								break;
							case 1:
								f1 =   p[s.x3][j+s.y1] + p[s.x3][j+s.y3] - p[s.x1][j+s.y1] - p[s.x1][j+s.y3]
									+ REAL(2)*(p[s.x1][j+s.y2] - p[s.x3][j+s.y2]);
								break;
							case 2:
								f1 =   p[s.x1][j+s.y1] - p[s.x1][j+s.y3] + p[s.x4][j+s.y3] - p[s.x4][j+s.y1]
									+ REAL(3)*(p[s.x2][j+s.y3] - p[s.x2][j+s.y1] + p[s.x3][j+s.y1]  - p[s.x3][j+s.y3]);
								break;
							case 3:
								f1 =   p[s.x1][j+s.y1] - p[s.x1][j+s.y4] + p[s.x3][j+s.y4] - p[s.x3][j+s.y1]
									+ REAL(3)*(p[s.x3][j+s.y2] - p[s.x3][j+s.y3] + p[s.x1][j+s.y3] - p[s.x1][j+s.y2]);
								break;
							case 4:
								f1 =   p[s.x1][j+s.y1] + p[s.x1][j+s.y3] + p[s.x3][j+s.y1] + p[s.x3][j+s.y3]
									- REAL(2)*(p[s.x2][j+s.y1] + p[s.x2][j+s.y3] + p[s.x1][j+s.y2] + p[s.x3][j+s.y2])
									+ REAL(4)*p[s.x2][j+s.y2];
								break;
							default:
								;
						}
						if(s.parity!=0)
							if(f1<sq*s.thresh)
								value += ac[k].alphas[t];
							else ;
						else
							if(f1>=sq*s.thresh)
								value += ac[k].alphas[t];
							else ;
					}
					if(value<ac[k].thresh) 
					{
						result = 0;
						break;
					}
				}
				if(result!=0) 
				{
					const REAL r = 1.0/ratio;
					rect.left = (long)(j*r);rect.top = (long)(i*r);
					rect.right = (long)((j+sy)*r);rect.bottom = (long)((i+sx)*r);
					results.push_back(rect);
				}
			}
		ratio = ratio * REAL(0.8);
		procface.Resize(image,REAL(0.8));
		SwapIntImage(procface,image);
	}

	//total_fp += results.size();

	PostProcess(results,2);
	PostProcess(results,0);
	DrawResults(original,results);
//	original.Save(filename+"_result.JPG");
}

inline int SizeOfRect(const MRect& rect)
{
	return rect.Height()*rect.Width();
}

void PostProcess(vector<MRect>& result,const int combine_min)
{
	vector<MRect> res1;
	vector<MRect> resmax;
	vector<int> res2;
	bool yet;
	MRect rectInter;
	
	for(unsigned int i=0,size_i=result.size();i<size_i;i++)
	{
		yet = false;
		MRect& result_i = result[i];
		for(unsigned int j=0,size_r=res1.size();j<size_r;j++)
		{
			MRect& resmax_j = resmax[j];
			if(rectInter.IntersectRect(result_i,resmax_j))
			{
				if(
					SizeOfRect(rectInter)>0.6*SizeOfRect(result_i) 
				 && SizeOfRect(rectInter)>0.6*SizeOfRect(resmax_j)
				  )
				{
					MRect& res1_j = res1[j];
					resmax_j.UnionRect(resmax_j,result_i);
					res1_j.bottom += result_i.bottom;
					res1_j.top += result_i.top;
					res1_j.left += result_i.left;
					res1_j.right += result_i.right;
					res2[j]++;
					yet = true;
					break;
				}
			}
		}
		if(yet==false)
		{
			res1.push_back(result_i);
			resmax.push_back(result_i);
			res2.push_back(1);
		}
	}

	for(unsigned int i=0,size=res1.size();i<size;i++)
	{
		const int count = res2[i];
		MRect& res1_i = res1[i];
		res1_i.top /= count;
		res1_i.bottom /= count;
		res1_i.left /= count;
		res1_i.right /= count;
	}

	result.clear();
	for(unsigned int i=0,size=res1.size();i<size;i++) 
		if(res2[i]>combine_min)
			result.push_back(res1[i]);
}
