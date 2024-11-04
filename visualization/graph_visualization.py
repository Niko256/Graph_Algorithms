from manim import *
import json
import math

class MovingVertices(Scene):
    def construct(self):
        with open('../build/files/graph_parameters.json', 'r') as file:
            graph_data = json.load(file)
            
        vertex_count = graph_data['vertex_count']
        edges = [(edge['from'], edge['to']) for edge in graph_data['edges']]
        
        title = Text("Graph Visualization", color=BLUE).scale(1.5)
        self.play(Write(title))
        self.play(title.animate.to_edge(UP))
        
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

        for v, label in zip(vertex_mobjects, vertex_labels):
            self.play(
                Create(v),
                Write(label),
                v.animate.scale(1.2).set_color(BLUE_C),
                rate_func=smooth,
                run_time=0.4  
            )
            self.play(v.animate.scale(1/1.2).set_color(BLUE_A), run_time=0.2)  # Уменьшено время анимации

        
        edge_mobjects = []
        for edge in edges:
            start_center = vertex_mobjects[edge[0]].get_center()
            end_center = vertex_mobjects[edge[1]].get_center()
    
            direction = end_center - start_center
            direction = direction / np.linalg.norm(direction)
    
            start_point = start_center + direction * 0.3
            end_point = end_center - direction * 0.3
    
            edge_mobject = Line(
                start_point, end_point,
                stroke_width=2,
                color=BLUE_B
            )
            edge_mobjects.append(edge_mobject)


        edge_groups = [edge_mobjects[i:i+3] for i in range(0, len(edge_mobjects), 3)]
        for group in edge_groups:
            self.play(
                *[Create(edge) for edge in group],
                *[Flash(edge.get_start(), color=BLUE_A, flash_radius=0.3) for edge in group],
                run_time=0.3  # Уменьшено время анимации
            )

        self.play(
            *[vertex.animate.set_color(BLUE_D) for vertex in vertex_mobjects],
            *[edge.animate.set_color(BLUE_C) for edge in edge_mobjects],
            run_time=0.5  
        )
        
        self.wait(3)  
