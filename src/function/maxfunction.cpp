#include "maxfunction.h"
#include "numbervalue.h"
#include "vectorvalue.h"
#include "rangevalue.h"
#include "onceonly.h"
#include "rmath.h"

MaxFunction::MaxFunction() : Function("max")
{
	addParameter("values");
}

static decimal maximum(QList<Value*> values,bool& ok)
{
	decimal v=0.0;
	OnceOnly first;
	foreach(Value* a,values) {
		NumberValue* nextVal=dynamic_cast<NumberValue*>(a);
		if(nextVal) {
			if(first())
				v=nextVal->getNumber();
			else
				v=r_max(v,nextVal->getNumber());

			ok=true;
			continue;
		}
		VectorValue* vecVal=dynamic_cast<VectorValue*>(a);
		if(vecVal) {
			v=r_max(v,maximum(vecVal->getChildren(),ok));
			continue;
		}
		RangeValue* rngVal=dynamic_cast<RangeValue*>(a);
		if(rngVal) {
			QList<Value*> rng;
			rng.append(rngVal->getStart());
			rng.append(rngVal->getFinish());
			v=r_max(v,maximum(rng,ok));
		}
	}
	return v;
}

Value* MaxFunction::evaluate(Context* ctx)
{
	QList<Value*> values=ctx->getArguments();

	bool ok=false;
	decimal v=maximum(values,ok);
	if(!ok)
		return new Value();

	return new NumberValue(v);
}
