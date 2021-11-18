CC = g++
RM = /bin/rm -rf

CFLAGS = -O3 -Wall

INCDIRS = -I. -I./src -I./src/vendor -I./include/vendor -I./src/algorithm -I./include/algorithm
LIBDIRS = -L.
LIBS = -lGL -lGLEW -lglut -lm -ldl

BIN = sample

OBJS = src/main.o src/camera.o src/quaternion.o src/volumerender.o src/ui.o \
		src/vendor/imgui.o src/vendor/imgui_demo.o src/vendor/imgui_draw.o \
		src/vendor/imgui_tables.o src/vendor/imgui_widgets.o src/vendor/imgui_impl_glut.o \
		src/vendor/imgui_impl_opengl3.o src/algorithm/domainsearch.o src/algorithm/node.o \
		src/arcball.o src/cube.o src/raytracing/render.o


# compile all '.o' files from their like named '.cpp' files and then link
#   them into a file name ${BIN}
${BIN} : ${OBJS}
	${CC} -g ${OBJS} ${LIBDIRS} ${LIBS} -o $@ 

.cpp.o :
	${CC} ${CFLAGS} ${INCDIRS} -c $< -o $@

# specify clobber and clean as phony so they still run even if files
#   exist with the same names
.PHONY : clean remake
clean :
	${RM} ${BIN}
	${RM} ${OBJS}

remake : clean ${BIN}

#make a list of dependencies using makedepend
depend:
	makedepend -- $(CFLAGS) -- -Y $(SRCS)
