#include <iostream>

#include "include/utils/functional_utils.hpp"
#include "include/engine/value.hpp"
#include "include/engine/utils.hpp"
auto test_inital_value( ) -> void {
    using namespace PlexiStruct;
    Engine::Operations op = Engine::Operations::DIVIDE;
    std::cout << op << std::endl;

    Engine::ScalarValue x = Engine::ScalarValue(1.0);
    Engine::ScalarValue y = Engine::ScalarValue(2.0);

    std::cout << x << std::endl;
    std::cout << y << std::endl;

    std::cout << x + y << std::endl;
    std::cout << x - y - y<< std::endl;
}

auto test_trace_builder() -> void {
    using namespace PlexiStruct;
    Engine::ScalarValue x = Engine::ScalarValue(1.0);
    Engine::ScalarValue y = Engine::ScalarValue(2.0);
    Engine::ScalarValue z = Engine::ScalarValue(2.0);
    auto result = x + (y - z);
    std::cout << result << std::endl;

    Utils::TraceBuilder trace_builder = Utils::TraceBuilder<double>::of(result);
    Utils::TraceObj<double> trace_obj = trace_builder.get_trace();
    std::cout<< "Printing Nodes \n" << std::endl;
    for (const auto& node : trace_obj.nodes) {
        std::cout << "Node \n" << std::endl;
        std::cout << node << std::endl;
        std::cout << "======================================"<< std::endl;
    }

    for (const auto& edge : trace_obj.edges) {
        std::cout << "Edge \n" << std::endl;
        std::cout << edge << std::endl;
    }
}

auto test_graph() -> void {
    using namespace PlexiStruct;
    using namespace PlexiStruct;
    auto x = Engine::ScalarValue(1.0);
    auto y = Engine::ScalarValue(2.0);
    auto z = Engine::ScalarValue(2.0);
    auto result = x + (y - z);

    Utils::ComputationGraphBuilder<double> comp_graph(result, "simple_graph");
    auto render_result  = comp_graph
        .build()
        .render("../first_computation_graph.png");

    std::cout <<"Result : " << result << std::endl;
}

auto test_graph_viz_hello_world() -> void {
    using namespace PlexiStruct;
    Utils::hello_world_graphs();
}


int main() {
    PlexiStruct::functional::test_evaluation();
    return 0;
}