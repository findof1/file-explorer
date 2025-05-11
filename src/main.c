#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <assert.h>
#include <windows.h>
#include "fileUtils.h"
#define MAX_FILES 23

/*
Note:
(void) prior to certain funcions means we don't care about the return value. Reasonings:
initscr() - returns a window that is already automaticially stored at stdsrc
cbreak() - there are no error conditions
noecho() - there are no error conditions
keypad() - there are no error conditions
*/

typedef struct
{
  int filesInCurrentDirectory;
  int currentFiles;
  File files[MAX_FILES];
  int scrollPosition;
  int cursorY;
  char currentDirectory[MAX_PATH];
} AppData;

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
void drawWindow(WINDOW *window, AppData *appData)
{
  assert(wclear(window) == OK && "wclear failed");
  assert(box(window, 0, 0) == OK && "box failed");

  assert(mvwprintw(window, 1, 2, "%s", appData->currentDirectory) == OK && "mvwprintw failed");

  for (int i = 0; i < appData->currentFiles; i++)
  {
    assert(mvwprintw(window, 3 + i, 2, "%s", appData->files[i].filename) == OK && "mvwprintw failed");
  }

  wmove(window, appData->cursorY + 3, 1);

  refresh();
  assert(wrefresh(window) == OK && "wrefresh failed");
}

void updateFileVariables(AppData *data)
{
  data->filesInCurrentDirectory = getDirectoryFileAmount(data->currentDirectory);
  assert(data->filesInCurrentDirectory != -1 && "Invalid dir passed into getDirectoryFileAmount");

  data->currentFiles = listDirectoryContents(data->currentDirectory, data->files, MAX_FILES, data->scrollPosition * (MAX_FILES + 1));
  assert(data->currentFiles != -1 && "Invalid dir passed into listDirectoryContents");
}

void handleInput(const int keyPressed, AppData *appData)
{

  switch (keyPressed)
  {
  case KEY_BACKSPACE:
  {
    appData->scrollPosition = 0;
    appData->cursorY = 0;

    assert(moveUpDirectory(appData->currentDirectory) == true && "Invalid dir passed into moveUpDirectory");
    updateFileVariables(appData);
    break;
  }

  // 10 is KEY_ENTER, ncurses KEY_ENTER is a different value that doesn't work for me
  case 10:
  {
    if (!appData->files[appData->cursorY].isDirectory)
    {
      break;
    }

    moveIntoDirectory(appData->currentDirectory, appData->files[appData->cursorY].filename);

    appData->scrollPosition = 0;
    appData->cursorY = 0;

    updateFileVariables(appData);
    break;
  }

  case KEY_UP:
  case 'w':
  {
    appData->cursorY--;
    if (appData->cursorY >= 0)
    {
      break;
    }

    bool canScrollUp = appData->scrollPosition > 0;
    if (canScrollUp)
    {
      appData->scrollPosition--;
      appData->cursorY = MAX_FILES - 1;

      updateFileVariables(appData);
    }
    else
    {
      appData->cursorY = 0;
    }

    break;
  }

  case KEY_DOWN:
  case 's':
  {
    appData->cursorY++;
    if (appData->cursorY < appData->currentFiles)
    {
      break;
    }

    bool canScrollDown = (appData->filesInCurrentDirectory / MAX_FILES) - appData->scrollPosition > 0;
    if (canScrollDown)
    {
      appData->scrollPosition++;
      appData->cursorY = 0;

      updateFileVariables(appData);
    }
    else
    {
      appData->cursorY = appData->currentFiles - 1;
    }

    break;
  }
  }
}

int main()
{
  initNcurses();

  AppData appData;
  appData.currentFiles = 0;
  appData.filesInCurrentDirectory = 0;
  appData.cursorY = 0;
  appData.scrollPosition = 0;
  strcpy(appData.currentDirectory, "C:\\Users\\lucas\\OneDrive\\Desktop\\Coding\\C\\file-explorer");

  WINDOW *window = createWindow();

  updateFileVariables(&appData);

  int keyPressed = -1;

  do
  {
    drawWindow(window, &appData);

    keyPressed = wgetch(window);

    handleInput(keyPressed, &appData);

  } while (keyPressed != 27); // 27 is KEY_ESCAPE, but there is no macro for it in ncurses for some reason

  endwin();
  return EXIT_SUCCESS;
}