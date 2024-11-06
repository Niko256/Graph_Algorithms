from manim import *
import json
import networkx as nx
import math

class ConnectedComponentsVisualization(Scene):
    def construct(self):
        with open("../build/files/graph_parameters.json", "r") as file:
            graph_data = json.load(file)
            
        with open("../build/files/components.json", "r") as file:
            components_data = json.load(file)

        title = Text("Connected Components", color="#FF69B4").scale(1.2)
        self.play(Write(title))
        self.play(title.animate.to_edge(UP, buff=0.5))

        vertices = range(graph_data["vertex_count"])
        radius = 2.5
        angle = 2 * PI / len(vertices)

        vertex_objects = {}
        vertex_labels = {}
        for i in vertices:
            x = radius * math.cos(i * angle)
            y = radius * math.sin(i * angle)
            circle = Circle(radius=0.3, color=BLUE_A).move_to([x, y, 0])
            label = Text(str(i)).scale(0.8).move_to([x, y, 0])
            vertex_objects[i] = circle
            vertex_labels[i] = label

        edges = {}
        for edge in graph_data["edges"]:
            start, end = edge["from"], edge["to"]
            start_pos = vertex_objects[start].get_center()
            end_pos = vertex_objects[end].get_center()
            line = Line(start_pos, end_pos, color=BLUE_A)
            edges[(start, end)] = line

        self.play(
            *[Create(circle) for circle in vertex_objects.values()],
            *[Write(label) for label in vertex_labels.values()],
            run_time=2
        )
        self.play(*[Create(line) for line in edges.values()], run_time=1.5)

        status_text = Text("").scale(0.7)
        status_text.to_corner(DL, buff=0.5)

        colors = [RED_A, GREEN_A, YELLOW_A, PURPLE_A, GOLD_A, TEAL_A]  # Цвета для компонент
        
        for i, component in enumerate(components_data["components"]):
            component_color = colors[i % len(colors)]
            vertices_in_component = component["vertices"]
            
            self.play(
                *[vertex_objects[v].animate.set_color(component_color) for v in vertices_in_component],
                *[edge.animate.set_color(component_color) 
                  for (start, end), edge in edges.items() 
                  if start in vertices_in_component and end in vertices_in_component],
                run_time=1
            )
            
            status = Text(f"Component {i+1}: {vertices_in_component}", color=component_color).scale(0.7)
            status.to_corner(DL, buff=0.5)
            self.play(ReplacementTransform(status_text, status))
            status_text = status
            
            self.wait(0.5)

        final_status = Text(f"Found {components_data['components_count']} components!", 
                          color="#4169E1").scale(0.7)
        final_status.to_corner(DL, buff=0.5)
        self.play(ReplacementTransform(status_text, final_status))
        
        self.wait(3)
