


#ifndef _libint2_src_bin_libint_dg_h_
#define _libint2_src_bin_libint_dg_h_

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <assert.h>
#include <rr.h>

using namespace std;


namespace libint2 {

  /** DirectedGraph is an implementation of a directed graph
      composed of vertices represented by DGVertex objects. The objects
      are allocated on free store and the graph is implemented as
      vector<DGVertex*>.
  */
  
  class DirectedGraph {

    vector<DGVertex*> stack_;

    static const unsigned int default_size_ = 10000;
    unsigned int first_free_;

    // adds a vertex to the graph
    void add_vertex(DGVertex*);
    /** This function is used to implement (recursive) append_target().
        vertex is appended to the graph and then RR is applied to is.
     */
    template <class I, class RR> void recurse(I* vertex);


  public:
    /** This constructor doesn't do much. Actual initialization of the graph
        must be done using append_target */
    DirectedGraph();
    ~DirectedGraph();

    /** append_target appends I to the graph as a target vertex and applies
        RR to it. append_target can be called multiple times on the same
        graph if more than one target vertex is needed.

        I must derive from DGVertex. RR must derive from RecurrenceRelation.
        RR has a constructor which takes const I& as the only argument.
        RR must have a public member const I* child(unsigned int) .

        NOTE TO SELF : need to implement these restrictions using
        standard Bjarne Stroustrup's approach.

    */
    template <class I, class RR> void append_target(I*);

  };

  /// Apply RR to target
  template <class I, class RR>
    void
    DirectedGraph::append_target(I* target)
    {
      target->make_a_target();
      recurse<I,RR>(target);
    };

  /// Apply RR to target
  template <class I, class RR>
    void
    DirectedGraph::recurse(I* vertex)
    {
      add_vertex(vertex);
      
      RR* rr0 = new RR(vertex);
      const int num_children = rr0->num_children();
      
      for(int c=0; c<num_children; c++) {
        
        DGVertex* child = rr0->child(c);
        add_vertex(child);
        
        DGArc* arc = new DGArcRel<RR>(vertex,child,rr0);
        vertex->add_exit_arc(arc);
        
        recurse<I,RR>(static_cast<I*>(child));
        
      }
    };

};


#endif
