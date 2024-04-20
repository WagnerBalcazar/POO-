from turtle import *
from colorsys import hsv_to_rgb

def draw_colorful_spiral():
    bgcolor("black")
    tracer(2)
    pensize(2)
    h = 0  

    for i in range(180):
        c = hsv_to_rgb(h, 1, 1)
        color(c)
        
        up()
        goto(-8, 25)
        down()
        forward(i)
        
        right(89)
        fillcolor(c)
        begin_fill()
        circle(90, 100)
        end_fill()
        
        left(179)
        back(i/2)
        left(6)
        
        h += 0.005

def main():
    draw_colorful_spiral()
    done()

if __name__ == "__main__":
    main()

