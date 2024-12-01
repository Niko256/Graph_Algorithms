from manim import *
import json
import math

class GraphVisualizationStatic(Scene):
    def construct(self):
        with open('../build/files/graph_parameters.json', 'r') as file:
            graph_data = json.load(file)
            
        vertex_count = graph_data['vertex_count']
        edges = [(edge['from'], edge['to']) for edge in graph_data['edges']]
        
        vertices = list(range(vertex_count))
        radius = 2.5
        vertex_mobjects = [
            Circle(radius=0.3, color=BLUE_A)
            .set_fill(BLUE, opacity=0.5)
            .move_to([
                radius * math.cos(2 * math.pi * i / vertex_count),
                radius * math.sin(2 * math.pi * i / vertex_count),
                0
            ]) for i in vertices
        ]

        vertex_labels = []
        for i in vertices:
            vertex_center = vertex_mobjects[i].get_center()
            label = (
                Text(str(i))
                .scale(0.8)
                .set_color(WHITE)
                .move_to(vertex_center)
            )
            vertex_labels.append(label)

        edge_mobjects = []
        for edge in edges:
            start_center = vertex_mobjects[edge[0]].get_center()
            end_center = vertex_mobjects[edge[1]].get_center()
            edge_mobject = Line(
                start_center, end_center,
                stroke_width=2,
                color=BLUE_B
            )
            edge_mobjects.append(edge_mobject)

        for edge in edge_mobjects:
            self.add(edge)
        for vertex, label in zip(vertex_mobjects, vertex_labels):
            self.add(vertex)
            self.add(label)

        self.wait(0.1)
