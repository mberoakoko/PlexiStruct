//
// Created by mbero on 21/06/2025.
//

#ifndef UTILS_HPP
#define UTILS_HPP
#include <bits/stdc++.h>
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>


namespace PlexiStruct::Utils {
    inline auto release_graph_viz_context(GVC_t* context) -> void {
        gvFreeContext(context);
    }

    inline auto release_graph(Agraph_t* graph) -> void {
        agclose(graph);
    }

    const std::shared_ptr<GVC_t> GRAPH_VIZ_CONTEXT(gvContext(), release_graph_viz_context);

    enum class OutputFormat {
        PNG
    };
    class Renderer {
        std::string file_name_;
        OutputFormat outputFormat_;
    public:
        explicit Renderer(const std::string& file_name, OutputFormat output_format = OutputFormat::PNG)
        : file_name_(file_name), outputFormat_(output_format) {

        };
        static auto to_format_str(OutputFormat format) -> std::string {
            switch (format) {
                case OutputFormat::PNG: return "png";
                default: return "png";
            }
        }
        auto with(const std::shared_ptr<Agraph_t>& graph) const -> int  {
            const auto result = gvRenderFilename(GRAPH_VIZ_CONTEXT.get(), graph.get(), to_format_str(outputFormat_).c_str(), file_name_.c_str());
            if (result  == 0) {
                std::cout << "Graph successfully rendered to " << file_name_ << std::endl;
            } else {
                std::cerr << "Critical Error Failed to render to  " << file_name_ << std::endl;
            }
            return result;
        }
    };

    class TraceBuilder {

    };

    inline auto hello_world_graphs() -> void {
        if (GRAPH_VIZ_CONTEXT == nullptr) {
            std::cerr << "Fatal ERROR! graph context is null" << std::endl;
        }
        std::string name = "hello_world_graphs";
        std::shared_ptr<Agraph_t> graph(agopen(const_cast<char *>(name.c_str()), Agdirected, nullptr), release_graph);
        char temp_hello[] = "Node 1";
        char temp_workd[] = "Node 2";
        Agnode_t* node_1 = agnode(graph.get(), temp_hello, 1);
        Agnode_t* node_2 = agnode(graph.get(), temp_workd, 1);
        char shape[] = "shape";
        char box[] = "box";
        char empty[] = "";
        agsafeset(graph.get(), shape, box, empty);
        Agedge_t* edge = agedge(graph.get(), node_1, node_2, empty, 0);
        const char* output_filename = "hello_world_graph.png";
        char algo_name[] = "dot";
        gvLayout(GRAPH_VIZ_CONTEXT.get(), graph.get(), algo_name );
        Renderer renderer(output_filename);
        auto result = renderer.with(graph);

    }
}
#endif //UTILS_HPP
