#include "sinfunction.h"
#include "numbervalue.h"
#include "rmath.h"

SinFunction::SinFunction() : Function("sin")
{
	addParameter("value");
}

Value* SinFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		decimal num=numVal->getNumber();

		return new NumberValue(r_sin_deg(num));
	}
	return new Value();
}
