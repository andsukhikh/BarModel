#ifndef SCHEME_H
#define SCHEME_H

#include <memory>

#include "temperature.h"
#include "properties.h"
#include "boundary.h"


struct IScheme
{
	std::shared_ptr<Temperature> temp;

	std::shared_ptr<Properties> prop;

	std::shared_ptr<Boundary> boundary_conditions;

	std::size_t time_partitions;
	double time_end;

	double x_step;
	double y_step;

	double Q_extend;
	double CONST;

	std::shared_ptr<Temperature> new_temp = temp;

	virtual ~IScheme() = 0;
	virtual Temperature evaluate() = 0;
};


struct ExplicitSchemeInner : public IScheme
{
	Temperature evaluate() override;
};


struct ExplicitSchemeLowerLeft : public IScheme
{
	Temperature evaluate() override;
};


struct ExplicitSchemeUpperLeft : public IScheme
{
	Temperature evaluate() override;
};


struct ExplicitSchemeUpperRight : public IScheme
{
	Temperature evaluate() override;
};


struct ExplicitSchemeLowerRight : public IScheme
{
	Temperature evaluate() override;
};


struct ExplicitSchemeLeft : public IScheme
{
	Temperature evaluate() override;
};


struct ExplicitSchemeRight : public IScheme
{
	Temperature evaluate() override;
};


struct ExplicitSchemeDown : public IScheme
{
	Temperature evaluate() override;
};


struct ExplicitSchemeUp : public IScheme
{
	Temperature evaluate() override;
};


class Context
{
protected:
	std::shared_ptr<IScheme> strategy;
public:
	virtual ~Context() {}
	virtual void use_strategy() = 0;
	virtual Context& set_strategy(std::shared_ptr<IScheme> str) = 0;
};


class ExplicitScheme : public IScheme, public Context
{
public:
	Temperature evaluate() override;
private:
	void calculate_at(std::size_t i, std::size_t j);

	void use_strategy() override;

	Context& set_strategy(std::shared_ptr<IScheme> str) override;

	const bool check_criterion() const;
};

#endif // !SCHEME_H
