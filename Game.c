#include<graphics.h>

#include<stdlib.h>

#include<time.h>

#include<conio.h>

#include<dos.h>

#include<alloc.h>

#include<stdio.h>

union REGS i, o;
/* initlize the mouse */
mouse_init() {
  i.x.ax = 0;
  int86(0x33, & i, & o);
  return (o.x.ax);
}
/* Showing mouse pointer */
void show_mouse_ptr() {
  i.x.ax = 1;
  int86(0x33, & i, & o);
}
/* Hide mouse pointer */
void hide_mouse_ptr() {
  i.x.ax = 2;
  int86(0x33, & i, & o);
}
/* get mouse position & its button click */
void get_mouse_pos(int * button, int * x, int * y) {
  i.x.ax = 3;
  int86(0x33, & i, & o);
  * button = o.x.bx;
  * x = o.x.cx;
  * y = o.x.dx;
}
/* Draw The Boss */
void target(int y) {
  rectangle(getmaxx(), y, getmaxx() - 10, y + 10);
  rectangle(getmaxx() - 10, y + 3, getmaxx() - 13, y + 7);
}
/* Draw The player */
void source(int y) {
  rectangle(5, y, 15, y + 10);
  rectangle(0, y - 5, 5, y + 2);
  rectangle(0, y + 8, 5, y + 15);
  rectangle(15, y + 3, 18, y + 7);
}
int pt = 0; //Point
int dt = 24; //Speed
/* increase point */
void point() {
  pt++;
}
/* Well Come Screen */
void intro() {
  char ch;
  sss:
    cleardevice();
  outtextxy(((getmaxx() / 2) - 99), getmaxy() / 2, "I.Intro P.Play X.Exit");
  ch = getch();
  if (ch == 'I' || ch == 'i')
    goto intro;
  if (ch == 'P' || ch == 'p')
    goto play;
  if (ch == 'X' || ch == 'x') {
    closegraph();
    exit(1);
  }
  goto sss;
  intro:
    cleardevice();
  printf("Left Click-->Bullet Fire\nRight Click-->Line Fire\nMouse Move-->Position Change\nHit ballon,Boss for points and make 5000 point to win.\nIf Ballon cross your Area point will decrease");
  getch();
  play:
    cleardevice();
  outtextxy(100, 100, "F.Fast N.Normal S.Slow");
  ch = getch();
  if (ch == 'f' || ch == 'F')
    dt = 12;
  else if (ch == 'n' || ch == 'N')
    dt = 24;
  else if (ch == 'S' || ch == 's')
    dt = 40;
  else
    goto play;

}
/* Begining of GAME.C */
void main() {
  int gd = DETECT, gm, i, j = 0, t = 0, button, x, y;
  int times = 0, k = 0;
  int temp = 0;
  int firefrom_x[5000], firefrom_y[5000];
  int targety = 0;
  int point_p_x[5000], point_p_y[5000], point_l_x[5000], point_l_y[5000], run = 0, rn = 0;
  char ch;
  int re_fire_x[5000], re_fire_y[5000];
  int line_fire_x[5000], n = 0, m = 0;
  initgraph( & gd, & gm, "C:\\tc\\BGI");
  intro();
  sos:
    dt -= 4;
  mouse_init();
  pt = 0;
  while (!kbhit()) {
    cleardevice();
    get_mouse_pos( & button, & x, & y);
    /* Bullet fire */
    if (button == 1) {
      firefrom_y[t] = y + 5;
      firefrom_x[t] = 15;
      t++;
      if (t > 4999)
        t = 0;

    }
    /*Line Fire */
    if (button == 2) {
      if (n < 5) {
        line_fire_x[n] = 10;
        n++;
        delay(100);
      }
    }
    /* Moving Line Fire and hit Ballon */
    for (i = 0; i < n; i++) {
      line(line_fire_x[i], 0, line_fire_x[i], getmaxy());
      line_fire_x[i] += 2;
      for (m = 0; m < k; m++) {
        if (line_fire_x[i] < re_fire_x[m] + 10 && line_fire_x[i] > re_fire_x[m] - 10)
          re_fire_x[m] = -50;
      }
    }
    /* ON Ballon hit & Moving Bullet */
    for (i = 0; i < t; i++) {
      firefrom_x[i] += 2;
      putpixel(firefrom_x[i], firefrom_y[i], WHITE);
      if (temp > k - 1)
        temp = 0;
      else
        temp++;
      if (firefrom_x[i] > re_fire_x[temp] - 10 && firefrom_x[i] < re_fire_x[temp] + 10 && firefrom_y[i] < re_fire_y[temp] + 10 && firefrom_y[i] > re_fire_y[temp] - 10) {
        point();
        re_fire_x[temp] = -50;
        firefrom_x[i] = getmaxx() + 10;
      }
    }
    /* if ballon hit my Area */
    for (i = 0; i < k; i++) {
      if (re_fire_x[i] < 30 && re_fire_x[i] > -10)
        pt -= 1;
    }
    /* if i hit boss */
    for (i = 0; i < t; i++) {
      if (firefrom_x[i] > getmaxx() - 13 && firefrom_x[i] < getmaxx() && firefrom_y[i] > targety && firefrom_y[i] < targety + 10)
        pt += 1;
    }
    times++;
    targety++;
    if (targety > getmaxy())
      targety = 0;
    target(targety);
    source(y);
    /* Generate Ballon */
    if (rand() % 100 == 0 || rand() % 3 == 4) {
      times = 0;
      re_fire_x[k] = getmaxx();
      re_fire_y[k] = targety;
      k++;
    }
    if (rand() % 100 == 1) {
      if (rand() % 10 == 0) {
        //Line point
        point_l_x[run] = getmaxx();
        point_l_y[run] = targety;
        run++;
      } else {
        //Point p
        point_p_x[rn] = getmaxx();
        point_p_y[rn] = targety;
        rn++;
      }
    }
    /* Moving Line Point */
    for (i = 0; i < run; i++) {
      circle(point_l_x[i], point_l_y[i], 15);
      outtextxy(point_l_x[i], point_l_y[i], "L");
      point_l_x[i] -= 2;
      for (j = 0; j < t; j++)
        if (firefrom_x[j] < point_l_x[i] + 15 && firefrom_x[j] > point_l_x[i] - 15 && firefrom_y[j] < point_l_y[i] + 15 && firefrom_y[j] > point_l_y[i] - 15) {
          pt += 5;
          point_l_x[i] = -50;
          if (n < 5)
            n--;
        }
    }
    /* Moving Point */
    for (i = 0; i < rn; i++) {
      circle(point_p_x[i], point_p_y[i], 15);
      outtextxy(point_p_x[i], point_p_y[i], "P");
      point_p_x[i] -= 2;
      for (j = 0; j < t; j++)
        if (firefrom_x[j] < point_p_x[i] + 15 && firefrom_x[j] > point_p_x[i] - 15 && firefrom_y[j] < point_p_y[i] + 15 && firefrom_y[j] > point_p_y[i] - 15) {
          pt += 5;
          point_p_x[i] = -50;
        }
    }
    if (rn > 4995)
      rn = 0;
    if (run > 4995)
      run = 0;
    if (k > 4996)
      k = 0;
    /* Ballon Move */
    for (j = 0; j < k; j++) {
      re_fire_x[j] -= 2;
      fillellipse(re_fire_x[j], re_fire_y[j], 0, 44 / 7);
      circle(re_fire_x[j], re_fire_y[j], 10);
      circle(re_fire_x[j] + 20, re_fire_y[j], 6);
      circle(re_fire_x[j] + 35, re_fire_y[j], 3);
    }

    delay(dt);
    if (pt > 1000)
      break;
    if (pt < -500) {
      dt = dt;
      break;
    }

  }
  printf("POINT:%d", pt);
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  settextstyle(2, HORIZ_DIR, 6);
  outtextxy(getmaxx() / 2, getmaxy() / 2, "Do You Want's To Play Again(Y/N)");
  getch();
  ch = getch();
  if (ch == 'y' || ch == 'Y')
    goto sos;
  closegraph();
}
