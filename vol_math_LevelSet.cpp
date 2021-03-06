#include "vol_math_LevelSet.h"
//#include "CImg.h" 

#include <iostream>
#define pi 3.141592653
#include<math.h>
#include<stdio.h>
#include <string.h>

//using namespace cimg_library;
using namespace std;
PIXTYPE *p;
PIXTYPE	*q;
int i,j;

LevelSet::~LevelSet(void)
{
    cout<<"Destructor called"<<endl; 
}

//*ImageF Dirac(ImageF x,float sigma);*/

//qym
//Raw2D cos(Raw2D &x);
//Raw2D* sin(Raw2D *s);

Raw2D cos(Raw2D &xdata)
{

    int m=xdata.getXsize();
    int n=xdata.getYsize();
    Raw2D x=new Raw2D(m,n);
    for (i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
        {
            x.put(i,j,cos(double(xdata.get(i,j))));
        }

    }
    return x;
}

Raw2D Dirac(Raw2D *x,double sigma)
{
    Raw2D ret(x);
    PIXTYPE temp=((1.0/2.0)/sigma);
    double temp2=(cos((2/sigma)*pi)+1)*temp;

    //qym
#if 1
    Raw2D tmp_f = (ret/sigma)*pi;
    Raw2D f= (cos(tmp_f)+1)*temp;
#else
    Raw2D f= (cos((ret/sigma)*pi)+1)*temp;
#endif

    //IShowImg(f);
    //Raw2D *f=new Raw2D(((-1)**ret**ret)/4.0+255*2/3);
    //IShowImg(ret);
    Raw2D b = regFunction(ret, -sigma, sigma);
    //IShowImg(b);
    ret = f*b;
    //IShowImg(ret);

    return ret;
}
/************************************************************************/
/* natural                                                                     */
/************************************************************************/
/*
   Raw2D* gradientx(Raw2D *g)
   {
   int n=g->getXsize();
   int m=g->getYsize();
   Raw2D* ret=new Raw2D(n, m);
   int i,j;

   for(int i=0;i<n;i++)
   {
   for(j=0;j<m;j++)
   {
   if (i-1>=0 && j-1>=0)
   {
   if (g->get(i-1,j)-g->get(i,j)>0)
   {
   ret->put(i,j,(g->get(i-1,j)-g->get(i,j)));
   }
   else if (g->get(i-1,j)-g->get(i,j)<0)
   {
   ret->put(i,j,(g->get(i-1,j)-g->get(i,j)));
   }
   else 
   {
   ret->put(i,j,0);
   }
   }
   else 
   ret->put(i,j,0);
   }
   }
   return ret;
   }*/

/*
   Raw2D* gradientxgorignal(Raw2D *g)
   {
   int n=g->getXsize();
   int m=g->getYsize();
   Raw2D* ret=new Raw2D(g);
   int i,j;
   int temp1,temp2;

   for(int i=0;i<n;i++)
   {
   for(j=0;j<m;j++)
   {

   if(j>0)
   temp1=j-1;
   else 
   temp1=0;
   / *if(j<m-1)
   temp2=j+1;
   else 
   temp2=m-1;* /
   ret->put(i,j,(g->get(i,temp1+1)-g->get(i,temp1)));
   }
   }
   return ret;
   }*/

/************************************************************************/
/* central gradient                                                                     */
/************************************************************************/
Raw2D* gradientxgc(Raw2D *g)
{
    int n=g->getXsize();
    int m=g->getYsize();
    Raw2D* ret=new Raw2D(n,m);
    int i,j;
    int temp1,temp2;

    for(int i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            if(j>0)
                temp1=j-1;
            else 
                temp1=0;
            if(j<m-1)
                temp2=j+1;
            else 
                temp2=m-1;
            ret->put(i,j,(g->get(i,temp2)-g->get(i,temp1))/2.0);
        }
    }

    return ret;
}
/************************************************************************/
/* symmetry                                                                     */
/************************************************************************/
/*
   Raw2D* gradientxg(Raw2D *g)
   {
   int n=g->getXsize();
   int m=g->getYsize();
   Raw2D* ret=new Raw2D(g);
   int i,j;

   for(int i=0;i<n;i++)
   {
   for(j=0;j<m;j++)
   {

   if (i+1<n&&j+1<m)
   {

   if (g->get(i+1,j)-g->get(i,j)>0)
   {
   ret->put(i,j,(g->get(i+1,j)-g->get(i,j)));
   }
   else if (g->get(i+1,j)-g->get(i,j)<0)
   {
   ret->put(i+1,j,(-1*g->get(i+1,j)+g->get(i,j)));
   }
//else ret->put(i,j,0);
}
}
}

return ret;
}*/

/*
   Raw2D* gradienty(Raw2D* g )
   {
   int n=g->getXsize();
   int m=g->getYsize();
   Raw2D* ret=new Raw2D(n, m);
   int i,j;
//PIXTYPE *gg=(*g).gety();
//PIXTYPE *p=(*ret).gety();
for(i=0;i<n;i++)
{
for(j=0;j<m;j++)
{
if (i+1<n&&j+1<m)
{
if (g->get(i,j+1)-g->get(i,j)>0)
{
ret->put(i,j,(g->get(i,j+1)-g->get(i,j)));
}
else if (g->get(i,j+1)-g->get(i,j)<0)
{
ret->put(i,j,(g->get(i,j+1)-g->get(i,j)));
}
else ret->put(i,j,0);
}
else ret->put(i,j,0);
}
}
return ret;
}*/

/*
   Raw2D* gradientyg(Raw2D* g )
   {
   int n=g->getXsize();
   int m=g->getYsize();
   Raw2D* ret=new Raw2D(n, m);
   int i,j;
   for(i=0;i<n;i++)
   {
   for(j=0;j<m;j++)
   {
   if (i+1<n&&j+1<m)
   {
   if (g->get(i,j+1)-g->get(i,j)>0)
   {
   ret->put(i,j,(g->get(i,j+1)-g->get(i,j)));
   }
   else if (g->get(i,j+1)-g->get(i,j)<0)
   {
   ret->put(i,j+1,(-1*g->get(i,j+1)+g->get(i,j)));
   }
//else ret->put(i,j,0);
}

}
}
return ret;

}*/


Raw2D* gradientygc(Raw2D *g)
{
    int n=g->getXsize();
    int m=g->getYsize();
    Raw2D* ret=new Raw2D(n, m);
    int i,j;
    int temp1,temp2;

    for(int i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {

            if(i>0)
                temp1=i-1;
            else 
                temp1=0;
            if(i<n-1)
                temp2=i+1;
            else 
                temp2=n-1;
            ret->put(i,j,(g->get(temp2,j) - g->get(temp1,j))/2.0);
        }
    }

    return ret;
}
/*
   Raw2D* gradientygcorignal(Raw2D *g)
   {
   int n=g->getXsize();
   int m=g->getYsize();
   Raw2D* ret=new Raw2D(n, m);
   int i,j;
   int temp1,temp2;

   for(int i=0;i<n;i++)
   {
   for(j=0;j<m;j++)
   {

   if(i>0)
   temp1=i-1;
   else 
   temp1=0;
//if(i<n-1)
//	temp2=i+1;
//else 
//	temp2=n-1;
int val=(g->get(temp1+1,j)-g->get(temp1,j));
if (val<0)
{
cout<<val<<endl;
}
ret->put(i,j,val);
}
}
return ret;
}*/

void NeumannBoundCond(Raw2D *img)
{
    int nrow=img->getXsize();
    int ncol=img->getYsize();
    int i,j;
    //the four point SDF
    img->putXY(0,img->getXY(2*ncol+2));
    img->putXY(ncol-1,img->get(2,ncol-3));
    img->putXY((nrow-1)*ncol,img->getXY((nrow-3)*ncol+2));
    img->putXY(nrow*ncol-1,img->get(nrow-3,ncol-3));
    //first and the last column SDF
    for(i=2;i<nrow-2;i++)
    {
        img->putXY((i-1)*ncol,img->getXY((i-1)*ncol+2));
        img->putXY(ncol*(i-1)+ncol-1,img->getXY(ncol*(i-1)-3));

    }
    //first and last row SDF
    for(j=2;j<ncol-2;j++)
    {
        img->putXY(j,img->getXY(2*ncol+j));
        img->putXY(ncol*(nrow-2)+j,img->getXY(ncol*(nrow-4)+j));
    }
    for (i=0;i<nrow;i++)
    {
        for(j=0;j<ncol;j++)
        {
            img->put(i,j,img->get(i,j));
        }
    }
}

Raw2D ImageFSqrt(Raw2D &x,Raw2D &y)
{
    Raw2D  ret = new Raw2D (x);
    PIXTYPE val=0;
    int m=x.getXsize();
    int n=x.getYsize();
    for (i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
        {
            val=(double)sqrt(double(x.get(i,j)*x.get(i,j) + y.get(i,j)*y.get(i,j)));
            ret.put(i,j,val);
        }
    }
    return ret;
}

//curvature
Raw2D div(Raw2D &x, Raw2D &y)
{
    Raw2D ret(x.getXsize(),x.getYsize());
    Raw2D *gradx = gradientxgc(&x),
          *grady = gradientygc(&y);
    ret = (*gradx + *grady);

    delete gradx;
    delete grady;
    return ret;
}


Raw2D* del2(Raw2D *phi)
{
    int m=phi->getXsize();
    int n=phi->getYsize();
    Raw2D *ret2=new Raw2D(m,n);

    for (int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {

            if (i+1<m && j+1<n && i-1>=0 && j-1>=0)
            {
                PIXTYPE value = 0.25*(phi->get(i+1, j) + phi->get(i-1, j) + phi->get(i, j-1) + phi->get(i, j+1) - 4*(phi->get(i,j)));
                //if(value!=0)
                //	cout<<value<<endl;
                ret2->put(i, j, (PIXTYPE)value);
            }
            else 
            {
                ret2->put(i, j,0);
            }
        }
    }
    return ret2;
}

Raw2D regFunction(Raw2D &s,double m,double n)
{
    int l = s.getXsize(),
        k = s.getYsize();
    Raw2D ss(l,k);
    PIXTYPE val=0;
    for (i=0;i<l;i++)
    {
        for(j=0;j<k;j++)
        {
            val=s.get(i,j);
            if(val>=m && val<=n)
            {
                //ss->put(i,j,255);//unsigned char version
                ss.put(i,j,1);
            }
            else if(s.get(i,j)==m||s.get(i,j)==n) 
            {
                ss.put(i,j,0);
            }
            else 
            {
                ss.put(i,j,0);
            }
        }
    }

    return ss;
}

Raw2D* distReg_p2(Raw2D *phi)
{
    Raw2D *ret=new Raw2D(phi);
    int m=phi->getXsize();
    int n=phi->getYsize();
    Raw2D *phi_x=new Raw2D(gradientxgc(phi));
    Raw2D *phi_y=new Raw2D(gradientygc(phi));
    //ImageF s=ImageFSqrt(((*phi_x)*(*phi_x)) + ((*phi_y)*(*phi_y)));
    Raw2D s(ImageFSqrt(*phi_x,*phi_y));

    //qym for lvalue
#if 0
    Raw2D &a=regFunction(s,0,1);
    Raw2D &b=regFunction(s,1,10);//need to be changed.
    Raw2D &ps=a*(*sin(&(s*2.0*pi)))/(2.0*pi)+b*(s-1);
    //IShowImg(ps);
    Raw2D &dps=((regFunction(ps,0,0)*(-1)+1)*ps+regFunction(ps,0,0))/((regFunction(s,0,0)*(-1)+1)*s+regFunction(s,0,0));
    /*IShowImg(dps);*/
    Raw2D &div2=div(dps**phi_x-*phi_x,dps**phi_y-*phi_y);
    //IShowImg(div2);
    Raw2D &f=div2-*del2(phi)*4.0;
#else
    Raw2D a=regFunction(s,0,1);
    Raw2D b=regFunction(s,1,10);//need to be changed.
    Raw2D ps=a*(*sin(&(s*2.0*pi)))/(2.0*pi)+b*(s-1);
    Raw2D dps=((regFunction(ps,0,0)*(-1)+1)*ps+regFunction(ps,0,0))/((regFunction(s,0,0)*(-1)+1)*s+regFunction(s,0,0));

    Raw2D tmp_a = dps**phi_x-*phi_x;
    Raw2D tmp_b = dps**phi_y-*phi_y;
    Raw2D div2=div(tmp_a,tmp_b);
    Raw2D f=div2-*del2(phi)*4.0;
#endif
    //IShowImg(f);
    *ret=f;
    return ret;
}

Raw2D* sin(Raw2D *s)
{
    //overload sin
    int m=s->getXsize();
    int n=s->getYsize();
    Raw2D *ss=new Raw2D(m,n);
    for (i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
        {
            ss->put(i,j,sin(double(s->get(i,j))));
        }

    }
    return ss;
}

void write(Raw2D &destImg)
{
    //FILE *p=fopen("F:\\ls1.raw","ab+");
    //	fwrite(destImg.gety(),sizeof(PIXTYPE),destImg.getXsize()*destImg.getYsize(),p);
    //fclose(p);
}

void LevelSet::initialg(Raw2D *g)
{
    Raw2D *gx=gradientxgc(g);
    Raw2D *gy=gradientygc(g);
    //g=*ret;
    *g = (*gx)* (*gx)+(*gy)*(*gy);
    //  add( multi(gx,gx), multi(gy, gy))
    //IShowImg(g);
    //g=(255/(g+1));//or  unsigned char version
    //g=-255/(g+1)+255;unsigned char version
    //g=-1*g+255; unsigned char version
    *g = (*g)+1.0;
    *g = 1.0/(*g);
    //IShowImg(g);

    delete gx;
    delete gy;
}
//
//ImageF & pow(PIXTYPE e,ImageF &x)
//{
//	int m=x.getXsize();
//	int n=x.getYsize();
//	ImageF * ret=new ImageF(x);
//	for (int i=0;i<m;i++)
//	{
//		for (int j=0;j<n;j++)
//		{
//			ret->put(i,j,pow(e,x.get(i,j)));
//
//		}
//	}
//	return *ret;
//}
/// \brief
void LevelSet::drlse_edge(Raw2D *phi,Raw2D *g,float lambda,float mu,float alfa,float epsilon,int timestep, int iter,const char * potentialFunction)
{
    //IShowImg(*phi);
    int m=g->getXsize();
    int n=g->getYsize();
    //IShowImg(g);
    Raw2D *vx=gradientxgc(g);
    Raw2D *vy=gradientygc(g);
    *vx=(*vx);
    *vy=(*vy);
    //IShowImg(*vx);
    //IShowImg(*vy);
    Raw2D *diracPhi;
    Raw2D *areaTerm;
    Raw2D *edgeTerm;
    Raw2D *distRegTerm = new Raw2D(m,n);
    //CImg <double> sourceimage(phi->getXsize(),phi->getYsize(),1,1,0);
    //CImgDisplay disp(256,256,"",1);
    for(int i=0;i<iter;i++)
    {
        NeumannBoundCond(phi);
        Raw2D *phi_x = gradientxgc(phi);
        Raw2D *phi_y = gradientygc(phi);
        Raw2D s = ImageFSqrt(*phi_x, *phi_y);
        float smallNumber=1e-10;
        Raw2D *Nx = new Raw2D(*phi_x/(s + smallNumber));
        Raw2D *Ny = new Raw2D(*phi_y/(s + smallNumber));
        Raw2D *curvature = new Raw2D(div(*Nx,*Ny));

        char *p1="single_well";
        if (0 == strcmp(potentialFunction, p1))
        {
            /*
               compute distance regularization term in equation (13) 
               with the single-well potential p1.
             */
            *distRegTerm= ((*del2(phi))*4.0 - (*curvature));
        }
        else if (0 == strcmp(potentialFunction, "double_well"))
        {
            *distRegTerm=distReg_p2(phi);  // compute the distance regularization term in eqaution (13) with the double-well potential p2.
        }
        else printf("EEROR");
        diracPhi=new Raw2D(Dirac(phi,epsilon));
        areaTerm=new Raw2D(( (*g) *(*diracPhi))); 
        edgeTerm=new Raw2D(m,n);
        Raw2D edge1=(*diracPhi) * ((*vx) * (*Nx)+(*vy) * (*Ny));
        Raw2D edge2=(*diracPhi) * ( (*g) * (*curvature));
        //IShowImg(*g);
        //IShowImg(edge2);
        *edgeTerm =edge1+edge2;
        *phi=*phi +((*distRegTerm)*mu* double(timestep) +(*edgeTerm)*lambda + (*areaTerm)*alfa);
        //IShowImg(*distRegTerm);
        //IShowImg(*edgeTerm);
        //IShowImg(*areaTerm);
        //IShowImg(*vx);
        //IShowImg(*vx);
        ////IShowImg(*phi);

        /*IShowImg(*curvature);*/
        //IShowImg(*phi_x);
        //IShowImg(*phi_y);
        //IShowImg(*Nx);
        //IShowImg(*Ny);
        //IShowImg(*g);
        //IShowImg(s);

        //phi_0=*phi;
        //IShowImg(*phi);
        /*	
            cimg_for_insideXY(sourceimage,x,y,0)
            {
            PIXTYPE val=phi->get(x,y);
            if (val>0&&val<1)
            {
            sourceimage(x,y,0)=(double)(val);
            }
            else if (val>=1)
            {
            sourceimage(x,y,0)=(double)(1);
            }

            }
            sourceimage.display(disp.wait(20));
            delete phi_x;
            delete phi_y;
         */
    }	
        //IShowImg(*diracPhi);
        //IShowImg(*edgeTerm);
        //IShowImg(*areaTerm);
        //initialg(phi);
        //IShowImg(*phi);
        //IShowImg(*distRegTerm);
        delete vx;
        delete vy;

}
//ImageF& LevelSet::uchar2double(ImageF &img)
//{
//	int m=img.getXsize();
//	int n=img.getYsize();
//	for (int i=0;i<m;i++)
//	{
//		for (int j=0;j<n;j++)
//		{
//			img.put(i,j,double(img.get(i,j)));
//		}
//	}
//}
/*
void LevelSet::testout(Raw2D *ret)
{

    int i=0;

    while (i<100)
    {
        //PIXTYPE test = p[i];
        i++;
        cout<<"no====>>:"<<i<<"p[i]="<<ret->getXY(i)<<endl;
    }
}
*/
