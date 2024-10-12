import subprocess
import os

def run_command(command):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    if process.returncode != 0:
        print(f"Command failed: {command}")
        print(stderr.decode())
        exit(1)
    return stdout.decode()

def main():
    # Создаем директорию build, если она не существует
    if not os.path.exists("build"):
        os.makedirs("build")

    # Переходим в директорию build
    os.chdir("build")

    # Запускаем CMake
    run_command("cmake ..")

    # Запускаем make
    run_command("make")

    # Запускаем исполняемый файл для генерации логов
    run_command("./Graphs")

    # Создаем виртуальное окружение внутри директории build, если оно не существует
    if not os.path.exists("myenv"):
        run_command("python -m venv myenv")

    # Активируем виртуальное окружение
    activate_script = os.path.join("myenv", "bin", "activate")
    run_command(f"source {activate_script}")

    # Устанавливаем Manim в виртуальное окружение
    run_command("pip install manim")

    # Возвращаемся в корневую директорию
    os.chdir("..")

    # Запускаем Manim для визуализации
    run_command("build/myenv/bin/manim -pql visualization/visualize.py GraphVisualization")

if __name__ == "__main__":
    main()
