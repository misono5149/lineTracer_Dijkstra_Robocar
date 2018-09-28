#
# Makfile
#

CC= arm-linux-gcc

OBJS= main.o uart_api.o robot_protocol.o keyboard.o


all: LineTracer_exam

LineTracer_exam: $(OBJS)
	$(CC)  -o $@ $^

clean:
	rm -f $(OBJS) LineTracer_exam
