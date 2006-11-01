
#include <dg.h>
#include <dgvertex.h>

#ifndef _libint2_src_bin_libint_dgtempl_h_
#define _libint2_src_bin_libint_dgtempl_h_

using namespace std;


namespace libint2 {

  /// Apply RR to target
  template <class I, class RR>
    void
    DirectedGraph::append_target(const SafePtr<I>& target)
    {
      target->make_a_target();
      recurse<I,RR>(target);
    };

  /// Apply RR to target
  template <class I, class RR>
    void
    DirectedGraph::recurse(const SafePtr<I>& vertex)
    {
      try { add_vertex(vertex); }
      catch (VertexAlreadyOnStack& e) { return; }
      
      SafePtr<RR> rr0(new RR(vertex));
      const int num_children = rr0->num_children();
      
      for(int c=0; c<num_children; c++) {
        
        SafePtr<DGVertex> child = rr0->child(c);
        SafePtr<DGArc> arc(new DGArcRel<RR>(vertex,child,rr0));
        vertex->add_exit_arc(arc);
        
	SafePtr<I> child_cast = dynamic_pointer_cast<I,DGVertex>(child);
	if (child_cast == 0)
	  throw std::runtime_error("DirectedGraph::recurse(const SafePtr<I>& vertex) -- dynamic cast failed, most probably this is a logic error!");
        recurse<I,RR>(child_cast);
        
      }
    };

  /// Apply RR recursively starting with vertex
  template <class RR>
    void
    DirectedGraph::recurse(const SafePtr<DGVertex>& vertex)
    {
      try { add_vertex(vertex); }
      catch (VertexAlreadyOnStack& e) { return; }
      
      typedef typename RR::TargetType TT;
      SafePtr<TT> tptr = dynamic_pointer_cast<TT,DGVertex>(vertex);
      if (tptr == 0)
        return;
      
      SafePtr<RR> rr0(new RR(tptr));
      const int num_children = rr0->num_children();
      
      for(int c=0; c<num_children; c++) {
        
        SafePtr<DGVertex> child = rr0->child(c);
        SafePtr<DGArc> arc(new DGArcRel<RR>(vertex,child,rr0));
        vertex->add_exit_arc(arc);
        
        recurse<RR>(child);
      }
    };

  /// Apply RR to all classes already on the graph
  template <class RR>
    void
    DirectedGraph::apply_to_all()
    {
      typedef typename RR::TargetType TT;
      const int num_vertices_on_graph = first_free_;
      for(int v=0; v<num_vertices_on_graph; v++) {
        if (stack_[v]->num_exit_arcs() != 0)
          continue;
        SafePtr<TT> tptr = dynamic_pointer_cast<TT,DGVertex>(stack_[v]);
        if (tptr == 0)
          continue;
      
        SafePtr<RR> rr0(new RR(tptr));
        const int num_children = rr0->num_children();
      
        for(int c=0; c<num_children; c++) {
        
          SafePtr<DGVertex> child = rr0->child(c);
          SafePtr<DGArc> arc(new DGArcRel<RR>(tptr,child,rr0));
          tptr->add_exit_arc(arc);
        
          recurse<RR>(child);
        
        }
      }
    }

  template <class RR>
    unsigned int
    DirectedGraph::num_children_on(const SafePtr<RR>& rr) const
    {
      unsigned int nchildren = rr->num_children();
      unsigned int nchildren_on_stack = 0;
      for(int c=0; c<nchildren; c++) {
        try {
          vertex_is_on(rr->rr_child(c));
        }
        catch (VertexAlreadyOnStack& a) {
          continue;
        }
        nchildren_on_stack++;
      }

      return nchildren_on_stack;
    }
    
  template <DirectedGraph::DGVertexMethodPtr method>
    void
    DirectedGraph::apply_at(const SafePtr<DGVertex>& vertex) const
    {
      ((vertex.get())->*method)();
      const unsigned int nchildren = vertex->num_exit_arcs();
      for(int c=0; c<nchildren; c++)
        apply_at<method>(vertex->exit_arc(c)->dest());
    }

  template <class Method>
    void
    DirectedGraph::foreach(Method& m)
    {
      const int num_vertices_on_graph = first_free_;
      for(int v=0; v<num_vertices_on_graph; v++) {
	m(stack_[v]);
      }
    }

};


#endif

