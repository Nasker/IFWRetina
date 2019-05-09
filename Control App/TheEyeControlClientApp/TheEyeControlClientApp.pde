import oscP5.*;
import netP5.*;
import controlP5.*;

ControlP5 cp5;
OscP5 oscP5;
NetAddress ledMatrixMicroHost;
NetAddress cameraMicroHost;
MatrixGraphic matrix;
PImage ifaeLogo;
//RectSelector rSelect;

void setup() {
  size(1200, 800);
  oscP5 = new OscP5(this, 4321);
  ledMatrixMicroHost = new NetAddress("10.10.10.3", 1234);
  cameraMicroHost = new NetAddress("10.10.10.1", 5678);
  matrix = new MatrixGraphic();
  //rSelect = new RectSelector(int(width*0.2), 600, 320, 240);
  ifaeLogo = loadImage("logoIfae.png");

  cp5 = new ControlP5(this);
  cp5.addButton("RotateMatrix").setValue(0).setPosition(width*0.08, 300).setSize(200, 50);
  cp5.addButton("Binarise").setValue(0).setPosition(width*0.08, 400).setSize(200, 50);
  cp5.addSlider("Threshold").setPosition(width*0.08, 500).setSize(200, 50).setRange(0, 255).setValue(140);
}

void draw() {
  background(64);
  matrix.display(int(width*0.3333), 0, 800);
  image(ifaeLogo,17,23,1808/5,540/5);
  //rSelect.display();
  println(mouseX + " " + mouseY);
}

void oscEvent(OscMessage theOscMessage) {
  for (int line = 0; line < matrix.rows; line++) {
    if (theOscMessage.checkAddrPattern("/camera/grayArray/line"+line)==true) {
      for (int dot = 0; dot < matrix.cols; dot++) {
        matrix.update(line, dot, theOscMessage.get(dot).intValue());
      }
    }
  }
}
int rotation = 1;
public void RotateMatrix(int value) {
  println("Rotate Matrix Switch switch");
  OscMessage thresholdMsg = new OscMessage("/matrix/rotate");
  rotation++;
  if (rotation > 3) rotation = 0;
  thresholdMsg.add(rotation);
  oscP5.send(thresholdMsg, ledMatrixMicroHost);
}

public void Binarise(int value) {
  println("Binarise switch");
  matrix.switchBinarise();
}

public void Threshold(int value) {
  matrix.setBinariseThreshold(value);
  println("Setting binarize threshold to " + value);
  OscMessage thresholdMsg = new OscMessage("/camera/adjustThreshold");
  thresholdMsg.add(value);
  oscP5.send(thresholdMsg, ledMatrixMicroHost);
  println("Sending threshold control message");
}
/*
void mouseDragged() {
 rSelect.setPosition(mouseX, mouseY);
 }
 
 void mouseWheel(MouseEvent event) {
 rSelect.setSize(int (event.getCount()));
 }
 */
