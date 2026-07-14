#include <ncursesw/ncurses.h>
#include <locale.h>   // 新增：设置中文编码

int main(void)
{
    setlocale(LC_ALL, "");  // 新增：必须放在 initscr 之前！

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    attron(COLOR_PAIR(1));
    mvprintw(3, 5, "WSL ncurses 测试成功！按q退出");
    attroff(COLOR_PAIR(1));
    refresh();

    int ch;
    while((ch = getch()) != 'q');

    endwin();
    return 0;
}
