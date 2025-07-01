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

Brainfuck is a relatively simple language that only has a few keywords. And if for example the '+' operation gets called a lot of times, we can collapse these instructions down into a single operation in an IR step.
