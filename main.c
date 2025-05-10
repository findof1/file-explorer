#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <assert.h>
#include <windows.h>
#include "fileUtils.h"

/*
Note:
(void) prior to certain funcions means we don't care about the return value. Reasonings:
initscr() - returns a window that is already automaticially stored at stdsrc
cbreak() - there are no error conditions
noecho() - there are no error conditions
keypad() - there are no error conditions
*/

void initNcurses()
{
  (void)initscr();
  (void)cbreak();
  (void)noecho();
}

WINDOW *createWindow()
{
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);
  WINDOW *window = newwin(yMax - 2, xMax - 2, 1, 1);
  (void)keypad(window, true);
  return window;
}

// note: cursorY can be between 0 and amountOfFiles
void drawWindow(WINDOW *window, int key, const File *files, int amountOfFiles, char *currentDirectory, int cursorY)
{
  assert(wclear(window) == OK && "wclear failed");
  assert(box(window, 0, 0) == OK && "box failed");

  if (key != -1)
  {
    assert(mvwprintw(window, 1, 2, "You Pressed Key: %d. Which is: %c", key, key) == OK && "mvwprintw failed");
  }

  assert(mvwprintw(window, 2, 2, "%s", currentDirectory) == OK && "mvwprintw failed");

  for (int i = 0; i < amountOfFiles; i++)
  {
    assert(mvwprintw(window, 4 + i, 2, "%s", files[i].filename) == OK && "mvwprintw failed");
  }

  wmove(window, cursorY + 4, 1);

  refresh();
  assert(wrefresh(window) == OK && "wrefresh failed");
}

int main()
{
  initNcurses();

  WINDOW *window = createWindow();

  const int maxFiles = 23;
  int currentFiles = 0;
  File files[maxFiles];

  char currentDirectory[MAX_PATH] = "C:\\Users\\lucas\\OneDrive\\Desktop\\Coding\\C\\file-explorer";
  currentFiles = listDirectoryContents(currentDirectory, files, maxFiles);
  assert(currentFiles != -1 && "Invalid dir passed into listDirectoryContents");

  int cursorY = 0;

  // pass in -1 for key, so it doesn't print what key you pressed.
  drawWindow(window, -1, files, currentFiles, currentDirectory, cursorY);

  int keyPressed = -1;

  do
  {
    keyPressed = wgetch(window);

    if (keyPressed == KEY_BACKSPACE)
    {
      assert(moveUpDirectory(currentDirectory) == true && "Invalid dir passed into moveUpDirectory");
      currentFiles = listDirectoryContents(currentDirectory, files, maxFiles);
      assert(currentFiles != -1 && "Invalid dir passed into listDirectoryContents");
      cursorY = 0;
    }
    if (keyPressed == KEY_UP || keyPressed == 'w')
    {
      cursorY--;
      if (cursorY < 0)
      {
        cursorY = 0;
      }
    }
    if (keyPressed == KEY_DOWN || keyPressed == 's')
    {
      cursorY++;
      if (cursorY >= currentFiles)
      {
        cursorY = currentFiles - 1;
      }
    }
    if (keyPressed == 10 && files[cursorY].isDirectory) // 10 is KEY_ENTER, ncurses KEY_ENTER is a different value that doesn't work for me
    {
      strcat(currentDirectory, "\\");
      strcat(currentDirectory, files[cursorY].filename);
      currentFiles = listDirectoryContents(currentDirectory, files, maxFiles);
      assert(currentFiles != -1 && "Invalid dir passed into listDirectoryContents");
      cursorY = 0;
    }

    drawWindow(window, keyPressed, files, currentFiles, currentDirectory, cursorY);

  } while (keyPressed != 27); // 27 is KEY_ESCAPE, but there is no macro for it in ncurses for some reason

  endwin();
  return EXIT_SUCCESS;
}