
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef FUZZY_DEF
#define FUZZY_DEF
#ifndef READONLY
#define FUZZY_DATA_LENGTH 40
#else
#define FUZZY_DATA_LENGTH
#endif
#define FUZZY_NAME_LENGTH 40

//#define USE_INT	// not as accurate but will work on simple processors
#ifdef USE_INT
#define VALUE_TYPE int
#define VALUE_SCALE	1000	// if VALUE_TYPE is float then this can be 1
#else
#define VALUE_TYPE double
#endif
#ifdef __cplusplus
namespace Crystal   {
using namespace std;
namespace Fuzzy	{
extern "C" {
#endif
struct VC
{
  VALUE_TYPE Value;
  VALUE_TYPE Certainty;
};
struct TFuzzyXY
{
    VALUE_TYPE x;
    VALUE_TYPE y;
};


//class TFuzzy
struct TFuzzy
{
int DataLength;
struct TFuzzyXY Data[FUZZY_DATA_LENGTH];
};

void TFuzzyInit(struct TFuzzy *data);
int TFuzzyAdd(struct TFuzzy *data,VALUE_TYPE X,VALUE_TYPE Y);	// returns the index of the TFuzzyXY added
void TFuzzyAddFuzzy(struct TFuzzy *data,struct TFuzzyXY f);
void TFuzzyAddFuzzyp(struct TFuzzy *data,const struct TFuzzyXY *f);
void TFuzzySetValueAt(struct TFuzzy *data,int i,VALUE_TYPE x,VALUE_TYPE y);
const struct TFuzzyXY *GetItemc(const struct TFuzzy *data,int i);
struct TFuzzyXY *GetItem(struct TFuzzy *data,int i);
const char *TFuzzyGetName(const struct TFuzzy *data);
void TFuzzySetName(struct TFuzzy *data,const char *name);
int TFuzzySetValueXY(struct TFuzzy *data,VALUE_TYPE x,VALUE_TYPE y);
void DeleteItem(struct TFuzzy *data,int i);
/// A Fuzzy Logic Implimenation
int TFuzzyAddPoint(struct TFuzzy *data,VALUE_TYPE x,VALUE_TYPE y);

    VALUE_TYPE ValueAt(const struct TFuzzy *data,unsigned int idx);
    VALUE_TYPE IndexAt(const struct TFuzzy *data,unsigned int idx );
    VALUE_TYPE Value(const struct TFuzzy *Data,VALUE_TYPE InValue);
    VALUE_TYPE LowestRange(const struct TFuzzy *data);
    VALUE_TYPE HighestRange(const struct TFuzzy *data);
    void SwapXY(struct TFuzzy *data);
    // show how equal two fuzzy sets are
    VALUE_TYPE Equality(const struct TFuzzy *data,struct TFuzzy *compare);
    // will attempt to delete datapoints which would normally fall on the calcuated path by factor amount
    void Optimize(struct TFuzzy *data,VALUE_TYPE factor);
    // will increase the number of samples, by adding points between current points, but still along the data path
    void IncreaseSamples(const struct TFuzzy *data);
    VALUE_TYPE InhibitValue(const struct TFuzzy *data,VALUE_TYPE In,VALUE_TYPE InhibitPercent);
    void SimpleInhibit(const struct TFuzzy *data,VALUE_TYPE InhibitPercent);

    void CopyAndInhibit(struct TFuzzy *data,const struct TFuzzy *Fuzzy);
    // helper routine for CopyAndInhibit
    void CopyAndInhibitTemp(struct TFuzzy *data,const struct TFuzzy *Fuzzy,struct TFuzzy *Temp);

    // restrict the range to within Floor and Ceil
    void Range(struct TFuzzy *data,VALUE_TYPE Floor,VALUE_TYPE Ceil);

    // returns the higher of two results
    VALUE_TYPE Or(const struct TFuzzy *data,const struct TFuzzy *f, VALUE_TYPE In);
    /*
     This is to be used when more then 2 data's must be or'd,
        eg.
        OrResult(Fuzzy1,ValueIn,Or(Fuzzy2,Fuzzy3,ValueIn);
     */
    VALUE_TYPE OrResult(const struct TFuzzy *data,VALUE_TYPE In,VALUE_TYPE CurrentResult);
    // returns the lower of two results
    VALUE_TYPE And(const struct TFuzzy *data,const struct TFuzzy *f, VALUE_TYPE In);
    // used when more then two fuzzys must be anded (similar to OrResult)
    VALUE_TYPE AndResult(const struct TFuzzy *data,VALUE_TYPE In,VALUE_TYPE CurrentResult);
    // returns the differnce between two results
    VALUE_TYPE XOr(const struct TFuzzy *data,const struct TFuzzy *f, VALUE_TYPE In);
    // returns the TrueValue - the result, TrueValue is whatever value in the Fuzzy that would represent true (1.0 if classical fuzzy logic)
    VALUE_TYPE Not(const struct TFuzzy *data,VALUE_TYPE In,VALUE_TYPE TrueValue);

#ifdef VALIDATING
virtual bool Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

#ifdef __cplusplus
}	//extern "C"
}	//namespace Fuzzy
using namespace Fuzzy;

class ClassFuzzy
{
TFuzzy f;
public:
    ClassFuzzy() { TFuzzyInit(&f); }
int TFuzzyAddPoint(VALUE_TYPE x,VALUE_TYPE y)
{
    return Crystal::TFuzzyAddPoint(&f,x,y);
}

int TFuzzyAdd(VALUE_TYPE X,VALUE_TYPE Y)	// returns the index of the TFuzzyXY added
{
    return Crystal::TFuzzyAdd(&f,X,Y);
}
void TFuzzyAddFuzzy(const struct TFuzzyXY &xy)
{
    return Crystal::TFuzzyAddFuzzy(&f,xy);
}

void TFuzzyAddFuzzyp(const struct TFuzzyXY *F)
{
    Crystal::TFuzzyAddFuzzyp(&f,F);
}
void TFuzzySetValueAt(int i,VALUE_TYPE x,VALUE_TYPE y)
{
    Crystal::TFuzzySetValueAt(&f,i,x,y);
}
const struct TFuzzyXY *GetItemc(int i) const
{
    return Crystal::GetItemc(&f,i);
}
struct TFuzzyXY *GetItem(int i)
{
    return Crystal::GetItem(&f,i);
}
const char *TFuzzyGetName() const
{
    return Crystal::TFuzzyGetName(&f);
}
void TFuzzySetName(const char *name)
{
    return Crystal::TFuzzySetName(&f,name);
}
int Count() const
{
    return f.DataLength;
}
void Clear()
{
    f.DataLength = 0;
}
int TFuzzySetValueXY(VALUE_TYPE x,VALUE_TYPE y)
{
    return Crystal::TFuzzySetValueXY(&f,x,y);
}
void DeleteItem(int i)
{
    return Crystal::DeleteItem(&f,i);
}
/// A Fuzzy Logic Implimenation

VALUE_TYPE ValueAt(unsigned int idx) const
{
    return Crystal::ValueAt(&f,idx);
}
VALUE_TYPE IndexAt(unsigned int idx ) const
{
    return Crystal::IndexAt(&f,idx );
}
VALUE_TYPE Value(VALUE_TYPE InValue) const
{
    return Crystal::Value(&f,InValue);
}
VALUE_TYPE LowestRange() const
{
    return Crystal::LowestRange(&f);
}
VALUE_TYPE HighestRange() const
{
    return Crystal::HighestRange(&f);
}
void SwapXY()
{
    return Crystal::SwapXY(&f);
}
VALUE_TYPE Equality(struct TFuzzy *compare) const
{
    return Crystal::Equality(&f,compare);
}
void Optimize(VALUE_TYPE factor)
{
    Crystal::Optimize(&f,factor);
}
void IncreaseSamples()
{
    Crystal::IncreaseSamples(&f);
}
int Length()const  { return f.DataLength; }
VALUE_TYPE InhibitValue(VALUE_TYPE In,VALUE_TYPE InhibitPercent)
{
    return Crystal::InhibitValue(&f,In,InhibitPercent);
}
void SimpleInhibit(VALUE_TYPE InhibitPercent)
{
    Crystal::SimpleInhibit(&f,InhibitPercent);
}
void CopyAndInhibit(const struct TFuzzy *Fuzzy)
{
    Crystal::CopyAndInhibit(&f,Fuzzy);
}
void CopyAndInhibitTemp(const struct TFuzzy *Fuzzy,struct TFuzzy *Temp)
{
    Crystal::CopyAndInhibitTemp(&f,Fuzzy,Temp);
}

    // restrict the range to within Floor and Ceil
void Range(VALUE_TYPE Floor,VALUE_TYPE Ceil)
{
    return Crystal::Range(&f,Floor,Ceil);
}


VALUE_TYPE Or(const ClassFuzzy *F, VALUE_TYPE v) const
{
    return Crystal::Or(&f,&F->f, v);
}
VALUE_TYPE And(const ClassFuzzy *F, VALUE_TYPE v) const
{
    return Crystal::And(&f,&F->f, v);
}
VALUE_TYPE XOr(const ClassFuzzy *F, VALUE_TYPE v) const
{
    return Crystal::XOr(&f,&F->f, v);
}
VALUE_TYPE Not(VALUE_TYPE v,VALUE_TYPE TrueValue) const
{
    return Crystal::Not(&f,v,TrueValue);
}
};
}	//namespace Crystal
#endif


#endif //FUZZY_DEF
