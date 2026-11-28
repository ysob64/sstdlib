/*
  ___ ___ _____ ___  _    ___ ___ 
 / __/ __|_   _|   \| |  |_ _| _ )
 \__ \__ \ | | | |) | |__ | || _ \
 |___/___/ |_| |___/|____|___|___/ 

by ysob64
Under Unlicense

*/


// The one rule in this file is that every function starts with either the first letter of the type (if it is explicit enough)
// or the first letters of said type, where the type chosen is the one where we will make operations on it.

// For example, if I convert a string to an int, the original type that I got the data from (and so on operating on that data)
// is a string, so s_ToInt would be the name of that function.

#include <stdio.h>
#include <stdlib.h>

#ifndef SSTDLIB_NO_MATH
	
#define EULER_NUMBER 2.718281828459045235360287471352
#define PI 3.1415926535897932384626433

#define LN_10 2.302585092994045684017991454684
#define LN_2 0.693147180559945309417232121458

typedef struct FractionData
{
	int numerator;
	int denominator;
}fraction;

//shortcut structure for simplifying power calcs.
// for example, instead of calculating 2^4 * 2^3 individually, we could simply add up
// the powers giving only one functions call for 2^7;
// note that of course, its only useful for the case where we do know the bases.
typedef struct powerfData
{
	float pow;
	float base;
}powf;

typedef struct powerdData
{
	double pow;
	double base;
}powd;

typedef struct poweriData
{
	int pow;
	int base;
}powi;

#endif

// A v_memcopy copy for avoiding the inclusion of string.h
void *v_memcopy(void *dest, const void *src, int len)
{
  if(dest == NULL || src == NULL) return NULL;
  if(sizeof(dest[0]) != sizeof(src[0])) return NULL;

  switch(sizeof(dest[0]))
  {
  	case sizeof(int):
  		int *di = dest;
			const int *si = src;
			while (len--)
			  *di++ = *si++;
			return di;
		
		case sizeof(long):
  		long *dd = dest;
			const long *sd = src;
			while (len--)
			  *dd++ = *sd++;
			return dd;
		
		case sizeof(char):
  		char *dc = dest;
			const char *sc = src;
			while (len--)
			  *dc++ = *sc++;
			return dc;
  }
  
}

#ifndef SSTDLIB_NO_STRING

typedef struct StringData
{
	char *value;
	int length; //how many characters
	int size; // buffer size
	bool isInitialized;
} string;

void s_Free(string *src)
{
	free(src->value);
	src->isInitialized=false;
	src->size=0;
	src->length=0;
}

int s_len(const string src)
{
	if(src.value==NULL || src.isInitialized==false)
	{
		return -1;
	}

	int count=0;
	while(src.value[count] != '\0')
	{
		count++;
	}

	return count;
}

int charArray_len(const char *src)
{
	int count=0;
	while(src[count] != '\0')
	{
		count++;
	}

	return count;
}

int s_Init(string *src, char *str)
{
	const int len = charArray_len(str);

	if((src->value=(char *)malloc(len*sizeof(char))) == NULL)
	{
		return 1;
	}

	v_memcopy(src->value,str,len);
	src->isInitialized=true;
	src->size=len+1;
	src->length=len;

	
	return 0;
}

int s_set(string *src,string str)
{
	if(str.length >= src->size)
	{
		if((src->value=realloc(src->value,(str.length+1)*sizeof(char))) == NULL)
		{
			return 1;
		}

		src->size=str.length+1;
	}

	v_memcopy(src->value,str.value,str.length);
	src->length=s_len(*src);
	return 0;
}

void s_Print(const string src, const char end)
{
	if(!src.isInitialized)
	{
		return;
	}

	printf("%s%c",src.value,end);
}

void s_Println(const string src)
{
	if(!src.isInitialized)
	{
		return;
	}

	printf("%s\n",src.value);
}

int s_add(string *src,const string str)
{
	if(!src->isInitialized || !str.isInitialized)
	{
		return 1;
	}

	const int freeSpace=src->size-src->length-1;
	if(freeSpace < (src->length)+str.length)
	{
		char *cp=(char *)malloc(src->size);
		v_memcopy(cp,src->value,src->size);

		if((src->value=realloc(src->value,((src->length)+str.length+1)*sizeof(char))) == NULL)
		{
			return 1;
		}

		src->size=(src->length)+str.length+1;
		src->length=(src->length)+str.length;

		int count=0;
		while(cp[count]!='\0')
		{
			src->value[count]=cp[count];
			count++;
		}

		const int offset=count;
		while(str.value[count-offset]!='\0')
		{
			src->value[count]=str.value[count-offset];
			count++;
		}

		src->value[count]='\0';
		return 0;
	}
	else
	{
		for(int x=0;x<=str.length+1;x++)
		{
			src->value[src->length+x]=str.value[x];
		}

		src->length=s_len(*src);

		return 0;
	}
}

//backward : if false, clear from right to left, else from left to right
// ClearChar : With what character should the string be cleared with ?
int s_Clear(string *src,const int steps,const bool backward,const char ClearChar)
{
	if(!src->isInitialized)
	{
		return 1;
	}

	if(steps==0)
	{
		return 0;
	}

	if(!backward)
	{
		for(int x=0;x<=steps;x++)
		{
			src->value[x]=ClearChar;
		}
	}
	else
	{
		for(int x=src->length;x>=((src->length)-steps);x--)
		{
			src->value[x]=ClearChar;
		}
	}

	src->length=s_len(*src);
	return 0;
}

bool s_Equal(const string src,const string str)
{
	if(!src.isInitialized || !str.isInitialized)
	{
		return 0;
	}

	int count=0;
	while(src.value[count]==str.value[count] && src.value[count] != '\0' && str.value[count] != '\0')
	{
		count++;
	}
	count--;

	return src.length==count;
}

//If the first string src start with the second string str, (steps) with how many character to include from the beginning
bool s_StartWith(const string src,const string str, int steps)
{
	if(!src.isInitialized || !str.isInitialized)
	{
		return 0;
	}

	if(steps==0)
	{
		steps=str.length;
	}

	int count=0;
	while(src.value[count]==str.value[count] && count<steps)
	{
		count++;
	}

	return steps==count;
}

//If the first string src end with the second string str, (steps) with how many character to include from the end
bool s_EndWith(const string src, const string str, int steps)
{
	if(!src.isInitialized || !str.isInitialized)
	{
		return 0;
	}

	if(steps==0)
	{
		steps=str.length;
	}

	int count=src.length;
	int count2=str.length;
	while(count2>str.length-steps && src.value[count]==str.value[count2])
	{
		count--;
		count2--;
	}

	return count2==str.length-steps;
}

//return an array that contains every number inside a string
int *s_ToInts(const string src)
{
	if(!src.isInitialized)
	{
		return NULL;
	}

	int *result=malloc(sizeof(int));
	int ncount=0;
	int pow=1;
	bool isInSentence = (src.value[src.length] < '0' || src.value[src.length] > '9') ? true : false;

	for(int x=src.length;x>=0;x--)
	{
		if(src.value[x] < '0' || src.value[x] > '9')
		{
			if(!isInSentence)
			{
				if(src.value[x] == '-')
				{
					result[ncount]*=-1;
				}

				ncount++;
				result=realloc(result,ncount*sizeof(int));
				pow=1;
				isInSentence=true;
			}
			continue;
		}
		else
		{
			result[ncount] += (src.value[x]-'0')*pow;
			pow*=10;
			isInSentence=false;
		}
		
	}

	return result;
}

//Convert a single string number to an int
int s_ToInt(const string src)
{
	if(!src.isInitialized)
	{
		return 1;
	}

	int result=0;
	int pow=1;
	bool isNegative= src.value[0]=='-' ? true : false;

	for(int x=src.length;x>=0;x--)
	{
		if(x==0 && isNegative)
		{
			result*=-1;
			break;
		}

		result += (src.value[x]-'0')*pow;
		pow*=10;
	}

	return result;
}

//Convert a single string number to a float
float s_ToFloat(const string src)
{
	if(!src.isInitialized)
	{
		return 1;
	}

	float result=0.0f;
	float pow=1.0f;
	bool isNegative= src.value[0]=='-' ? true : false;
	bool pastDot=false;

	int npowExp=1;
	const float CorrectedApprox=0.00001f; //By adding it, we fix the case of approximation to 0.9999... At the cost of possibly making other a bit more approximated
											//But its still better to do it because it is in most case

	for(int x=src.length;x>=0;x--)
	{
		if(src.value[x] == '.')
		{
			pastDot=true;
			continue;
		}

		if(!pastDot)
		{
			npowExp*=10;
		}

		if(x==0 && isNegative)
		{
			result=-result;
			break;
		}

		result += (src.value[x]-'0')*pow; //We actually "convert" from float int here (so 1.21 is 121) to avoid even more floating imprecision
		pow*=10;
	}

	return (result/npowExp)+CorrectedApprox;
}

//Convert a single string number to a double
double s_ToDouble(const string src)
{
	if(!src.isInitialized)
	{
		return 1;
	}

	double result=0.0;
	double pow=1.0;
	bool isNegative= src.value[0]=='-' ? true : false;
	bool pastDot=false;

	for(int x=src.length;x>=0;x--)
	{
		if(src.value[x] == '.')
		{
			break;
		}

		pow/=10.0;
	}

	for(int x=src.length;x>=0;x--)
	{
		if(src.value[x] == '.')
		{
			continue;
		}

		if(x==0 && isNegative)
		{
			result*=-1;
			break;
		}

		result += (src.value[x]-'0')*pow;
		pow*=10;
	}

	return result;
}

#endif

#ifndef SSTDLIB_NO_INTEGER

#ifdef SSTDLIB_NO_STRING

char *i_ToStr(const int src)
{
	char *result;
	float cp=src;
	int len=0;
	bool isNegative= src<0 ? true : false;
	while(((int)cp)!=0)
	{
		cp/=10;
		len++;
	}

	if((result=malloc((sizeof(char)*(len+isNegative)))) == NULL)
	{
		return NULL;
	}
	cp=src;
	const int lenght=len;
	len=0; //re-use for counter

	if(isNegative)
	{
		cp=-cp;
	}

	while(((int)cp)!=0)
	{
		result[lenght-len-(!isNegative)]=(((int)cp)%10+'0');
		cp/=10;
		len++;
	}

	if(isNegative)result[0]='-';

	return result;
}

#else

string i_ToStr(const int src)
{
	char *result; //For optimization reason, it is much better to use the char *, as we set each array element individually, where my string type isn't made for this.
	float cp=src;
	int len=0;
	bool isNegative = src<0 ? true : false;
	while(((int)cp)!=0)
	{
		cp/=10;
		len++;
	}

	if((result=malloc((sizeof(char)*(len+isNegative)))) == NULL)
	{
		string error;
		return error;
	}

	cp=src;
	const int lenght=len;
	len=0; //re-use for counter

	if(isNegative)
	{
		cp=-cp;
	}

	while(((int)cp)!=0)
	{
		result[lenght-len-(!isNegative)]=(((int)cp)%10+'0');
		cp/=10;
		len++;
	}

	if(isNegative) result[0]='-';

	string strres;
	s_Init(&strres,result);
	return strres;
}

#endif

#ifndef SSTDLIB_NO_MATH

int i_abs(int src)
{
	return src<0 ? -src : src;
}

int i_pow(int src,int pow)
{
	pow=i_abs(pow);

	switch(pow)
	{
		default:
			break;
		case 0:
			return 1;
		case 1:
			return src;
	}

	for(int x=0;x<pow-1;x++)
	{
		src*=src;
	}

	return src;
}

//Factorial of src
// No decimal version is planned
int i_fact(int src)
{
	switch(src)
	{
		default:
			break;
		case 0:
			return 1;
		case 1:
			return 1;
		case 2:
			return 2;
	}

	for(int x=src-1;x>1;x--)
	{
		src*=x;
	}

	return src;
}

#endif

#endif

#ifndef SSTDLIB_NO_FLOAT

float f_DecimalPart(float src)
{
	return src-((int)src);
}

#ifdef SSTDLIB_NO_STRING

char *f_ToStr(const float src)
{
	bool isNegative= src<0 ? true : false;
	char *result;
	float cp=src;
	int len=0;
	int decLen=0;

	while(((int)cp)!=0)
	{
		cp/=10;
		len++;
	}

	cp=src;

	while(cp-((int)cp)!=0)
	{
		cp*=10;
		decLen++;
	}

	const int lenght=len+decLen+1;

	if((result=malloc((sizeof(char)*(lenght+isNegative)))) == NULL)
	{
		return NULL;
	}

	int count=0;

	if(isNegative)
	{
		cp=-cp;
	}

	while(((int)cp)!=0)
	{
		result[lenght-count-(!isNegative)]=(((int)cp)%10+'0');
		count++;
		if(count==decLen)
		{
			result[lenght-count-(!isNegative)]='.';
			count++;
		}
		cp/=10;
	}

	if(isNegative)result[0]='-';

	return result;
}

#else

string f_ToStr(const float src)
{
	char *result;
	float cp=src;
	int len=0;
	int decLen=0;
	bool isNegative= src<0 ? true : false;

	while(((int)cp)!=0)
	{
		cp/=10;
		len++;
	}

	cp=src;

	while(cp-((int)cp)!=0)
	{
		cp*=10;
		decLen++;
	}

	if(isNegative)
	{
		cp=-cp;
	}

	const int lenght=len+decLen+1;

	if((result=malloc((sizeof(char)*(lenght+isNegative)))) == NULL)
	{
		string error;
		return error;
	}

	int count=0;

	while(((int)cp)!=0)
	{
		result[lenght-count-(!isNegative)]=(((int)cp)%10+'0');
		count++;
		if(count==decLen)
		{
			result[lenght-count-(!isNegative)]='.';
			count++;
		}
		cp/=10;
	}

	if(isNegative)result[0]='-';

	string strres;
	s_Init(&strres,result);

	return strres;
}

#endif


#ifndef SSTDLIB_NO_MATH

float f_abs(const float src)
{
	return src<0 ? -src : src;
}

float f_powi(float src,int pow)
{
	pow=i_abs(pow);

	switch(pow)
	{
		default:
			break;
		case 0:
			return 1;
		case 1:
			return src;
	}

	bool n=pow<0?true:false;

	if(src == 0.0f)
	{
		return 0.0f;
	}
	else if(src == 1.0f)
	{
		return 1.0f;
	}

	for(int x=0;x<pow-1;x++)
	{
		src*=src;
	}

	return n?1/src:src;
}

float f_sqrt(const float src)
{
	float guess=src/4;
	float nextGuess=(guess+src/guess)/2;

	if(guess<nextGuess) //where the threshold is 16 in this case
	{
		while(f_abs(nextGuess - guess) > 0.0001)
		{
			guess=((guess+src/guess)/2);
			nextGuess=((guess+src/guess)/2);
		}
	}
	else
	{
		while(guess-nextGuess > 0.0001)
		{
			guess=((guess+src/guess)/2);
			nextGuess=((guess+src/guess)/2);
		}
	}

	return guess;
}

//precision : how far should the nextguess be from the current guess ? (in decimal : ex : 0.00001)
float f_sqrtP(const float src,const float precision)
{
	float guess=src/4;
	float nextGuess=(guess+src/guess)/2;

	if(guess<nextGuess) //where the threshold is 16 in this case
	{
		while(f_abs(nextGuess - guess) > precision)
		{
			guess=((guess+src/guess)/2);
			nextGuess=((guess+src/guess)/2);
		}
	}
	else
	{
		while(guess-nextGuess > precision)
		{
			guess=((guess+src/guess)/2);
			nextGuess=((guess+src/guess)/2);
		}
	}

	return guess;
}

//(copy past from the double version)
//hex number table for square roots :
// 0x1FC00000 : for more accuracy on power of 5 (100% accuracy for 25, then 98,948689959% for 625, ...)
// 0x1FC00000 : (same) for power of 2 (great for floats as returns within its range of max value)
// 0x1FB40000 : (same) power of 3 (looks like great for float too but I wouldn't bet)
// 0x1FBE0000 : (same) power of 7
// I precise POWER an NOT multiple, only with powers of x you will get the closest (if not 100% accurate) square root
float f_fastSqrt(const float src)
{
	unsigned int i = *(int *) &src;

	const int input=(int)src;
	int selector=((input%5==0) << 3); //Now THAT is some really dedicated hate towards if statements
	selector+=(((selector==0)&&input%3==0) << 2);
	selector+=(((selector==0)&&input%2==0) << 1);
	selector+=((selector==0)&&input%7==0);

	switch(selector)
	{
		default:
			i=(i >> 1) + 0x1FC00000;
			break;
		case 2:
			i=(i >> 1) + 0x1FC00000;
			break;
		case 4:
			i=(i >> 1) + 0x1FB40000;
			break;
		case 8:
			i=(i >> 1) + 0x1FC00000; // 5 for float precision is the same as 2 (tested but impossible to get closer than 5.125 for sqrt(25))
			break;
		case 1:
			i=(i >> 1) + 0x1FBE0000;
			break;
	}

	float result = *(float *) &i;

	if(selector==8)
	{
		result-=0.125f;
	}

	result=(result+src/result)/2;
	result=(result+src/result)/2;//second iteration makes it EXTREMELY accurate

	return result;
}


float f_fasterSqrt(const double src)
{
	unsigned int i = *(int *) &src;

	i=(i >> 1) + 0x1FC00000; //we take multiple of 2 as they are alot more of a possibilty

	float result = *(float *) &i;

	result=(result+src/result)/2;

	return result;
}

//end sqrts functions

//Natural logarithm, with step the precision required
float f_ln(const float src, const int step)
{
	if(src <= 1.0)
	{
		return 0.0;
	}

	float result=0.0;
	const float d=(src-1.0)/step;
	for(float x=1;x<=src;x+=d)
	{
		result+=(1/x)*d;
	}

	return result;
}

//base 10 logarithm
float f_log10(const float src, const int step)
{
	if(src <= 1.0)
	{
		return 0.0;
	}

	float result=0.0;
	const float d=(src-1.0)/step;
	for(float x=1;x<=4;x+=d)
	{
		result+=(1/x)*d;
	}

	return result/LN_10;
}

//logarithm of any x base, with step being used for both ln(src) and ln(base)
float f_logx(const float src, const int base, const int step)
{
	if(src <= 1.0)
	{
		return 0.0;
	}

	if(base <= 1.0)
	{
		return 0.0;
	}

	float result=0.0;
	const float d=(src-1.0)/step;
	for(float x=1;x<=4;x+=d)
	{
		result+=(1/x)*d;
	}

	float baseResult=0.0;
	const float bd=(base-1.0)/step;
	for(float x=1;x<=4;x+=bd)
	{
		baseResult+=(1/x)*bd;
	}

	return result/baseResult;
}

//decimal power by convergent sequence, not very precise but good enough, and all iteration > 7 deson't really have any effects
float f_pow(float src,float pow)
{
	if(pow == 0.0)
	{
		return 1.0;
	}
	else if(pow == 1.0)
	{
		return src;
	}

	bool nNum=false;

	if(((int)src)<0)
	{
		src=i_abs(src);
		nNum=true;
	}

	if(src == 0.0)
	{
		return 0.0;
	}
	else if(src == 1.0)
	{
		return 1.0;
	}

	//bool n=pow<0?true:false;
	const float Num = pow*f_ln(src,10000);
	float result=0.0;

	for(int x=0;x<7;x++)
	{
		result+=f_powi(Num,x)/i_fact(x);
	}

	result=nNum?-result:result;
	return result;
}

#endif

#endif

#ifndef SSTDLIB_NO_DOUBLE

#ifdef SSTDLIB_NO_STRING

char *d_ToStr(const double src)
{
	bool isNegative= src<0 ? true : false;
	char *result;
	double cp=src;
	int len=0;
	int decLen=0;

	while(((int)cp)!=0)
	{
		cp/=10;
		len++;
	}

	cp=src;

	while(cp-((int)cp)!=0)
	{
		cp*=10;
		decLen++;
	}

	const int lenght=len+decLen+1;

	if((result=malloc((sizeof(char)*(lenght+isNegative)))) == NULL)
	{
		return NULL;
	}

	int count=0;

	if(isNegative)
	{
		cp=-cp;
	}

	while(((int)cp)!=0)
	{
		result[lenght-count-(!isNegative)]=(((int)cp)%10+'0');
		count++;
		if(count==decLen)
		{
			result[lenght-count-(!isNegative)]='.';
			count++;
		}
		cp/=10;
	}

	if(isNegative)result[0]='-';

	return result;
}

#else

string d_ToStr(const double src)
{
	bool isNegative= src<0 ? true : false;
	char *result;
	double cp=src;
	int len=0;
	int decLen=0;

	while(((int)cp)!=0)
	{
		cp/=10;
		len++;
	}

	cp=src;

	while(cp-((int)cp)!=0)
	{
		cp*=10;
		decLen++;
	}

	const int lenght=len+decLen+1;

	if((result=malloc((sizeof(char)*(lenght+isNegative)))) == NULL)
	{
		string error;
		return error;
	}

	int count=0;

	if(isNegative)
	{
		cp=-cp;
	}

	while(((int)cp)!=0)
	{
		result[lenght-count-(!isNegative)]=(((int)cp)%10+'0');
		count++;
		if(count==decLen)
		{
			result[lenght-count-(!isNegative)]='.';
			count++;
		}
		cp/=10;
	}

	if(isNegative)result[0]='-';

	string strres;
	s_Init(&strres,result);
	return strres;
}

#endif


#ifndef SSTDLIB_NO_MATH

double d_abs(const double src)
{
	return src<0 ? -src : src;
}


//Square roots functions

double d_sqrt(const double src)
{
	double guess=src/4;
	double nextGuess=(guess+src/guess)/2;

	if(guess<nextGuess) //where the threshold is 16 in this case
	{
		while(d_abs(nextGuess - guess) > 0.0001)
		{
			guess=((guess+src/guess)/2);
			nextGuess=((guess+src/guess)/2);
		}
	}
	else
	{
		while(guess-nextGuess > 0.0001)
		{
			guess=((guess+src/guess)/2);
			nextGuess=((guess+src/guess)/2);
		}
	}

	return guess;
}

//precision : how far should the nextguess be from the current guess ? (in decimal : ex : 0.00001)
double d_sqrtP(const double src,const double precision)
{
	double guess=src/4;
	double nextGuess=(guess+src/guess)/2;

	if(guess<nextGuess) //where the threshold is 16 in this case
	{
		while(d_abs(nextGuess - guess) > precision)
		{
			guess=((guess+src/guess)/2);
			nextGuess=((guess+src/guess)/2);
		}
	}
	else
	{
		while(guess-nextGuess > precision)
		{
			guess=((guess+src/guess)/2);
			nextGuess=((guess+src/guess)/2);
		}
	}

	return guess;
}


//hex number table for square roots :
// 0x1FF7800000000000 : for more accuracy on power of 5 (100% accuracy for 25, then 98,948689959% for 625, ...)
// 0x1FF8000000000000 : (same) for power of 2 (great for floats as returns within its range of max value)
// 0x1FF7000000000000 : (same) power of 3 (looks like great for float too but I wouldn't bet)
// 0x1FF7C00000000000 : (same) power of 7
// I precise POWER an NOT multiple, only with powers of x you will get the closest (if not 100% accurate) square root
double d_fastSqrt(const double src)
{
	unsigned long i = *(long *) &src;

	const int input=(int)src;
	int selector=((input%5==0) << 3); //Now THAT is some really dedicated hate towards if statements
	selector+=(((selector==0)&&input%3==0) << 2);
	selector+=(((selector==0)&&input%2==0) << 1);
	selector+=((selector==0)&&input%7==0);

	switch(selector)
	{
		default:
			i=(i >> 1) + 0x1FF8000000000000;
			break;
		case 2:
			i=(i >> 1) + 0x1FF8000000000000;
			break;
		case 4:
			i=(i >> 1) + 0x1FF7000000000000;
			break;
		case 8:
			i=(i >> 1) + 0x1FF7800000000000;
			break;
		case 1:
			i=(i >> 1) + 0x1FF7C00000000000;
			break;
	}

	double result = *(double *) &i;

	result=(result+src/result)/2;
	result=(result+src/result)/2;//second iteration makes it EXTREMELY accurate, it can only be removed for 32 bit float precision

	return result;
}


double d_fasterSqrt(const double src)
{
	unsigned long i = *(long *) &src;

	i=(i >> 1) + 0x1FF8000000000000; //we take multiple of 2 as they are alot more of a possibilty

	double result = *(double *) &i;

	result=(result+src/result)/2;

	return result;
}

//end sqrts functions

//Natural logarithm
double d_ln(const double src, const int step)
{
	if(src <= 1.0)
	{
		return 0.0;
	}

	double result=0.0;
	const double d=(src-1.0)/step;
	for(double x=1;x<=src;x+=d)
	{
		result+=(1/x)*d;
	}

	return result;
}

//base 10 logarithm
double d_log10(const double src, const int step)
{
	if(src <= 1.0)
	{
		return 0.0;
	}

	double result=0.0;
	const double d=(src-1.0)/step;
	for(double x=1;x<=4;x+=d)
	{
		result+=(1/x)*d;
	}

	return result/LN_10;
}

//logarithm of any x base, with step being used for both ln(src) and ln(base)
double d_logx(const double src, const int base, const int step)
{
	if(src <= 1.0)
	{
		return 0.0;
	}

	if(base <= 1.0)
	{
		return 0.0;
	}

	double result=0.0;
	const double d=(src-1.0)/step;
	for(double x=1;x<=4;x+=d)
	{
		result+=(1/x)*d;
	}

	double baseResult=0.0;
	const double bd=(base-1.0)/step;
	for(double x=1;x<=4;x+=bd)
	{
		baseResult+=(1/x)*bd;
	}

	return result/baseResult;
}

double d_powi(double src, int pow)
{
	pow=i_abs(pow);

	switch(pow)
	{
		default:
			break;
		case 0:
			return 1;
		case 1:
			return src;
	}

	bool n=pow<0?true:false;

	if(src == 0.0)
	{
		return 0.0;
	}
	else if(src == 1.0)
	{
		return 1.0;
	}

	for(int x=0;x<pow-1;x++)
	{
		src*=src;
	}

	return n?1/src:src;
}

//decimal power by convergent sequence, not very precise but good enough, and all iteration > 7 deson't really have any effects
double d_pow(double src,double pow)
{
	if(pow == 0.0)
	{
		return 1.0;
	}
	else if(pow == 1.0)
	{
		return src;
	}

	bool nNum=false;

	if(((int)src)<0)
	{
		src=i_abs(src);
		nNum=true;
	}

	if(src == 0.0)
	{
		return 0.0;
	}
	else if(src == 1.0)
	{
		return 1.0;
	}

	//bool n=pow<0?true:false;
	const double Num = pow*d_ln(src,10000);
	double result=0.0;

	for(int x=0;x<7;x++)
	{
		result+=d_powi(Num,x)/i_fact(x);
	}

	result=nNum?-result:result;
	return result;
}

#endif

#endif
