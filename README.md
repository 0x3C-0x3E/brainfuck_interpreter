# Brainfuck interpreter written in C

## Usage

To run a .bf file, just provide the filepath as the second argument

```
 > ./bf_intep ./yourfile.bf
```

## Compiling

This branch has only a single .c file so you can compile with:

```
 > gcc main.c -o bf_interp -O3
```

or whatever you want to name your interpreter

## How it works

IR stands for intermediate representation and describes a intermediate step especially in compliers, where the source code has been paresed and an AST has been constructed but there are no direct machine code instruction written.
I use an IR step only for doing optimisation because Brainfuck is a relatively simple language that only has a few keywords. So if for example the '+' operation gets called a lot of times, we can collapse these instructions down into a single operation.


