import json
import networkx as nx
from manim import *

class GraphVisualization(Scene):
    def construct(self):
        with open("../build/files/graph_parameters.json", "r") as file:
            graph_data = json.load(file)
        
        with open("../build/files/dfs_parameters.json", "r") as file:
            dfs_parameters = json.load(file)
            start_vertex = dfs_parameters["start_vertex"]
        
        G = nx.Graph()
        for edge in graph_data["edges"]:
            G.add_edge(edge["from"], edge["to"], weight=edge["weight"])
        
        k_value = 0.8 if len(G.nodes()) <= 5 else 1.5  # Увеличиваем расстояние между вершинами, если их больше пяти
        pos = nx.spring_layout(G, scale=3, k=k_value)
        nodes = [Dot(np.array([pos[i][0], pos[i][1], 0]), color=DARK_GRAY, radius=0.2) for i in G.nodes()]
        node_labels = [Text(str(i), color=WHITE).next_to(nodes[i], UP) for i in G.nodes()]  # Метки вершин
        edges = [Line(np.array([pos[edge[0]][0], pos[edge[0]][1], 0]), np.array([pos[edge[1]][0], pos[edge[1]][1], 0]), color=WHITE) for edge in G.edges()]
        
        algorithm_title = Text("Depth-First Search (DFS)", color=BLUE).scale(1.5).move_to(ORIGIN)
        self.play(Write(algorithm_title), run_time=2)
        self.wait(1)
        self.play(FadeOut(algorithm_title), run_time=1)
        
        self.play(*[Create(node) for node in nodes], run_time=2)
        self.play(*[Write(label) for label in node_labels], run_time=2)
        self.play(*[Create(edge) for edge in edges], run_time=2)
        
        small_node_labels = [label.copy().scale(0.5).move_to(label.get_center()) for label in node_labels]
        self.play(*[Transform(label, small_label) for label, small_label in zip(node_labels, small_node_labels)], run_time=1)
        
        edge_indices = {(edge[0], edge[1]): i for i, edge in enumerate(G.edges())}
        edge_indices.update({(edge[1], edge[0]): i for i, edge in enumerate(G.edges())})
        
        dfs_edges = list(nx.dfs_edges(G, source=start_vertex))
        visited = set()
        step_description = Text("").to_edge(DOWN)
        
        for i, edge in enumerate(dfs_edges):
            edge_index = edge_indices[(edge[0], edge[1])]
            self.play(Indicate(edges[edge_index], color=YELLOW), run_time=1)
            
            if edge[0] not in visited:
                visited.add(edge[0])
                self.play(nodes[edge[0]].animate.set_color(GREEN), run_time=1)
                step_description.become(Text(f"Step {i+1}: Visit node {edge[0]}", color=GREEN).to_edge(DOWN))
                self.play(Write(step_description), run_time=1)
            
            if edge[1] not in visited:
                visited.add(edge[1])
                self.play(nodes[edge[1]].animate.set_color(GREEN), run_time=1)
                step_description.become(Text(f"Step {i+1}: Visit node {edge[1]}", color=GREEN).to_edge(DOWN))
                self.play(Write(step_description), run_time=1)
        
        for node in nodes:
            self.play(node.animate.set_color(BLUE), run_time=1)
        
        self.wait(2)

if __name__ == "__main__":
    import manim.__main__
    manim.__main__.main(["-pql", __file__, "GraphVisualization"])
