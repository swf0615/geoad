#include "vol_math_filter_Interface.h"

//qym
#include "vol_math_RawImage.h"
using namespace std;
extern size_t  globalProgressChanged = 0;
extern size_t volatile progressStep = 0;
Raw& MultiThread(int method,int threadcount,Raw &src,void *para);
void MultiThreadsipl(int method,int threadcount,Raw &src,void *para);
void MultiThreadsY(int method,int threadcount,Raw &src,void *para);
void  MultiThreadptr(int method,int datatype,int threadcount,Raw *src,Raw *ret,void *para);
void  MultiThreadsYptr(int method,int datatype,int threadcount,Raw *src,Raw *ret,void *para);
void progress(int type,int total ,int step,bool &cancled)
{

	printf(" %f\n",(float)step*100/total);
}
void * doAnistropicI(ImageVolume & src,AnistropicI & para)
{
	//int method,int threadcount,Raw &src,void *para
	Raw *indata=(Raw *)ImageVolume2Raw(src);
	//Raw *ret =new Raw( MultiThreadsipl(2,para.threadcount,indata,(void *)&para),true); 
	//	return ret;
	 MultiThreadsY(2,para.threadcount,*indata,(void *)&para);
	 return indata;
}
bool  doAnistropicI(ImageVolume * src, ImageVolume *ret,AnistropicI &para )
{
	
	Raw *indata=(Raw *)ImageVolume2Raw(src);
	Raw *outdata=(Raw *)ImageVolume2Raw(ret);
	//Raw *ret =new Raw( MultiThreadsipl(2,para.threadcount,indata,(void *)&para),true); 
	//	return ret;
	MultiThreadptr(2,src->PixelType,para.threadcount,indata,outdata,(void *)&para);
	ImageVolume *res =(ImageVolume*) Raw2ImageVolume(*outdata,ret->PixelType);
	memcpy(ret->Data,res->Data,outdata->size()*sizeof(unsigned char));
	 //return indata;
	//memcpy((unsigned char*)ret->Data,outdata->getdata(),outdata->size()*sizeof(unsigned char));
	 return true;
}
 /**
  \brief	Executes the anistropic  interface operation
 
  \param [in,out]	para	The para.
 
  \return	null if it fails, else.
  */
bool doAnistropicIY(ImageVolume * src, ImageVolume *ret,AnistropicI &para )
{
	Raw *indata=(Raw *)ImageVolume2Raw(src);
	Raw *outdata=(Raw *)ImageVolume2Raw(ret);
	//Raw *ret =new Raw( MultiThreadsipl(2,para.threadcount,indata,(void *)&para),true); 
	//	return ret;
	MultiThreadsYptr(2,src->PixelType,para.threadcount,indata,outdata,(void *)&para);
	 //return indata;
	//memcpy((unsigned char*)ret->Data,outdata->getdata(),outdata->size()*sizeof(unsigned char));
	ImageVolume *res =(ImageVolume*) Raw2ImageVolume(*outdata,ret->PixelType);
	memcpy(ret->Data,res->Data,outdata->size()*sizeof(unsigned char)); 
	return true;
}
 void * doAnistropicI (Process & para)
 {
	 PIXTYPE * in = new PIXTYPE[para.xsize*para.ysize*para.window_size];
	 PIXTYPE *val ;
	 for ( int i = 0; i < para.window_size; i++ )
	 {
		 val = in;
		 PIXTYPE ** slice = para.slices;
		 val += para.xsize*para.ysize;
		 slice ++;
		 memcpy(val, *slice , para.xsize*para.ysize*sizeof(PIXTYPE));
	 }
	 Raw *indata= new Raw(para.xsize,para.ysize,para.window_size,val);
	 //Raw *ret =new Raw( MultiThreadsipl(2,para.threadcount,indata,(void *)&para),true); 
	 //	return ret;
	 
	 //MultiThreadsYptr(2,para.threadcount,*indata,(void *)&para);
	 return indata;
 }

/**
 \brief	Executes the bilateral i operation.

 \param [in,out]	src 	Source for the.
 \param [in,out]	para	The para.

 \return	null if it fails, else.
 */

void * doBilateralI( ImageVolume & src,BilateralFilterI & para)
{
	Raw *indata=(Raw *)ImageVolume2Raw(src);
	//ThreeDim_Bilateral  *bila=new ThreeDim_Bilateral(&indata,para.sigmaD,para.sigmaR);
	//indata=bila->apply(indata);
	//return Raw2ImageVolume(indata,src.PixelType);
	//Raw * ret=new Raw(MultiThreadsipl(3,para.threadcount,indata,(void *)&para),true);
	MultiThreadsY(3,para.threadcount,*indata,(void *)&para);
	return indata;
}
bool doBilateralI (ImageVolume * src, ImageVolume *ret,BilateralFilterI &para)
{
	Raw *indata=(Raw *)ImageVolume2Raw(src);
	Raw *outdata=(Raw *)ImageVolume2Raw(ret);
	//Raw *ret =new Raw( MultiThreadsipl(2,para.threadcount,indata,(void *)&para),true); 
	//	return ret;
	MultiThreadptr(3,src->PixelType,para.threadcount,indata,outdata,(void *)&para);
	 //return indata;
	memcpy((unsigned char*)ret->Data,outdata->getdata(),outdata->size()*sizeof(unsigned char));
	 return true;

}
extern void *doGuassFilterI (Process & para)
{
	 PIXTYPE ** in = para.slices;
	 PIXTYPE *val = para.result ;
	 PIXTYPE *cur = para.result;
	 for ( int i = 0; i < para.window_size ; i++ )
	 {
		 PIXTYPE *slice = *(in + i);
		 
		 memcpy(cur, slice , para.xsize*para.ysize*sizeof(PIXTYPE));
		// if( i != para.window_size - 1)
		 cur += para.xsize*para.ysize;
	 }
	 Raw *indata= new Raw(para.xsize,para.ysize,para.window_size,para.result,true);
	 Raw *ret =new Raw(para.xsize,para.ysize,para.window_size,para.result ,true); 
	 //	return ret;
	 MultiThreadsYptr(2,para.datatype,para.threadcount,indata,ret,(void *)&para);
	 return indata;
}
void * doGuassFilterI( ImageVolume & src,GuassFilterI & para)
{
	Raw *indata=(Raw *)ImageVolume2Raw(src);
	//Filter *guass = new Filter();
	//Raw *ret=guass->guass3DFilter(&indata,para.halfsize);
	//return Raw2ImageVolume(*ret,src.PixelType);
	MultiThreadsY(4,para.threadcount,*indata,(void *) &para);
	return indata;
}
bool doGuassFilterI (ImageVolume * src, ImageVolume *ret,GuassFilterI &para)
{
	Raw *indata=(Raw *)ImageVolume2Raw(src);
	Raw *outdata=(Raw *)ImageVolume2Raw(ret);
	//Raw *ret =new Raw( MultiThreadsipl(2,para.threadcount,indata,(void *)&para),true); 
	//	return ret;
	MultiThreadptr(4,src->PixelType,para.threadcount,indata,outdata,(void *)&para);
	 //return indata;
	ImageVolume *res =(ImageVolume*) Raw2ImageVolume(*outdata,ret->PixelType);
	memcpy(ret->Data,res->Data,outdata->size()*sizeof(unsigned char));
	return true;

}
bool doGuassFilterIY (ImageVolume * src, ImageVolume *ret,GuassFilterI &para)
{
	Raw *indata=(Raw *)ImageVolume2Raw(src);
	Raw *outdata=(Raw *)ImageVolume2Raw(ret);
	//Raw *ret =new Raw( MultiThreadsipl(2,para.threadcount,indata,(void *)&para),true); 
	//	return ret;
	MultiThreadsYptr(4,src->PixelType,para.threadcount,indata,outdata,(void *)&para);
	 //return indata;
	ImageVolume *res =(ImageVolume*) Raw2ImageVolume(*outdata,ret->PixelType);
	memcpy(ret->Data,res->Data,outdata->size()*sizeof(unsigned char));
	return true;
}
void * doTrilateralfilterI(ImageVolume &src, TrilateralfilterI & para)
{
	Raw *indata =(Raw*)ImageVolume2Raw(src);
	//Raw  *ret =new Raw (MultiThreadsipl(1,para.threadcount,indata,(void *)&para),true);
	MultiThreadsY(1,para.threadcount,*indata,(void *)&para);
	return indata;
}
bool doTrilateralfilterI ( ImageVolume * src, ImageVolume *ret,TrilateralfilterI &para)
{
	Raw *indata=(Raw *)ImageVolume2Raw(src);
	Raw *outdata=(Raw *)ImageVolume2Raw(ret);
	//Raw *ret =new Raw( MultiThreadsipl(2,para.threadcount,indata,(void *)&para),true); 
	//	return ret;
	MultiThreadptr(1,src->PixelType,para.threadcount,indata,outdata,(void *)&para);
	 //return indata;
	ImageVolume *res =(ImageVolume*) Raw2ImageVolume(*outdata,ret->PixelType);
	memcpy(ret->Data,res->Data,outdata->size()*sizeof(unsigned char));
	 return true;

}
bool doTrilateralfilterIY ( ImageVolume * src, ImageVolume *ret,TrilateralfilterI &para)
{
	Raw *indata=(Raw *)ImageVolume2Raw(src);
	Raw *outdata=(Raw *)ImageVolume2Raw(ret);
	//Raw *ret =new Raw( MultiThreadsipl(2,para.threadcount,indata,(void *)&para),true); 
	//	return ret;
	MultiThreadsYptr(1,src->PixelType,para.threadcount,indata,outdata,(void *)&para);
	 //return indata;
	ImageVolume *res =(ImageVolume*) Raw2ImageVolume(*outdata,ret->PixelType);
	memcpy(ret->Data,res->Data,outdata->size()*sizeof(unsigned char));
	 return true;

}
//extern void * doMultiOstuI (ImageVolume &src,MultiOstuI &para)
//{
//	Raw &indata =*(Raw *)ImageVolume2Raw(src);
//	OTSU *ostu =new OTSU(indata);
//	if (para.method==1)
//	{
//		
//		ostu->Otsu_MultiVal(indata,para.classnum);
//	} 
//	else
//	{
//		//OTSU *ostu =new OTSU(indata);
//		ostu->Otsu_MultiVal(indata);
//	}
//	//delete ostu;
//	return &ostu->array;
//
//}
//extern void * dolowPassI2D (Image2D &src,lowPassI &para)
//{
//	Raw2D indata = Image2D2Raw2D(src);
//	FourierFilter2 * ff = new FourierFilter2();
//	ff->lowpass_trans(lowPassI.thresold);
//	return ff->fraw2d;
//}

// void *dolowPassI (ImageVolume &src,lowPassI &para)
//{
//	Raw indata = *(Raw *)ImageVolume2Raw(src);
//	FourierFilter3 * ff3 = new FourierFilter3(indata);
//	ff3->lowpass_trans(para.threshold,progress);
//	Raw * ret= new Raw(*(ff3->fraw),true);
//	return ret;
//}
 void *doAnistropicykfour_diff (ImageVolume &src,AnistropicI &para)
 {
	 Raw indata = *(Raw *)ImageVolume2Raw(src);
	 //WipeNioisePde * pde =new WipeNioisePde();
	 Raw *outdata=new Raw(src);
	 WipeNioisePde *pde=new WipeNioisePde(indata,*outdata,1,para.time,para.val,2,progress);//two pointers,one in,one out
	Filter * guass = new Filter();
	guass->guass3DFilter(outdata,3);
	  delete pde;
	 return outdata;
 }



 void *doAnistropicI2D (Image2D &src,AnistropicI & para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata=Image2D2Raw2D(src);//to be test
	Raw2D indata=Image2D2Raw2D(src);//to be test
	Anistropic2D * pde=new Anistropic2D(indata,para.time,para.val,para.method);//to be ask
	delete pde;
	return &indata;
}
extern void *doBilateralI2D (Image2D & src, BilateralFilterI &para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata=Image2D2Raw2D(src);
	Raw2D indata=Image2D2Raw2D(src);
	BilateralFilter *b=new BilateralFilter(&indata,6,3);
	b->apply(indata);
	return Raw2D2Image2D(indata,src.PixelType);
}
extern void * doGuassFilterI2D (Image2D &src, GuassFilterI &para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata = Image2D2Raw2D(src);
	Raw2D indata = Image2D2Raw2D(src);
	Filter *guass = new Filter(); 
	Raw2D ret=guass->guassFilter(&indata,para.halfsize);
	return Raw2D2Image2D(ret,src.PixelType);
}
extern void * doTrilateralfilterI2D ( Image2D &src, TrilateralfilterI &para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata = Image2D2Raw2D(src);
	Raw2D indata = Image2D2Raw2D(src);
	Trilateralfilter2D * tf=new Trilateralfilter2D();
	Raw2D *ret=tf->TrilateralFilter2D(&indata,para.sigmaC);
	return Raw2D2Image2D(*ret,src.PixelType);
}
extern void * doMultiOstuI2D (Image2D &src,MultiOstuI &para)
{
	return NULL;

}

///=========================================================================================================////
struct  AnistropicP
{
	Raw *src;
	int time;
	int val; //val=1
	int method; 

	/*
	method:
	1:Perona_Malik();
	2:four_diff
	*/
	AnistropicP(Raw *data,int time,int val,int method)
	{
		this->src=data;
		this->time=time;
		this->val=val;
		this->method=method;
	}
	AnistropicP()
	{

	}


};
struct  AnistropicPsipl
{
	Raw *src;
	Raw *ret;
	int iter;
	int time;
	int val; //val=1
	int method; 

	/*
	method:
	1:Perona_Malik();
	2:four_diff
	*/
	AnistropicPsipl(Raw *data,Raw *ret,int iter,int time,int val,int method)
	{
		this->src = data;
		this->ret = ret;
		this->iter =  iter;
		this->time = time;
		this->val = val;
		this->method = method;
	}
	AnistropicPsipl()
	{

	}


};
struct  TrilateralfilterP
{
	float sigmaC;//sigmaC=1
	Raw *src;
	//Raw * ret;
	TrilateralfilterP( Raw *src, float sigmaC )
	{
		this->src = src;
		//this->ret = ret;
		this->sigmaC = sigmaC;

	}
	TrilateralfilterP()
	{

	}
};
struct  TrilateralfilterPSipl
{
	float sigmaC;//sigmaC=1
	Raw *src;
	Raw * ret;
	int iter;
	TrilateralfilterPSipl( Raw *src, Raw *ret, float sigmaC,int iter )
	{
		this->src = src;
		this->ret = ret;
		this->sigmaC = sigmaC;
		this->iter = iter;

	}
	TrilateralfilterPSipl()
	{

	}
};
struct BilateralFilterP
{
	Raw *src;
	double sigmaD;
	double sigmaR;
	BilateralFilterP(Raw *src,double sigmaD,double sigmaR)
	{
		this->src=src;
		this->sigmaD=sigmaD;
		this->sigmaR=sigmaR;
	}
	BilateralFilterP()
	{
		//this->src=NULL;
		this->sigmaD=0;
		this->sigmaR=0;
	}
};
struct BilateralFilterPSipl
{
	Raw *src;
	Raw *ret;
	int iter;
	double sigmaD;
	double sigmaR;
	BilateralFilterPSipl(Raw *src,Raw *ret,int iter,double sigmaD,double sigmaR)
	{
		this->src = src;
		this->ret = ret;
		this->iter = iter;
		this->sigmaD = sigmaD;
		this->sigmaR = sigmaR;
	}
	BilateralFilterPSipl()
	{
		//this->src=NULL;
		this->sigmaD = 0;
		this->sigmaR = 0;
	}
};

struct GuassFilterP
{
	Raw *src;
	int halfsize;
	GuassFilterP(Raw *src,int halfsize)
	{
		this->src=src;
		this->halfsize=halfsize;
	}
	GuassFilterP()
	{

	}
};

struct GuassFilterPSipl
{
	Raw *src;
	Raw *ret;
	int halfsize;
	int iter;
	GuassFilterPSipl(Raw *src,Raw *ret,int iter,int halfsize)
	{
		this->src = src;
		this->ret = ret;
		this->iter = iter;
		this->halfsize = halfsize;
	}
	GuassFilterPSipl()
	{

	}
};

struct lowPassP
{
	Raw *src;
	float threshold;
	lowPassP(Raw *src, float threshold)
	{
		this->src= src;
		this->threshold =threshold;
	}
	lowPassP()
	{

	}
};
struct MultiOstuP
{

};

void * singleBilateral( void *para)
{
	BilateralFilterP *p= (BilateralFilterP *) para;
	Raw *indata = p->src;
	ThreeDim_Bilateral  *bila=new ThreeDim_Bilateral(indata,p->sigmaD,p->sigmaR);
	bila->apply(*indata);
	return NULL;

}
void * singleBilateralSipl( void *para)
{
	BilateralFilterPSipl *p= (BilateralFilterPSipl *) para;
	Raw *indata = p->src;
	Raw *outdata = p->ret;
	ThreeDim_Bilateral  *bila=new ThreeDim_Bilateral(indata,*outdata,p->sigmaD,p->sigmaR,progress);
	bila->applySipl(p->iter);
	return NULL;

}
void * singleGuassFilter(void * para)
{
	GuassFilterP *p=(GuassFilterP*) para;
	Raw *indata = p->src;
	Filter *guass=new Filter();
	guass->guass3DFilter(indata,p->halfsize);
	return NULL;
}
void * singleGuassFilterSipl(void * para)
{
	GuassFilterPSipl *p=(GuassFilterPSipl*) para;
	Raw *indata = p->src;
	Raw *outdata = p->ret;
	Filter *guass=new Filter();
	guass->guass3DFilterSipl(indata,outdata,p->iter,p->halfsize/2,progress);
	return NULL;
}
void * singleFastGuassFilterSipl(void * para)
{
	GuassFilterPSipl *p=(GuassFilterPSipl*) para;
	Raw *indata = p->src;
	Raw *outdata = p->ret;
	//Filter *guass=new Filter();
	//guass->guass3DFilterSipl(indata,outdata,p->iter,p->halfsize/2,progress);
	FastGauss *test=new FastGauss();
	//outdata=
	test->fg(indata,outdata);
	return NULL;
}
void * singleTrilateralfilter(void *para)
{
	TrilateralfilterP *p=(TrilateralfilterP*) para; 
	Raw *indata=p->src;
	Trilateralfilter f(indata);
	//f.TrilateralFilter(p->sigmaC);

	return &indata;
}
void * singleTrilateralfilterSipl(void *para)
{
	TrilateralfilterPSipl *p=(TrilateralfilterPSipl*) para; 
	Raw *indata = p->src;
	Raw *outdata = p->ret;
	Trilateralfilter f(indata,outdata,p->iter,progress);
	f.TrilateralFilter(*indata,*outdata,p->sigmaC);

	return NULL;
}
void * singleAnistropicFilter(void * para)
{
	AnistropicP *p=(AnistropicP*) para; 
	Raw *indata=(p->src);
	WipeNioisePde * pde=new WipeNioisePde(*indata,p->time,p->val,p->method);
	delete pde;
	return NULL;
}

void * singleAnistropicFilterSipl(void * para)
{
	AnistropicPsipl *p=(AnistropicPsipl*) para; 
	Raw *indata=(p->src);
	Raw *outdata=(p->ret);
	//WipeNioisePde *pde =new WipeNioisePde();
		//pde->ProgressChanged = progress;
	WipeNioisePde *pde=new WipeNioisePde(indata,outdata,p->iter,p->time,p->val,p->method,progress);//two pointers,one in,one out
	
	//delete pde;
	return NULL;
}
//bool single(Raw *src, Raw *ret, int size,int position)
//{
//		PIXTYPE *data=ret->getdata() + position;
//		Raw * d=new Raw(src->getXsize(),src->getYsize(),size,data,true);
//		raw.push_back(d);
//		int ret;
//		TrilateralfilterI *p=(TrilateralfilterI*)para;
//		parms[i]=TrilateralfilterPSipl(src,raw[i],p->sigmaC,i);
//		pthread_attr_t *attr;
//		ret=pthread_create(&threads[i],NULL,singleTrilateralfilterSipl,&parms[i]);
//		cout <<i<<endl;
//
//}
Raw & MultiThread(int method,int threadcount,Raw &src,void *para)
{
	//divide into slices
	//create+join
	//single
	Raw  *ret = new Raw(src);
	int countvar=0;
	if (threadcount>=1)
	{
		vector <Raw *> raw;
		std::vector<pthread_t>threads;threads.resize(threadcount);
		std::vector<int>pids;pids.resize(threadcount);
		std::vector<int> res;res.resize(threadcount);
		switch (method)
		{
		case 1 :
			{
				
					
				vector<TrilateralfilterP>parms;parms.resize(threadcount);
				int znewsize = src.getZsize()/threadcount;
				for (int i = 0; i < threadcount; i++ )
				{
					
					PIXTYPE *data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
					Raw * d=new Raw(src.getXsize(),src.getYsize(),znewsize,data,true);
					raw.push_back(d);
					int ret;
					TrilateralfilterI *p=(TrilateralfilterI*)para;
					parms[i]=TrilateralfilterP(raw[i],p->sigmaC);
					pthread_attr_t *attr;
					ret=pthread_create(&threads[i],NULL,singleTrilateralfilter,&parms[i]);
					cout <<i<<endl;
				}
				for(int i=0;i<threadcount;i++)
				{
					pthread_join(threads[i], NULL);
				}
				
			}
			
			break;
		case 2:
			{
				
					
					vector<AnistropicP> parms; 
					parms.resize(threadcount);
					PIXTYPE *data;
					int znewsize = src.getZsize()/threadcount;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						Raw * d= new Raw(src.getXsize(),src.getYsize(),znewsize,data,true);
						raw.push_back(d);
						int pret;
						AnistropicI *p = (AnistropicI*)para;
						parms[i] = AnistropicP(raw[i],p->time,p->val,p->method);
						pthread_attr_t *attr;
						pret=pthread_create(&threads[i],NULL,singleAnistropicFilter,(void *)&parms[i]);
						cout <<i<<endl;
			
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}
			}
				break;
				
		case 3:
			{
				
					vector<BilateralFilterP>parms;parms.resize(threadcount);
					int znewsize = src.getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						raw.push_back(new Raw(src.getXsize(),src.getYsize(),znewsize,data,true));
						int retid;
						BilateralFilterI *p=(BilateralFilterI*)para;
						parms[i]=BilateralFilterP(raw[i],p->sigmaD,p->sigmaR);
						pthread_attr_t *attr;
						retid=pthread_create(&threads[i],NULL,singleBilateral,&parms[i]);

						cout <<i<<endl;
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}
			}
				
				break;
			
		case 4:
			{
				
					vector<GuassFilterP>parms;parms.resize(threadcount);
					int znewsize = src.getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						raw.push_back(new Raw(src.getXsize(),src.getYsize(),znewsize,data,true));
						int ret;
						GuassFilterI *p=(GuassFilterI*)para;
						parms[i]=GuassFilterP(raw[i],p->halfsize);
						pthread_attr_t *attr;
						ret=pthread_create(&threads[i],NULL,singleGuassFilter,&parms[i]);
						cout <<i<<endl;
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}

			}
				break;
			//}
		}//switch... 
		//ret=new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),out);
			//ret = ret1;
	}//if...
	else
	{
		cout << "threadcount is too few"<<endl;
	}
	for (int i = 0; i < src.size(); i++ )
	{
		if (src.getXYZ(i)!= ret->getXYZ(i))
		{
			countvar ++;
		}
		
	}
	
	cout << countvar <<endl;
	return *ret;

	/*
	1\trilaterfilter
	2\anistropic
	3\bilateralfilter
	4\guass

	*/
}
void  MultiThreadsipl(int method,int threadcount,Raw &src,void *para)
{
	//divide into slices
	//create+join
	//single
	Raw  *ret = new Raw(src);
	int countvar=0;
	if (threadcount>=1)
	{
		vector <Raw *> raw;
		std::vector<pthread_t>threads;threads.resize(threadcount);
		std::vector<int>pids;pids.resize(threadcount);
		std::vector<int> res;res.resize(threadcount);
		switch (method)
		{
		case 1 :
			{
				vector<TrilateralfilterPSipl>parms;parms.resize(threadcount);
				int znewsize = src.getZsize()/threadcount;

				for (int i = 0; i < threadcount; i++ )
				{
					
					PIXTYPE *data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
					Raw * d=new Raw(src.getXsize(),src.getYsize(),znewsize,data,true);
					raw.push_back(d);
					int ret;
					TrilateralfilterI *p=(TrilateralfilterI*)para;
					parms[i]=TrilateralfilterPSipl(&src,raw[i],p->sigmaC,i);
					pthread_attr_t *attr;
					ret=pthread_create(&threads[i],NULL,singleTrilateralfilterSipl,&parms[i]);
					cout <<i<<endl;
				}
				for(int i=0;i<threadcount;i++)
				{
					pthread_join(threads[i], NULL);
				}
				

			}
			break;
		case 2:
			{
					
					vector<AnistropicPsipl> parms; 
					parms.resize(threadcount);
					PIXTYPE *data;
					int znewsize = src.getZsize()/threadcount;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						Raw * d= new Raw(src.getXsize(),src.getYsize(),znewsize,data,true);
						raw.push_back(d);
						int pret;
						AnistropicI *p = (AnistropicI*)para;
						parms[i] = AnistropicPsipl(&src,raw[i],i,p->time,p->val,p->method);
						pthread_attr_t *attr;
						pret=pthread_create(&threads[i],NULL,singleAnistropicFilterSipl,(void *)&parms[i]);
						cout <<i<<endl;
			
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}
				}
				break;
			//}
		case 3:
			{
				
					vector<BilateralFilterPSipl>parms;parms.resize(threadcount);
					int znewsize = src.getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						raw.push_back(new Raw(src.getXsize(),src.getYsize(),znewsize,data,true));
						int retid;
						BilateralFilterI *p=(BilateralFilterI*)para;
						parms[i]=BilateralFilterPSipl(&src,raw[i],i,p->sigmaD,p->sigmaR);
						pthread_attr_t *attr;
						retid=pthread_create(&threads[i],NULL,singleBilateralSipl,&parms[i]);

						std::cout <<i<<endl;
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}

				}
				break;
			//}
		case 4:
			{
				
					vector<GuassFilterPSipl>parms;parms.resize(threadcount);
					int znewsize = src.getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						raw.push_back(new Raw(src.getXsize(),src.getYsize(),znewsize,data,true));
						int ret;
						GuassFilterI *p=(GuassFilterI*)para;
						parms[i]=GuassFilterPSipl(&src,raw[i],i,p->halfsize);
						pthread_attr_t *attr;
						ret=pthread_create(&threads[i],NULL,singleGuassFilterSipl,&parms[i]);
						std::cout <<i<<endl;
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}

				}
				break;
		
		}//switch... 
		//ret=new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),out);
			//ret = ret1;
	}//if...
	else
	{
		std::cout << "threadcount is too few"<<endl;
	}
	for (int i = 0; i < src.size(); i++ )
	{
		if (src.getXYZ(i)!= ret->getXYZ(i))
		{
			countvar ++;
		}

	}
	src = *ret;
	std::cout << countvar <<endl;
	delete  ret;

	/*
	1\trilaterfilter
	2\anistropic
	3\bilateralfilter
	4\guass

	*/
}
void  MultiThreadptr(int method,int datatype,int threadcount,Raw *src,Raw *ret,void *para)
{
	//divide into slices
	//create+join
	//single
	int datasize = 1;
	if(datatype == 1)
	{
		datasize = 1;
	}
	else if(datatype == 2)
	{
		datasize = 2;
	}
	else
	{
		datasize = 4;
	}
	memcpy(ret->getdata(),src->getdata(),src->size()*datasize);
	int countvar=0;
	if (threadcount >= 1)
	{
		vector <Raw *> raw;
		std::vector<pthread_t>threads;threads.resize(threadcount);
		std::vector<int>pids;pids.resize(threadcount);
		std::vector<int> res;res.resize(threadcount);
		int znewsize = src->getZsize()/threadcount;
		int zleft = src->getZsize()%threadcount;
		switch (method)
		{
		case 1 :
			{
				
				vector<TrilateralfilterPSipl>parms;
				parms.resize(threadcount);
				for (int i = 0; i < threadcount; i++ )
				{
					if(i < threadcount-1 || zleft == 0)
					{
						PIXTYPE *data=ret->getdata() + znewsize * i * (src->getXsize() * src->getYsize());
						Raw * d=new Raw(src->getXsize(),src->getYsize(),znewsize,data,true);
						raw.push_back(d);
						int ret;
						TrilateralfilterI *p=(TrilateralfilterI*)para;
						parms[i]=TrilateralfilterPSipl(src,raw[i],p->sigmaC,i);
						pthread_attr_t *attr;
						ret=pthread_create(&threads[i],NULL,singleTrilateralfilterSipl,&parms[i]);
						std::cout <<i<<endl;
					}
					else if(zleft != 0 && i == threadcount-1)
					{
						PIXTYPE *data = ret->getdata() + znewsize * i * (src->getXsize() * src->getYsize());
						Raw * d = new Raw(src->getXsize(),src->getYsize(),zleft+znewsize,data,true);
						raw.push_back(d);
						int ret;
						TrilateralfilterI *p=(TrilateralfilterI*)para;
						parms[i]=TrilateralfilterPSipl(src,raw[i],p->sigmaC,i);
						pthread_attr_t *attr;
						ret=pthread_create(&threads[i],NULL,singleTrilateralfilterSipl,&parms[i]);
						std::cout <<i<<endl;

					}
				}//for
				for(int i = 0;i < threadcount;i++)
				{
					pthread_join(threads[i], NULL);
				}




			}//case 1
			break;
		case 2:
			{
					vector<AnistropicPsipl> parms; 
					parms.resize(threadcount+1);
					PIXTYPE *data;
					int znewsize = src->getZsize()/threadcount;
					for (int i = 0; i < threadcount; i++ )
					{
						if(i < threadcount-1 || zleft == 0)
						{
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							Raw * d= new Raw(src->getXsize(),src->getYsize(),znewsize,data,true);
							raw.push_back(d);
							int pret;
							AnistropicI *p = (AnistropicI*)para;
							parms[i] = AnistropicPsipl(src,raw[i],i,p->time,p->val,p->method);
							pthread_attr_t *attr;
							pret=pthread_create(&threads[i],NULL,singleAnistropicFilterSipl,(void *)&parms[i]);
							std::cout <<i<<endl;
						}
						else if(zleft != 0 && i == threadcount-1)
						{
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							Raw * d= new Raw(src->getXsize(),src->getYsize(),zleft+znewsize,data,true);
							raw.push_back(d);
							int pret;
							AnistropicI *p = (AnistropicI*)para;
							parms[i] = AnistropicPsipl(src,raw[i],i,p->time,p->val,p->method);
							pthread_attr_t *attr;
							pret=pthread_create(&threads[i],NULL,singleAnistropicFilterSipl,(void *)&parms[i]);
							std::cout <<i<<endl;

						}
					}

					for(int i = 0;i < threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}
			}

					break;
		
		case 3:
			{
					vector<BilateralFilterPSipl>parms;
					parms.resize(threadcount+1);
					int znewsize = src->getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						if(i < threadcount-1 || zleft == 0)
						{
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							raw.push_back(new Raw(src->getXsize(),src->getYsize(),znewsize,data,true));
							int retid;
							BilateralFilterI *p=(BilateralFilterI*)para;
							parms[i]=BilateralFilterPSipl(src,raw[i],i,p->sigmaD,p->sigmaR);
							pthread_attr_t *attr;
							retid=pthread_create(&threads[i],NULL,singleBilateralSipl,&parms[i]);

							std::cout <<i<<endl;
						}
						else if(zleft != 0 && i == threadcount-1)
						{
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							raw.push_back(new Raw(src->getXsize(),src->getYsize(),zleft+znewsize,data,true));
							int retid;
							BilateralFilterI *p=(BilateralFilterI*)para;
							parms[i]=BilateralFilterPSipl(src,raw[i],i,p->sigmaD,p->sigmaR);
							pthread_attr_t *attr;
							retid=pthread_create(&threads[i],NULL,singleBilateralSipl,&parms[i]);

							std::cout <<i<<endl;

						}
					}//for..

					for(int i = 0;i <=threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}

				//}

				for(int i = 0;i < threadcount;i++)
				{
					pthread_join(threads[i], NULL);
				}
		
			}//case 3
			break;
			
		case 4:
			{
				
					vector<GuassFilterPSipl>parms;parms.resize(threadcount+1);
					int znewsize = src->getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						if(i < threadcount-1 || zleft == 0)
						{
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							raw.push_back(new Raw(src->getXsize(),src->getYsize(),znewsize,data,true));
							int ret;
							GuassFilterI *p=(GuassFilterI*)para;
							parms[i]=GuassFilterPSipl( src,raw[i],i,p->halfsize);
							pthread_attr_t *attr;
							ret=pthread_create(&threads[i],NULL,singleFastGuassFilterSipl,&parms[i]);
							cout <<i<<endl;
						}
						else if(zleft != 0 && i == threadcount-1)
						{	
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							raw.push_back(new Raw(src->getXsize(),src->getYsize(),zleft+znewsize,data,true));
							int ret;
							GuassFilterI *p=(GuassFilterI*)para;
							parms[i]=GuassFilterPSipl( src,raw[i],i,p->halfsize);
							pthread_attr_t *attr;
							ret=pthread_create(&threads[i],NULL,singleFastGuassFilterSipl,&parms[i]);
							cout <<i<<endl;
						}
					}//..for

				for(int i = 0;i < threadcount;i++)
				{
					pthread_join(threads[i], NULL);
				}


			}//case 4
			break;
		
		}//switch... 

	}//if...
	else
	{
		cout << "threadcount is too few"<<endl;
	}
	for (int i = 0; i < src->size(); i++ )
	{
		if (src->getXYZ(i)!= ret->getXYZ(i))
		{
			countvar ++;
		}

	}


	cout << countvar <<endl;
	//delete  ret;

	/*
	1\trilaterfilter
	2\anistropic
	3\bilateralfilter
	4\guass

	*/
}
void  MultiThreadsY(int method,int threadcount,Raw &src,void *para)
{
	//divide into slices
	//create+join
	//single
	PIXTYPE * data =new PIXTYPE[src.size()];
	for (int i = 0; i < src.getYsize(); i++)
	{
		for (int j = 0; j < src.getZsize(); j++)
		{
			for (int k = 0; k < src.getXsize(); k++)
			{
				data[k + j*src.getXsize() + i*src.getXsize()*src.getZsize()] = src.get(k,i,j); 
				//data++;
			}
		}
	}

	Raw  *ret = new Raw(src.getXsize(),src.getZsize(),src.getYsize(),data,true);
	src = Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),data);
	int countvar=0;
	if (threadcount>=1)
	{
		vector <Raw *> raw;
		std::vector<pthread_t>threads;threads.resize(threadcount);
		std::vector<int>pids;pids.resize(threadcount);
		std::vector<int> res;res.resize(threadcount);
		switch (method)
		{
		case 1 :
			{
				vector<TrilateralfilterPSipl>parms;parms.resize(threadcount);
				int znewsize = src.getZsize()/threadcount;

				for (int i = 0; i < threadcount; i++ )
				{
					
					PIXTYPE *data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
					Raw * d=new Raw(ret->getXsize(),ret->getYsize(),znewsize,data,true);
					raw.push_back(d);
					int ret;
					TrilateralfilterI *p=(TrilateralfilterI*)para;
					parms[i]=TrilateralfilterPSipl(&src,raw[i],p->sigmaC,i);
					pthread_attr_t *attr;
					ret=pthread_create(&threads[i],NULL,singleTrilateralfilterSipl,&parms[i]);
					cout <<i<<endl;
				}
				for(int i=0;i<threadcount;i++)
				{
					pthread_join(threads[i], NULL);
				}
				

			}
			break;
		case 2:
			{
				{
					
					vector<AnistropicPsipl> parms; 
					parms.resize(threadcount);
					PIXTYPE *data;
					int znewsize = src.getZsize()/threadcount;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata() + znewsize*i*src.getXsize()*src.getYsize();
						Raw * d= new Raw(src.getXsize(),src.getYsize(),znewsize,data,true);
						raw.push_back(d);
						int pret;
						AnistropicI *p = (AnistropicI*)para;
						parms[i] = AnistropicPsipl(&src,raw[i],i,p->time,p->val,p->method);
						pthread_attr_t *attr;
						pret=pthread_create(&threads[i],NULL,singleAnistropicFilterSipl,(void *)&parms[i]);
						cout <<i<<endl;
			
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}
				}
				break;
			}
		case 3:
			{
				{
					vector<BilateralFilterPSipl>parms;parms.resize(threadcount);
					int znewsize = src.getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						raw.push_back(new Raw(src.getXsize(),src.getYsize(),znewsize,data,true));
						int retid;
						BilateralFilterI *p=(BilateralFilterI*)para;
						parms[i]=BilateralFilterPSipl(&src,raw[i],i,p->sigmaD,p->sigmaR);
						pthread_attr_t *attr;
						retid=pthread_create(&threads[i],NULL,singleBilateralSipl,&parms[i]);

						cout <<i<<endl;
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}

				}
				break;
			}
		case 4:
			{
				{
					vector<GuassFilterPSipl>parms;parms.resize(threadcount);
					int znewsize = src.getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						raw.push_back(new Raw(src.getXsize(),src.getYsize(),znewsize,data,true));
						int ret;
						GuassFilterI *p=(GuassFilterI*)para;
						parms[i]=GuassFilterPSipl(&src,raw[i],i,p->halfsize);
						pthread_attr_t *attr;
						ret=pthread_create(&threads[i],NULL,singleGuassFilterSipl,&parms[i]);
						cout <<i<<endl;
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}

				}
				break;
			}
		}//switch... 
		//ret=new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),out);
			//ret = ret1;
	}//if...
	else
	{
		cout << "threadcount is too few"<<endl;
	}

	for (int i = 0; i < src.size(); i++ )
	{
		if (src.getXYZ(i)!= ret->getXYZ(i))
		{
			countvar ++;
		}

	}
	//src = *ret;
	PIXTYPE *outdata =new PIXTYPE[src.size()];
	src = Raw(ret->getXsize(),ret->getZsize(),src.getYsize(),outdata,true);
	for ( int i =0; i <src.getZsize();i++)
	{
		for ( int j = 0; j < src.getYsize(); j ++)
		{
			for (int k = 0; k < src.getXsize(); k++)
			{
				src.put( k, j, i, ret->get(k, i, j) );
			}
		}
	}
	cout << countvar <<endl;
	delete  ret;

	/*
	1\trilaterfilter
	2\anistropic
	3\bilateralfilter
	4\guass

	*/
}
void  MultiThreadsYptr(int method,int datatype,int threadcount,Raw *src,Raw *res,void *para)
{
	//divide into slices
	//create+join
	//single
	int datasize = 1;
	if(datatype == 1)
	{
		datasize = 1;
	}
	else if(datatype == 2)
	{
		datasize = 2;
	}
	else
	{
		datasize = 4;
	}

	PIXTYPE * datatp = res->getdata();
	for (int i = 0; i < src->getYsize(); i++)
	{
		for (int j = 0; j < src->getZsize(); j++)
		{
			for (int k = 0; k < src->getXsize(); k++)
			{
				datatp[k + j*src->getXsize() + i*src->getXsize()*src->getZsize()] =
					src->get(k,i,j); 
				//data++;
			}
		}
	}

	Raw  *ret = new Raw(src->getXsize(),src->getZsize(),src->getYsize(),datatp,true);
	*src = Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),datatp);
	//memcpy(res->getdata() ,data, src->size()*datasize);
	//res = ret;
	/*int datasize = 1;
	if(datatype == 1)
	{
		datasize = 1;
	}
	else if(datatype == 2)
	{
		datasize = 2;
	}
	else
	{
		datasize = 4;
	}*/
	//memcpy(ret->getdata(),src->getdata(),src->size()*datasize);
	int countvar=0;
	if (threadcount >= 1)
	{
		vector <Raw *> raw;
		std::vector<pthread_t>threads;threads.resize(threadcount);
		std::vector<int>pids;pids.resize(threadcount);
		std::vector<int> res;res.resize(threadcount);
		int znewsize = src->getZsize()/threadcount;
		int zleft = src->getZsize()%threadcount;
		switch (method)
		{
		case 1 :
			{
				
				vector<TrilateralfilterPSipl>parms;
				parms.resize(threadcount);
				for (int i = 0; i < threadcount; i++ )
				{
					if(i < threadcount-1 || zleft == 0)
					{
						PIXTYPE *data=ret->getdata() + znewsize * i * (src->getXsize() * src->getYsize());
						Raw * d=new Raw(src->getXsize(),src->getYsize(),znewsize,data,true);
						raw.push_back(d);
						int ret;
						TrilateralfilterI *p=(TrilateralfilterI*)para;
						parms[i]=TrilateralfilterPSipl(src,raw[i],p->sigmaC,i);
						pthread_attr_t *attr;
						ret=pthread_create(&threads[i],NULL,singleTrilateralfilterSipl,&parms[i]);
						std::cout <<i<<endl;
					}
					else if(zleft != 0 && i == threadcount-1)
					{
						PIXTYPE *data = ret->getdata() + znewsize * i * (src->getXsize() * src->getYsize());
						Raw * d = new Raw(src->getXsize(),src->getYsize(),zleft+znewsize,data,true);
						raw.push_back(d);
						int ret;
						TrilateralfilterI *p=(TrilateralfilterI*)para;
						parms[i]=TrilateralfilterPSipl(src,raw[i],p->sigmaC,i);
						pthread_attr_t *attr;
						ret=pthread_create(&threads[i],NULL,singleTrilateralfilterSipl,&parms[i]);
						std::cout <<i<<endl;

					}
				}//for
				for(int i = 0;i < threadcount;i++)
				{
					pthread_join(threads[i], NULL);
				}




			}//case 1
			break;
		case 2:
			{
					vector<AnistropicPsipl> parms; 
					parms.resize(threadcount+1);
					PIXTYPE *data;
					int znewsize = src->getZsize()/threadcount;
					for (int i = 0; i < threadcount; i++ )
					{
						if(i < threadcount-1 || zleft == 0)
						{
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							Raw * d= new Raw(src->getXsize(),src->getYsize(),znewsize,data,true);
							raw.push_back(d);
							int pret;
							AnistropicI *p = (AnistropicI*)para;
							parms[i] = AnistropicPsipl(src,raw[i],i,p->time,p->val,p->method);
							pthread_attr_t *attr;
							pret=pthread_create(&threads[i],NULL,singleAnistropicFilterSipl,(void *)&parms[i]);
							std::cout <<i<<endl;
						}
						else if(zleft != 0 && i == threadcount-1)
						{
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							Raw * d= new Raw(src->getXsize(),src->getYsize(),zleft+znewsize,data,true);
							raw.push_back(d);
							int pret;
							AnistropicI *p = (AnistropicI*)para;
							parms[i] = AnistropicPsipl(src,raw[i],i,p->time,p->val,p->method);
							pthread_attr_t *attr;
							pret=pthread_create(&threads[i],NULL,singleAnistropicFilterSipl,(void *)&parms[i]);
							std::cout <<i<<endl;

						}
					}

					for(int i = 0;i < threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}
			}

					break;
		
		case 3:
			{
					vector<BilateralFilterPSipl>parms;
					parms.resize(threadcount+1);
					int znewsize = src->getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						if(i < threadcount-1 || zleft == 0)
						{
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							raw.push_back(new Raw(src->getXsize(),src->getYsize(),znewsize,data,true));
							int retid;
							BilateralFilterI *p=(BilateralFilterI*)para;
							parms[i]=BilateralFilterPSipl(src,raw[i],i,p->sigmaD,p->sigmaR);
							pthread_attr_t *attr;
							retid=pthread_create(&threads[i],NULL,singleBilateralSipl,&parms[i]);

							std::cout <<i<<endl;
						}
						else if(zleft != 0 && i == threadcount-1)
						{
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							raw.push_back(new Raw(src->getXsize(),src->getYsize(),zleft+znewsize,data,true));
							int retid;
							BilateralFilterI *p=(BilateralFilterI*)para;
							parms[i]=BilateralFilterPSipl(src,raw[i],i,p->sigmaD,p->sigmaR);
							pthread_attr_t *attr;
							retid=pthread_create(&threads[i],NULL,singleBilateralSipl,&parms[i]);

							std::cout <<i<<endl;

						}
					}//for..

					for(int i = 0;i <=threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}

				//}

				for(int i = 0;i < threadcount;i++)
				{
					pthread_join(threads[i], NULL);
				}
		
			}//case 3
			break;
			
		case 4:
			{
				
					vector<GuassFilterPSipl>parms;parms.resize(threadcount+1);
					int znewsize = src->getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						if(i < threadcount-1 || zleft == 0)
						{
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							raw.push_back(new Raw(src->getXsize(),src->getYsize(),znewsize,data,true));
							int ret;
							GuassFilterI *p=(GuassFilterI*)para;
							parms[i]=GuassFilterPSipl( src,raw[i],i,p->halfsize);
							pthread_attr_t *attr;
							ret=pthread_create(&threads[i],NULL,singleGuassFilterSipl,&parms[i]);
							cout <<i<<endl;
						}
						else if(zleft != 0 && i == threadcount-1)
						{	
							data=ret->getdata()+znewsize*i*src->getXsize()*src->getYsize();
							raw.push_back(new Raw(src->getXsize(),src->getYsize(),zleft+znewsize,data,true));
							int ret;
							GuassFilterI *p=(GuassFilterI*)para;
							parms[i]=GuassFilterPSipl( src,raw[i],i,p->halfsize);
							pthread_attr_t *attr;
							ret=pthread_create(&threads[i],NULL,singleGuassFilterSipl,&parms[i]);
							cout <<i<<endl;
						}
					}//..for

				for(int i = 0;i < threadcount;i++)
				{
					pthread_join(threads[i], NULL);
				}


			}//case 4
			break;
		
		}//switch... 

	}//if...
	else
	{
		cout << "threadcount is too few"<<endl;
	}
	for (int i = 0; i < src->size(); i++ )
	{
		if (src->getXYZ(i)!= ret->getXYZ(i))
		{
			countvar ++;
		}

	}

	//change data order
	//src = new Raw(*ret);
	PIXTYPE *outdata =new PIXTYPE [src->size()];
	//*src = Raw(ret->getXsize(),ret->getZsize(),src->getYsize());
	
	for ( int i = 0; i < src->getYsize(); i++)
	{
		for ( int j = 0; j < src->getZsize(); j ++)
		{
			for (int k = 0; k < src->getXsize(); k++)
			{
				//src->put( k, j, i, ret->get(k, i, j) );
				outdata[k + j*src->getXsize() + i*src->getXsize()*src->getZsize()] = ret->get(k, i, j);
			}
		}
	}
	cout << countvar <<endl;
	*res = Raw (src->getXsize(),src->getZsize(),src->getYsize(),outdata);
	//memcpy(res->getdata(),outdata,src->size()*datasize);
	//delete  ret;

	/*
	1\trilaterfilter
	2\anistropic
	3\bilateralfilter
	4\guass

	*/
}