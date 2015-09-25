#include "tanhfunction.h"
#include "numbervalue.h"
#include "rmath.h"

TanhFunction::TanhFunction() : Function("tanh")
{
	addParameter("value");
}

Value* TanhFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_tanh_deg(num));
	}
	return new Value();
}

