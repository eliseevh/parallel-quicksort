FROM opencilk:v1.0

# Install gdb for debugging
RUN apt update -y
RUN apt install gdb -y
