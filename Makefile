LD_FLAGS = -lmraa -lm

demo.x: Color_Sensor.c Demos.c Main.c
	gcc $^ -o $@ $LD_FLAGS
