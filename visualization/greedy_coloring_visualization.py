from manim import *
import json
import math

class GreedyColoringVisualization(Scene):
    def construct(self):
        with open("../build/files/graph_parameters.json", "r") as file:
            graph_data = json.load(file)
            
        with open("../build/files/greedy_coloring_results.json", "r") as file:
            coloring_data = json.load(file)

        # Заголовок
        title = Text("Greedy Graph Coloring", color="#FF69B4").scale(1.2)
        self.play(Write(title))
        self.play(title.animate.to_edge(UP, buff=0.5))

        vertex_colors = ["#FF6B6B", "#4ECDC4", "#45B7D1", "#96CEB4", 
                        "#FFEEAD", "#D4A5A5", "#9B59B6", "#3498DB",
                        "#F1C40F", "#1ABC9C"]

        vertices = range(graph_data["vertex_count"])
        radius = 2.5
        angle = 2 * PI / len(vertices)

        vertex_objects = {}
        vertex_labels = {}
        
        for i in vertices:
            x = radius * math.cos(i * angle)
            y = radius * math.sin(i * angle)
            circle = Circle(radius=0.3, color=WHITE)
            circle.set_fill("#40E0D0", opacity=0.5)
            circle.move_to([x, y, 0])
            label = Text(str(i)).scale(0.8).move_to([x, y, 0])
            vertex_objects[i] = circle
            vertex_labels[i] = label

        edges = {}
        for edge in graph_data["edges"]:
            start_pos = vertex_objects[edge["from"]].get_center()
            end_pos = vertex_objects[edge["to"]].get_center()
            line = Line(start_pos, end_pos, color="#B0C4DE")
            edges[(edge["from"], edge["to"])] = line

        self.play(
            *[Create(circle) for circle in vertex_objects.values()],
            *[Write(label) for label in vertex_labels.values()],
            run_time=2
        )
        self.play(*[Create(line) for line in edges.values()], run_time=1.5)

        legend = VGroup()
        used_colors = set()
        for vertex_id, color_index in coloring_data["coloring"].items():
            used_colors.add(color_index)
        
        for i, color_index in enumerate(sorted(used_colors)):
            color = vertex_colors[color_index % len(vertex_colors)]
            dot = Dot(color=color).scale(1.5)
            text = Text(f"Color {color_index}", color=color).scale(0.6)
            legend.add(VGroup(dot, text).arrange(RIGHT, buff=0.2))
        
        legend.arrange(DOWN, aligned_edge=LEFT, buff=0.2)
        legend.to_corner(DR, buff=0.5)
        self.play(Create(legend))

        status_text = Text("").scale(0.7)
        status_text.to_corner(DL, buff=0.5)

        for vertex_id, color_index in coloring_data["coloring"].items():
            vertex_id = int(vertex_id)
            color = vertex_colors[color_index % len(vertex_colors)]
            
            neighbors = [edge["from"] for edge in graph_data["edges"] if edge["from"] == vertex_id or edge["to"] == vertex_id] + [edge["to"] for edge in graph_data["edges"] if edge["from"] == vertex_id or edge["to"] == vertex_id]
            
            self.play(
                vertex_objects[vertex_id].animate.set_fill(color, opacity=0.8),
                vertex_objects[vertex_id].animate.set_stroke(color),
                run_time=0.8
            )
            
            status = Text(f"Coloring vertex {vertex_id} with color {color_index}", 
                         color=color).scale(0.7)
            status.to_corner(DL, buff=0.5)
            self.play(ReplacementTransform(status_text, status))
            status_text = status
            
            self.wait(0.3)

        final_status = Text(f"Graph colored using {len(used_colors)} colors!", 
                          color="#4169E1").scale(0.7)
        final_status.to_corner(DL, buff=0.5)
        self.play(ReplacementTransform(status_text, final_status))
        
        self.wait(3)
