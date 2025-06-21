#include <iostream>

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

auto test_graph_viz_hello_world() -> void {
    using namespace PlexiStruct;
    Utils::hello_world_graphs();
}

int main() {
    test_graph_viz_hello_world();
    return 0;
}