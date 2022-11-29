//main.c

#include "gba.h"

void sync(){
  while(REG_DISPLAY_VCOUNT >= 160);
  while(REG_DISPLAY_VCOUNT < 160);
}

int s1=0;
int s2=0;
int s3=0;
int s4=0;
int s5=0;
int s6=0;
int s7=0;
int s8=0;
int s9=0;

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

void initHash(int y, int x, int height, int width, int thickness){
  int temp=(height-thickness*2)/3;
  int temp2=(width-thickness*2)/3;
  x1.h = x2.h = thickness;
  x1.w = x2.w = width;
  y1.h = y2.h = height;
  y1.w = y2.w = thickness;
  
  y1.y = y2.y = y;
  y1.x = x+temp2;
  y2.x = x+width-temp2-thickness;

  x1.x = x2.x = x;
  x1.y = y+temp;
  x2.y = y+height-temp-thickness;
}

void drawHash(int y, int x, int height, int width, int thickness, uint16 color){
  drawRect(x1, color);
  drawRect(x2, color);
  drawRect(y1, color);
  drawRect(y2, color);
}

void initCircles(int y, int x, int width, int height, int thickness){
  p1.r = p2.r = p3.r = p4.r = p5.r = p6.r = p7.r = p8.r = p9.r = (height-thickness*2)/6;
  p1.x = p4.x = p7.x = x;
  p1.y = p2.y = p3.y = y;
  p2.x = p5.x = p8.x = x+(width-thickness*2)/3+thickness;
  p3.x = p6.x = p9.x = x+(width-thickness*2)*2/3+2*thickness+1;
  p4.y = p5.y = p6.y = y+(height-thickness*2)/3+thickness;
  p7.y = p8.y = p9.y = y+(height-thickness*2)*2/3+2*thickness+1;

}

void drawCircles(int y, int x, int width, int height, int thickness, uint16 color){
  drawCircle(p1, color);
  drawCircle(p2, color);
  drawCircle(p3, color);
  drawCircle(p4, color);
  drawCircle(p5, color);
  drawCircle(p6, color);
  drawCircle(p7, color);
  drawCircle(p8, color);
  drawCircle(p9, color);
}

void initDrawBoard(int y, int x, int width, int height, int thickness, uint16 color){
  winner.x=0;
  winner.y=0;
  winner.h=SCREEN_HEIGHT;
  winner.w=SCREEN_WIDTH;
  drawRect(winner, color);
  drawHash(y, x, height, width, thickness, 0XFFFF);
  drawCircles(y, x, width, height, thickness, 0X0000);
}

int checkWinner(){
  if (s1==s2==s3&&s1!=0){
    return s1;
  }
  if(s4==s5==s6&&s4!=0){
    return s4;
  }
  if(s7==s8==s9&&s7!=0){
    return s7;
  }
  if(s1==s4==s7&&s1!=0){
    return s7;
  }
  if(s2==s5==s8&&s2!=0){
    return s2;
  }
  if(s3==s6==s9&&s3!=0){
    return s3;
  }
  if(s1==s5==s9&&s1!=0){
    return s1;
  }
  if(s3==s5==s7&&s3!=0){
    return s3;
  }
  return 0;

}

int checkSpace(int location){
  if(location==1&&s1==0){return 1;}
  if(location==2&&s2==0){return 1;}
  if(location==3&&s3==0){return 1;}
  if(location==4&&s4==0){return 1;}
  if(location==5&&s5==0){return 1;}
  if(location==6&&s6==0){return 1;}
  if(location==7&&s7==0){return 1;}
  if(location==8&&s8==0){return 1;}
  if(location==9&&s9==0){return 1;}
  return 0;
}

//returns location of move command. IE, if 1 moves left, it returns 2
//0=up, 1=down, 2=left, 3=right
int move(int direction, int location){
  if(direction==0){
    //go up
    if(location<=3){return location;}
    if(location>3){
      if(checkSpace(location-3)==1){return location-3;}
      return location;
      }
  }
  if(direction==1){
    //go down
    if(location>=7){return location;}
    if(location<7){
      if(checkSpace(location+3)==1){return location+3;}
      return location;
      }
  }
  if(direction==2){
    //go left
    if(location==1||location==4||location==7){return location;}
    else{
      if(checkSpace(location-1)==1){return location-1;}
      return location;
      }
  }
  if(direction==3){
    //go right
    if(location==3||location==6||location==9){return location;}
    else{
      if(checkSpace(location+1)==1){return location+1;}
      return location;
      }

  }
}

int checkUserInput(){
  
}

int main(){
  REG_DISPLAY = VIDEOMODE | BGMODE;
  
  initHash(0,(SCREEN_WIDTH-SCREEN_HEIGHT)/2,SCREEN_HEIGHT,SCREEN_HEIGHT,3);
  initCircles(0,(SCREEN_WIDTH-SCREEN_HEIGHT)/2,SCREEN_HEIGHT,SCREEN_HEIGHT,3);

  while(1){
    sync();
    drawHash(0,(SCREEN_WIDTH-SCREEN_HEIGHT)/2,SCREEN_HEIGHT,SCREEN_HEIGHT,3,0xFFFF);
    drawCircles(0,(SCREEN_WIDTH-SCREEN_HEIGHT)/2,SCREEN_HEIGHT,SCREEN_HEIGHT,3,0xFFFF);
  }
  
  
}