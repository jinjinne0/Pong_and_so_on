# Pong_and_so_on
I created two basic 2D games, "Pong", "Shooting".

# DEMO
"Pong":<p>
![demo](https://raw.github.com/wiki/jinjinne0/Pong_and_so_on/images.gif/Pong_sample.gif)

"Shooting":<p>


# Requirement
OS: macOS Big Sur version 11.2.1<p>
Programing Language: c++<p>
compiler: gcc version 10.2.0 (Homebrew GCC 10.2.0_4)<p>
→ make sure command "g++" call this compiler

# Installation
run this code in a directory you want to install this game.

git clone git@github.com:jinjinne0/Pong_and_so_on.git

# Usage
If you want to play one of two, run this code.

cd "path/to"/Pong_and_so_on/"one of them you want to play"/<p>
make<p>
./Main

If you run "make", and output error "Library not loaded....SDL..", install SDL by homebrew and try again.

"Pong": Keep a ball bouncing around on the screen with by sliding a paddle on the left of screen. The more Time Pass, the ball and paddle acceralate. If the ball go off the screen, the game will end and the points you`ve got will be displayed on terminal.

Keyboard
[A] start or restart the game<p> 
[S] pause the game<p>
[E] slide the paddle up<p>
[D] slide the paddle down<p> 

"Shooting":

# Note
It`s trainig to make me really understand single-threaded Game class,
so libraries used are just standerd c++-library and SDL2.
Still I don`t learn how to display strings on SDL window, so the point get displayed on Terminal.

# Author
zer0

# Licence
"Pong_and_so_on" is under MIT Licence.