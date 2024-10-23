from manim import *
import json
import math

class MovingVertices(Scene):
    def construct(self):
        with open('../build/files/graph_parameters.json', 'r') as file:
            graph_data = json.load(file)

        vertex_count = graph_data['vertex_count']
        edges = [(edge['from'], edge['to']) for edge in graph_data['edges']]

        vertices = list(range(vertex_count))

        radius = 2.5
        vertex_mobjects = [Dot(radius=0.15).move_to([radius * math.cos(2 * math.pi * i / vertex_count),
                                                     radius * math.sin(2 * math.pi * i / vertex_count),
                                                     0]).set_color(RED)
                           for i in vertices]

        vertex_labels = []
        for i in vertices:
            vertex_center = vertex_mobjects[i].get_center()
            label_position = vertex_center + 0.5 * normalize([-vertex_center[1], vertex_center[0], 0])
            label = Text(str(i)).move_to(label_position)
            vertex_labels.append(label)

        self.play(*[Create(v) for v in vertex_mobjects], *[Write(label) for label in vertex_labels])
        self.wait()

        edge_mobjects = []
        for edge in edges:
            start_vertex = vertex_mobjects[edge[0]]
            end_vertex = vertex_mobjects[edge[1]]
            edge_mobject = Line(start_vertex.get_center(), end_vertex.get_center())
            edge_mobjects.append(edge_mobject)

        self.play(*[Create(e) for e in edge_mobjects])
        self.wait(2)  

if __name__ == "__main__":
    scene = MovingVertices()
    scene.render()
