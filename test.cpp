#include "dfs.h"
#include "bfs.h"
#include "node.h"
#include "search.h"

#include <cpplinq.hpp>

#include <iostream> //tmp

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

namespace {
typedef node<std::string> string_node;
string_node::connection_container_type example_problem() {
//           +–+                   
//       +–––+A+–––+               
//       |   +++   |               
//       |    |    |               
//       |    |    |               
//      +++  +++  +++              
//  +–––+B|  |C|  |E+–––+          
//  |   +++  +++  +++   |          
//  |    |    |    |    |          
// +++  +++  +++   |   ++–+    +––+
// |D|  |F|  |G|   |   |E1+––––+E2|
// +–+  +++  +–+   |   ++–+    ++–+
//       |         |    |       |  
//       +–––––––––+    |  +––+ |  
//                      +––+E3+–+  
//                         +––+    
	auto print_destructor_id = [](std::string const& id) {
		std::cout << "~" << id << std::endl;
	};

    auto A( string_node::create("A",print_destructor_id) );
    
    auto B = A->add("B");
    auto C = A->add("C");
    auto E = A->add("E");

    auto D = B->add("D");
    auto F = B->add("F");
    auto G = C->add("G");
    F->add(E);

    auto E1 = E->add("E1");
    auto E2 = E1->add("E2");
    auto E3 = E2->add("E3");
    E3->add(E1);

    return {
        A,B,C,E,D,F,G,E1,E2,E3
    };
}
}

TEST_CASE("not a test yet") {
    auto graph( example_problem() );
    auto separate=[]{ std::cout<<"----"<<std::endl; };
    
    auto predicate=[](string_node::connection_ptr v){ 
        std::cout<< v->get_id() <<std::endl;
        return false;
    };

    visit< dfs_strategy<string_node::connection_ptr> >(graph.front(),predicate);
    separate();
    visit< bfs_strategy<string_node::connection_ptr> >(graph.front(),predicate);
    separate();
}
