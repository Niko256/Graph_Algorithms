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

        weights_table = VGroup()
        weights_table_title = Text("Edge Weights:", color="#4169E1").scale(0.8)
        weights_table.add(weights_table_title)
        
        for edge in graph_data["edges"]:
            edge_text = Text(
                f"{edge['from']} → {edge['to']}: {edge['weight']}", 
                color="#B0C4DE"
            ).scale(0.6)
            weights_table.add(edge_text)
        
        weights_table.arrange(DOWN, aligned_edge=LEFT, buff=0.2)
        weights_table.to_corner(LEFT + UP, buff=0.5)
        self.play(Create(weights_table))

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
            distance_pos = [pos[0], pos[1] + 0.5, 0]  
            distance_label = Text("∞").scale(0.6).move_to(distance_pos)
            
            vertex_objects[i] = circle
            vertex_labels[i] = vertex_label
            distance_labels[i] = distance_label

        edges = {}
        for edge in graph_data["edges"]:
            start = vertex_objects[edge["from"]].get_center()
            end = vertex_objects[edge["to"]].get_center()
            line = Line(start, end, color="#B0C4DE")
            edges[(edge["from"], edge["to"])] = line

        self.play(
            *[Create(circle) for circle in vertex_objects.values()],
            *[Write(label) for label in vertex_labels.values()],
            *[Write(label) for label in distance_labels.values()],
            run_time=2
        )
        self.play(
            *[Create(line) for line in edges.values()],
            run_time=1.5
        )

        legend = VGroup(
            Dot(color="#40E0D0").scale(1.5), Text("Unvisited", color="#40E0D0"),
            Dot(color="#FF69B4").scale(1.5), Text("Current", color="#FF69B4"),
            Dot(color="#4169E1").scale(1.5), Text("Visited", color="#4169E1")
        ).arrange(RIGHT, buff=0.3).scale(0.8)
        legend.to_corner(DR, buff=0.5)
        self.play(Create(legend))

        current_path_text = Text("").scale(0.7)
        current_path_text.to_edge(DOWN, buff=0.5)
        
        self.play(
            vertex_objects[start_vertex].animate.set_color("#FF69B4"),
            Transform(
                distance_labels[start_vertex],
                Text("0").scale(0.6).move_to(distance_labels[start_vertex].get_center())
            )
        )

        for vertex_id, paths in dijkstra_results["paths"].items():
            vertex_id = int(vertex_id)
            if vertex_id != start_vertex:
                path = paths
                distance = dijkstra_results["distances"][str(vertex_id)]
        
                path_str = f"Path to {vertex_id}: {' → '.join(map(str, path))} (Total: {distance})"
                new_path_text = Text(path_str, color="#4169E1").scale(0.7)
                new_path_text.to_edge(DOWN, buff=0.5)
        
                self.play(Transform(current_path_text, new_path_text))
        
                for i in range(len(path) - 1):
                    current, next_vertex = path[i], path[i + 1]
                    edge = edges.get((current, next_vertex), edges.get((next_vertex, current)))
            
                    try:
                        weight_index = next(
                            i for i, item in enumerate(weights_table) 
                            if str(current) in item.text and str(next_vertex) in item.text
                        )
                
                        self.play(
                            weights_table[weight_index].animate.set_color("#FF69B4"),
                            vertex_objects[next_vertex].animate.set_color("#FF69B4"),
                            edge.animate.set_color("#FF69B4").set_stroke(width=4),
                            Transform(
                                distance_labels[next_vertex],
                                Text(str(distance)).scale(0.6).move_to(distance_labels[next_vertex].get_center())
                            ),
                            run_time=0.8
                        )
                
                        self.play(
                            weights_table[weight_index].animate.set_color("#4169E1"),
                            vertex_objects[current].animate.set_color("#4169E1"),
                            run_time=0.5
                        )
                    except StopIteration:
                        self.play(
                            vertex_objects[next_vertex].animate.set_color("#FF69B4"),
                            edge.animate.set_color("#FF69B4").set_stroke(width=4),
                            Transform(
                                distance_labels[next_vertex],
                                Text(str(distance)).scale(0.6).move_to(distance_labels[next_vertex].get_center())
                            ),
                            run_time=0.8
                        )
                
                        self.play(
                            vertex_objects[current].animate.set_color("#4169E1"),
                            run_time=0.5
                        )

            self.wait(3)
