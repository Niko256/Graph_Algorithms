from manim import *
import sys

class GraphVisualization(Scene):
    def construct(self):
        if len(sys.argv) < 2:
            print("Usage: python visualize.py <logfile>")
            return

        logfile = sys.argv[1]
        vertices = {}
        edges = []
        steps = []

        with open(logfile, 'r') as file:
            for line in file:
                steps.append(line.strip())

        for step in steps:
            self.play(Write(Text(step).shift(UP * 3)))
            self.wait(1)

        self.wait(2)

if __name__ == "__main__":
    GraphVisualization().render()
