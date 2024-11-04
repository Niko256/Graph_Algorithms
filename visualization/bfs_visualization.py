from manim import *
import json
import networkx as nx
import math

class BFSVisualization(Scene):
    def construct(self):
        with open("../build/files/graph_parameters.json", "r") as file:
            graph_data = json.load(file)
        
        with open("../build/files/bfs_parameters.json", "r") as file:
            bfs_parameters = json.load(file)
            start_vertex = bfs_parameters["start_vertex"]

        vertices = range(graph_data["vertex_count"])
        radius = 2.5
        angle = 2 * PI / len(vertices)

        title = Text("Breadth-First Search", color="#FF69B4").scale(1.2)  # Малиновый
        self.play(Write(title))
        self.play(title.animate.to_edge(UP, buff=0.5))

        vertex_objects = {}
        vertex_labels = {}
        for i in vertices:
            pos = [radius * math.cos(angle * i), radius * math.sin(angle * i), 0]
            circle = Circle(radius=0.3, color=WHITE)
            circle.set_fill("#40E0D0", opacity=0.5)  
            circle.move_to(pos)
            label = Text(str(i)).scale(0.8).move_to(pos)
            vertex_objects[i] = circle
            vertex_labels[i] = label

        edges = {}
        for edge in graph_data["edges"]:
            start = vertex_objects[edge["from"]].get_center()
            end = vertex_objects[edge["to"]].get_center()
            line = Line(start, end, color="#B0C4DE")  # Светло-стальной синий
            edges[(edge["from"], edge["to"])] = line

        self.play(
            *[Create(circle) for circle in vertex_objects.values()],
            *[Write(label) for label in vertex_labels.values()],
            run_time=2
        )
        self.play(*[Create(line) for line in edges.values()], run_time=1.5)

        legend = VGroup(
            Dot(color="#40E0D0").scale(1.5), Text("Unvisited", color="#40E0D0"),
            Dot(color="#FF69B4").scale(1.5), Text("Current", color="#FF69B4"),
            Dot(color="#4169E1").scale(1.5), Text("Visited", color="#4169E1")
        ).arrange(RIGHT, buff=0.3).scale(0.8)
        legend.to_corner(DR, buff=0.5)  
        self.play(Create(legend))

        G = nx.Graph()
        for edge in graph_data["edges"]:
            G.add_edge(edge["from"], edge["to"])

        bfs_path = list(nx.bfs_edges(G, source=start_vertex))
        visited = set([start_vertex])
        queue = [start_vertex]

        status_text = Text("").scale(0.7)
        status_text.to_corner(DL, buff=0.5)
        self.play(vertex_objects[start_vertex].animate.set_color("#FF69B4"))

        while queue:
            current = queue.pop(0)
            neighbors = [v for _, v in bfs_path if _ == current and v not in visited]
            
            if neighbors:
                for next_vertex in neighbors:
                    queue.append(next_vertex)
                    visited.add(next_vertex)
                    
                    edge_highlight = edges.get((current, next_vertex), 
                                            edges.get((next_vertex, current)))
                    
                    self.play(
                        vertex_objects[current].animate.set_color("#4169E1"),
                        vertex_objects[next_vertex].animate.set_color("#FF69B4"),
                        edge_highlight.animate.set_color("#FF69B4").set_stroke(width=4),
                        run_time=0.8
                    )
                    
                    status = Text(f"Exploring vertex {next_vertex}", color="#4169E1").scale(0.7)
                    status.to_corner(DL, buff=0.5)
                    self.play(ReplacementTransform(status_text, status))
                    status_text = status
            else:
                self.play(
                    vertex_objects[current].animate.set_color("#4169E1"),
                    run_time=0.8
                )

        final_status = Text("BFS Complete!", color="#4169E1").scale(0.7)
        final_status.to_corner(DL, buff=0.5)
        self.play(
            ReplacementTransform(status_text, final_status),
            *[vertex_objects[v].animate.set_color("#4169E1") for v in vertices],
            *[edge.animate.set_color("#40E0D0") for edge in edges.values()]
        )
        
        self.wait(3)
