#ifndef _FOL_H
#define _FOL_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>

using namespace std;


class BaseFormula;
typedef shared_ptr<BaseFormula> Formula;

class BaseFormula : public enable_shared_from_this<BaseFormula> {



public:

  enum Type { T_TRUE, T_FALSE, T_ATOM, T_NOT, 
	      T_AND, T_OR, T_IMP, T_IFF, T_FORALL, T_EXISTS };

  virtual void printFormula(ostream & ostr) const = 0;
  virtual Type getType() const = 0;
  virtual ~BaseFormula() {}

};



class AtomicFormula : public BaseFormula {
public:
};


class LogicConstant : public AtomicFormula {

public:
};


class True : public LogicConstant {

public:
  virtual void printFormula(ostream & ostr) const
  {
    ostr << "true";
  }

  virtual Type getType() const
  {
    return T_TRUE;
  }

};


class False : public LogicConstant {

public:
  virtual void printFormula(ostream & ostr) const
  {
    ostr << "false";
  }

  virtual Type getType() const
  {
    return T_FALSE;
  }
};


class Atom : public AtomicFormula {
private:
  string _var;
public:
  Atom(const string &var) : _var(var) {}

  string getVariable() const
  {
    return _var;
  }

  virtual Type getType() const
  {
    return T_ATOM;
  }

  virtual void printFormula(ostream & ostr) const 
  {
    ostr << _var;
  }
};




class UnaryConjective : public BaseFormula {
protected:
   Formula _op;
public:
  UnaryConjective(const Formula & op)
    :_op(op)
  {}

  const Formula & getOperand() const
  {
    return _op;
  }
};

class Not : public UnaryConjective {
public:
  Not(const Formula & op)
    :UnaryConjective(op)
  {}

  virtual void printFormula(ostream & ostr) const
  { 
    ostr << "~";
    Type op_type = _op->getType();

    if(op_type == T_AND || op_type == T_OR || 
       op_type == T_IMP || op_type == T_IFF)
      ostr << "(";

    _op->printFormula(ostr);

    if(op_type == T_AND || op_type == T_OR || 
       op_type == T_IMP || op_type == T_IFF)
      ostr << ")";
  }

  virtual Type getType() const
  {
    return T_NOT;
  }
};


class BinaryConjective : public BaseFormula {
protected:
   Formula _op1, _op2;
public:
  BinaryConjective(const Formula & op1, const Formula & op2)
    :_op1(op1),
     _op2(op2)
  {}

  const Formula & getOperand1() const
  {
    return _op1;
  }

  const Formula & getOperand2() const
  {
    return _op2;
  }
};


class And : public BinaryConjective {
public:
  And(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

  virtual void printFormula(ostream & ostr) const
  {
    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();

    if(op1_type == T_OR || op1_type == T_IMP || 
       op1_type == T_IFF)
      ostr << "(";
    
    _op1->printFormula(ostr);
    
    if(op1_type == T_OR || op1_type == T_IMP || 
       op1_type == T_IFF)
      ostr << ")";

    ostr << " & ";

    if(op2_type == T_OR || op2_type == T_IMP || 
       op2_type == T_IFF || op2_type == T_AND)
      ostr << "(";
    
    _op2->printFormula(ostr);

    if(op2_type == T_OR || op2_type == T_IMP || 
       op2_type == T_IFF || op2_type == T_AND)
      ostr << ")";
  }

  virtual Type getType() const
  {
    return T_AND;
  }
 };



class Or : public BinaryConjective {
public:
  Or(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

  virtual void printFormula(ostream & ostr) const
  {

    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();

    if(op1_type == T_IMP || op1_type == T_IFF)
      ostr << "(";
    
    _op1->printFormula(ostr);
    
    if(op1_type == T_IMP || op1_type == T_IFF)
      ostr << ")";

    ostr << " | ";

    if(op2_type == T_IMP || 
       op2_type == T_IFF || op2_type == T_OR)
      ostr << "(";
    
    _op2->printFormula(ostr);

    if(op2_type == T_IMP || 
       op2_type == T_IFF || op2_type == T_OR)
      ostr << ")";
  }

  virtual Type getType() const
  {
    return T_OR;
  }
};


class Imp : public BinaryConjective {
public:
  Imp(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}
  
  virtual void printFormula(ostream & ostr) const
  {

    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();

    if(op1_type == T_IFF)
      ostr << "(";
    
    _op1->printFormula(ostr);
    
    if(op1_type == T_IFF)
      ostr << ")";

    ostr << " => ";

    if(op2_type == T_IMP || op2_type == T_IFF)
      ostr << "(";
    
    _op2->printFormula(ostr);

    if(op2_type == T_IMP || op2_type == T_IFF)
      ostr << ")";
  }
  
  virtual Type getType() const
  {
    return T_IMP;
  }
};



class Iff : public BinaryConjective {

public:
  Iff(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

  virtual void printFormula(ostream & ostr) const
  {

    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();
    
    _op1->printFormula(ostr);
    
    ostr << " <=> ";

    if(op2_type == T_IFF)
      ostr << "(";
    
    _op2->printFormula(ostr);

    if(op2_type == T_IFF)
      ostr << ")";

  }
  
  virtual Type getType() const
  {
    return T_IFF;
  }
};


inline
ostream & operator << (ostream & ostr, const Formula & f)
{
  f->printFormula(ostr);
}

extern Formula parsed_formula;

#endif // _FOL_H
