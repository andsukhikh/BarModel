#ifndef SCHEME_H
#define SCHEME_H

#include <memory>

#include "temperature.h"
#include "properties.h"
#include "boundary.h"


struct IScheme
{
	Temperature temp;

	std::shared_ptr<Properties> prop;

	std::shared_ptr<Boundary> boundary_conditions;

	std::size_t time_partitions;
	double time_end;
	double time_step;

	double x_step;
	double y_step;

	double Q_extend;
	double CONST;

	Temperature new_temp;

	virtual ~IScheme() = default;
	virtual void evaluate() = 0;
};


//struct IExplicitScheme
//{
//	virtual ~IExplicitScheme() = default;
//	virtual void evaluate(std::size_t i, std::size_t j) = 0;
//};
//
//
//class ExplicitSchemeSwitcher
//{
//protected:
//	std::shared_ptr<IExplicitScheme> strategy_;
//public:
//	virtual ~ExplicitSchemeSwitcher() = default;
//	virtual void use_strategy(std::size_t i, std::size_t j) = 0;
//	virtual ExplicitSchemeSwitcher& set_strategy(std::shared_ptr<IExplicitScheme> str) = 0;
//};


class ExplicitScheme : public IScheme/*, public ExplicitSchemeSwitcher*/
{
public:
	void evaluate() override;
private:
	void inner_explicit_scheme(std::size_t i, std::size_t j);

	void lower_left_explicit_scheme(std::size_t i, std::size_t j);
	void upper_left_explicit_scheme(std::size_t i, std::size_t j);
	void upper_right_explicit_scheme(std::size_t i, std::size_t j);
	void lower_right_explicit_scheme(std::size_t i, std::size_t j);

	void left_explicit_scheme(std::size_t i, std::size_t j);
	void right_explicit_scheme(std::size_t i, std::size_t j);
	void up_explicit_scheme(std::size_t i, std::size_t j);
	void down_explicit_scheme(std::size_t i, std::size_t j);

	//void use_strategy(std::size_t i, std::size_t j) override;

	//ExplicitSchemeSwitcher& set_strategy(std::shared_ptr<IExplicitScheme> strategy) override;

	const bool check_criterion() const;
};

//struct ExplicitSchemeInner : public IExplicitScheme
//{
//	std::shared_ptr<ExplicitScheme> scheme_;
//public:
//	ExplicitSchemeInner(ExplicitScheme scheme);
//
//	void evaluate(std::size_t i, std::size_t j) override;
//};
//
//
//struct ExplicitSchemeLowerLeft : public IExplicitScheme
//{
//	std::shared_ptr<ExplicitScheme> scheme_;
//public:
//	ExplicitSchemeLowerLeft(ExplicitScheme scheme);
//
//	void evaluate(std::size_t i, std::size_t j) override;
//};
//
//
//struct ExplicitSchemeUpperLeft : public IExplicitScheme
//{
//	std::shared_ptr<ExplicitScheme> scheme_;
//public:
//	ExplicitSchemeUpperLeft(ExplicitScheme scheme);
//
//	void evaluate(std::size_t i, std::size_t j) override;
//};
//
//
//struct ExplicitSchemeUpperRight : public IExplicitScheme
//{
//	std::shared_ptr<ExplicitScheme> scheme_;
//public:
//	ExplicitSchemeUpperRight(ExplicitScheme scheme);
//
//	void evaluate(std::size_t i, std::size_t j) override;
//};
//
//
//struct ExplicitSchemeLowerRight : public IExplicitScheme
//{
//	std::shared_ptr<ExplicitScheme> scheme_;
//public:
//	ExplicitSchemeLowerRight(ExplicitScheme scheme);
//
//	void evaluate(std::size_t i, std::size_t j) override;
//};
//
//
//struct ExplicitSchemeLeft : public IExplicitScheme
//{
//	std::shared_ptr<ExplicitScheme> scheme_;
//public:
//	ExplicitSchemeLeft(ExplicitScheme scheme);
//
//	void evaluate(std::size_t i, std::size_t j) override;
//};
//
//
//struct ExplicitSchemeRight : public IExplicitScheme
//{
//	std::shared_ptr<ExplicitScheme> scheme_;
//public:
//	ExplicitSchemeRight(ExplicitScheme scheme);
//
//	void evaluate(std::size_t i, std::size_t j) override;
//};
//
//
//struct ExplicitSchemeDown : public IExplicitScheme
//{
//	std::shared_ptr<ExplicitScheme> scheme_;
//public:
//	ExplicitSchemeDown(ExplicitScheme scheme);
//
//	void evaluate(std::size_t i, std::size_t j) override;
//};
//
//
//struct ExplicitSchemeUp : public IExplicitScheme
//{
//	std::shared_ptr<ExplicitScheme> scheme_;
//public:
//	ExplicitSchemeUp(ExplicitScheme scheme);
//
//	void evaluate(std::size_t i, std::size_t j) override;
//};

#endif // !SCHEME_H
