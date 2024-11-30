from manim import *
import json
import networkx as nx
import math

class ShortestPathsUnweightedVisualization(Scene):
    def construct(self):
        with open("../build/files/graph_parameters.json", "r") as file:
            graph_data = json.load(file)
            
        with open("../build/files/shortest_paths_unweighted.json", "r") as file:
            paths_data = json.load(file)

        title = Text("Unweighted Shortest Paths", color="#FF69B4").scale(1.2)
        self.play(Write(title))
        self.play(title.animate.to_edge(UP, buff=0.5))

        vertices = range(graph_data["vertex_count"])
        radius = 2.5
        angle = 2 * PI / len(vertices)

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
            line = Line(start, end, color="#B0C4DE")
            edges[(edge["from"], edge["to"])] = line

        self.play(
            *[Create(circle) for circle in vertex_objects.values()],
            *[Write(label) for label in vertex_labels.values()],
            run_time=2
        )
        self.play(*[Create(line) for line in edges.values()], run_time=1.5)

        legend = VGroup(
            Dot(color="#40E0D0").scale(1.5), Text("Unvisited", color="#40E0D0"),
            Dot(color="#FF69B4").scale(1.5), Text("Current Path", color="#FF69B4"),
            Dot(color="#4169E1").scale(1.5), Text("Shortest Path", color="#4169E1")
        ).arrange(RIGHT, buff=0.3).scale(0.8)
        legend.to_corner(DR, buff=0.5)
        self.play(Create(legend))

        status_text = Text("").scale(0.7)
        status_text.to_corner(DL, buff=0.5)

        for vertex_id, path in paths_data["paths"].items():
            if len(path) > 0:  # Если путь существует
                distance = paths_data["distances"][vertex_id]
                
                path_edges = []
                for i in range(len(path) - 1):
                    edge = edges.get((path[i], path[i+1]), edges.get((path[i+1], path[i])))
                    if edge:
                        path_edges.append(edge)
                
                self.play(
                    *[vertex_objects[v].animate.set_color("#FF69B4") for v in path],
                    *[edge.animate.set_color("#FF69B4").set_stroke(width=4) for edge in path_edges],
                    run_time=0.8
                )
                
                new_status = Text(f"d({vertex_id}) = {distance}", 
                                color="#4169E1").scale(0.7)
                new_status.to_corner(DL, buff=0.5)
                self.play(ReplacementTransform(status_text, new_status))
                status_text = new_status
                
                self.wait(0.5)
                
                self.play(
                    *[vertex_objects[v].animate.set_color("#4169E1") for v in path],
                    *[edge.animate.set_color("#4169E1").set_stroke(width=2) for edge in path_edges],
                    run_time=0.5
                )

        self.wait(3)
