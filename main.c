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
void drawWindow(WINDOW *window, const File *files, int amountOfFiles, char *currentDirectory, int cursorY)
{
  assert(wclear(window) == OK && "wclear failed");
  assert(box(window, 0, 0) == OK && "box failed");

  assert(mvwprintw(window, 1, 2, "%s", currentDirectory) == OK && "mvwprintw failed");

  for (int i = 0; i < amountOfFiles; i++)
  {
    assert(mvwprintw(window, 3 + i, 2, "%s", files[i].filename) == OK && "mvwprintw failed");
  }

  wmove(window, cursorY + 3, 1);

  refresh();
  assert(wrefresh(window) == OK && "wrefresh failed");
}

void updateFileVariables(int *filesInCurrentDirectory, int *currentFiles, File *files, const int maxFiles, const int scrollPosition, char *currentDirectory)
{
  *filesInCurrentDirectory = getDirectoryFileAmount(currentDirectory);
  assert(*filesInCurrentDirectory != -1 && "Invalid dir passed into getDirectoryFileAmount");

  *currentFiles = listDirectoryContents(currentDirectory, files, maxFiles, scrollPosition * (maxFiles + 1));
  assert(*currentFiles != -1 && "Invalid dir passed into listDirectoryContents");
}

int main()
{
  initNcurses();

  int scrollPosition = 0;
  int cursorY = 0;

  const int maxFiles = 23;
  File files[maxFiles];
  int currentFiles = 0;
  int filesInCurrentDirectory = 0;

  char currentDirectory[MAX_PATH] = "C:\\Users\\lucas\\OneDrive\\Desktop\\Coding\\C\\file-explorer";

  WINDOW *window = createWindow();

  updateFileVariables(&filesInCurrentDirectory, &currentFiles, files, maxFiles, scrollPosition, currentDirectory);

  int keyPressed = -1;

  do
  {
    drawWindow(window, files, currentFiles, currentDirectory, cursorY);

    keyPressed = wgetch(window);

    if (keyPressed == KEY_BACKSPACE)
    {
      scrollPosition = 0;
      cursorY = 0;

      assert(moveUpDirectory(currentDirectory) == true && "Invalid dir passed into moveUpDirectory");
      updateFileVariables(&filesInCurrentDirectory, &currentFiles, files, maxFiles, scrollPosition, currentDirectory);
    }
    if (keyPressed == KEY_UP || keyPressed == 'w')
    {
      cursorY--;
      if (cursorY < 0)
      {
        if (scrollPosition > 0)
        {
          scrollPosition--;
          cursorY = maxFiles - 1;

          updateFileVariables(&filesInCurrentDirectory, &currentFiles, files, maxFiles, scrollPosition, currentDirectory);
        }
        else
        {
          cursorY = 0;
        }
      }
    }
    if (keyPressed == KEY_DOWN || keyPressed == 's')
    {
      cursorY++;
      if (cursorY >= currentFiles)
      {
        if ((filesInCurrentDirectory / maxFiles) - scrollPosition > 0)
        {
          scrollPosition++;
          cursorY = 0;

          updateFileVariables(&filesInCurrentDirectory, &currentFiles, files, maxFiles, scrollPosition, currentDirectory);
        }
        else
        {
          cursorY = currentFiles - 1;
        }
      }
    }
    // 10 is KEY_ENTER, ncurses KEY_ENTER is a different value that doesn't work for me
    if (keyPressed == 10 && files[cursorY].isDirectory)
    {
      moveIntoDirectory(currentDirectory, files[cursorY].filename);

      scrollPosition = 0;
      cursorY = 0;

      updateFileVariables(&filesInCurrentDirectory, &currentFiles, files, maxFiles, scrollPosition, currentDirectory);
    }

  } while (keyPressed != 27); // 27 is KEY_ESCAPE, but there is no macro for it in ncurses for some reason

  endwin();
  return EXIT_SUCCESS;
}