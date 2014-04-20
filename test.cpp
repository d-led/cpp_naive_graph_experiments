#include "dfs.h"
#include "bfs.h"
#include "node.h"
#include "search.h"

#include <cpplinq.hpp>

#include <vector>
#include <algorithm>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

namespace {
    typedef node<std::string> string_node;

    template<typename OnDelete>
    string_node::connection_container_type example_problem(OnDelete on_delete) {
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

        auto A(string_node::create("A", on_delete));

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
            A, B, C, E, D, F, G, E1, E2, E3
        };
    }
}

TEST_CASE("cpplinq sanity check") {
    using namespace cpplinq;

    int seq1[] = { 1, 2, 3 };
    std::vector<int> seq2 = { 1, 2, 3 };
    bool cpplinq_works = from_array(seq1) >> sequence_equal(from(seq2));
    REQUIRE(cpplinq_works);
}

TEST_CASE("full traversal") {
    using namespace cpplinq;
    SECTION("testing for memory leaks") {
        typedef std::vector<std::string> strings;
        strings deleted,expected;
        {
            auto graph(example_problem([&deleted](std::string const& id) {
                deleted.push_back(id);
            }));

            expected = from(graph)
            >> select([](string_node::connection_ptr node) {
                return node->get_id();
            })
            >> to_vector();
            std::sort(expected.begin(), expected.end());
            REQUIRE(!expected.empty());

            SECTION("dfs") {
                strings visited;
                visit< dfs_strategy<string_node::connection_ptr> >(graph.front(),
                    [&visited](string_node::connection_ptr v){
                    visited.push_back(v->get_id());
                    return false;
                });
                std::sort(visited.begin(), visited.end());
                CHECK((from(visited) >> sequence_equal(from(expected))));
            }

            SECTION("bfs") {
                strings visited;
                visit< bfs_strategy<string_node::connection_ptr> >(graph.front(),
                    [&visited](string_node::connection_ptr v){
                    visited.push_back(v->get_id());
                    return false;
                });
                std::sort(visited.begin(), visited.end());
                CHECK((from(visited) >> sequence_equal(from(expected))));
            }
        }
        REQUIRE(!deleted.empty());
        std::sort(deleted.begin(), deleted.end());
        CHECK((from(deleted) >> sequence_equal(from(expected))));
    }
}
