/*
 * DoubleParametersArray.h
 *
 *  Created on: Mar 25, 2012
 *      Author: feanor
 */

#ifndef DOUBLEPARAMETERSARRAY_H_
#define DOUBLEPARAMETERSARRAY_H_

#include "DiscreteParameters.h"
#include "DoubleParameter.h"
#include "CompositeDiscreteParameters.h"
#include "ConstChromosomeSizeParemeters.h"


class DoubleParametersArray : public DiscreteParameters
{
public:
	DoubleParametersArray(double minValue, double maxValue, double desiredPrecision, size_t numberOfParameters);

	DoubleParametersArray* clone() const;

	bool checkConstraints() const;

	size_t getChromosomeSize() const;

	void setChromosome(vector<bool>::iterator from, vector<bool>::iterator to) const;

	void setFromChromosome(vector<bool>::const_iterator from, vector<bool>::const_iterator to);

    double getMaxValue() const;
    double getMinValue() const;
    size_t getNumberOfParameters() const;
    double getActualPrecision() const;
    vector<double> getValues() const;

protected:
    const DoubleParameter* getDoubleParameter(size_t index) const;

private:
	double _minValue;
	double _maxValue;
	double _desiredPrecision;

	CompositeDiscreteParameters _doubleParameters;
};

#endif /* DOUBLEPARAMETERSARRAY_H_ */