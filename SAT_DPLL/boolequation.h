// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


#ifndef BOOLEQUATION_H
#define BOOLEQUATION_H

#include "boolinterval.h"




class IBranchStrategy;

class BoolEquation
{
private:
	IBranchStrategy* strategy = nullptr;
	BoolInterval **cnf;//множество интервалов
	BoolInterval *root;//Корень уравнения
	int cnfSize; // Размер КНФ
	int count; //количество дизъюнкций
	BBV mask; //маска для столбцов
public:
	BoolEquation(BoolInterval **cnf, BoolInterval *root, int cnfSize, int count, BBV mask);
	BoolEquation(BoolEquation &equation);
	bool Rule2RowNull(BoolInterval *interval);
	bool Rule1Row1(BoolInterval *interval);
	void Simplify(int ixCol, char value);
	void Rule3ColNull(BBV vector);
	int ChooseColForBranching();
	bool Rule5Col1(BBV vector);
	bool Rule4Col0(BBV vector);
	int CheckRules();

    IBranchStrategy* GetStrategy() {return strategy;}
	BoolInterval* GetRoot()        {return root;}
	BoolInterval** GetCnf()        {return cnf;}
	int GetcnfSize()               {return cnfSize;}
	int GetCount()                 {return count;}
	BBV GetMask()                  {return mask;}	

	void SetStrategy(IBranchStrategy* strategy_) {strategy = strategy_;}
	
};

class IBranchStrategy {
public:
    virtual ~IBranchStrategy() = default;
    // Возвращает индекс столбца (или -1, если мы выбираем строку иначе)
    virtual int chooseVarForBranching(BoolEquation* equation) = 0;
};

class ColumnStrategy : public IBranchStrategy {
public:
    int chooseVarForBranching(BoolEquation* equation) override;
};


#endif // BOOLEQUATION_H
