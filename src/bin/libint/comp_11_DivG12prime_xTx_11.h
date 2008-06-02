
#ifndef _libint2_src_bin_libint_cr11divg12primextx11_h_
#define _libint2_src_bin_libint_cr11divg12primextx11_h_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <assert.h>
#include <dgvertex.h>
#include <rr.h>
#include <rr.templ.h>
#include <integral.h>
#include <divg12primexTx_11_11.h>
#include <algebra.h>
#include <flop.h>
#include <prefactors.h>
#include <context.h>
#include <default_params.h>

using namespace std;

namespace libint2 {

  /** Compute relation integrals of the
      \f$ \left( \nabla \cdot g12' \right) \left( g12' \cdot \nabla \right) \f$ operators.
      I<BFSet> is the integral set specialization that describes the
      integrals of the this operator operator.
  */
  template <template <class> class I, class BFSet>
  class CR_11_DivG12prime_xTx_11 : public RecurrenceRelation
    {

  public:
    typedef RecurrenceRelation ParentType;
    typedef BFSet BasisFunctionType;
    typedef CR_11_DivG12prime_xTx_11<I,BFSet> ThisType;
    typedef I<BFSet> TargetType;
    typedef R12kG12_11_11<BFSet,0> ChildType;
    /// The type of expressions in which RecurrenceRelations result.
    typedef RecurrenceRelation::ExprType ExprType;

    /** Use Instance() to obtain an instance of RR. This function is provided to avoid
        issues with getting a SafePtr from constructor (as needed for registry to work).
    */
    static SafePtr<ThisType> Instance(const SafePtr<TargetType>&);
    ~CR_11_DivG12prime_xTx_11() {}

    /// Implementation of RecurrenceRelation::num_children()
    const unsigned int num_children() const { return nchildren_; };
    /// target() returns pointer to the i-th child
    SafePtr<TargetType> target() const { return target_; };
    /// child(i) returns pointer to the i-th child
    SafePtr<ChildType> child(unsigned int i) const;
    /// Implementation of RecurrenceRelation::rr_target()
    SafePtr<DGVertex> rr_target() const { return static_pointer_cast<DGVertex,TargetType>(target()); }
    /// Implementation of RecurrenceRelation::rr_child()
    SafePtr<DGVertex> rr_child(unsigned int i) const { return dynamic_pointer_cast<DGVertex,ChildType>(child(i)); }
    /// Implementation of RecurrenceRelation::is_simple()
    bool is_simple() const {
      return TrivialBFSet<BFSet>::result;
    }

    const std::string cpp_function_name() {}
    const std::string cpp_source_name() {}
    const std::string cpp_header_name() {}
    std::ostream& cpp_source(std::ostream&) {}

  private:
    /**
      dir specifies which quantum number is incremented.
      For example, dir can be 0 (x), 1(y), or 2(z) if BFSet is
      a Cartesian Gaussian.
     */
    CR_11_DivG12prime_xTx_11(const SafePtr<TargetType>&);

#if 0
    /// registers this RR with the stack, if needed
    bool register_with_rrstack() const;
#endif
    
    static const unsigned int max_nchildren_ = 18;    
    SafePtr<TargetType> target_;
    SafePtr<ChildType> children_[max_nchildren_];
    unsigned int nchildren_;
 
    std::string generate_label() const
    {
      ostringstream os;
      os << "RR ( " << rr_target()->label() << " )";
      return os.str();
    }


  };

  template <template <class> class I, class F>
    SafePtr< CR_11_DivG12prime_xTx_11<I,F> >
    CR_11_DivG12prime_xTx_11<I,F>::Instance(const SafePtr<TargetType>& Tint)
    {
      SafePtr<ThisType> this_ptr(new ThisType(Tint));
      // Do post-construction duties
      if (this_ptr->num_children() != 0) {
        this_ptr->register_with_rrstack<ThisType>();
      }
      return this_ptr;
    }

  template <template <class> class I, class F>
    CR_11_DivG12prime_xTx_11<I,F>::CR_11_DivG12prime_xTx_11(const SafePtr<I<F> >& Tint) :
    ParentType(), target_(Tint), nchildren_(0)
    {
      F sh_a(Tint->bra(0,0));
      F sh_b(Tint->ket(0,0));
      F sh_c(Tint->bra(1,0));
      F sh_d(Tint->ket(1,0));

      vector<F> bra;
      vector<F> ket;
      bra.push_back(sh_a);
      bra.push_back(sh_c);
      ket.push_back(sh_b);
      ket.push_back(sh_d);

      vector<F>* bra_ref = &bra;
      vector<F>* ket_ref = &ket;

      const unsigned int ndirs = is_simple() ? 3 : 1;
      for(int xyz=0; xyz<ndirs; xyz++) {
        
	// a+1_i c+1_i
	bra_ref->operator[](0).inc(xyz);
	bra_ref->operator[](1).inc(xyz);
	int next_child = nchildren_;
	children_[next_child] = ChildType::Instance(bra[0],ket[0],bra[1],ket[1],0);
	++nchildren_;

	if (is_simple()) {
	  SafePtr<ExprType> expr0_ptr(new ExprType(ExprType::OperatorTypes::Times,prefactors.N_i[1],rr_child(next_child)));
	  add_expr(expr0_ptr);
	  nflops_ += 1;
	}
	bra_ref->operator[](0).dec(xyz);
	bra_ref->operator[](1).dec(xyz);
      }
    }
  
  template <template <class> class I, class F>
    SafePtr< typename CR_11_DivG12prime_xTx_11<I,F>::ChildType >
    CR_11_DivG12prime_xTx_11<I,F>::child(unsigned int i) const
    {
      assert(i>=0 && i<nchildren_);
      unsigned int nc=0;
      for(int c=0; c<max_nchildren_; c++) {
        if (children_[c] != 0) {
          if (nc == i)
            return children_[c];
          nc++;
        }
      }
    };

  /// Useful typedefs
  typedef CR_11_DivG12prime_xTx_11<DivG12prime_xTx_11_11,CGShell> CR_11_DivG12prime_xTx_11_sq;
  typedef CR_11_DivG12prime_xTx_11<DivG12prime_xTx_11_11,CGF> CR_11_DivG12prime_xTx_11_int;

};

#endif
