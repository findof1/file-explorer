#include <stdio.h>
#include <ncurses.h>

int main()
{
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  printw("Hello, explorer!");
  refresh();
  getch();

  endwin(); // End ncurses mode
  return 0;
}