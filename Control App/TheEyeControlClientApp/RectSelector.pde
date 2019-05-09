class RectSelector {
  int posX = 0;
  int posY = 0;
  int size = 80;
  int frameSizeX;
  int frameSizeY;
  int framePosX;
  int framePosY;
  int minSize = 64;

  public RectSelector(int framePosX, int framePosY, int frameSizeX, int frameSizeY) {
    this.frameSizeX = frameSizeX;
    this.frameSizeY = frameSizeY;
    this.framePosX = framePosX;
    this.framePosY = framePosY;
  }

  public void setPosition(int posX, int posY) {
    this.posX = posX;
    this.posY = posY;
  }

  public void setSize(int inc) {
    this.size += inc;
    this.size = constrain(this.size, this.minSize, this.frameSizeY);
  }

  public void display() {
    this.posX = constrain(this.posX, this.framePosX, (this.framePosX + this.frameSizeX) - size);
    this.posY = constrain(this.posY, this.framePosY, (this.framePosY + this.frameSizeY) - size);
    fill(127);
    rect(this.framePosX, this.framePosY, this.frameSizeX, this.frameSizeY);
    fill(255);
    rect(this.posX, this.posY, this.size, this.size);
    noFill();
    stroke(0, 255, 0);
    rect(this.posX, this.posY, this.size, this.size);
  }
}
