#include "gba.h"

void sync(){
  while(REG_DISPLAY_VCOUNT >= 160);
  while(REG_DISPLAY_VCOUNT < 160);
}

int s1=0, s2=0, s3=0, s4=0, s5=0, s6=0, s7=0, s8=0, s9=0;//Board positions
int currentLocation=0, playerMoving=0, gameWon=0;//general info to keep game running

uint16 colorGenerator(int r, int g, int b){//creates GBA color from RGB 256 values
  uint16 color = 0;
  int r1=r/8;
  int g1=g/8;
  int b1=b/8;
  color = (r1<<0) | (g1<<5) | (b1<<10);
  return color;
}


uint16 BLACK=0X0000;
uint16 WHITE=0XFFFF;
uint16 RED=0X001F;
uint16 GREEN=0X03E0;
uint16 BLUE=0X3E00;
uint16 YELLOW=0X03FF;
uint16 CYAN=0X7FE0;
uint16 MAGENTA=0X7C1F;
uint16 GRAY=0X7BEF;
uint16 BROWN=0X0C63;
uint16 ORANGE=0X01FF;
uint16 PINK=0X7C1F;
uint16 PURPLE=0X780F;
uint16 TIE=0X7BEF;
uint16 player1Color=0X001F;//red
uint16 player2Color=0X44C0;//blue
uint16 player1ColorSelect=0X3dff;//light red
uint16 player2ColorSelect=0X7FE0;

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
  pixel.h=pixel.w=1;
  pixel.x=pixel.y=0;
  
  y1.y = y2.y = y;
  y1.x = x+temp2;
  y2.x = x+width-temp2-thickness;

  x1.x = x2.x = x;
  x1.y = y+temp;
  x2.y = y+height-temp-thickness;
}

void drawHash(uint16 color){
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

void drawCircles(uint16 color){
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

void drawCircleInt(int location, uint16 color){
  if(location==1){drawCircle(p1, color);}
  if(location==2){drawCircle(p2, color);}
  if(location==3){drawCircle(p3, color);}
  if(location==4){drawCircle(p4, color);}
  if(location==5){drawCircle(p5, color);}
  if(location==6){drawCircle(p6, color);}
  if(location==7){drawCircle(p7, color);}
  if(location==8){drawCircle(p8, color);}
  if(location==9){drawCircle(p9, color);}
}

void initDrawBoard(){
  winner.x=0;
  winner.y=0;
  winner.h=SCREEN_HEIGHT;
  winner.w=SCREEN_WIDTH;
  drawRect(winner, BLACK);
  drawHash(WHITE);
  drawCircles(BLACK);
}

int checkWinner(){//returns winning player or 0 if no winner
  if (((s1==s2)&&(s2==s3))&&(s1!=0)){return s1;}
  if (((s4==s5)&&(s5==s6))&&(s4!=0)){return s4;}
  if (((s7==s8)&&(s8==s9))&&(s7!=0)){return s7;}
  if (((s1==s4)&&(s4==s7))&&(s1!=0)){return s1;}
  if (((s2==s5)&&(s5==s8))&&(s2!=0)){return s2;}
  if (((s3==s6)&&(s6==s9))&&(s3!=0)){return s3;}
  if (((s1==s5)&&(s5==s9))&&(s1!=0)){return s1;}
  if (((s3==s5)&&(s5==s7))&&(s3!=0)){return s3;}
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

void updateBGBoard(int location, int player){
    if(location==1){s1=player;}
    if(location==2){s2=player;}
    if(location==3){s3=player;}
    if(location==4){s4=player;}
    if(location==5){s5=player;}
    if(location==6){s6=player;}
    if(location==7){s7=player;}
    if(location==8){s8=player;}
    if(location==9){s9=player;}
}

void playerTempMove(int location, int player){
  if(player==1){
    drawCircleInt(location, player1ColorSelect);
  }
  if(player==2){
    drawCircleInt(location, player2ColorSelect);
  }
}

void playerMove(int location, int player){//does not check if spot is available(needs to be done in main function)
  if(player==1){
    drawCircleInt(location, player1Color);
  }else if(player==2){
    drawCircleInt(location, player2Color);
  }else if(player==0){
    drawCircleInt(location, BLACK);
  }
  updateBGBoard(location,player);
}

int findEmptySpace(){
  if(s1==0){return 1;}
  if(s2==0){return 2;}
  if(s3==0){return 3;}
  if(s4==0){return 4;}
  if(s5==0){return 5;}
  if(s6==0){return 6;}
  if(s7==0){return 7;}
  if(s8==0){return 8;}
  if(s9==0){return 9;}
  return 0;
}

void drawBoard(){
  drawRect(winner, BLACK);
  drawHash(WHITE);
  drawCircles(BLACK);
  playerMove(1,s1);
  playerMove(2,s2);
  playerMove(3,s3);
  playerMove(4,s4);
  playerMove(5,s5);
  playerMove(6,s6);
  playerMove(7,s7);
  playerMove(8,s8);
  playerMove(9,s9);
}

int tempMove(int direction, int location){
  if(direction==0){
    //go up
    if(location<=3){return location;}
    return location-3;
  }
  if(direction==1){
    //go down
    if(location>=7){return location;}
    return location+3;
  }
  if(direction==2){
    //go left
    if(location==1||location==4||location==7){return location;}
    return location-1;
  }
  if(direction==3){
    //go right
    if(location==3||location==6||location==9){return location;}
    return location+1;
  }
  return location;
}

int checkUserInput(){
  if(!(REG_DISPLAY_INPUT & UP)){
    while(!(REG_DISPLAY_INPUT & UP)){}//delay
    return 0;
  }
  if(!(REG_DISPLAY_INPUT & DOWN)){
    while(!(REG_DISPLAY_INPUT & DOWN)){}
    return 1;
  }
  if(!(REG_DISPLAY_INPUT & LEFT)){
    while(!(REG_DISPLAY_INPUT & LEFT)){}
    return 2;
  }
  if(!(REG_DISPLAY_INPUT & RIGHT)){
    while(!(REG_DISPLAY_INPUT & RIGHT)){}
    return 3;
  }
  if(!(REG_DISPLAY_INPUT & SELECT)){
    while(!(REG_DISPLAY_INPUT & SELECT)){}
    return 4;
  }
  return -1;
}

int checkTie(){
  if(findEmptySpace()==0){
    return 1;
  }
  return 0;
}

void restartGame(){
  s1=0;
  s2=0;
  s3=0;
  s4=0;
  s5=0;
  s6=0;
  s7=0;
  s8=0;
  s9=0;
  initDrawBoard();
  playerMoving=0;
  drawBoard();
 }

int main(){
  REG_DISPLAY = VIDEOMODE | BGMODE;
  //initiate board
  initHash(0,(SCREEN_WIDTH-SCREEN_HEIGHT)/2,SCREEN_HEIGHT,SCREEN_HEIGHT,3);
  initCircles(0,(SCREEN_WIDTH-SCREEN_HEIGHT)/2,SCREEN_HEIGHT,SCREEN_HEIGHT,3);
  initDrawBoard();

  while(1){
    sync();
    if(playerMoving==0){
      playerMoving=1;
    }
    if(currentLocation==0&&gameWon==0){//if no current location, find empty space and move to it
      currentLocation=findEmptySpace();
      playerTempMove(currentLocation, playerMoving);
    }
    int temp= checkUserInput();//check for user input
    
    if(-1<temp&&temp<4&&gameWon==0){//if user input is valid and game is not won
      drawBoard();
      currentLocation=tempMove(temp,currentLocation);
      playerTempMove(currentLocation, playerMoving);//draws temp move
    }//if user input is a move command, move player
    
    if(temp==4){//if player presses select, do appropiate action
    if(gameWon!=0){
      restartGame();
      gameWon=0;
    }
      if(checkSpace(currentLocation)==1){//if space is not empty, do nothing
        playerMove(currentLocation, playerMoving);//moves player on the BG board
        currentLocation=0;

        if(checkWinner()==1){
          drawRect(winner, player1Color);
          gameWon=1;
        }else if(checkWinner()==2){
          drawRect(winner, player2Color);
          gameWon=2;
        }else if(checkTie()==1){
            drawRect(winner, WHITE);
            gameWon=3;
          }
        if (playerMoving==1){playerMoving=2;}//switches player
        else{playerMoving=1;}
      }
    }
  }
}