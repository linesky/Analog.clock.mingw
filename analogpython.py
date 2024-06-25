import tkinter as tk
import math
import time

class AnalogClock:
    def __init__(self, root):
        self.root = root
        self.root.title("Analog Clock")
        self.root.configure(bg='yellow')

        self.canvas = tk.Canvas(self.root, width=400, height=400, bg='yellow')
        self.canvas.pack()

        self.center_x = 200
        self.center_y = 200
        self.radius = 100

        self.update_clock()

    def update_clock(self):
        self.canvas.delete("hands")
        self.draw_face()
        self.draw_hands()
        self.root.after(1000, self.update_clock)

    def draw_face(self):
        self.canvas.create_oval(self.center_x - self.radius, self.center_y - self.radius,
                                self.center_x + self.radius, self.center_y + self.radius,
                                outline='black', width=2)

        for i in range(12):
            angle = math.pi / 6 * i
            x_start = self.center_x + self.radius * 0.8 * math.sin(angle)
            y_start = self.center_y - self.radius * 0.8 * math.cos(angle)
            x_end = self.center_x + self.radius * 0.9 * math.sin(angle)
            y_end = self.center_y - self.radius * 0.9 * math.cos(angle)
            self.canvas.create_line(x_start, y_start, x_end, y_end, fill='black', width=2)

    def draw_hands(self):
        current_time = time.localtime()
        hours = current_time.tm_hour % 12
        minutes = current_time.tm_min
        seconds = current_time.tm_sec

        hour_angle = math.pi / 6 * hours + math.pi / 360 * minutes
        minute_angle = math.pi / 30 * minutes
        second_angle = math.pi / 30 * seconds

        self.draw_hand(hour_angle, self.radius * 0.5, 6)
        self.draw_hand(minute_angle, self.radius * 0.8, 4)
        self.draw_hand(second_angle, self.radius * 0.9, 2, color='red')

    def draw_hand(self, angle, length, width, color='black'):
        x_end = self.center_x + length * math.sin(angle)
        y_end = self.center_y - length * math.cos(angle)
        self.canvas.create_line(self.center_x, self.center_y, x_end, y_end, fill=color, width=width, tags="hands")

if __name__ == "__main__":
    root = tk.Tk()
    clock = AnalogClock(root)
    root.mainloop()

