class MatrixGraphic {
  int cellSize = 50;
  int cols = 8;
  int rows = 8;
  int grayArray[][];
  boolean binary = true;
  int threshold;

  MatrixGraphic() {
    grayArray = new int[cols][rows];
  }

  void update(int line, int dot, int value) {
    grayArray[line][dot] = value;
  }

  void display(int xOffset, int yOffset, int size) {
    cellSize = int(size / 8);
    for (int j = 0; j < rows; j++) {
      for (int i = 0; i < cols; i++) {
        int x = i*cellSize + xOffset;
        int y = j*cellSize + yOffset;
        int luma = grayArray[j][i];
        pushMatrix();
        translate(x+cellSize/2, y+cellSize/2);
        rectMode(CENTER);
        int binaryLuma = 0;
        if (luma > threshold)  binaryLuma = 255;
        if (binary) fill(binaryLuma);
        else fill(luma);
        noStroke();
        rect(0, 0, cellSize+6, cellSize+6);
        popMatrix();
      }
    }
  }
  
  void setBinariseThreshold(int threshold){
    this.threshold = threshold;
  }

  void switchBinarise() {
    this.binary = !this.binary;
  }
}
