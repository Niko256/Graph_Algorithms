from manim import *
import os

class GraphVisualization(Scene):
    def construct(self):
        log_file = os.path.join("build", "algorithm_log.txt")
        with open(log_file, "r") as f:
            lines = f.readlines()

        vertices = {}
        edges = []
        step_description = ""
        algorithm_name = ""

        for line in lines:
            if line.startswith("Step:"):
                step_description = line.strip().split(": ")[1]
                algorithm_name = step_description.split(" - ")[0]
                self.wait(1)
                self.clear()
                self.add(Text(step_description).to_edge(UP))
            elif line.startswith("Vertex Count:"):
                pass
            elif line.startswith("Directed:"):
                pass
            elif line.startswith("Vertex "):
                parts = line.strip().split(" -> ")
                vertex = int(parts[0].split(" ")[1])
                edges_str = parts[1].strip().split(" ")
                for edge_str in edges_str:
                    if edge_str:
                        edge = eval(edge_str)
                        edges.append((vertex, edge[0], edge[1]))
            elif line.startswith("----------------------------------------"):
                self.draw_graph(vertices, edges)
                vertices.clear()
                edges.clear()

    def draw_graph(self, vertices, edges):
        vertex_mobjects = {v: Dot(point=np.array([v * 2 - 5, 0, 0])) for v in vertices}
        edge_mobjects = []

        for edge in edges:
            start, end, weight = edge
            if start not in vertex_mobjects:
                vertex_mobjects[start] = Dot(point=np.array([start * 2 - 5, 0, 0]))
            if end not in vertex_mobjects:
                vertex_mobjects[end] = Dot(point=np.array([end * 2 - 5, 0, 0]))

            edge_line = Line(vertex_mobjects[start].get_center(), vertex_mobjects[end].get_center())
            edge_mobjects.append(edge_line)
            self.add(edge_line)

        for vertex, dot in vertex_mobjects.items():
            self.add(dot)
            self.add(Text(str(vertex)).next_to(dot, DOWN))

        self.wait(1)

if __name__ == "__main__":
    scene = GraphVisualization()
    scene.render()
