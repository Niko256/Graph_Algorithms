from manim import *
import json
import networkx as nx
import math

class DijkstraVisualization(Scene):
    def construct(self):
        with open("../build/files/graph_parameters.json", "r") as file:
            graph_data = json.load(file)
        
        with open("../build/files/dijkstra_parameters.json", "r") as file:
            dijkstra_parameters = json.load(file)
            start_vertex = dijkstra_parameters["start_vertex"]
            
        with open("../build/files/dijkstra_results.json", "r") as file:
            dijkstra_results = json.load(file)

        title = Text("Dijkstra's Algorithm", color="#FF69B4").scale(1.2)
        self.play(Write(title))
        self.play(title.animate.to_edge(UP, buff=0.5))

        vertices = range(graph_data["vertex_count"])
        radius = 2.5
        angle = 2 * PI / len(vertices)

        vertex_objects = {}
        vertex_labels = {}
        distance_labels = {}  
        
        for i in vertices:
            pos = [radius * math.cos(angle * i), radius * math.sin(angle * i), 0]
            circle = Circle(radius=0.3, color=WHITE)
            circle.set_fill("#40E0D0", opacity=0.5)
            circle.move_to(pos)
            
            vertex_label = Text(str(i)).scale(0.8).move_to(pos)
            
            distance = dijkstra_results["distances"].get(str(i), "∞")
            distance_label = Text(f"d={distance}").scale(0.6)
            distance_label.next_to(circle, DOWN, buff=0.1)
            
            vertex_objects[i] = circle
            vertex_labels[i] = vertex_label
            distance_labels[i] = distance_label

        edges = {}
        for edge in graph_data["edges"]:
            start = vertex_objects[edge["from"]].get_center()
            end = vertex_objects[edge["to"]].get_center()
            line = Line(start, end, color="#B0C4DE")
            edges[(edge["from"], edge["to"])] = line
            
            weight_label = Text(str(edge["weight"])).scale(0.6)
            weight_label.move_to((start + end) / 2)
            edges[(edge["from"], edge["to"], "weight")] = weight_label

        self.play(
            *[Create(circle) for circle in vertex_objects.values()],
            *[Write(label) for label in vertex_labels.values()],
            run_time=2
        )
        self.play(
            *[Create(edge) for edge in edges.values() if not isinstance(edge, Text)],
            *[Write(edge) for edge in edges.values() if isinstance(edge, Text)],
            run_time=1.5
        )
        self.play(*[Write(label) for label in distance_labels.values()])

        legend = VGroup(
            Dot(color="#40E0D0").scale(1.5), Text("Unvisited", color="#40E0D0"),
            Dot(color="#FF69B4").scale(1.5), Text("Processing", color="#FF69B4"),
            Dot(color="#4169E1").scale(1.5), Text("Processed", color="#4169E1")
        ).arrange(RIGHT, buff=0.3).scale(0.8)
        legend.to_corner(DR, buff=0.5)
        self.play(Create(legend))

        for vertex_id, path in dijkstra_results["paths"].items():
            if len(path) > 1:
                # Подсвечиваем путь
                path_edges = []
                for i in range(len(path) - 1):
                    edge = edges.get((path[i], path[i+1]), edges.get((path[i+1], path[i])))
                    if edge:
                        path_edges.append(edge)
                
                self.play(
                    *[edge.animate.set_color("#FF69B4").set_stroke(width=4) for edge in path_edges],
                    vertex_objects[int(vertex_id)].animate.set_color("#4169E1"),
                    run_time=0.8
                )
                
                new_distance = Text(
                    f"d={dijkstra_results['distances'][vertex_id]}", 
                    color="#4169E1"
                ).scale(0.6)
                new_distance.move_to(distance_labels[int(vertex_id)])
                self.play(
                    Transform(distance_labels[int(vertex_id)], new_distance),
                    run_time=0.5
                )

        final_status = Text("Shortest paths found!", color="#4169E1").scale(0.7)
        final_status.to_corner(DL, buff=0.5)
        self.play(Write(final_status))
        
        self.wait(3)
