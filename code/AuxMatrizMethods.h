/*

        NOTE: THE MATRIX INVERSION FUNCTION LIKELY NEEDS TO BE REPLACED BY ONE THAT IS MULTITHREADED (EITHER VIA CPU OR GPU).
		      THIS IS BECAUSE THE MATRIX INVERSION IN OUR IMPLEMENTATION IS OF SUFFICIENT SIZE AS TO CAUSE COMPUTATIONAL DELAY.

*/

#if !defined(__STD_MATRIX_H)
#define __STD_MATRIX_H


#  include <cmath>
#  include <cstdio>
#  include <cstdlib>
#  include <string>
#  include <iostream>

#include <stdexcept>
#  define _NO_THROW               throw ()
#  define _THROW_MATRIX_ERROR     throw (matrix_error)

#ifndef __MINMAX_DEFINED
#  define max(a,b)    (((a) > (b)) ? (a) : (b))
#  define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif


#ifndef _NO_NAMESPACE
namespace math {
#endif

class matrix_error : public logic_error
{
    public:
	matrix_error (const string& what_arg) : logic_error( what_arg) {}
};
#define REPORT_ERROR(ErrormMsg)  throw matrix_error( ErrormMsg);


#  define MAT_TEMPLATE  template <class T>
#  define matrixT  matrix<T>


MAT_TEMPLATE
class matrix
{
public:
   // Constructors
   matrix (const matrixT& m);
   matrix (size_t row = 6, size_t col = 6);

   // Destructor
   ~matrix ();

   // Assignment operators
   matrixT& operator = (const matrixT& m);

   // Value extraction method
   size_t RowNo () const { return _m->Row; }
   size_t ColNo () const { return _m->Col; }

   // Subscript operator
   T& operator () (size_t row, size_t col);
   T  operator () (size_t row, size_t col) const ;

   // Unary operators
   matrixT operator + () { return *this; }
   matrixT operator - ();

   // Combined assignment - calculation operators
   matrixT& operator += (const matrixT& m);
   matrixT& operator -= (const matrixT& m);
   matrixT& operator *= (const matrixT& m);
   matrixT& operator *= (const T& c);
   matrixT& operator /= (const T& c);
   matrixT& operator ^= (const size_t& pow);

   // Miscellaneous -methods
   void Null (const size_t& row, const size_t& col);
   void Null ();
   void Unit (const size_t& row);
   void Unit ();
   void SetSize (size_t row, size_t col);

   // Utility methods
   matrixT Solve (const matrixT& v) const;
   matrixT Adj ();
   matrixT Inv ();
   T Det () const;
   T Norm ();
   T Cofact (size_t row, size_t col);
   T Cond ();

   // Type of matrices
   bool IsSquare () { return (_m->Row == _m->Col); } 
   bool IsSingular ();
   bool IsDiagonal ();
   bool IsScalar ();
   bool IsUnit () ;
   bool IsNull () ;
   bool IsSymmetric ();
   bool IsSkewSymmetric ();
   bool IsUpperTriangular ();
   bool IsLowerTriangular ();

public:
    struct base_mat
    {
	T **Val;
	size_t Row, Col, RowSiz, ColSiz;
	int Refcnt;

	base_mat (size_t row, size_t col, T** v)
	{
	    Row = row; RowSiz = row;
	    Col = col; ColSiz = col;
	    Refcnt = 1;

	    Val = new T* [row];
	    size_t rowlen = col * sizeof(T);

	    for (size_t i=0; i < row; i++)
	    {
		Val[i] = new T [col];
		if (v) memcpy( Val[i], v[i], rowlen);
	    }
	}
	~base_mat ()
	{
	    for (size_t i=0; i < RowSiz; i++)
		delete [] Val[i];
	    delete [] Val;
	}
    };
    base_mat *_m;

    void clone ();
    void realloc (size_t row, size_t col);
    int pivot (size_t row);
};

#if defined(_MSC_VER) && _MSC_VER <= 1020
#  undef  _NO_THROW               // MSVC++ 4.0/4.2 does not support 
#  undef  _THROW_MATRIX_ERROR     // exception specification in definition
#  define _NO_THROW
#  define _THROW_MATRIX_ERROR
#endif

// constructor
MAT_TEMPLATE inline
matrixT::matrix (size_t row, size_t col)
{
  _m = new base_mat( row, col, 0);
}

// copy constructor
MAT_TEMPLATE inline
matrixT::matrix (const matrixT& m)
{
    _m = m._m;
    _m->Refcnt++;
}

// Internal copy constructor
MAT_TEMPLATE inline void
matrixT::clone ()
{
    _m->Refcnt--;
    _m = new base_mat( _m->Row, _m->Col, _m->Val);
}

// destructor
MAT_TEMPLATE inline
matrixT::~matrix ()
{
   if (--_m->Refcnt == 0) delete _m;
}

// assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator = (const matrixT& m) _NO_THROW
{
    m._m->Refcnt++;
    if (--_m->Refcnt == 0) delete _m;
    _m = m._m;
    return *this;
}

//  reallocation method
MAT_TEMPLATE inline void 
matrixT::realloc (size_t row, size_t col)
{
   if (row == _m->RowSiz && col == _m->ColSiz)
   {
      _m->Row = _m->RowSiz;
      _m->Col = _m->ColSiz;
      return;
   }

   base_mat *m1 = new base_mat( row, col, NULL);
   size_t colSize = min(_m->Col,col) * sizeof(T);
   size_t minRow = min(_m->Row,row);

   for (size_t i=0; i < minRow; i++)
      memcpy( m1->Val[i], _m->Val[i], colSize);

   if (--_m->Refcnt == 0) 
       delete _m;
   _m = m1;

   return;
}

// public method for resizing matrix
MAT_TEMPLATE inline void
matrixT::SetSize (size_t row, size_t col) _NO_THROW
{
   size_t i,j;
   size_t oldRow = _m->Row;
   size_t oldCol = _m->Col;

   if (row != _m->RowSiz || col != _m->ColSiz)
      realloc( row, col);

   for (i=oldRow; i < row; i++)
      for (j=0; j < col; j++)
	 _m->Val[i][j] = T(0);

   for (i=0; i < row; i++)                      
      for (j=oldCol; j < col; j++)
	 _m->Val[i][j] = T(0);

   return;
}

// subscript operator to get/set individual elements
MAT_TEMPLATE inline T&
matrixT::operator () (size_t row, size_t col)
{
   if (row >= _m->Row || col >= _m->Col)
      REPORT_ERROR( "matrixT::operator(): Index out of range!");
   if (_m->Refcnt > 1) clone();
   return _m->Val[row][col];
}

// subscript operator to get/set individual elements
MAT_TEMPLATE inline T
matrixT::operator () (size_t row, size_t col) const 
{
   if (row >= _m->Row || col >= _m->Col)
      REPORT_ERROR( "matrixT::operator(): Index out of range!");
   return _m->Val[row][col];
}

// input stream function
MAT_TEMPLATE inline istream&
operator >> (istream& istrm, matrixT& m)
{
   for (size_t i=0; i < m.RowNo(); i++)
      for (size_t j=0; j < m.ColNo(); j++)
      {
         T x;
         istrm >> x;
         m(i,j) = x;
      }
   return istrm;
}

// output stream function
MAT_TEMPLATE inline ostream&
operator << (ostream& ostrm, const matrixT& m)
{
   for (size_t i=0; i < m.RowNo(); i++)
   {
      for (size_t j=0; j < m.ColNo(); j++)
      {
         T x = m(i,j);
         ostrm << x << '\t';
      }
      ostrm << endl;
   }
   return ostrm;
}


// logical equal-to operator
MAT_TEMPLATE inline bool
operator == (const matrixT& m1, const matrixT& m2) _NO_THROW
{
   if (m1.RowNo() != m2.RowNo() || m1.ColNo() != m2.ColNo())
      return false;

   for (size_t i=0; i < m1.RowNo(); i++)
      for (size_t j=0; j < m1.ColNo(); j++)
	      if (m1(i,j) != m2(i,j))
	         return false;

   return true;
}

// logical no-equal-to operator
MAT_TEMPLATE inline bool
operator != (const matrixT& m1, const matrixT& m2)
{
    return (m1 == m2) ? false : true;
}

// combined addition and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator += (const matrixT& m)
{
   if (_m->Row != m._m->Row || _m->Col != m._m->Col)
      REPORT_ERROR( "matrixT::operator+= : Inconsistent matrix sizes in addition!");
   if (_m->Refcnt > 1) clone();
   for (size_t i=0; i < m._m->Row; i++)
      for (size_t j=0; j < m._m->Col; j++)
	 _m->Val[i][j] += m._m->Val[i][j];
   return *this;
}

// combined subtraction and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator -= (const matrixT& m)
{
   if (_m->Row != m._m->Row || _m->Col != m._m->Col)
      REPORT_ERROR( "matrixT::operator-= : Inconsistent matrix sizes in subtraction!");
   if (_m->Refcnt > 1) clone();
   for (size_t i=0; i < m._m->Row; i++)
      for (size_t j=0; j < m._m->Col; j++)
	 _m->Val[i][j] -= m._m->Val[i][j];
   return *this;
}

// combined scalar multiplication and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator *= (const T& c) _NO_THROW
{
    if (_m->Refcnt > 1) clone();
    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
	    _m->Val[i][j] *= c;
    return *this;
}

// combined matrix multiplication and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator *= (const matrixT& m) 
{
   if (_m->Col != m._m->Row)
      REPORT_ERROR( "matrixT::operator*= : Inconsistent matrix sizes in multiplication!");

   matrixT temp(_m->Row,m._m->Col);

   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < m._m->Col; j++)
      {
         temp._m->Val[i][j] = T(0);
         for (size_t k=0; k < _m->Col; k++)
            temp._m->Val[i][j] += _m->Val[i][k] * m._m->Val[k][j];
      }
   *this = temp;

   return *this;
}

// combined scalar division and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator /= (const T& c) _NO_THROW
{
    if (_m->Refcnt > 1) clone();
    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
	    _m->Val[i][j] /= c;

    return *this;
}

// combined power and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator ^= (const size_t& pow) 
{
	matrixT temp(*this);

	for (size_t i=2; i <= pow; i++)
      *this = *this * temp;

	return *this;
}

// unary negation operator
MAT_TEMPLATE inline matrixT
matrixT::operator - () _NO_THROW
{
   matrixT temp(_m->Row,_m->Col);

   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	 temp._m->Val[i][j] = - _m->Val[i][j];

   return temp;
}

// binary addition operator
MAT_TEMPLATE inline matrixT
operator + (const matrixT& m1, const matrixT& m2) 
{
   matrixT temp = m1;
   temp += m2;
   return temp;
}

// binary subtraction operator
MAT_TEMPLATE inline matrixT
operator - (const matrixT& m1, const matrixT& m2) 
{
   matrixT temp = m1;
   temp -= m2;
   return temp;
}

// binary scalar multiplication operator
MAT_TEMPLATE inline matrixT
operator * (const matrixT& m, const T& no) _NO_THROW
{
   matrixT temp = m;
   temp *= no;
   return temp;
}


// binary scalar multiplication operator
MAT_TEMPLATE inline matrixT
operator * (const T& no, const matrixT& m) _NO_THROW
{
   return (m * no);
}

// binary matrix multiplication operator
MAT_TEMPLATE inline matrixT
operator * (const matrixT& m1, const matrixT& m2) 
{
   matrixT temp = m1;
   temp *= m2;
   return temp;
}

// binary scalar division operator
MAT_TEMPLATE inline matrixT
operator / (const matrixT& m, const T& no) _NO_THROW
{
    return (m * (T(1) / no));
}


// binary scalar division operator
MAT_TEMPLATE inline matrixT
operator / (const T& no, const matrixT& m) 
{
    return (!m * no);
}

// binary matrix division operator
MAT_TEMPLATE inline matrixT
operator / (const matrixT& m1, const matrixT& m2) 
{
    return (m1 * !m2);
}

// binary power operator
MAT_TEMPLATE inline matrixT
operator ^ (const matrixT& m, const size_t& pow) 
{
   matrixT temp = m;
   temp ^= pow;
   return temp;
}

// unary transpose operator
MAT_TEMPLATE inline matrixT
operator ~ (const matrixT& m) _NO_THROW
{
   matrixT temp(m.ColNo(),m.RowNo());

   for (size_t i=0; i < m.RowNo(); i++)
      for (size_t j=0; j < m.ColNo(); j++)
      {
         T x = m(i,j);
	      temp(j,i) = x;
      }
   return temp;
}

// unary inversion operator
MAT_TEMPLATE inline matrixT
operator ! (const matrixT m) 
{
   matrixT temp = m;
   return temp.Inv();
}

// inversion function
MAT_TEMPLATE inline matrixT
matrixT::Inv () 
{
   size_t i,j,k;
   T a1,a2;
   T *rowptr;

   if (_m->Row != _m->Col)
      REPORT_ERROR( "matrixT::operator!: Inversion of a non-square matrix");

   matrixT temp(_m->Row,_m->Col);
   if (_m->Refcnt > 1){
	   clone();
	}

   temp.Unit();

	

   for (k=0; k < _m->Row; k++){
      int indx = pivot(k);
     
      if (indx != 0)
      {
	      rowptr = temp._m->Val[k];
	      temp._m->Val[k] = temp._m->Val[indx];
	      temp._m->Val[indx] = rowptr;
      }

      a1 = _m->Val[k][k];
      for (j=0; j < _m->Row; j++){
	      _m->Val[k][j] /= a1;
	      temp._m->Val[k][j] /= a1;
      }
      for (i=0; i < _m->Row; i++)
	   if (i != k){
	      a2 = _m->Val[i][k];
	      for (j=0; j < _m->Row; j++){
	         T temps = _m->Val[i][j] - (a2 * _m->Val[k][j]);
			 _m->Val[i][j] -= a2 * _m->Val[k][j];
	         temp._m->Val[i][j] -= a2 * temp._m->Val[k][j];
	      }
	   }
   }
   return temp;
}

// solve simultaneous equation
MAT_TEMPLATE inline matrixT
matrixT::Solve (const matrixT& v) const 
{
   size_t i,j,k;
   T a1;

   if (!(_m->Row == _m->Col && _m->Col == v._m->Row))
      REPORT_ERROR( "matrixT::Solve():Inconsistent matrices!");

   matrixT temp(_m->Row,_m->Col+v._m->Col);
   for (i=0; i < _m->Row; i++)
   {
      for (j=0; j < _m->Col; j++)
	 temp._m->Val[i][j] = _m->Val[i][j];
      for (k=0; k < v._m->Col; k++)
	 temp._m->Val[i][_m->Col+k] = v._m->Val[i][k];
   }
   for (k=0; k < _m->Row; k++)
   {
      int indx = temp.pivot(k);
      if (indx == -1)
	 REPORT_ERROR( "matrixT::Solve(): Singular matrix!");

      a1 = temp._m->Val[k][k];
      for (j=k; j < temp._m->Col; j++)
	 temp._m->Val[k][j] /= a1;

      for (i=k+1; i < _m->Row; i++)
      {
	 a1 = temp._m->Val[i][k];
	 for (j=k; j < temp._m->Col; j++)
	   temp._m->Val[i][j] -= a1 * temp._m->Val[k][j];
      }
   }
   matrixT s(v._m->Row,v._m->Col);
   for (k=0; k < v._m->Col; k++)
      for (int m=int(_m->Row)-1; m >= 0; m--)
      {
	 s._m->Val[m][k] = temp._m->Val[m][_m->Col+k];
	 for (j=m+1; j < _m->Col; j++)
	    s._m->Val[m][k] -= temp._m->Val[m][j] * s._m->Val[j][k];
      }
   return s;
}

// set zero to all elements of this matrix
MAT_TEMPLATE inline void
matrixT::Null (const size_t& row, const size_t& col) _NO_THROW
{
    if (row != _m->Row || col != _m->Col)
	realloc( row,col);

    if (_m->Refcnt > 1) 
	clone();

    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
	    _m->Val[i][j] = T(0);
    return;
}

// set zero to all elements of this matrix
MAT_TEMPLATE inline void
matrixT::Null() _NO_THROW
{
    if (_m->Refcnt > 1) clone();   
    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
		_m->Val[i][j] = T(0);
    return;
}

// set this matrix to unity
MAT_TEMPLATE inline void
matrixT::Unit (const size_t& row) _NO_THROW
{
    if (row != _m->Row || row != _m->Col)
	realloc( row, row);
	
    if (_m->Refcnt > 1) 
	clone();

    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
	    _m->Val[i][j] = i == j ? T(1) : T(0);
    return;
}

// set this matrix to unity
MAT_TEMPLATE inline void
matrixT::Unit () _NO_THROW
{
    if (_m->Refcnt > 1) clone();   
    size_t row = min(_m->Row,_m->Col);
    _m->Row = _m->Col = row;

    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
	    _m->Val[i][j] = i == j ? T(1) : T(0);
    return;
}

// private partial pivoting method
MAT_TEMPLATE inline int
matrixT::pivot (size_t row)
{
  int k = int(row);
  double amax,temp;

  amax = -1;
  for (size_t i=row; i < _m->Row; i++)
    if ( (temp = abs( _m->Val[i][row])) > amax && temp != 0.0)
     {
       amax = temp;
       k = (int)i;
     }
  if (_m->Val[k][row] == T(0))
     return -1;
  if (k != int(row))
  {
     T* rowptr = _m->Val[k];
     _m->Val[k] = _m->Val[row];
     _m->Val[row] = rowptr;
     return k;
  }
  return 0;
}

// calculate the determinant of a matrix
MAT_TEMPLATE inline T
matrixT::Det () const 
{
   size_t i,j,k;
   T piv,detVal = T(1);

   if (_m->Row != _m->Col)
      REPORT_ERROR( "matrixT::Det(): Determinant a non-square matrix!");
   
   matrixT temp(*this);
   if (temp._m->Refcnt > 1) temp.clone();

   for (k=0; k < _m->Row; k++)
   {
      int indx = temp.pivot(k);
      if (indx == -1)
	 return 0;
      if (indx != 0)
	 detVal = - detVal;
      detVal = detVal * temp._m->Val[k][k];
      for (i=k+1; i < _m->Row; i++)
      {
	 piv = temp._m->Val[i][k] / temp._m->Val[k][k];
	 for (j=k+1; j < _m->Row; j++)
	    temp._m->Val[i][j] -= piv * temp._m->Val[k][j];
      }
   }
   return detVal;
}

// calculate the norm of a matrix
MAT_TEMPLATE inline T
matrixT::Norm () _NO_THROW
{
   T retVal = T(0);

   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	 retVal += _m->Val[i][j] * _m->Val[i][j];
   retVal = sqrt( retVal);

   return retVal;
}

// calculate the condition number of a matrix
MAT_TEMPLATE inline T
matrixT::Cond () _NO_THROW
{
   matrixT inv = ! (*this);
   return (Norm() * inv.Norm());
}

// calculate the cofactor of a matrix for a given element
MAT_TEMPLATE inline T
matrixT::Cofact (size_t row, size_t col) 
{
   size_t i,i1,j,j1;

   if (_m->Row != _m->Col)
      REPORT_ERROR( "matrixT::Cofact(): Cofactor of a non-square matrix!");

   if (row > _m->Row || col > _m->Col)
      REPORT_ERROR( "matrixT::Cofact(): Index out of range!");

   matrixT temp (_m->Row-1,_m->Col-1);

   for (i=i1=0; i < _m->Row; i++)
   {
      if (i == row)
	continue;
      for (j=j1=0; j < _m->Col; j++)
      {
	 if (j == col)
	    continue;
	 temp._m->Val[i1][j1] = _m->Val[i][j];
	 j1++;
      }
      i1++;
   }
   T  cof = temp.Det();
   if ((row+col)%2 == 1)
      cof = -cof;

   return cof;
}


// calculate adjoin of a matrix
MAT_TEMPLATE inline matrixT
matrixT::Adj () 
{
   if (_m->Row != _m->Col)
      REPORT_ERROR( "matrixT::Adj(): Adjoin of a non-square matrix.");

   matrixT temp(_m->Row,_m->Col);

   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	  temp._m->Val[j][i] = Cofact(i,j);
   return temp;
}

// Determine if the matrix is singular
MAT_TEMPLATE inline bool
matrixT::IsSingular () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;
   return (Det() == T(0));
}

// Determine if the matrix is diagonal
MAT_TEMPLATE inline bool
matrixT::IsDiagonal () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;
   for (size_t i=0; i < _m->Row; i++)
     for (size_t j=0; j < _m->Col; j++)
	if (i != j && _m->Val[i][j] != T(0))
	  return false;
   return true;
}

// Determine if the matrix is scalar
MAT_TEMPLATE inline bool
matrixT::IsScalar () _NO_THROW
{
   if (!IsDiagonal())
     return false;
   T v = _m->Val[0][0];
   for (size_t i=1; i < _m->Row; i++)
     if (_m->Val[i][i] != v)
	return false;
   return true;
}

// Determine if the matrix is a unit matrix
MAT_TEMPLATE inline bool
matrixT::IsUnit () _NO_THROW
{
   if (IsScalar() && _m->Val[0][0] == T(1))
     return true;
   return false;
}

// Determine if this is a null matrix
MAT_TEMPLATE inline bool
matrixT::IsNull () _NO_THROW
{
   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	 if (_m->Val[i][j] != T(0))
	    return false;
   return true;
}

// Determine if the matrix is symmetric
MAT_TEMPLATE inline bool
matrixT::IsSymmetric () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;
   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	 if (_m->Val[i][j] != _m->Val[j][i])
	    return false;
   return true;
}
	   
// Determine if the matrix is skew-symmetric
MAT_TEMPLATE inline bool
matrixT::IsSkewSymmetric () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;
   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	 if (_m->Val[i][j] != -_m->Val[j][i])
	    return false;
   return true;
}
   
// Determine if the matrix is upper triangular
MAT_TEMPLATE inline bool
matrixT::IsUpperTriangular () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;
   for (size_t i=1; i < _m->Row; i++)
      for (size_t j=0; j < i-1; j++)
	 if (_m->Val[i][j] != T(0))
	    return false;
   return true;
}

// Determine if the matrix is lower triangular
MAT_TEMPLATE inline bool
matrixT::IsLowerTriangular () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;

   for (size_t j=1; j < _m->Col; j++)
      for (size_t i=0; i < j-1; i++)
	 if (_m->Val[i][j] != T(0))
	    return false;

   return true;
}

#ifndef _NO_NAMESPACE
} 
#endif

#endif //__STD_MATRIX_H
