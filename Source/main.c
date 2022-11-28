//main.c

#include "gba.h"

void sync(){
  while(REG_DISPLAY_VCOUNT >= 160);
  while(REG_DISPLAY_VCOUNT < 160);
}

void drawRect(struct Rect r, uint16 color){
  for(int i = 0; i < r.h; i++){
    for(int j = 0; j < r.w; j++){
      SCREENBUFFER[SCREEN_WIDTH * (i + r.y) + (j + r.x)] = color;
    }
  }
}

void drawCircle(struct Circle r, uint16 color){
  for(int x = 0; x < 2*r.r; x++){
    for(int y = 0; y < 2*r.r; y++){
        if((x-r.r)*(x-r.r) + (y-r.r)*(y-r.r) <= r.r*r.r){
            SCREENBUFFER[SCREEN_WIDTH * (y + r.y) + (x + r.x)] = color;
        }
    }
  }
}

void init7seg(int x, int y, int th, int len){
  a.w = f.h = len;
  a.h = f.w = th;

  d = g = a;
  b = c = e = f;
  
  a.x = d.x = e.x = f.x = g.x = x;
  b.x = c.x = a.x + a.w;
  
  a.y = b.y = f.y = y;
  c.y = e.y = g.y = b.y + b.h - a.h;
  d.y = g.y + b.h - a.h;
}

void clear7seg(){
  drawRect(a, 0x0000);
  drawRect(b, 0x0000);
  drawRect(c, 0x0000);
  drawRect(d, 0x0000);
  drawRect(e, 0x0000);
  drawRect(f, 0x0000);
  drawRect(g, 0x0000);
}

void draw7Seg10(uint8 num){
  clear7seg();
  uint8 w,x,y,z;
  w = num & 8;
  x = num & 4;
  y = num & 2;
  z = num & 1;
  //draw seven segment display
  if((y || w || (x && z) || (!x && !z)))
    drawRect(a, 0xFFFF);
  if(w || (!y && !z) || (y && z) || !x)
    drawRect(b, 0xFFFF);
  if(!y || x || w || z)
    drawRect(c, 0xFFFF);
  if(w || (y && !z) || (!x && y) || (!x && !z) || (x && z && !y))
    drawRect(d, 0xFFFF);
  if((!x && !z) || (y && !z) || (w && !z))
    drawRect(e, 0xFFFF);
  if((!y && !z) || (x && !y) || (x && !z) || w)
    drawRect(f, 0xFFFF);
  if((!x && y) || (x && !y) || (y && !z) || w)
    drawRect(g, 0xFFFF);
}

void draw7Seg(int num){
    uint8 ones = num % 10;
    uint8 tens = num / 10;
    init7seg(SCREEN_WIDTH/2-24, 0, 4, 16);
    draw7Seg10(tens);
    init7seg(SCREEN_WIDTH/2, 0, 4, 16);
    draw7Seg10(ones);
}

int main(){
  REG_DISPLAY = VIDEOMODE | BGMODE;
  
  init7seg(SCREEN_WIDTH/2, 0, 4, 16);
  
  int counter = 0;
  uint8 down = 0;
  
  ball.r = 20;
  
  ball.x = 120 - ball.r;
  ball.y = 80 - ball.r;
  
  player.w = 8;
  player.h = 32;
  
  player.x = 0;
  player.y = 0;
  
  int ballSpeedX = 1;
  int ballSpeedY = 1;
  
  int playerSpeedY = 2;
  
  uint16 ballColor = 0x1111;
  uint16 playerColor = 0x2222;
  
  while(1){

    draw7Seg(counter);
    drawCircle(prevBall, 0x0000);
    drawRect(prevPlayer, 0x0000);
    
    if(!(REG_DISPLAY_INPUT & A) && !down)
    {
      counter++;
      if(counter > 99)
        counter = 0;
      down = 1;
    }
    else if((REG_DISPLAY_INPUT & A))
    {
      down = 0;
    }
    
    //draw7Seg(counter);
    drawCircle(ball, ballColor);
    drawRect(player, playerColor);
    
    prevBall = ball;
    prevPlayer = player;
    
    //do all movement here after drawing
    ball.x += ballSpeedX;
    ball.y += ballSpeedY;
    
    if(!(REG_DISPLAY_INPUT & DOWN))
      player.y += playerSpeedY;
    if(!(REG_DISPLAY_INPUT & UP))
      player.y -= playerSpeedY;

    if(player.y < 0)
      player.y = 0;
    else if(player.y >= SCREEN_HEIGHT - player.h)
      player.y = SCREEN_HEIGHT - player.h;
    
    if(ball.y + ball.r*2 >= 160 || ball.y <= 0)
    {
      ballSpeedY = -ballSpeedY;
    }

    if(ball.x + ball.r*2 >= 240 || ball.x <= 0)
    {
      ballSpeedX = -ballSpeedX;
    }
    sync();
  }
}